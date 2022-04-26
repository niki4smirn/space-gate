#pragma once

#include <set>

#include <QObject>
#include <QWebSocketServer>

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/ServerModel/server_model.h"

class ServerController : public AbstractController {
 public:
  ServerController();
  ~ServerController() override = default;

  QString GetControllerName() const override;

  public Q_SLOTS:
  void OnByteArrayReceived(const QByteArray& message);
  void OnSocketConnect();
  void OnSocketDisconnect();

 protected:
  void OnTick() override;
  void Send(const proto::Event& event) override;
  void Handle(const proto::Event& event) override;

 private:
  void SendEventToRoom(const proto::Event& event) const;

  QWebSocketServer web_socket_server_;
  ServerModel server_model_;
};