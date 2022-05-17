#include "sample_minigame.h"
#include "Constants/constants.h"

const uint64_t SampleMinigame::players_count = 4;

SampleMinigame::SampleMinigame(std::vector<std::shared_ptr<User>>* players)
    : AbstractMinigame(players, 5, 3000) {}
