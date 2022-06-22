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

  void OnByteArrayReceived(const QByteArray& message);
  void OnSocketConnect();
  void OnSocketDisconnect();

 protected:
  void OnTick() override;
  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

 private:
  void SendEventToRoom(const events::EventWrapper& event) const;
  void SendEveryUser(events::EventWrapper event) const;

  void SendRoomsListEvent();

  static events::EventWrapper GenerateDeleteUserEvent(UserId user_id) ;

  QWebSocketServer web_socket_server_;
  ServerModel server_model_;
};
