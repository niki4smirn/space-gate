#include "constants.h"

namespace constants {

const QString kDefaultServerIp = "localhost";
constexpr int kDefaultPort = 1337;
constexpr int kTickTime = 50;
constexpr int kKeyTickTime = 15;

constexpr int kScoreToFinish = 20;
constexpr int kMinigamesAddingTimeFrequency = 30000;
constexpr int kMinigamesAddingTickFrequency =
    kMinigamesAddingTimeFrequency / kTickTime;
constexpr int kGameDecreaseTimeFrequency = 20000;
constexpr int kGameDecreaseTickFrequency =
    kGameDecreaseTimeFrequency / kTickTime;
constexpr int kMaxMinigamesCount = 4;
constexpr int kMaxRoomPlayersCount = 4;

constexpr int kStartAnimationDuration = 5000;

}  // namespace constants
