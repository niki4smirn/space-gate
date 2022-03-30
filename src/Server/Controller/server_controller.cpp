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
  proto::Event got_event;
  got_event.ParseFromArray(message.data(), message.size());

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket).lock();
  UserId user_id = user->GetId();
  got_event.set_sender_id(user_id);

  LogReceive(got_event);

  AddEventToHandle(got_event);
}

void ServerController::OnSocketConnect() {
  std::shared_ptr<QWebSocket> current_socket(
      web_socket_server_.nextPendingConnection());
  qInfo() << "Socket connected:" << current_socket.get();
  UserId new_user_id = server_model_.GetUnusedUserId();
  auto new_user = std::make_shared<User>(new_user_id,
                                         current_socket);
  // TODO(Everyone): replace with adding to handle queue
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
    // TODO(Everyone): replace with adding to handle queue
    server_model_.DeleteUser(user_id);
  }
}

QString ServerController::GetControllerName() const {
  return "Server";
}

void ServerController::OnTick() {}

void ServerController::Send(const proto::Event& event) {}

void ServerController::Handle(const proto::Event& event) {
  UserId user_id = event.sender_id();
  auto user = server_model_.GetUserById(user_id).lock();
  switch (event.type()) {
    case proto::EventType::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      server_model_.AddRoom(
          std::make_shared<RoomController>(new_room_id, user));
      break;
    }
    case proto::EventType::kEnterRoom: {
      if (server_model_.ExistsRoom(event.arguments(0)) &&
          !server_model_.IsInSomeRoom(user_id)) {
        server_model_.AddUserToRoom(user_id, event.arguments(0));
      }
      break;
    }
    case proto::EventType::kLeaveRoom: {
      if (server_model_.IsInSomeRoom(user_id)) {
        RoomId room_id = server_model_.GetRoomIdByUserId(user_id);
        server_model_.DeleteUserFromRoom(user_id, room_id);
      }
      break;
    }
    default: {}
  }
}
