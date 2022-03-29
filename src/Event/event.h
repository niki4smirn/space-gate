#pragma once

#include <QDebug>

struct Event{
  enum class Type {
    kNone,
    kCreateRoom,
    kEnterRoom,
    kLeaveRoom
  };

  Event(Type type, uint64_t argument);
  Event(const QByteArray& byte_array);

  Type type;
  uint64_t argument;

  QByteArray ToByteArray() const;

  friend QDebug& operator<<(QDebug& os, const Event& event);
};

