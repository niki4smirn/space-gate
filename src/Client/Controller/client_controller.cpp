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
}

void ClientController::OnDisconnect() {
  qInfo().noquote() << "Disconnected from" << server_url_;
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {}

void ClientController::Send(const proto::Event& event) {
  LogEvent(event, log::Type::kSend);
  socket_.sendBinaryMessage(event.SerializeAsString().data());
}

void ClientController::Handle(const proto::Event& event) {}
