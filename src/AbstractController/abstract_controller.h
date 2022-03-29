#pragma once

#include <queue>

#include <QObject>
#include <QTimer>

struct Event{
  friend QDebug& operator<<(QDebug& os, const Event& event);
};

class AbstractController : public QObject {
 public:
  ~AbstractController() override = default;

  virtual QString GetControllerName() const = 0;

 protected:
  AbstractController();

  virtual void OnTick() = 0;

  void Handle(const Event& event);
  virtual void Send(const Event& event) = 0;

  void LogSending(const Event& event) const;

  void StartTicking();

 private:
  void Tick();

  QTimer timer_;

  std::queue<Event> events_to_handle_;
  std::queue<Event> events_to_send_;
};


