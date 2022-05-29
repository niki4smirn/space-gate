#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QBasicTimer>

#include <vector>
#include <list>

#include "input_names.h"
#include "Constants/constants.h"

struct Key {
  explicit Key(input::Name key_pressed);
  input::Name key;
  int time = 0;
};

class InputController : public QObject {
  Q_OBJECT

 public:
  InputController();
  void KeyPressed(quint32 key_number);
  void MousePosStartTracking();
  void MousePosStopTracking();
  void MouseMove(const QPoint& pos);
  void MouseKeyPressed(const Qt::MouseButton& button);
  void timerEvent(QTimerEvent* event) override;

 private:
  void AddTime();
  void RemoveKeys();
  bool FindKey(input::Name key);

 private:
  QBasicTimer timer_;
  std::list<Key> keys_pressed_;
  bool is_mouse_tracking_ = false;
  int hold_time_ = 90;

 signals:
  void KeyEventToServer(input::Name key);
  void MouseMoveToServer(const QPoint& pos);
  void MouseKeyToServer(input::Name key);
};
