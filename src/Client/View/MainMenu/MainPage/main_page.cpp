#include "main_page.h"

MainPage::MainPage(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    play_button_(new QPushButton(this)),
    settings_button_(new QPushButton(this)),
    exit_button_(new QPushButton(this)) {

  setMouseTracking(true);
  InterfaceConfigure();
  SetLayout();
  Connect();
}

void MainPage::Connect() {
  connect(play_button_,
          &QPushButton::clicked,
          [this]() { emit Play(); });
  connect(exit_button_,
          &QPushButton::clicked,
          [this]() { emit CloseGame(); });
  connect(settings_button_, &QPushButton::clicked,
          [this]() { emit Settings(); });
}

void MainPage::InterfaceConfigure() {
  setStyleSheet("QPushButton {background-color: rgb(136, 247, 255); font-size: 40px;}");

  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  play_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  play_button_->setText("PLAY");
  play_button_->setFont(fonts::game_title_font);

  settings_button_->setFixedSize(size().width() / 5.5, size().width() / 7.7);
  settings_button_->setText("SETTINGS");
  settings_button_->setFont(fonts::game_title_font);

  exit_button_->setFixedSize(size().width() / 9.6, size().width() / 25.6);
  exit_button_->setText("EXIT");
  exit_button_->setFont(fonts::game_title_font);
}

void MainPage::SetLayout() {
  interface_layout_->addWidget(game_name_, 0, 0, 1, 2, Qt::AlignHCenter);
  interface_layout_->addWidget(play_button_, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
  interface_layout_->addWidget(settings_button_, 1, 1,
                               Qt::AlignLeft | Qt::AlignVCenter);
  interface_layout_->addWidget(exit_button_, 2, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}
