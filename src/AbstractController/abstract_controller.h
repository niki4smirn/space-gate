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

  virtual void Send(const proto::Event& event) = 0;
  virtual void Handle(const proto::Event& event) = 0;

  void LogSending(const proto::Event& event) const;
  void LogHandling(const proto::Event& event) const;
  void LogReceive(const proto::Event& event) const;

  void AddEventToHandle(const proto::Event& event);
  void AddEventToSend(const proto::Event& event);

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<proto::Event> events_to_handle_;
  std::queue<proto::Event> events_to_send_;
};
