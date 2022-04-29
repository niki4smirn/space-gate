#include "client_main_menu.h"

#include <QFont>
#include <QFontDatabase>

ClientMainMenu::ClientMainMenu() :
  background_(new BackgroundWidget),
  background_layout_(new QGridLayout),
  game_name_(new QLabel("SpaceGate")),
  interface_layout_(new QGridLayout),
  play_(new QPushButton("PLAY")),
  start_game_(new QPushButton("START")),
  settings_(new QPushButton("SETTINGS")),
  exit_(new QPushButton("EXIT")),
  create_room_(new QPushButton("CREATE")),
  join_room_(new QPushButton("JOIN")),
  back_(new QPushButton("BACK")),
  interface_(new QWidget){

  background_layout_->addWidget(background_, 0, 0);
  background_layout_->setContentsMargins(0, 0, 0, 0);


  int id = QFontDatabase::addApplicationFont("Paladins.otf");
  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont Paladins(family);

  play_->setFixedSize(350, 250);
  play_->setFont(Paladins);
  play_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");
  settings_->setFixedSize(350, 250);
  settings_->setFont(Paladins);
  settings_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");
  exit_->setFixedSize(300, 150);
  exit_->setFont(Paladins);
  exit_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");
  create_room_->setFixedSize(350, 250);
  create_room_->setFont(Paladins);
  create_room_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");
  join_room_->setFixedSize(350, 250);
  join_room_->setFont(Paladins);
  join_room_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");
  back_->setFixedSize(300, 150);
  back_->setFont(Paladins);
  back_->setStyleSheet("background-color: rgb(136, 247, 255); font-size: 40px;");


  game_name_->setStyleSheet("QLabel {color : #88bcff; }");
  Paladins.setPointSize(105);
  Paladins.setBold(true);
  game_name_->setFont(Paladins);
  interface_layout_->setSpacing(50);
  interface_layout_->addWidget(game_name_, 0, 0, 1, 2, Qt::AlignHCenter);
  interface_layout_->addWidget(play_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_, 2, 0, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);
  interface_->setLayout(interface_layout_);

  background_layout_->addWidget(interface_, 0, 0);
  setLayout(background_layout_);

  Connect();


}
void ClientMainMenu::Play() {
  //game_name_->setVisible(false);
  create_room_->setVisible(false);
  join_room_->setVisible(false);
  back_->setVisible(false);
  play_->setVisible(false);
  settings_->setVisible(false);
  exit_->setVisible(false);
  emit Start(true);
}
void ClientMainMenu::Connect() {
  connect(play_, &QPushButton::clicked,this, &ClientMainMenu::ChooseRoomOption);
  connect(create_room_, &QPushButton::clicked,this, &ClientMainMenu::Play);
  connect(back_, &QPushButton::clicked,this, &ClientMainMenu::Back);
  connect(this, &ClientMainMenu::Start, background_, &BackgroundWidget::SetState);
}
void ClientMainMenu::ChooseRoomOption() {
  play_->setVisible(false);
  settings_->setVisible(false);
  exit_->setVisible(false);

  interface_layout_->removeWidget(play_);
  interface_layout_->removeWidget(settings_);
  interface_layout_->removeWidget(exit_);

  interface_layout_->addWidget(create_room_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(join_room_, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(back_, 2, 0, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);

  create_room_->setVisible(true);
  join_room_->setVisible(true);
  back_->setVisible(true);
}
void ClientMainMenu::Back() {
  create_room_->setVisible(false);
  join_room_->setVisible(false);
  back_->setVisible(false);

  interface_layout_->removeWidget(create_room_);
  interface_layout_->removeWidget(join_room_);
  interface_layout_->removeWidget(back_);

  interface_layout_->addWidget(play_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_, 2, 0, 1, 2, Qt::AlignHCenter | Qt::AlignVCenter);

  play_->setVisible(true);
  settings_->setVisible(true);
  exit_->setVisible(true);

}
