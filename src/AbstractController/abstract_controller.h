#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

#include "Protobuf/events.pb.h"

#include "src/Helpers/helpers.h"
#include "src/Helpers/logging.h"

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

  void AddEventToHandle(const events::EventWrapper& event);
  void AddEventToSend(const events::EventWrapper& event);

  virtual void PrepareToClose();

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  virtual void Send(const events::EventWrapper& event) = 0;
  virtual void Handle(const events::EventWrapper& event) = 0;

  void LogEvent(const events::EventWrapper& event,
                logging::Type log_type) const;

  void StartTicking();
  void HandleAndSend();

 private:
  void Tick();

  QTimer timer_;

  std::queue<events::EventWrapper> events_to_handle_;
  std::queue<events::EventWrapper> events_to_send_;
};
