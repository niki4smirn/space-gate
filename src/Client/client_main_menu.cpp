#include "client_main_menu.h"

ClientMainMenu::ClientMainMenu() :
  background_(new BackgroundWidget),
  background_layout_(new QGridLayout),
  game_name_(new QLabel("SpaceGate")),
  interface_layout_(new QGridLayout),
  play_(new QPushButton("PLAY")),
  start_game_(new QPushButton("START")),
  settings_(new QPushButton("SETTINGS")),
  exit_(new QPushButton("EXIT")),
  interface_(new QWidget){

  background_layout_->addWidget(background_, 0, 0);
  background_layout_->setContentsMargins(0, 0, 0, 0);

  game_name_->setStyleSheet("QLabel {color : blue; }");
  QFont font = game_name_->font();
  font.setPointSize(72);
  font.setBold(true);
  game_name_->setFont(font);
  interface_layout_->setSpacing(0);
  interface_layout_->addWidget(game_name_, 0, 3, Qt::AlignCenter);
  interface_layout_->addWidget(play_, 1, 3, Qt::AlignCenter);
  interface_->setLayout(interface_layout_);

  background_layout_->addWidget(interface_, 0, 0);
  setLayout(background_layout_);

  Connect();


}
void ClientMainMenu::Play() {
  game_name_->setVisible(false);
  play_->setVisible(false);
  emit Start(true);
}
void ClientMainMenu::Connect() {
  connect(play_, &QPushButton::clicked,this, &ClientMainMenu::Play);
  connect(this, &ClientMainMenu::Start, background_, &BackgroundWidget::SetState);
}
