#include "client_controller.h"

ClientController::ClientController(const QUrl& url) : server_url_(url) {
  qInfo().noquote() << "Connecting to" << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  socket_.open(url);
  StartTicking();
}

void ClientController::OnConnect() {
  qInfo().noquote() << "Connected to" << server_url_;
  // proto::Event create_room;
  // create_room.set_type(proto::Event::kCreateRoom);
  // create_room.set_receiver_type(proto::Event::kServer);
  // Send(create_room);
  // proto::Event change_waiting_status;
  // change_waiting_status.set_type(proto::Event::kChangeWaitingStatus);
  // change_waiting_status.set_receiver_type(proto::Event::kRoom);
  // Send(change_waiting_status);
}

void ClientController::OnDisconnect() {
  qInfo().noquote() << "Disconnected from" << server_url_;
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {
  for (const auto& event : events_to_send_) {
    socket_.sendBinaryMessage(event.SerializeAsString().data());
  }
  events_to_send_.clear();
}

void ClientController::Send(const proto::Event& event) {
  LogSending(event);
  events_to_send_.push_back(event);
}

void ClientController::Handle(const proto::Event& event) {}
