#include "constants.h"

namespace constants {

const QString kDefaultServerIp = "localhost";
constexpr int kDefaultPort = 1337;
constexpr int kTickTime = 50;

constexpr int kScoreToFinish = 20;
constexpr int kMinigamesCount = 1;
constexpr int kMinigamesAddingFrequency = kTickTime * 60;
constexpr int kMaxMinigames = 4;
}  // namespace constant
