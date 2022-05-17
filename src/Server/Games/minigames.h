#pragma once

#include <cstdint>

#include "abstract_minigame.h"
#include "SampleMinigame/sample_minigame.h"

using MinigameId = uint64_t;

static const uint64_t kMinigamesCount = 1;

enum class MinigameType {
  kSample = 1,
};
