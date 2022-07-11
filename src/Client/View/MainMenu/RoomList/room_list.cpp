#include "room_list.h"

RoomList::RoomList(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    create_room_button_(new QPushButton(this)),
    join_room_button_(new QPushButton(this)),
    back_button_(new QPushButton(this)),
    rooms_list_(new QListWidget(this)){

  setMouseTracking(true);
  InterfaceConfigure();
  SetLayout();
  Connect();
}

void RoomList::Connect() {
  connect(create_room_button_,
          &QPushButton::clicked,
          [this]() { emit CreateRoom(); });
  connect(join_room_button_,
          &QPushButton::clicked,
          [this]() { emit JoinRoom(); });
  connect(back_button_, &QPushButton::clicked,
          [this]() { emit Back(); });
}

void RoomList::InterfaceConfigure() {
  setStyleSheet("QPushButton {background-color: rgb(136, 247, 255); font-size: 40px;}");

  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  create_room_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  create_room_button_->setText("CREATE");
  create_room_button_->setFont(fonts::game_title_font);

  join_room_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  join_room_button_->setText("JOIN");
  join_room_button_->setFont(fonts::game_title_font);

  back_button_->setFixedSize(size().width() / 9.6, size().width() / 25.6);
  back_button_->setText("BACK");
  back_button_->setFont(fonts::game_title_font);
}

void RoomList::SetLayout() {
  interface_layout_->addWidget(rooms_list_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(create_room_button_,
                               2,
                               0,
                               Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(join_room_button_, 2, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_button_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}

void RoomList::UpdateRoomList(const server_events::RoomsList& room_list) {
  int current_row = rooms_list_->currentRow();
  rooms_list_->clear();
  for (auto room : room_list.ids()) {
    rooms_list_->addItem(QString::number(room));
  }
  if (current_row >= rooms_list_->count()) {
    current_row = rooms_list_->count() - 1;
  }
  if (rooms_list_->count() != 0) {
    rooms_list_->setCurrentRow(current_row);
  }
}
