#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

#include "Protobuf/scheme.pb.h"

#include "src/Helpers/helpers.h"
#include "src/Helpers/logging.h"

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

  void AddEventToHandle(const proto::Event& event);
  void AddEventToSend(const proto::Event& event);

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  virtual void Send(const proto::Event& event) = 0;
  virtual void Handle(const proto::Event& event) = 0;

  void LogEvent(const proto::Event& event, log::Type log_type) const;

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<proto::Event> events_to_handle_;
  std::queue<proto::Event> events_to_send_;
};