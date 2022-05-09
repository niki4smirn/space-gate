#include "logging.h"

namespace log {

QString GetProcessStringByType(Type type) {
  static const std::unordered_map<Type, QString> type_to_str = {
      {Type::kHandle, " handling "},
      {Type::kSend, " sending "},
      {Type::kReceive, " received "},
  };
  return type_to_str.at(type);
}

}  // namespace log
