#pragma once

#include <QString>
#include <QDebug>

#define LOG qDebug().noquote().nospace()
#define ERROR_LOG(msg) qFatal("%s", \
                                (std::string(__FILE__) + ':' + \
                                std::to_string(__LINE__) + ' ' + \
                                std::string(msg)).c_str())
#define ASSERT(condition, msg) \
    ((condition) ? \
    static_cast<void>(0) : \
    static_cast<void>(ERROR_LOG(std::string(" Assertion failed: ") + (msg))));

namespace logging {

enum class Type {
  kHandle,
  kSend,
  kReceive
};

QString GetProcessStringByType(Type type);

}  // namespace logging
