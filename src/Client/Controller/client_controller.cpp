#include "client_controller.h"

#include <QDebug>

ClientController::ClientController(const QUrl& url) : server_url_(url) {
  qInfo() << "Connecting to" << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  socket_.open(url);
}

void ClientController::OnConnect() {
  qInfo() << "Connected to" << server_url_;
}

void ClientController::OnDisconnect() {
  qInfo() << "Disconnected from" << server_url_;
}
