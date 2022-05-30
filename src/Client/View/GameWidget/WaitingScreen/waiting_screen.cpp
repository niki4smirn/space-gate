#include "waiting_screen.h"

WaitingScreen::WaitingScreen(QWidget* parent) :
    QWidget(parent),
    text_(new QLabel("Waiting for players...", this)),
    leave_button_(new QPushButton("Leave", this)) {
  connect(leave_button_, &QPushButton::clicked, [&](){
    emit Leave();
  });
}
