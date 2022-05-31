#include "helpers.h"

namespace helpers {

uint64_t GetMinigamePlayersCountByType(MinigameType type) {
  switch (type) {
    case MinigameType::kSample: {
      return 2;
    }
    default: {
      return 0;
    }
  }
}

}  // namespace helpers

QDebug& operator<<(QDebug& os, const std::string& string) {
  os << QString::fromStdString(string);
  return os;
}
