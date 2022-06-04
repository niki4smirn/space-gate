#pragma once

#include <cstdint>

using MinigameId = uint64_t;

enum class MinigameType {
  kNone = 0,

  kTerminal = 1,

  kLast
};
