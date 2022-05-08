#include "client_main_menu.h"

#include <QFont>
#include <QFontDatabase>

ClientMainMenu::ClientMainMenu(QWidget* parent) :
    QWidget(parent),
    background_(new BackgroundWidget(this)),
    background_layout_(new QGridLayout),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    play_(new QPushButton(this)),
    start_game_(new QPushButton(this)),
    settings_(new QPushButton(this)),
    exit_(new QPushButton(this)),
    create_room_(new QPushButton(this)),
    join_room_(new QPushButton(this)),
    back_to_game_option_(new QPushButton(this)),
    back_to_start_(new QPushButton(this)),
    ready_status_(new QPushButton(this)),
    player_list_(new QListWidget(this)),
    room_list_(new QListWidget(this)),
    nothing_here_(new QLabel(this)),
    interface_(new QWidget(this)) {
  QString family =
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
          "../Resources/Fonts/Paladins.otf")).at(0);
  font_ = QFont(family);

  ButtonsConfigure();

  game_name_->setStyleSheet("QLabel {color : #88bcff; }");
  font_.setPointSize(105);
  font_.setBold(true);
  game_name_->setFont(font_);

  SetLayouts();
  SetStartWidgetsPos();
  Connect();
}

void ClientMainMenu::SetStartWidgetsPos() {
  RemoveAllWidgets();

  game_name_->setVisible(true);
  play_->setVisible(true);
  settings_->setVisible(true);
  exit_->setVisible(true);

  interface_layout_->addWidget(game_name_, 0, 0, 1, 2, Qt::AlignHCenter);
  interface_layout_->addWidget(play_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_, 1, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_, 2, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void ClientMainMenu::SetLayouts() {
  background_layout_->addWidget(background_, 0, 0);
  background_layout_->setContentsMargins(0, 0, 0, 0);
  interface_layout_->setSpacing(50);
  interface_->setLayout(interface_layout_);
  background_layout_->addWidget(interface_, 0, 0);
  setLayout(background_layout_);
}

void ClientMainMenu::ButtonsConfigure() {
  play_->setFixedSize(350, 250);
  play_->setText("PLAY");
  play_->setFont(font_);
  play_->setStyleSheet("background-color: rgb(136, 247, 255);font-size: 40px;");

  settings_->setFixedSize(350, 250);
  settings_->setText("SETTINGS");
  settings_->setFont(font_);
  settings_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  exit_->setFixedSize(200, 75);
  exit_->setText("EXIT");
  exit_->setFont(font_);
  exit_->setStyleSheet("background-color: rgb(136, 247, 255);font-size: 40px;");

  create_room_->setFixedSize(250, 150);
  create_room_->setText("CREATE");
  create_room_->setFont(font_);
  create_room_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  join_room_->setFixedSize(250, 150);
  join_room_->setText("JOIN");
  join_room_->setFont(font_);
  join_room_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  back_to_start_->setFixedSize(200, 75);
  back_to_start_->setText("BACK");
  back_to_start_->setFont(font_);
  back_to_start_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  ready_status_->setFixedSize(250, 150);
  ready_status_->setText("READY");
  ready_status_->setFont(font_);
  ready_status_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 25px;");

  start_game_->setFixedSize(250, 150);
  start_game_->setText("START");
  start_game_->setFont(font_);
  start_game_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  back_to_game_option_->setFixedSize(200, 75);
  back_to_game_option_->setText("BACK");
  back_to_game_option_->setFont(font_);
  back_to_game_option_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");
}

void ClientMainMenu::Connect() {
  connect(play_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::ChooseRoomOption);
  connect(create_room_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::CreateRoom);
  connect(start_game_, &QPushButton::clicked, this, &ClientMainMenu::StartGame);
  connect(join_room_, &QPushButton::clicked, this, &ClientMainMenu::JoinRoom);
  connect(back_to_game_option_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::BackToGameOption);
  connect(exit_,
          &QPushButton::clicked,
          [this]() { emit Close(); });
  connect(back_to_start_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::BackToStart);
  connect(settings_, &QPushButton::clicked, this, &ClientMainMenu::Settings);
  connect(this,
          &ClientMainMenu::StartEffect,
          background_,
          &BackgroundWidget::SetLightEffect);
  connect(ready_status_,
          &QPushButton::clicked, this, &ClientMainMenu::ReadyButtonPressEvent);
}

void ClientMainMenu::StartGame() {
  RemoveAllWidgets();
  emit StartEffect(true);
}

void ClientMainMenu::ChooseRoomOption() {
  RemoveAllWidgets();

  create_room_->setVisible(true);
  join_room_->setVisible(true);
  back_to_start_->setVisible(true);
  room_list_->setVisible(true);

  interface_layout_->addWidget(room_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(create_room_,
                               2,
                               0,
                               Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(join_room_, 2, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_to_start_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void ClientMainMenu::BackToGameOption() {
  ChooseRoomOption();
  emit LeaveRoom();
}

void ClientMainMenu::BackToStart() {
  SetStartWidgetsPos();
}

void ClientMainMenu::RemoveAllWidgets() {
  play_->setVisible(false);
  settings_->setVisible(false);
  exit_->setVisible(false);
  create_room_->setVisible(false);
  join_room_->setVisible(false);
  back_to_start_->setVisible(false);
  player_list_->setVisible(false);
  start_game_->setVisible(false);
  back_to_game_option_->setVisible(false);
  ready_status_->setVisible(false);
  room_list_->setVisible(false);
  nothing_here_->setVisible(false);

  interface_layout_->removeWidget(create_room_);
  interface_layout_->removeWidget(join_room_);
  interface_layout_->removeWidget(back_to_start_);
  interface_layout_->removeWidget(play_);
  interface_layout_->removeWidget(settings_);
  interface_layout_->removeWidget(exit_);
  interface_layout_->removeWidget(player_list_);
  interface_layout_->removeWidget(start_game_);
  interface_layout_->removeWidget(back_to_game_option_);
  interface_layout_->removeWidget(ready_status_);
  interface_layout_->removeWidget(room_list_);
  interface_layout_->removeWidget(nothing_here_);
}

void ClientMainMenu::CreateRoom() {
  player_list_->clear();
  RemoveAllWidgets();

  player_list_->setVisible(true);
  start_game_->setVisible(true);
  ready_status_->setVisible(true);
  back_to_game_option_->setVisible(true);

  interface_layout_->addWidget(player_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(start_game_, 2, 0,
                               Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(ready_status_, 2, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_to_game_option_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  emit CreateRoomSignal();
}

void ClientMainMenu::JoinRoom() {
  if (room_list_->currentRow() == -1) {
    return;
  }
  RoomId room_id = room_list_->currentItem()->text().toInt();
  emit JoinRoomSignal(room_id);

  player_list_->clear();
  RemoveAllWidgets();
  player_list_->setVisible(true);
  ready_status_->setVisible(true);
  back_to_game_option_->setVisible(true);

  interface_layout_->addWidget(player_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(ready_status_, 2, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(back_to_game_option_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void ClientMainMenu::Settings() {
  RemoveAllWidgets();

  nothing_here_->setVisible(true);
  back_to_start_->setVisible(true);

  nothing_here_->setText("Nothing Here");
  nothing_here_->setFont(font_);
  nothing_here_->setStyleSheet("QLabel {color : #88bcff; font-size: 40px;}");

  interface_layout_->addWidget(nothing_here_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(back_to_start_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void ClientMainMenu::UpdateRoomList(const server_events::RoomsList& room_list) {
  room_list_->clear();
  for (auto room : room_list.ids()) {
    room_list_->addItem(QString::number(room));
  }
}

void ClientMainMenu::UpdatePlayerList(
    const server_events::RoomInfo& room_info) {
  player_list_->clear();
  for (int i = 0; i < room_info.users().size(); i++) {
    player_list_->addItem(
        QString::fromStdString(room_info.users().at(i).nickname()));
    switch (room_info.users().at(i).is_ready()) {
      case server_events::RoomUser::kNotReady: {
        player_list_->item(i)->setBackground(QColorConstants::Red);
        break;
      }
      case server_events::RoomUser::kReady: {
        player_list_->item(i)->setBackground(QColorConstants::Green);
        break;
      }
      case server_events::RoomUser::kNone: {
        player_list_->item(i)->setBackground(QColorConstants::Gray);
        break;
      }
      default: {}
    }
  }
}

void ClientMainMenu::ReadyButtonPressEvent() {
  if (ready_status_->text() == "READY") {
    ready_status_->setText("NOT READY");
  } else {
    ready_status_->setText("READY");
  }
  emit ReadyButtonPressed();
}

