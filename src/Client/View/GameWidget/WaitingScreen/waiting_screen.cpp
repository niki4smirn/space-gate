#include "waiting_screen.h"

WaitingScreen::WaitingScreen(QWidget* parent) :
    QWidget(parent),
    text_(new QLabel("Waiting for players...", this)),
    leave_button_(new QPushButton("Leave", this)),
    layout_(new QGridLayout(this)) {
  layout_->addWidget(text_, 0, 0);
  layout_->addWidget(leave_button_, 1, 0);
  setLayout(layout_);
  connect(leave_button_, &QPushButton::clicked, [&](){
    emit Leave();
  });
}
