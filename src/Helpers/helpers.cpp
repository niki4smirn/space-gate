#include "helpers.h"

namespace helpers {

uint64_t GetMinigamePlayersCountByType(MinigameType type) {
  switch (type) {
    case MinigameType::kTerminal: {
      return 4;
    }
    case MinigameType::kHoleRepair: {
      return 2;
    }
    default: {
      return 0;
    }
  }
}

int32_t Range::GetLeftInclusive() const {
  return left_;
}

int32_t Range::GetRightInclusive() const {
  return right_;
}

int32_t Range::GetLeftExclusive() const {
  return left_ + 1;
}

int32_t Range::GetRightExclusive() const {
  return right_ - 1;
}

}  // namespace helpers

QDebug& operator<<(QDebug& os, const std::string& string) {
  os << QString::fromStdString(string);
  return os;
}
