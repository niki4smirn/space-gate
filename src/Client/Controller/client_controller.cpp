#include "client_controller.h"

ClientController::ClientController(const QUrl& url) :
  server_url_(url),
  view_(new ClientView){
  qInfo().noquote() << "Connecting to" << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  connect(&socket_, &QWebSocket::binaryMessageReceived, this,
          &ClientController::OnByteArrayReceived);
  socket_.open(url);
  StartTicking();
  Connect();
}

void ClientController::OnConnect() {
  qInfo().noquote() << "Connected to" << server_url_;
}

void ClientController::OnDisconnect() {
  qInfo().noquote() << "Disconnected from" << server_url_;
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {

}

void ClientController::Send(const events::EventWrapper& event) {
  LogEvent(event, game_log::Type::kSend);
  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  socket_.sendBinaryMessage(byte_array);
}

void ClientController::Handle(const events::EventWrapper& event) {}

void ClientController::OnByteArrayReceived(const QByteArray& message) {
  events::EventWrapper received_event;
  if (!received_event.ParseFromArray(message.data(), message.size())) {
    // fail
    return;
  }
  ParseMessage(received_event);

  LogEvent(received_event, game_log::Type::kReceive);
}
void ClientController::Connect() {
  connect(view_, &ClientView::ReadyButtonPressed, this, &ClientController::SendReadyStatus);
  connect(view_, &ClientView::CreateRoom, this, &ClientController::SendCreateRoomEvent);
  connect(view_, &ClientView::LeaveRoom, this, &ClientController::SendLeaveRoomEvent);
  connect(view_, &ClientView::JoinRoom, this, &ClientController::SendJoinRoomEvent);
}
void ClientController::SendReadyStatus() {
  events::EventWrapper ready_event;
  auto* event_to_room = new client_events::EventToRoom;
  auto* event_wrapper = new client_events::ClientEventWrapper;
  auto* wait_status_event = new client_events::ChangeWaitingStatus;
  event_to_room->set_allocated_change_waiting_status(wait_status_event);
  event_wrapper->set_allocated_event_to_room(event_to_room);
  ready_event.set_allocated_client_event(event_wrapper);
  AddEventToSend(ready_event);
}

void ClientController::ParseMessage(const events::EventWrapper& data) {
  if (data.type_case() == events::EventWrapper::kServerEvent) {
    switch (data.server_event().type_case()) {
      case server_events::ServerEventWrapper::kRoomInfo: {
        view_->MenuUpdatePlayerList(data.server_event().room_info());
        break;
      }
      default: {}
    }
  }
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

void ClientController::SendJoinRoomEvent(uint64_t room_id) {
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
