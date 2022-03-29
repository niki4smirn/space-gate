#pragma once

#include <set>

#include <QObject>
#include <QWebSocketServer>

#include "../Models/ServerModel/server_model.h"

class ServerController : public QObject {
 public:
  ServerController();
  ~ServerController() override = default;

  public Q_SLOTS:
  void OnByteArrayReceived(const QByteArray& message);
  void OnSocketConnect();
  void OnSocketDisconnect();

 private:
  QWebSocketServer web_socket_server_;
  ServerModel server_model_;
};
