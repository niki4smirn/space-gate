#pragma once

#include "src/Client/View/MainMenu/client_main_menu.h"
#include "src/Client/Controller/input_controller.h"
#include "src/Server/Models/RoomModel/room_model.h"

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
  void mouseMoveEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  InputController* input_controller_;
  ClientMainMenu* main_menu_;
  QStackedWidget* stacked_widget_;

 signals:
  void ReadyButtonPressed();
  void CreateRoom();
  void LeaveRoom();
  void JoinRoom(uint64_t room_id);
  void KeyEventToServer(std::string& key);
  void MouseMoveToServer(const QPoint& pos);
};
