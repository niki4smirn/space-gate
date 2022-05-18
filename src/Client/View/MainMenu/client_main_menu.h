#pragma once

#include "src/Client/View/MainMenu/Background/background_widget.h"
#include "src/Server/Models/RoomModel/room_model.h"
#include "Protobuf/client_events.pb.h"
#include "Protobuf/server_events.pb.h"

#include <QGridLayout>
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
  explicit ClientMainMenu(QWidget* parent = nullptr);
  void UpdateRoomsList(const server_events::RoomsList& room_list);
  void UpdatePlayerList(const server_events::RoomInfo& room_info);
  void SetCenterPos(QPoint pos);
  void PlayStartEffect();

 private:
  void RemoveAllWidgets();
  void Connect();
  void ButtonsConfigure();
  void SetStartWidgetsPos();
  void SetLayouts();
  void SetMouseTracking();

  void ChooseRoomOption();
  void BackToStart();
  void BackToGameOption();
  void CreateRoom();
  void JoinRoom();
  void Settings();
  void ReadyButtonPressEvent();

 private:
  BackgroundWidget* background_;
  QGridLayout* interface_layout_;
  QGridLayout* background_layout_;
  QListWidget* player_list_;
  QListWidget* rooms_list_;
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

  static QColor StatusToColor(server_events::RoomUser::Status status);

 signals:
  void StartEffect(bool state);
  void Close();
  void ReadyButtonPressed();
  void CreateRoomSignal();
  void LeaveRoom();
  void JoinRoomSignal(RoomId room_id);
  void StartGame();
};

