#pragma once

#include <set>

#include <QObject>
#include <QWebSocketServer>

#include "src/AbstractController/abstract_controller.h"
#include "../Models/ServerModel/server_model.h"

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
  QWebSocketServer web_socket_server_;
  ServerModel server_model_;
};
