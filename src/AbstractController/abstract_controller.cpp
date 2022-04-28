#include "abstract_controller.h"

#include "Constants/constants.h"

namespace log {

QString GetProcessStringByType(Type type) {
  static const std::unordered_map<Type, QString> type_to_str = {
      {Type::kHandle, " handling "},
      {Type::kSend, " sending "},
      {Type::kReceive, " received "},
  };
  return type_to_str.at(type);
}

}  // namespace log

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::LogEvent(
    const events::EventWrapper& event,
    log::Type log_type) const {
  qDebug().noquote().nospace() << GetControllerName()
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

void AbstractController::AddEventToHandle(const events::EventWrapper& event) {
  events_to_handle_.push(event);
}

void AbstractController::AddEventToSend(const events::EventWrapper& event) {
  events_to_send_.push(event);
}
