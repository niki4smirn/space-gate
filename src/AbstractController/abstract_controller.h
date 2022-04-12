#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

#include "Protobuf/scheme.pb.h"

#include "src/Helpers/helpers.h"

namespace log {

enum class Type {
  kHandle,
  kSend,
  kReceive
};

QString GetProcessStringByType(Type type);

}  // namespace log

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

  void AddEventToHandle(const events::Wrapper& event);
  void AddEventToSend(const events::Wrapper& event);

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  virtual void Send(const events::Wrapper& event) = 0;
  virtual void Handle(const events::Wrapper& event) = 0;

  void LogEvent(const events::Wrapper& event, log::Type log_type) const;

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<events::Wrapper> events_to_handle_;
  std::queue<events::Wrapper> events_to_send_;
};
