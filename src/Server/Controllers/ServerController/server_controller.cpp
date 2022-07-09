#include "server_controller.h"

#include "src/Helpers/Constants/constants.h"

ServerController::ServerController()
  :  web_socket_server_("", QWebSocketServer::NonSecureMode) {
  if (web_socket_server_.listen(QHostAddress::Any, constants::kDefaultPort)) {
    connect(&web_socket_server_,
            &QWebSocketServer::newConnection,
            this,
            &ServerController::OnSocketConnect);
  }
  connect(&server_model_, &ServerModel::SendRoomsList,
          this, &ServerController::SendRoomsListEvent);
  StartTicking();
}

void ServerController::OnByteArrayReceived(const QByteArray& message) {
  events::EventWrapper received_event;
  if (!received_event.ParseFromArray(message.data(), message.size())) {
    LOG << GetControllerName() << " failed to parse message";
    return;
  }

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket);
  UserId user_id = user->GetId();

  auto* client_event = received_event.mutable_client_event();
  client_event->set_sender_id(user_id);

  LogEvent(received_event, logging::Type::kReceive);

  if (client_event->has_event_to_server()) {
    AddEventToHandle(received_event);
  } else {
    AddEventToSend(received_event);
  }
}

void ServerController::OnSocketConnect() {
  std::shared_ptr<QWebSocket> current_socket(
      web_socket_server_.nextPendingConnection());
  LOG << "Socket connected: " << current_socket.get();
  UserId new_user_id = server_model_.GetUnusedUserId();
  auto new_user = std::make_shared<User>(new_user_id,
                                         current_socket);
  server_model_.AddUser(new_user);

  connect(new_user->GetSocket().get(),
          &QWebSocket::binaryMessageReceived,
          this,
          &ServerController::OnByteArrayReceived);

  connect(new_user->GetSocket().get(),
          &QWebSocket::disconnected,
          this,
          &ServerController::OnSocketDisconnect);
}

void ServerController::OnSocketDisconnect() {
  auto web_socket = qobject_cast<QWebSocket*>(sender());
  LOG << "Socket disconnected: " << web_socket;
  if (web_socket) {
    UserId user_id = server_model_.GetUserBySocket(web_socket)->GetId();
    AddEventToHandle(GenerateDeleteUserEvent(user_id));
  }
}

QString ServerController::GetControllerName() const {
  return "Server";
}

void ServerController::OnTick() {}

void ServerController::Send(const events::EventWrapper& event) {
  switch (event.type_case()) {
    case events::EventWrapper::kServerEvent: {
      const auto& users = server_model_.GetUsers();
      if (!users.empty()) {
        LogEvent(event, logging::Type::kSend);
      }
      SendEveryUser(event);
      break;
    }
    case events::EventWrapper::kClientEvent: {
      LogEvent(event, logging::Type::kSend);
      switch (event.client_event().receiver_case()) {
        case client_events::ClientEventWrapper::kEventToRoom: {
          SendEventToRoom(event);
          break;
        }
        case client_events::ClientEventWrapper::kEventToGame: {
          auto room =
              server_model_.GetRoomByUserId(event.client_event().sender_id());
          room->SendEventToGame(event);

          break;
        }
        default: {}
      }
    }
    default: {}
  }
}

void ServerController::HandleClientEvent(const events::EventWrapper& event) {
  const auto& client_event = event.client_event();
  UserId user_id = client_event.sender_id();
  const auto& event_to_server = client_event.event_to_server();
  auto user = server_model_.GetUserById(user_id);
  switch (event_to_server.type_case()) {
    case client_events::EventToServer::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      auto new_room = std::make_shared<RoomController>(new_room_id, user);
      server_model_.AddRoom(new_room);
      connect(new_room.get(), &RoomController::SendRoomsList, [&]() {
        emit SendRoomsListEvent();
      });
      server_model_.AddUserToRoom(user_id, new_room_id);
      break;
    }
    case client_events::EventToServer::kEnterRoom: {
      auto room_id = event_to_server.enter_room().room_id();
      if (!server_model_.ExistsRoom(room_id)) {
        break;
      }
      auto room = server_model_.GetRoomById(room_id);
      if (room->IsInGame() ||
          room->GetPlayersCount() >= constants::kMaxRoomPlayersCount) {
        break;
      }
      if (server_model_.IsInSomeRoom(user_id)) {
        break;
      }
      server_model_.AddUserToRoom(user_id, room_id);
      break;
    }
    case client_events::EventToServer::kLeaveRoom: {
      if (server_model_.IsInSomeRoom(user_id)) {
        server_model_.DeleteUserFromRoom(user_id);
      }
      break;
    }
    default: {}
  }
}

void ServerController::HandleInternalEvent(const events::EventWrapper& event) {
  switch (event.internal_event().type_case()) {
    case internal_events::InternalEventWrapper::kDeleteUser: {
      UserId user_id = event.internal_event().delete_user().user_id();
      if (server_model_.IsInSomeRoom(user_id)) {
        auto room = server_model_.GetRoomByUserId(user_id);
        room->DeleteUser(user_id);
        if (room->IsEmpty()) {
          server_model_.DeleteRoom(room->GetId());
        }
      }
      server_model_.DeleteUser(user_id);
      break;
    }
    default: {}
  }
}

void ServerController::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);
  switch (event.type_case()) {
    case events::EventWrapper::kClientEvent: {
      HandleClientEvent(event);
      break;
    }
    case events::EventWrapper::kInternalEvent: {
      HandleInternalEvent(event);
      break;
    }
    default: {}
  }
}

void ServerController::SendEventToRoom(
    const events::EventWrapper& event) const {
  server_model_.GetRoomByUserId(
      event.client_event().sender_id())->AddEventToHandle(event);
}

void ServerController::SendRoomsListEvent() {
  auto* rooms_list = new server_events::RoomsList;
  for (const auto& [room_id, room_ptr] : server_model_.GetRooms()) {
    if (!room_ptr->IsInGame() &&
        room_ptr->GetPlayersCount() < constants::kMaxRoomPlayersCount) {
      rooms_list->add_ids(room_id);
    }
  }
  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_allocated_rooms_list(rooms_list);
  events::EventWrapper event_wrapper;
  event_wrapper.set_allocated_server_event(server_event);
  AddEventToSend(event_wrapper);
}

void ServerController::SendEveryUser(events::EventWrapper event) const {
  const auto& users = server_model_.GetUsers();
  for (const auto& [user_id, user_ptr] : users) {
    event.mutable_server_event()->set_receiver_id(user_id);
    auto serialized = event.SerializeAsString();
    QByteArray byte_array(serialized.data(), serialized.size());
    user_ptr->GetSocket()->sendBinaryMessage(byte_array);
  }
}

events::EventWrapper ServerController::GenerateDeleteUserEvent(
    UserId user_id) {
  auto* delete_user_event = new internal_events::DeleteUser;
  delete_user_event->set_user_id(user_id);
  auto* internal_event_wrapper = new internal_events::InternalEventWrapper;
  internal_event_wrapper->set_allocated_delete_user(delete_user_event);

  events::EventWrapper event_wrapper;
  event_wrapper.set_allocated_internal_event(internal_event_wrapper);
  return event_wrapper;
}
