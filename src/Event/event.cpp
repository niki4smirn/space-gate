#include "event.h"

#include <QDataStream>

QDebug& operator<<(QDebug& os, const Event& event) {
  os << static_cast<int>(event.type) << event.argument;
  return os;
}

QByteArray Event::ToByteArray() const {
  QByteArray res;
  QDataStream stream(&res, QIODevice::WriteOnly);
  stream << static_cast<int>(type) << static_cast<quint64>(argument);
  return res;
}

Event::Event(Event::Type type, uint64_t argument)
    : type(type), argument(argument){}

Event::Event(const QByteArray& byte_array) {
  QDataStream data_stream(byte_array);
  int type_in_int;
  data_stream >> type_in_int;
  type = static_cast<Type>(type_in_int);
  quint64 got_argument;
  data_stream >> got_argument;
  argument = static_cast<uint64_t>(got_argument);
}
