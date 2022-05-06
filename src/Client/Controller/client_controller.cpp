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
}
void ClientController::SendReadyStatus() {
  events::EventWrapper ready_event;
  client_events::EventToRoom* event_to_room;
  client_events::ClientEventWrapper* event_wrapper;
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
