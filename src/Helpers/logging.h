#pragma once

#include <QString>

#define LOG qDebug().noquote().nospace()
#define ASSERT(condition, message) \
    ((condition) ? \
    static_cast<void>(0) : \
    static_cast<void>(LOG << __FILE__ << ':' << __LINE__ \
                          << " Assertation failed: " << (message)));   \
    ((condition) ? \
    static_cast<void>(0) : \
    static_cast<void>(std::abort()))

namespace log {

enum class Type {
  kHandle,
  kSend,
  kReceive
};

QString GetProcessStringByType(Type type);

}  // namespace log
