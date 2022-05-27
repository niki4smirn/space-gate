#pragma once

#include "minigames_types.h"

#include <QDebug>

namespace helpers {

uint64_t GetMinigamePlayersCountByType(MinigameType type);

}  // namespace helpers

QDebug& operator<<(QDebug& os, const std::string& string);
