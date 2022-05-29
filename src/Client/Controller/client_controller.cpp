#include "client_controller.h"

ClientController::ClientController(const QUrl& url) :
    server_url_(url),
    view_(new ClientView) {
  LOG << "Connecting to " << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  connect(&socket_, &QWebSocket::binaryMessageReceived, this,
          &ClientController::OnByteArrayReceived);
  socket_.open(url);
  StartTicking();
  ConnectView();
}

void ClientController::OnConnect() {
  LOG << "Connected to " << server_url_;
}

void ClientController::OnDisconnect() {
  LOG << "Disconnected from " << server_url_;
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {
}

void ClientController::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);
  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  socket_.sendBinaryMessage(byte_array);
}

void ClientController::Handle(const events::EventWrapper& event) {
  switch (event.type_case()) {
    case events::EventWrapper::kServerEvent: {
      const auto& server_event = event.server_event();
      switch (server_event.type_case()) {
        case server_events::ServerEventWrapper::kRoomInfo: {
          view_->UpdateRoomInfoMenu(server_event.room_info(),
                                    server_event.receiver_id());
          break;
        }
        case server_events::ServerEventWrapper::kRoomsList: {
          view_->UpdateRoomsListMenu(server_event.rooms_list());
          break;
        }
        case server_events::ServerEventWrapper::kStartGame: {
          view_->PlayStartEffect();
        }
        default: {}
      }
      break;
    }
    default: {}
  }
}

void ClientController::OnByteArrayReceived(const QByteArray& message) {
  events::EventWrapper received_event;
  if (!received_event.ParseFromArray(message.data(), message.size())) {
    LOG << GetControllerName() << " failed to parse message";
    return;
  }
  AddEventToHandle(received_event);
  LogEvent(received_event, logging::Type::kReceive);
}

void ClientController::ConnectView() {
  connect(view_,
          &ClientView::ReadyButtonPressed,
          this,
          &ClientController::SendReadyStatus);
  connect(view_,
          &ClientView::CreateRoom,
          this,
          &ClientController::SendCreateRoomEvent);
  connect(view_,
          &ClientView::LeaveRoom,
          this,
          &ClientController::SendLeaveRoomEvent);
  connect(view_,
          &ClientView::JoinRoom,
          this,
          &ClientController::SendJoinRoomEvent);
  connect(view_,
          &ClientView::StartGame,
          this,
          &ClientController::SendStartGameEvent);
  connect(view_,
          &ClientView::KeyEventToServer,
          this,
          &ClientController::SendKeyEvent);
  connect(view_,
          &ClientView::MouseMoveToServer,
          this,
          &ClientController::SendMouseMoveEvent);
}

void ClientController::SendReadyStatus() {
  auto* wait_status_event = new client_events::ChangeWaitingStatus;
  auto* event_to_room = new client_events::EventToRoom;
  event_to_room->set_allocated_change_waiting_status(wait_status_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_room(event_to_room);

  events::EventWrapper ready_event;
  ready_event.set_allocated_client_event(event_wrapper);
  AddEventToSend(ready_event);
}

void ClientController::SendCreateRoomEvent() {
  auto* create_room_event = new client_events::CreateRoom;
  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_create_room(create_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendLeaveRoomEvent() {
  auto* leave_room_event = new client_events::LeaveRoom;
  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_leave_room(leave_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendJoinRoomEvent(RoomId room_id) {
  auto* join_room_event = new client_events::EnterRoom;
  join_room_event->set_room_id(room_id);

  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_enter_room(join_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendStartGameEvent() {
  auto* start_game_event = new client_events::StartGame;
  auto* event_to_room = new client_events::EventToRoom;
  event_to_room->set_allocated_start_game(start_game_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_room(event_to_room);

  events::EventWrapper start_event;
  start_event.set_allocated_client_event(event_wrapper);
  AddEventToSend(start_event);
}

void ClientController::SendKeyEvent(input::Name key) {
  LOG << input::InputNameToString(key);
}

void ClientController::SendMouseMoveEvent(const QPoint& pos) {
  LOG << "x = " << pos.x() << " y = " << pos.y();
}
