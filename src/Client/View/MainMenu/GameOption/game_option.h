#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

#include "Resources/Fonts/fonts.h"
#include "Protobuf/server_events.pb.h"

class GameOption : public QWidget {
 Q_OBJECT

 public:
  GameOption(QWidget* parent = nullptr);
  void UpdateRoomList(const server_events::RoomsList& room_list);

 private:
  void Connect();
  void InterfaceConfigure();
  void SetLayout();

 private:
  QGridLayout* interface_layout_;
  QPushButton* back_button_;
  QPushButton* create_room_button_;
  QPushButton* join_room_button_;
  QListWidget* rooms_list_;
  QLabel* game_name_;

 signals:
  void CreateRoom();
  void JoinRoom();
  void Back();
};
