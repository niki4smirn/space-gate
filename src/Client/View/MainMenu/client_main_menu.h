#pragma once

#include "src/Client/View/MainMenu/Background/background_widget.h"
#include "src/Client/View/MainMenu/ChiefRoomLobby/chief_room_lobby.h"
#include "src/Client/View/MainMenu/GuestRoomLobby/guest_room_lobby.h"
#include "src/Client/View/MainMenu/MainPage/main_page.h"
#include "src/Client/View/MainMenu/RoomList/room_list.h"
#include "src/Client/View/MainMenu/SettingsMenu/settings_menu.h"
#include "src/Server/Models/RoomModel/room_model.h"
#include "Protobuf/client_events.pb.h"
#include "Protobuf/server_events.pb.h"

#include <QGridLayout>
#include <QWidget>
#include <QStackedWidget>

class ClientMainMenu : public QWidget {
  Q_OBJECT

 public:
  explicit ClientMainMenu(QWidget* parent = nullptr);
  void UpdateRoomsList(const server_events::RoomsList& room_list);
  void UpdatePlayersList(const server_events::RoomInfo& room_info);
  void SetCenterPos(QPoint pos);
  void PlayStartEffect();
  void UpdateInterface(bool new_is_chief);
  void BackToStart();
  void BackToLobby();

 private:
  void RemoveAllWidgets();
  void ResetAllWidgets();
  void Connect();
  void ButtonsConfigure();
  void ShowStartWidget();
  void SetLayouts();
  void SetMouseTracking();

  void ChooseRoomOption();
  void BackToGameOption();
  void CreateRoom();
  void JoinRoom();
  void Settings();
  void ReadyButtonPressEvent();

 private:
  BackgroundWidget* background_;
  QGridLayout* background_layout_;
  QLabel* game_name_;
  QLabel* nothing_here_;
  QWidget* interface_;
  QFont font_;

  bool is_chief_{true};
  static QColor StatusToColor(server_events::RoomUser::Status status);

  void ShowRoomChiefInterface();
  void ShowRoomGuestInterface();

 signals:
  void StartEffect(bool state);
  void Close();
  void ReadyButtonPressed();
  void CreateRoomSignal();
  void LeaveRoom();
  void JoinRoomSignal(RoomId room_id);
  void StartGame();
};

