#pragma once

#include "src/Client/View/MainMenu/Background/background_widget.h"
#include "src/Client/View/MainMenu/ChiefRoomLobby/chief_room_lobby.h"
#include "src/Client/View/MainMenu/GuestRoomLobby/guest_room_lobby.h"
#include "src/Client/View/MainMenu/MainPage/main_page.h"
#include "src/Client/View/MainMenu/RoomsList/rooms_list.h"
#include "src/Client/View/MainMenu/SettingsMenu/settings_menu.h"
#include "src/Client/View/MainMenu/LightEffect/light_effect.h"
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
  void BackToMainPage();
  void BackToLobby();

 private:
  void ConnectChiefRoomLobby();
  void ConnectGuestRoomLobby();
  void ConnectMainPage();
  void ConnectRoomList();
  void ConnectSettingsMenu();
  void AddStackedWidgets();
  void Connect();
  void SetLayouts();

  void CreateRoom();
  void JoinRoom();

 private:
  QStackedWidget* stacked_widget_;
  QGridLayout* background_layout_;
  BackgroundWidget* background_;
  ChiefRoomLobby* chief_room_lobby_;
  GuestRoomLobby* guest_room_lobby_;
  LightEffect* light_effect_;
  MainPage* main_page_;
  RoomsList* rooms_list_;
  SettingsMenu* settings_menu_;

  bool is_chief_{true};

 signals:
  void Close();
  void ReadyButtonPressed();
  void CreateRoomSignal();
  void LeaveRoom();
  void JoinRoomSignal(RoomId room_id);
  void StartGame();
};

