#include "settings_menu.h"

SettingsMenu::SettingsMenu(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)),
    interface_layout_(new QGridLayout),
    back_button_(new QPushButton(this)) {

  setMouseTracking(true);
  InterfaceConfigure();
  SetLayout();
  Connect();
}

void SettingsMenu::Connect() {
  connect(back_button_, &QPushButton::clicked,
          [this]() { emit Back(); });
}

void SettingsMenu::InterfaceConfigure() {
  setStyleSheet("QPushButton {background-color: rgb(136, 247, 255); font-size: 40px;}");

  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  nothing_here_->setText("Nothing Here");
  nothing_here_->setFont(fonts::game_title_font);
  nothing_here_->setStyleSheet("QLabel {color : #88bcff; font-size: 40px;}");

  back_button_->setFixedSize(size().width() / 9.6, size().width() / 25.6);
  back_button_->setText("BACK");
  back_button_->setFont(fonts::game_title_font);
}

void SettingsMenu::SetLayout() {
  interface_layout_->addWidget(nothing_here_, 1, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
  interface_layout_->addWidget(back_button_, 3, 0, 1, 2,
                               Qt::AlignHCenter | Qt::AlignVCenter);
}
