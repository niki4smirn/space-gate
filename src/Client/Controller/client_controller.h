#pragma once

#include <QObject>
#include <QUrl>
#include <QWebSocket>

#include "src/AbstractController/abstract_controller.h"
#include "src/Client/client_view.h"

class ClientController : public AbstractController {
 public:
  explicit ClientController(const QUrl& url);
  ~ClientController() override = default;

  QString GetControllerName() const override;

 public Q_SLOTS:
  void OnByteArrayReceived(const QByteArray& message);
  void OnConnect();
  void OnDisconnect();

 private Q_SLOTS:
  void SendReadyStatus();
  void SendCreateRoomEvent();
  void SendLeaveRoomEvent();
  void SendJoinRoomEvent(uint64_t room_id);

 protected:
  void OnTick() override;
  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;
  void Connect();
  void ParseMessage(const events::EventWrapper& date);

 private:
  QUrl server_url_;
  QWebSocket socket_;
  ClientView* view_;
};
