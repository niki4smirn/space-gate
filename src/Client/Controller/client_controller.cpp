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
  ParseMessage(message);

  LogEvent(received_event, game_log::Type::kReceive);
}
void ClientController::Connect() {
  connect(view_, &ClientView::ReadyButtonPressed, this, &ClientController::SendReadyStatus);
  connect(view_, &ClientView::CreateRoom, this, &ClientController::SendCreateRoomEvent);
}
void ClientController::SendReadyStatus() {
  events::EventWrapper ready_event;
  auto* event_to_room = new client_events::EventToRoom;
  auto* event_wrapper = new client_events::ClientEventWrapper;
  auto* wait_status_event = new client_events::ChangeWaitingStatus;
  event_to_room->set_allocated_change_waiting_status(wait_status_event);
  event_wrapper->set_allocated_event_to_room(event_to_room);
  ready_event.set_allocated_client_event(event_wrapper);
  Send(ready_event);
}

void ClientController::ParseMessage(const QByteArray& message) {
  server_events::ServerEventWrapper data;
  data.ParseFromArray(message, message.size());
  switch (data.type_case()) {
    case server_events::ServerEventWrapper::kRoomInfo:{
      view_->MenuUpdatePlayerList(data.room_info());
      break;
    }
    default: {}
  }
}

void ClientController::SendCreateRoomEvent() {
  events::EventWrapper create_room_event;
  auto* event_to_server = new client_events::EventToServer;
  auto* event_wrapper = new client_events::ClientEventWrapper;
  auto* create_event = new client_events::CreateRoom;
  event_to_server->set_allocated_create_room(create_event);
  event_wrapper->set_allocated_event_to_server(event_to_server);
  create_room_event.set_allocated_client_event(event_wrapper);
  Send(create_room_event);
}
