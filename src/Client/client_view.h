#pragma once

#include "client_main_menu.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>

class ClientView : public QMainWindow {
  Q_OBJECT
 public:
  ClientView();
  void MenuUpdatePlayerList(const server_events::RoomInfo& room_info);
  void MenuUpdateRoomList(const server_events::RoomsList& room_list);

 private:
  void CloseWindow();
  void AddWidgets();
  void Connect();

 private:
  ClientMainMenu* main_menu_;
  QStackedWidget* stacked_widget_;

  signals:
  void ReadyButtonPressed();
  void CreateRoom();
  void LeaveRoom();
  void JoinRoom(uint64_t room_id);
};
