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
  void KeyRelease(quint32 key_number);

 private:
  std::set<std::string> keys_pressed_;
  int prev_size_ = 0;

  signals:
  void KeyEventToServer(std::set<std::string> keys_);

};

