#include "client_controller.h"

#include <iostream>

#include <QDebug>

ClientController::ClientController(const QUrl& url) : server_url_(url) {
  qInfo() << "Connecting to" << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  socket_.open(url);
  StartTicking();
}

void ClientController::OnConnect() {
  qInfo() << "Connected to" << server_url_;
}

void ClientController::OnDisconnect() {
  qInfo() << "Disconnected from" << server_url_;
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {
  for (const auto& event : events_to_send_) {
    socket_.sendBinaryMessage(event.ToByteArray());
  }
  events_to_send_.clear();
}

void ClientController::Send(const Event& event) {
  LogSending(event);
  events_to_send_.push_back(event);
}
