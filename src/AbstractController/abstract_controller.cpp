#include "abstract_controller.h"

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::LogHandling(const proto::Event& event) const {
  qDebug().noquote().nospace() << GetControllerName()
      << " handling " << event.ShortDebugString();
}

void AbstractController::LogSending(const proto::Event& event) const {
  qDebug().noquote().nospace() << GetControllerName()
      << " sending " << event.ShortDebugString();
}

void AbstractController::LogReceive(const proto::Event& event) const {
  qInfo().noquote().nospace() << GetControllerName()
      << " received " << event.ShortDebugString();
}

void AbstractController::StartTicking() {
  // TODO(Everyone): remove magic number
  timer_.start(50);
}

void AbstractController::Tick() {
  this->OnTick();

  while (!events_to_handle_.empty()) {
    this->LogHandling(events_to_handle_.front());
    events_to_handle_.pop();
  }

  while (!events_to_send_.empty()) {
    this->Send(events_to_send_.front());
    events_to_send_.pop();
  }
}

void AbstractController::AddEventToHandle(const proto::Event& event) {
  events_to_handle_.push(event);
}

void AbstractController::AddEventToSend(const proto::Event& event) {
  events_to_send_.push(event);
}
