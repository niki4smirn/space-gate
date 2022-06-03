#pragma once

#include "src/Client/Games/minigames_types.h"

#include <QDebug>
#include <QRandomGenerator64>

namespace helpers {

uint64_t GetMinigamePlayersCountByType(MinigameType type);

class Range {
 public:
  template<typename T1, typename T2>
  Range(T1 left, T2 right);

  int32_t GetLeftInclusive() const;
  int32_t GetRightInclusive() const;

  int32_t GetLeftExclusive() const;
  int32_t GetRightExclusive() const;

 private:
  // are inclusive
  int32_t left_;
  int32_t right_;
};

template<typename T1, typename T2>
Range::Range(T1 left, T2 right) :
  left_(static_cast<int32_t>(left)),
  right_(static_cast<int32_t>(right)) {}

template<typename T>
T GetRandomInRange(const Range& range) {
  return static_cast<T>(QRandomGenerator64::global()->bounded(
      range.GetLeftInclusive(),
      range.GetRightInclusive()));
}

}  // namespace helpers

QDebug& operator<<(QDebug& os, const std::string& string);

// loop for [begin, end)
#define ENUM_LOOP(begin, end, element) \
                            decltype(begin) (element); \
                            for(int _ = static_cast<int>(begin); \
                            _ < static_cast<int>(end);           \
                            ++_, (element) = static_cast<decltype(begin)>(_))
