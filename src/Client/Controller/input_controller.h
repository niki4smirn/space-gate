#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QBasicTimer>

#include <vector>
#include <list>

#include "key_names.h"
#include "Constants/constants.h"

struct Key {
  explicit Key(const std::string& pressed_key);
  std::string key;
  int time;
};

class InputController : public QObject {
  Q_OBJECT

 public:
  explicit InputController(QWidget* parent);
  void KeyPressed(quint32 key_number);
  void MousePosStartTracking();
  void MousePosStopTracking();
  void MouseMove(const QPoint& pos);
  void timerEvent(QTimerEvent* event) override;

 private:
  void AddTime();
  void RemoveKeys();
  bool FindKey(const std::string& key);

 private:
  QBasicTimer timer_;
  std::list<Key> keys_pressed_;
  bool track_mouse_ = false;
  int hold_time_ = 90;

 signals:
  void KeyEventToServer(const std::string& key);
  void MouseMoveToServer(const QPoint& pos);
};
