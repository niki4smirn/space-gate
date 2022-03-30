#include "abstract_controller.h"

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::Handle(const proto::Event& event) {
  qDebug().noquote() << GetControllerName()
      << " handling " << event.ShortDebugString();
  // TODO(niki4smirn): somehow execute corresponding function
}

void AbstractController::LogSending(const proto::Event& event) const {
  qDebug().noquote() << GetControllerName()
      << " sending " << event.ShortDebugString();
}

void AbstractController::StartTicking() {
  // TODO(Everyone): remove magic number
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
