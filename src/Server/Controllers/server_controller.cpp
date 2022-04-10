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
  proto::Event received_event;
  received_event.ParseFromArray(message.data(), message.size());

  auto message_socket = qobject_cast<QWebSocket*>(sender());
  auto user = server_model_.GetUserBySocket(message_socket);
  UserId user_id = user->GetId();
  received_event.set_sender_id(user_id);

  LogEvent(received_event, log::Type::kReceive);

  switch (received_event.receiver_type()) {
    case proto::Event::kServer: {
      AddEventToHandle(received_event);
      break;
    }
    case proto::Event::kRoom: {
      AddEventToSend(received_event);
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

void ServerController::Send(const proto::Event& event) {
  LogEvent(event, log::Type::kSend);
  switch (event.receiver_type()) {
    case proto::Event::kRoom: {
      SendEventToRoom(event);
      break;
    }
    default: {}
  }
}

void ServerController::Handle(const proto::Event& event) {
  LogEvent(event, log::Type::kHandle);
  UserId user_id = event.sender_id();
  auto user = server_model_.GetUserById(user_id);
  switch (event.type()) {
    case proto::Event::kCreateRoom: {
      RoomId new_room_id = server_model_.GetUnusedRoomId();
      server_model_.AddRoom(
          std::make_shared<RoomController>(new_room_id, user));
      server_model_.AddUserToRoom(user_id, new_room_id);
      auto room = server_model_.GetRoomById(new_room_id);
      room->AddUser(user);
      break;
    }
    case proto::Event::kEnterRoom: {
      auto room_id = event.arguments(0);
      if (server_model_.ExistsRoom(room_id) &&
          !server_model_.IsInSomeRoom(user_id)) {
        server_model_.AddUserToRoom(user_id, room_id);
      }
      break;
    }
    case proto::Event::kLeaveRoom: {
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

void ServerController::SendEventToRoom(const proto::Event& event) const {
  server_model_.GetRoomByUserId(event.sender_id())->AddEventToHandle(event);
}
