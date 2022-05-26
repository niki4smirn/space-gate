#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QBasicTimer>

#include <vector>

#include "key_names.h"

struct Key {
  explicit Key(std::string pressed_key);
  std::string key;
  int time;
};

class InputController : public QObject{
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
  std::vector<Key> keys_pressed_;
  bool mouse_pressed_ = false;
  int tick_ = 15;
  int hold_time_ = 240;

  signals:
  void KeyEventToServer(std::string key);
  void MouseMoveToServer(const QPoint& pos);

};

