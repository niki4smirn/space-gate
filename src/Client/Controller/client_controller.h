#pragma once

#include <QObject>
#include <QUrl>
#include <QWebSocket>

#include "src/AbstractController/abstract_controller.h"
#include "src/Client/View/client_view.h"

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
  void SendJoinRoomEvent(RoomId room_id);
  void SendKeyEvent(const std::string& key);
  void SendMouseMoveEvent(const QPoint& pos);

 protected:
  void OnTick() override;
  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;
  void ConnectView();

 private:
  QUrl server_url_;
  QWebSocket socket_;
  ClientView* view_;
};
