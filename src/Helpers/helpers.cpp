#include "helpers.h"

QDebug& operator<<(QDebug& os, const std::string& string) {
  os << QString::fromStdString(string);
  return os;
}
