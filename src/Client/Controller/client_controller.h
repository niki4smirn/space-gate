#pragma once

#include <QObject>
#include <QUrl>
#include <QWebSocket>

#include "src/AbstractController/abstract_controller.h"
#include "src/Client/InputController/input_controller.h"
#include "src/Client/View/client_view.h"

class ClientController : public AbstractController {
 public:
  explicit ClientController(const QUrl& url);
  ~ClientController() override = default;

  QString GetControllerName() const override;

  void OnByteArrayReceived(const QByteArray& message);
  void OnConnect();
  void OnDisconnect();

 private:
  void SendReadyStatus();
  void SendCreateRoomEvent();
  void SendLeaveRoomEvent();
  void SendJoinRoomEvent(RoomId room_id);
  void SendStartGameEvent();
  void SendKeyEvent(input::Name key);
  void SendMouseMoveEvent(const QPoint& pos);
  void SendJoinMinigame(int minigame_index);
  void SendLeaveMinigame();
  void HoleRepairMousePos(QPointF pos);
  void HoleRepairPlatePos(QPointF pos);

  void ConnectToServer();

 protected:
  void OnTick() override;
  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;
  void ConnectView();

 private:
  int MinigameIdByPos(int minigame_pos);
  std::optional<int> MinigamePosById(int minigame_id);

  QUrl server_url_;
  QWebSocket socket_;
  ClientView* view_;

  std::unordered_map<int, int> minigame_index_to_pos;
  std::unordered_map<int, int> minigame_pos_to_index;
};
