#include "server_controller.h"

ServerController::ServerController()
  :  web_socket_server_("", QWebSocketServer::NonSecureMode) {
  if (web_socket_server_.listen(QHostAddress::Any, 1337)) {
    connect(&web_socket_server_,
            &QWebSocketServer::newConnection,
            this,
            &ServerController::OnSocketConnect);
  }
  StartTicking();
}

void ServerController::OnByteArrayReceived(const QByteArray& message) {
  auto got_event = Event(message);
  qInfo() << "Got event" << got_event;

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket).lock();
  UserId user_id = user->GetId();

  switch (got_event.type) {
    case Event::Type::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      server_model_.AddRoom(
          std::make_shared<RoomController>(new_room_id, user));
      break;
    }
    case Event::Type::kEnterRoom: {
      if (server_model_.ExistsRoom(got_event.argument) &&
          !server_model_.IsInSomeRoom(user_id)) {
        server_model_.AddUserToRoom(user_id, got_event.argument);
      }
      break;
    }
    case Event::Type::kLeaveRoom: {
      if (server_model_.IsInSomeRoom(user_id)) {
        RoomId room_id = server_model_.GetRoomIdByUserId(user_id);
        server_model_.DeleteUserFromRoom(user_id, room_id);
      }
      break;
    }
    default: {}
  }
}

void ServerController::OnSocketConnect() {
  std::shared_ptr<QWebSocket> current_socket(
      web_socket_server_.nextPendingConnection());
  qInfo() << "Socket connected:" << current_socket.get();
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
  qInfo() << "Socket disconnected:" << web_socket;
  if (web_socket) {
    UserId user_id = server_model_.GetUserBySocket(web_socket).lock()->GetId();
    server_model_.DeleteUser(user_id);
  }
}

QString ServerController::GetControllerName() const {
  return "Server";
}

void ServerController::OnTick() {

}

void ServerController::Send(const Event& event) {

}
