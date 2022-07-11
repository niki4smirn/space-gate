#include "guest_room_lobby.h"

GuestRoomLobby::GuestRoomLobby(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    ready_button_(new QPushButton(this)),
    back_button_(new QPushButton(this)),
    player_list_(new QListWidget(this)){

  setMouseTracking(true);
  InterfaceConfigure();
  SetLayout();
  Connect();
}

void GuestRoomLobby::Connect() {
  connect(ready_button_, &QPushButton::clicked, this, &GuestRoomLobby::ReadyButtonPressEvent);
  connect(back_button_, &QPushButton::clicked,
          [this]() { emit Back(); });
}

void GuestRoomLobby::InterfaceConfigure() {
  setStyleSheet("QPushButton {background-color: rgb(136, 247, 255); font-size: 40px;}");

  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  ready_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  ready_button_->setText("JOIN");
  ready_button_->setFont(fonts::game_title_font);

  back_button_->setFixedSize(size().width() / 9.6, size().width() / 25.6);
  back_button_->setText("BACK");
  back_button_->setFont(fonts::game_title_font);
}

void GuestRoomLobby::SetLayout() {
  interface_layout_->addWidget(player_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(ready_button_, 2, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(back_button_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void GuestRoomLobby::UpdatePlayerList(const server_events::RoomInfo& room_info) {
  player_list_->clear();
  const auto& users = room_info.users();
  for (int i = 0; i < users.size(); i++) {
    player_list_->addItem(
        QString::fromStdString(users.at(i).nickname()));
    auto color = StatusToColor(users.at(i).ready_status());
    player_list_->item(i)->setBackground(color);
  }
}

void GuestRoomLobby::ReadyButtonPressEvent() {
  if (ready_button_->text() == "READY") {
    ready_button_->setText("NOT READY");
  } else {
    ready_button_->setText("READY");
  }
  emit Ready();
}

QColor GuestRoomLobby::StatusToColor(server_events::RoomUser::Status status) {
  QColor result;
  switch (status) {
    case server_events::RoomUser::kNotReady: {
      result = QColorConstants::Red;
      break;
    }
    case server_events::RoomUser::kReady: {
      result = QColorConstants::Green;
      break;
    }
    case server_events::RoomUser::kNone: {
      result = QColorConstants::Gray;
      break;
    }
    default: {}
  }
  return result;
}

