#include "abstract_minigame.h"

#include <random>

AbstractMinigame::AbstractMinigame(
    const std::vector<std::shared_ptr<User>>& players,
    uint64_t max_score,
    uint64_t duration)
    : duration_(duration),
      max_score_(max_score) {
  std::vector<uint64_t> roles(players.size());
  for (size_t i = 0; i < roles.size(); ++i) {
    roles[i] = i;
  }
  std::shuffle(roles.begin(),
              roles.end(),
              std::mt19937(std::random_device()()));

  for (auto& player : players) {
    auto id = player->GetId();
    players_[id] = player;
    role_id_by_player_id_[id] = roles.back();
    player_id_by_role_id_[roles.back()] = id;
    roles.pop_back();
  }
}

void AbstractMinigame::OnTick() {
  ++ticks_;
}

void AbstractMinigame::StartMinigame() {
  SendResponseMessages();
  StartTicking();
}
