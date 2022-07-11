#include "light_effect.h"

LightEffect::LightEffect(QWidget* parent) :
    QWidget(parent),
    game_name_(new QLabel("SpaceGate", this)) {

  setMouseTracking(true);
  game_name_->setStyleSheet("QLabel {color : #88bcff; font-size : 105; font-weight : bold }");
  game_name_->setFont(fonts::game_title_font);

  interface_layout_->addWidget(game_name_, 0, 0, Qt::AlignHCenter);
}
