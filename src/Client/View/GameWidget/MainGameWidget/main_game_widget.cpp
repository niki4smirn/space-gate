#include "main_game_widget.h"

MainGameWidget::MainGameWidget(QWidget* parent) :
  QWidget(parent),
  button1_(new QPushButton(this)),
  button2_(new QPushButton(this)),
  layout_(new QGridLayout(this)) {
  connect(button1_, &QPushButton::clicked, [&]() {
    emit JoinMinigame(0);
  });
  connect(button2_, &QPushButton::clicked, [&]() {
    emit JoinMinigame(1);
  });

  layout_->addWidget(button1_, 0, 0);
  layout_->addWidget(button2_, 0, 1);

  setLayout(layout_);
}
