#include "abstract_controller.h"

#include "Constants/constants.h"

namespace logging {

QString GetProcessStringByType(Type type) {
  static const std::unordered_map<Type, QString> type_to_str = {
      {Type::kHandle, " handling "},
      {Type::kSend, " sending "},
      {Type::kReceive, " received "},
  };
  return type_to_str.at(type);
}

}  // namespace logging

AbstractController::AbstractController() {
  connect(&timer_, &QTimer::timeout, this, &AbstractController::Tick);
}

void AbstractController::LogEvent(
    const events::EventWrapper& event,
    logging::Type log_type) const {
  qDebug().noquote().nospace() << GetControllerName()
                               << logging::GetProcessStringByType(log_type)
                               << event.ShortDebugString();
}

void AbstractController::StartTicking() {
  timer_.start(constants::kTickTime);
}

void AbstractController::Tick() {
  this->OnTick();

  while (!events_to_handle_.empty()) {
    auto& cur_event = events_to_handle_.front();
    this->Handle(cur_event);
    cur_event.Clear();
    events_to_handle_.pop();
  }

  while (!events_to_send_.empty()) {
    auto& cur_event = events_to_send_.front();
    this->Send(cur_event);
    cur_event.Clear();
    events_to_send_.pop();
  }
}

void AbstractController::AddEventToHandle(const events::EventWrapper& event) {
  events_to_handle_.push(event);
}

void AbstractController::AddEventToSend(const events::EventWrapper& event) {
  events_to_send_.push(event);
}
