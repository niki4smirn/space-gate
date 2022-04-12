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
  events::Wrapper received_event;
  received_event.ParseFromArray(message.data(), message.size());

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket);
  UserId user_id = user->GetId();
  received_event.set_sender_id(user_id);

  LogEvent(received_event, log::Type::kReceive);

  // TODO(niki4smirn): try to make this check prettier
  if (received_event.has_create_room() ||
      received_event.has_enter_room() ||
      received_event.has_leave_room()) {
    AddEventToHandle(received_event);
  }

  if (received_event.has_change_waiting_status()) {
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

void ServerController::Send(const events::Wrapper& event) {
  LogEvent(event, log::Type::kSend);

  // TODO(niki4smirn): try to make this check prettier
  if (event.has_change_waiting_status()) {
    SendEventToRoom(event);
  }
}

void ServerController::Handle(const events::Wrapper& event) {
  LogEvent(event, log::Type::kHandle);
  UserId user_id = event.sender_id();
  auto user = server_model_.GetUserById(user_id);
  if (event.has_create_room()) {
    RoomId new_room_id = server_model_.GetUnusedRoomId();
    server_model_.AddRoom(
        std::make_shared<RoomController>(new_room_id, user));
    server_model_.AddUserToRoom(user_id, new_room_id);
    auto room = server_model_.GetRoomById(new_room_id);
    room->AddUser(user);
  } else if (event.has_enter_room()) {
    auto room_id = event.enter_room().room_id();
    if (server_model_.ExistsRoom(room_id) &&
        !server_model_.IsInSomeRoom(user_id)) {
      server_model_.AddUserToRoom(user_id, room_id);
    }
  } else if (event.has_leave_room()) {
    if (server_model_.IsInSomeRoom(user_id)) {
      server_model_.DeleteUserFromRoom(user_id);
      auto room = server_model_.GetRoomByUserId(user_id);
      room->DeleteUser(user_id);
      if (room->IsEmpty()) {
        server_model_.DeleteRoom(room->GetId());
      }
    }
  }
}

void ServerController::SendEventToRoom(const events::Wrapper& event) const {
  server_model_.GetRoomByUserId(event.sender_id())->AddEventToHandle(event);
}
