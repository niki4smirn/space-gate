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

  client_events::ClientEventWrapper* client_event =
      received_event.mutable_client_event();

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket);
  UserId user_id = user->GetId();
  client_event->set_sender_id(user_id);

  LogEvent(received_event, log::Type::kReceive);

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

void ServerController::OnTick() {}

void ServerController::Send(const events::EventWrapper& event) {
  LogEvent(event, log::Type::kSend);
  const client_events::ClientEventWrapper& client_event = event.client_event();

  switch (client_event.receiver_case()) {
    case client_events::ClientEventWrapper::kEventToRoom: {
      SendEventToRoom(event);
      break;
    }
    default: {}
  }
}

void ServerController::Handle(const events::EventWrapper& event) {
  LogEvent(event, log::Type::kHandle);
  const client_events::ClientEventWrapper& client_event = event.client_event();
  UserId user_id = client_event.sender_id();
  const client_events::EventToServer& event_to_server =
      client_event.event_to_server();
  auto user = server_model_.GetUserById(user_id);
  switch (event_to_server.type_case()) {
    case client_events::EventToServer::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      server_model_.AddRoom(
          std::make_shared<RoomController>(new_room_id, user));
      server_model_.AddUserToRoom(user_id, new_room_id);
      auto room = server_model_.GetRoomById(new_room_id);
      room->AddUser(user);
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
        auto room = server_model_.GetRoomByUserId(user_id);
        room->DeleteUser(user_id);
        if (room->IsEmpty()) {
          server_model_.DeleteRoom(room->GetId());
        }
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
