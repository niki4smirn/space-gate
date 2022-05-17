#include "game_model.h"

const std::unordered_map<UserId,
                         std::shared_ptr<User>>& GameModel::GetPlayers() const {
  return players_;
}

void GameModel::AddPlayer(std::shared_ptr<User> player) {
  players_[player->GetId()] = std::move(player);
}

AbstractMinigame* GameModel::GetMinigameById(MinigameId id) {
  auto type = static_cast<MinigameType>(id);

  return minigames_[type];
}

void GameModel::AddPlayerToMinigame(UserId player_id,
                                    MinigameId minigame_id) {
  auto type = static_cast<MinigameType>(minigame_id);

  available_minigames_[type].push_back(players_[player_id]);

  // TODO(Andrei): remove magic number
  if (available_minigames_[type].size() == 4) {
    emit StartMinigame(type);
  }

  emit SendGameInfo();
}

const
  std::unordered_map
    <MinigameType, std::vector<std::shared_ptr<User>>>&
      GameModel::GetAvailableMinigames() const {
  return available_minigames_;
}

std::optional<MinigameType> GameModel::GetMinigameTypeByPlayerId(
    UserId id) const {
  if (minigame_by_player_id_.contains(id)) {
    return minigame_by_player_id_.at(id);
  }

  return std::nullopt;
}

uint64_t GameModel::GetProgress() const {
  return progress_;
}
