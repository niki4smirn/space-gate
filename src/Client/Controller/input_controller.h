#pragma once

#include <QKeyEvent>
#include <QMouseEvent>

#include <set>

#include "key_names.h"

class InputController : public QObject{
  Q_OBJECT

 public:
  explicit InputController(QWidget* parent);
  void KeyPressed(quint32 key_number);
  void KeyReleased(quint32 key_number);
  void MousePosStartTracking();
  void MousePosStopTracking();
  void MouseMove(const QPoint& pos);
  

 private:
  std::set<std::string> keys_pressed_;
  bool mouse_pressed_;

  signals:
  void KeyEventToServer(std::string key);
  void MouseMoveToServer(const QPoint& pos);

};

