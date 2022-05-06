#pragma once

#include "background_widget.h"
#include "Protobuf/client_events.pb.h"
#include "Protobuf/server_events.pb.h"

#include <string>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QListWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>

class ClientMainMenu : public QWidget {
  Q_OBJECT

 public:
  ClientMainMenu();
  void UpdateRoomList();
  void UpdatePlayerList(const server_events::RoomInfo& room_info);


 private:
  void RemoveAllWidgets();
  void Connect();
  void ButtonsConfigure();
  void SetStartWidgetsPos();
  void SetLayots();

 private Q_SLOTS:
  void StartGame();
  void ChooseRoomOption();
  void BackToStart();
  void BackToGameOption();
  void CreateRoom();
  void JoinRoom();
  void Settings();

 private:
  BackgroundWidget* background_;
  QGridLayout* interface_layout_;
  QGridLayout* background_layout_;
  QListWidget* player_list_;
  QListWidget* room_list_;
  QPushButton* settings_;
  QPushButton* exit_;
  QPushButton* back_to_start_;
  QPushButton* back_to_game_option_;
  QPushButton* play_;
  QPushButton* start_game_;
  QPushButton* create_room_;
  QPushButton* join_room_;
  QPushButton* ready_status_;
  QLabel* game_name_;
  QLabel* nothing_here_;
  QWidget* interface_;
  QFont font_;

 signals:
  void StartEffect(bool state);
  void Close();
  void ReadyButtonPressed();
};

