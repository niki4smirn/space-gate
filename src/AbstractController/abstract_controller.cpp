#include "abstract_controller.h"

#include "Constants/constants.h"

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::LogEvent(
    const proto::Event& event,
    log::Type log_type) const {
  LOG << GetControllerName()
      << log::GetProcessStringByType(log_type) << event.ShortDebugString();
}

void AbstractController::StartTicking() {
  timer_.start(constants::kTickTime);
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

void AbstractController::AddEventToHandle(const proto::Event& event) {
  events_to_handle_.push(event);
}

void AbstractController::AddEventToSend(const proto::Event& event) {
  events_to_send_.push(event);
}