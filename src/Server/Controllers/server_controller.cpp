#include "server_controller.h"

#include "Constants/constants.h"

ServerController::ServerController()
  :  web_socket_server_("", QWebSocketServer::NonSecureMode) {
  if (web_socket_server_.listen(QHostAddress::Any, constants::kDefaultPort)) {
    connect(&web_socket_server_,
            &QWebSocketServer::newConnection,
            this,
            &ServerController::OnSocketConnect);
  }
  StartTicking();
}

void ServerController::OnByteArrayReceived(const QByteArray& message) {
  events::EventWrapper received_event;
  if (!received_event.ParseFromArray(message.data(), message.size())) {
    // TODO(niki4smirn): handle parse fail
    return;
  }

  auto* client_event = received_event.mutable_client_event();

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket);
  UserId user_id = user->GetId();
  client_event->set_sender_id(user_id);

  LogEvent(received_event, logging::Type::kReceive);

  // TODO(niki4smirn): try to make this check prettier
  if (client_event->has_event_to_server()) {
    AddEventToHandle(received_event);
  } else {
    AddEventToSend(received_event);
  }
}

void ServerController::OnSocketConnect() {
  std::shared_ptr<QWebSocket> current_socket(
      web_socket_server_.nextPendingConnection());
  qInfo() << "Socket connected:" << current_socket.get();
  UserId new_user_id = server_model_.GetUnusedUserId();
  auto new_user = std::make_shared<User>(new_user_id,
                                         current_socket);
  // TODO(Everyone): replace with adding to handle queue
  // so, this TODO is not really relevant, because it may cause some
  // security problems
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
  qInfo() << "Socket disconnected:" << web_socket;
  if (web_socket) {
    auto user = server_model_.GetUserBySocket(web_socket);
    UserId user_id = user->GetId();
    // TODO(Everyone): replace with adding to handle queue
    // so, this TODO is not really relevant, because it may cause some
    // security problems
    if (server_model_.IsInSomeRoom(user_id)) {
      auto room = server_model_.GetRoomByUserId(user_id);
      room->DeleteUser(user_id);
      if (room->IsEmpty()) {
        server_model_.DeleteRoom(room->GetId());
      }
    }
    server_model_.DeleteUser(user_id);
  }
}

QString ServerController::GetControllerName() const {
  return "Server";
}

void ServerController::OnTick() {
  auto* rooms_list = new server_events::RoomsList;
  for (const auto& [room_id, room_ptr] : server_model_.GetRooms()) {
    rooms_list->add_ids(room_id);
  }
  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_allocated_rooms_list(rooms_list);
  events::EventWrapper event_wrapper;
  event_wrapper.set_allocated_server_event(server_event);
  AddEventToSend(event_wrapper);
}

void ServerController::Send(const events::EventWrapper& event) {
  switch (event.type_case()) {
    case events::EventWrapper::kServerEvent: {
      const auto& users = server_model_.GetUsers();
      if (!users.empty()) {
        LogEvent(event, logging::Type::kSend);
      }
      for (const auto& [user_id, user_ptr] : users) {
        auto serialized = event.SerializeAsString();
        QByteArray byte_array(serialized.data(), serialized.size());
        user_ptr->GetSocket()->sendBinaryMessage(byte_array);
      }
      break;
    }
    case events::EventWrapper::kClientEvent: {
      LogEvent(event, logging::Type::kSend);
      switch (event.client_event().receiver_case()) {
        case client_events::ClientEventWrapper::kEventToRoom: {
          SendEventToRoom(event);
          break;
        }
        default: {}
      }
    }
    default: {}
  }
}

void ServerController::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);
  const auto& client_event = event.client_event();
  UserId user_id = client_event.sender_id();
  const auto& event_to_server = client_event.event_to_server();
  auto user = server_model_.GetUserById(user_id);
  switch (event_to_server.type_case()) {
    case client_events::EventToServer::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      server_model_.AddRoom(
          std::make_shared<RoomController>(new_room_id, user));
      server_model_.AddUserToRoom(user_id, new_room_id);
      break;
    }
    case client_events::EventToServer::kEnterRoom: {
      auto room_id = event_to_server.enter_room().room_id();
      if (server_model_.ExistsRoom(room_id) &&
          !server_model_.IsInSomeRoom(user_id)) {
        server_model_.AddUserToRoom(user_id, room_id);
      }
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

void ServerController::SendEventToRoom(
    const events::EventWrapper& event) const {
  server_model_.GetRoomByUserId(
      event.client_event().sender_id())->AddEventToHandle(event);
}
