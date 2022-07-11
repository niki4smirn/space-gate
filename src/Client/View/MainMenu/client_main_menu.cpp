#include "client_main_menu.h"

ClientMainMenu::ClientMainMenu(QWidget* parent) :
    QWidget(parent),
    stacked_widget_(new QStackedWidget(this)),
    chief_room_lobby_(new ChiefRoomLobby(this)),
    guest_room_lobby_(new GuestRoomLobby(this)),
    main_page_(new MainPage(this)),
    rooms_list_(new RoomsList(this)),
    settings_menu_(new SettingsMenu(this)),
    light_effect_(new LightEffect(this)),
    background_(new BackgroundWidget(this)),
    background_layout_(new QGridLayout) {

  setMouseTracking(true);
  stacked_widget_->setMouseTracking(true);

  SetLayouts();
  AddStackedWidgets();
  Connect();
}

void ClientMainMenu::SetCenterPos(QPoint pos) {
  background_->SetCenterPos(pos);
}

void ClientMainMenu::SetLayouts() {
  background_layout_->addWidget(background_, 0, 0);
  background_layout_->setContentsMargins(0, 0, 0, 0);
  background_layout_->addWidget(stacked_widget_, 0, 0);
  setLayout(background_layout_);
}

void ClientMainMenu::Connect() {
  ConnectChiefRoomLobby();
  ConnectGuestRoomLobby();
  ConnectMainPage();
  ConnectRoomList();
  ConnectSettingsMenu();
}

void ClientMainMenu::CreateRoom() {
  stacked_widget_->setCurrentWidget(chief_room_lobby_);
  emit CreateRoomSignal();
}

void ClientMainMenu::JoinRoom() {
  if (rooms_list_->GetRow() == -1) {
    return;
  }
  RoomId room_id = rooms_list_->GetItem()->text().toInt();
  emit JoinRoomSignal(room_id);

  stacked_widget_->setCurrentWidget(guest_room_lobby_);
}

void ClientMainMenu::UpdateRoomsList(
    const server_events::RoomsList& room_list) {
  rooms_list_->UpdateRoomList(room_list);
}

void ClientMainMenu::UpdatePlayersList(
    const server_events::RoomInfo& room_info) {
 if (is_chief_) {
   chief_room_lobby_->UpdatePlayerList(room_info);
 }
 guest_room_lobby_->UpdatePlayerList(room_info);
}

void ClientMainMenu::PlayStartEffect() {
  stacked_widget_->setCurrentWidget(light_effect_);
  background_->SetLightEffect(true);
}

void ClientMainMenu::UpdateInterface(bool new_is_chief) {
  if (is_chief_ == new_is_chief) {
    return;
  }
  if (new_is_chief) {
    stacked_widget_->setCurrentWidget(chief_room_lobby_);
  } else {
    stacked_widget_->setCurrentWidget(guest_room_lobby_);
  }
}

void ClientMainMenu::ConnectChiefRoomLobby() {
  connect(chief_room_lobby_,
          &ChiefRoomLobby::Start,
          [this]() {emit StartGame();});
  connect(chief_room_lobby_,
          &ChiefRoomLobby::Ready,
          [this]() { emit ReadyButtonPressed();});
  connect(chief_room_lobby_,
          &ChiefRoomLobby::Back,
          [this]() { stacked_widget_->setCurrentWidget(rooms_list_);});
}

void ClientMainMenu::ConnectGuestRoomLobby() {
  connect(guest_room_lobby_,
          &GuestRoomLobby::Ready,
          [this]() { emit ReadyButtonPressed();});
  connect(guest_room_lobby_,
          &GuestRoomLobby::Back,
          [this]() { stacked_widget_->setCurrentWidget(rooms_list_);});
}

void ClientMainMenu::ConnectMainPage() {
  connect(main_page_,
          &MainPage::Play,
          [this]() {stacked_widget_->setCurrentWidget(rooms_list_);});
  connect(main_page_,
          &MainPage::Settings,
          [this]() {stacked_widget_->setCurrentWidget(settings_menu_);});
  connect(main_page_,
          &MainPage::CloseGame,
          [this]() { emit Close(); });
}

void ClientMainMenu::ConnectRoomList() {
  connect(rooms_list_,
          &RoomsList::CreateRoom, this, &ClientMainMenu::JoinRoom);
  connect(rooms_list_,
          &RoomsList::JoinRoom, this, &ClientMainMenu::CreateRoom);
  connect(rooms_list_,
          &RoomsList::Back,
          [this]() { stacked_widget_->setCurrentWidget(main_page_);});
}

void ClientMainMenu::ConnectSettingsMenu() {
  connect(settings_menu_,
          &SettingsMenu::Back,
          [this]() { stacked_widget_->setCurrentWidget(main_page_);});
}

void ClientMainMenu::AddStackedWidgets() {
  stacked_widget_->addWidget(chief_room_lobby_);
  stacked_widget_->addWidget(guest_room_lobby_);
  stacked_widget_->addWidget(main_page_);
  stacked_widget_->addWidget(rooms_list_);
  stacked_widget_->addWidget(settings_menu_);
}

void ClientMainMenu::BackToLobby() {
  background_->SetLightEffect(false);
  if (is_chief_) {
    stacked_widget_->setCurrentWidget(chief_room_lobby_);
  } else {
    stacked_widget_->setCurrentWidget(guest_room_lobby_);
  }
}

void ClientMainMenu::BackToMainPage() {
  stacked_widget_->setCurrentWidget(main_page_);
}
