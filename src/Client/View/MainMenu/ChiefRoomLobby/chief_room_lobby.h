#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

#include "Resources/Fonts/fonts.h"
#include "Protobuf/server_events.pb.h"

class ChiefRoomLobby : public QWidget {
 Q_OBJECT

 public:
  ChiefRoomLobby(QWidget* parent = nullptr);
  void UpdatePlayerList(const server_events::RoomsList& room_info);

 private:
  void Connect();
  void InterfaceConfigure();
  void SetLayout();

 private:
  QGridLayout* interface_layout_;
  QPushButton* back_button_;
  QPushButton* start_button_;
  QPushButton* ready_button_;
  QListWidget* player_list_;
  QLabel* game_name_;

 signals:
  void Start();
  void Ready();
  void Back();
};
