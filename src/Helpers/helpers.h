#pragma once

#include <QDebug>

namespace helpers {
}  // namespace helpers

QDebug& operator<<(QDebug& os, const std::string& string);
