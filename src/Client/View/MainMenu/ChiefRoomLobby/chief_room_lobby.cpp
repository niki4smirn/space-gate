#include "chief_room_lobby.h"

ChiefRoomLobby::ChiefRoomLobby(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    start_button_(new QPushButton(this)),
    ready_button_(new QPushButton(this)),
    back_button_(new QPushButton(this)),
    player_list_(new QListWidget(this)){

  setMouseTracking(true);
  InterfaceConfigure();
  SetLayout();
  Connect();
}

void ChiefRoomLobby::Connect() {
  connect(start_button_,
          &QPushButton::clicked,
          [this]() { emit Start(); });
  connect(ready_button_,
          &QPushButton::clicked,
          [this]() { emit Ready(); });
  connect(back_button_, &QPushButton::clicked,
          [this]() { emit Back(); });
}

void ChiefRoomLobby::InterfaceConfigure() {
  setStyleSheet("QPushButton {background-color: rgb(136, 247, 255); font-size: 40px;}");

  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  start_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  start_button_->setText("CREATE");
  start_button_->setFont(fonts::game_title_font);

  ready_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  ready_button_->setText("JOIN");
  ready_button_->setFont(fonts::game_title_font);

  back_button_->setFixedSize(size().width() / 9.6, size().width() / 25.6);
  back_button_->setText("BACK");
  back_button_->setFont(fonts::game_title_font);
}

void ChiefRoomLobby::SetLayout() {
  interface_layout_->addWidget(player_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(start_button_,
                               2,
                               0,
                               Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(ready_button_, 2, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_button_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void ChiefRoomLobby::UpdatePlayerList(const server_events::RoomsList& room_info) {
  player_list_->clear();
  const auto& users = room_info.users();
  for (int i = 0; i < users.size(); i++) {
    player_list_->addItem(
        QString::fromStdString(users.at(i).nickname()));
    auto color = StatusToColor(users.at(i).ready_status());
    player_list_->item(i)->setBackground(color);
  }
  bool can_start = std::all_of(users.begin(), users.end(),
                               [](const auto& user){
                                 return user.ready_status() == server_events::RoomUser::kReady;
                               });
  if (users.size() < 2) {
    can_start = false;
  }
  start_button_->setEnabled(can_start);
}
