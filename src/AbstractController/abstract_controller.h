#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

#include "Protobuf/events.pb.h"

#include "src/Helpers/helpers.h"

namespace game_log {

enum class Type {
  kHandle,
  kSend,
  kReceive
};

QString GetProcessStringByType(Type type);

}  // namespace game_log

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

  void AddEventToHandle(const events::EventWrapper& event);
  void AddEventToSend(const events::EventWrapper& event);

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  virtual void Send(const events::EventWrapper& event) = 0;
  virtual void Handle(const events::EventWrapper& event) = 0;

  void LogEvent(const events::EventWrapper& event, game_log::Type log_type) const;

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<events::EventWrapper> events_to_handle_;
  std::queue<events::EventWrapper> events_to_send_;
};
