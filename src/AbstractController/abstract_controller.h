#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

#include "Protobuf/scheme.pb.h"

#include "src/Helpers/helpers.h"

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  void Handle(const proto::Event& event);
  virtual void Send(const proto::Event& event) = 0;

  void LogSending(const proto::Event& event) const;

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<proto::Event> events_to_handle_;
  std::queue<proto::Event> events_to_send_;
};
