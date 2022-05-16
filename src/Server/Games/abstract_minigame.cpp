#include "abstract_minigame.h"

#include <random>

AbstractMinigame::AbstractMinigame(
    std::vector<std::shared_ptr<User>>* players,
    uint64_t complexity,
    uint64_t duration)
    : duration_(duration),
      complexity_(complexity) {
  std::vector<uint64_t> roles(players->size());
  for (size_t i = 0; i < roles.size(); ++i) {
    roles[i] = i;
  }
  std::shuffle(roles.begin(),
              roles.end(),
              std::mt19937(std::random_device()()));

  for (auto& player : *players) {
    auto id = player->GetId();
    players_ids_.push_back(id);
    players_[id] = std::move(player);
    roles_ids_[id] = roles.back();
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
