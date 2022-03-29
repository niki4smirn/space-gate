#include "abstract_controller.h"

#include <QDebug>

QDebug& operator<<(QDebug& os, const Event& event) {
  return os;
}

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::Handle(const Event& event) {
  qDebug() << GetControllerName() << " handling " << event;
  // TODO: somehow execute corresponding function
}

void AbstractController::LogSending(const Event& event) const {
  qDebug() << GetControllerName() << " sending " << event;
}

void AbstractController::StartTicking() {
  // TODO: remove magic number
  timer_.start(50);
}

void AbstractController::Tick() {
  this->OnTick();

  while (!events_to_handle_.empty()) {
    this->Handle(events_to_handle_.front());
    events_to_handle_.pop();
  }

  while (!events_to_send_.empty()) {
    this->Send(events_to_send_.front());
    events_to_send_.pop();
  }
}
