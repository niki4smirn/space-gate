#include "client_main_menu.h"

#include <QFont>
#include <QFontDatabase>

ClientMainMenu::ClientMainMenu() :
    background_(new BackgroundWidget),
    background_layout_(new QGridLayout),
    game_name_(new QLabel("SpaceGate")),
    interface_layout_(new QGridLayout),
    play_(new QPushButton),
    start_game_(new QPushButton),
    settings_(new QPushButton),
    exit_(new QPushButton),
    create_room_(new QPushButton),
    join_room_(new QPushButton),
    back_to_game_option_(new QPushButton),
    back_to_start_(new QPushButton),
    interface_(new QWidget) {

  int id = QFontDatabase::addApplicationFont("Paladins.otf");
  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  font_ = QFont(family);

  ButtonsConfigure();

  game_name_->setStyleSheet("QLabel {color : #88bcff; }");
  font_.setPointSize(105);
  font_.setBold(true);
  game_name_->setFont(font_);

  SetWidgetsPos();
  Connect();

}

void ClientMainMenu::SetWidgetsPos() {
  background_layout_->addWidget(background_, 0, 0);
  background_layout_->setContentsMargins(0, 0, 0, 0);
  interface_layout_->setSpacing(50);
  interface_layout_->addWidget(game_name_, 0, 0, 1, 2, Qt::AlignHCenter);
  interface_layout_->addWidget(play_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_,
                               1,
                               1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_,
                               2,
                               0,
                               1,
                               2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_->setLayout(interface_layout_);
  background_layout_->addWidget(interface_, 0, 0);
  setLayout(background_layout_);
}

void ClientMainMenu::ButtonsConfigure() {

  play_->setFixedSize(350, 250);
  play_->setText("PLAY");
  play_->setFont(font_);
  play_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");

  settings_->setFixedSize(350, 250);
  settings_->setText("SETTINGS");
  settings_->setFont(font_);
  settings_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  exit_->setFixedSize(300, 150);
  exit_->setText("EXIT");
  exit_->setFont(font_);
  exit_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");

  create_room_->setFixedSize(350, 250);
  create_room_->setText("CREATE");
  create_room_->setFont(font_);
  create_room_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  join_room_->setFixedSize(350, 250);
  join_room_->setText("JOIN");
  join_room_->setFont(font_);
  join_room_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");

  back_to_start_->setFixedSize(300, 150);
  back_to_start_->setText("BACK");
  back_to_start_->setFont(font_);
  back_to_start_->setStyleSheet(
      "background-color: rgb(136, 247, 255); font-size: 40px;");
}

void ClientMainMenu::Play() {
  create_room_->setVisible(false);
  join_room_->setVisible(false);
  back_to_start_->setVisible(false);
  play_->setVisible(false);
  settings_->setVisible(false);
  exit_->setVisible(false);
  emit Start(true);
}
void ClientMainMenu::Connect() {
  connect(play_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::ChooseRoomOption);
  connect(create_room_, &QPushButton::clicked, this, &ClientMainMenu::Play);
  connect(back_to_start_,
          &QPushButton::clicked,
          this,
          &ClientMainMenu::BackToStart);
  //connect(settings_, &QPushButton::clicked, this, &ClientMainMenu::Settings);
  connect(this,
          &ClientMainMenu::Start,
          background_,
          &BackgroundWidget::SetState);
}
void ClientMainMenu::ChooseRoomOption() {
  play_->setVisible(false);
  settings_->setVisible(false);
  exit_->setVisible(false);

  interface_layout_->removeWidget(play_);
  interface_layout_->removeWidget(settings_);
  interface_layout_->removeWidget(exit_);

  interface_layout_->addWidget(create_room_,
                               1,
                               0,
                               Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(join_room_,
                               1,
                               1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_to_start_,
                               2,
                               0,
                               1,
                               2,
                               Qt::AlignHCenter | Qt::AlignVCenter);

  create_room_->setVisible(true);
  join_room_->setVisible(true);
  back_to_start_->setVisible(true);
}

void ClientMainMenu::BackToGameOption() {

}

void ClientMainMenu::BackToStart() {
  create_room_->setVisible(false);
  join_room_->setVisible(false);
  back_to_start_->setVisible(false);

  interface_layout_->removeWidget(create_room_);
  interface_layout_->removeWidget(join_room_);
  interface_layout_->removeWidget(back_to_start_);

  interface_layout_->addWidget(play_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_,
                               1,
                               1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_,
                               2,
                               0,
                               1,
                               2,
                               Qt::AlignHCenter | Qt::AlignVCenter);

  play_->setVisible(true);
  settings_->setVisible(true);
  exit_->setVisible(true);
}

void ClientMainMenu::CloseButtonPressed() {
  emit Close();
}
