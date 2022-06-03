#include "game_model.h"

#include <utility>

#include "src/Helpers/Constants/constants.h"
#include "src/Helpers/helpers.h"

GameModel::GameModel(const std::vector<std::shared_ptr<User>>& players) {
  for (const auto& player : players) {
    auto player_id = player->GetId();
    players_[player_id] = player;
    free_users_ids_.insert(player_id);
  }
  emit SendGameInfo();
}

const std::unordered_map<UserId,
                         std::shared_ptr<User>>& GameModel::GetPlayers() const {
  return players_;
}

std::shared_ptr<AbstractMinigame> GameModel::GetMinigameByType(
    MinigameType type) {
  if (!minigames_.contains(type)) {
    return {nullptr};
  }
  return minigames_[type];
}

void GameModel::AddPlayerToMinigameQueue(UserId player_id,
                                         MinigameType type) {
  players_by_minigame_[type].push_back(players_[player_id]);
  free_users_ids_.erase(player_id);
  minigame_type_by_player_id_[player_id] = type;

  emit SendGameInfo();

  if (players_by_minigame_[type].size()
      == helpers::GetMinigamePlayersCountByType(type)) {
    emit StartMinigame(type);
  }
}

const
  std::unordered_map
    <MinigameType, std::vector<std::shared_ptr<User>>>&
      GameModel::GetPlayersByMinigame() const {
  return players_by_minigame_;
}

std::optional<MinigameType> GameModel::GetMinigameTypeByPlayerId(
    UserId id) const {
  if (!minigame_type_by_player_id_.contains(id)) {
    return std::nullopt;
  }

  return minigame_type_by_player_id_.at(id);
}

uint64_t GameModel::GetProgress() const {
  return progress_;
}

const std::vector<std::shared_ptr<User>>& GameModel::GetPlayersForMinigame(
    MinigameType type) {
  return players_by_minigame_[type];
}

void GameModel::AddScore(uint64_t score) {
  progress_ += score;

  if (progress_ >= constants::kScoreToFinish) {
    status_ = GameStatus::kEnded;
  }
}

std::vector<UserId> GameModel::GetFreePlayersIds() const {
  return {free_users_ids_.begin(), free_users_ids_.end()};
}

void GameModel::DeleteMinigame(MinigameType type) {
  auto& minigame = minigames_[type];
  minigame->PrepareToClose();

  minigames_.erase(type);

  for (const auto& player : players_by_minigame_[type]) {
    auto player_id = player->GetId();
    minigame_type_by_player_id_.erase(player_id);
    free_users_ids_.insert(player_id);
  }

  players_by_minigame_.erase(type);

  emit SendGameInfo();
}

void GameModel::AddCreatedMinigame(
    MinigameType type, std::shared_ptr<AbstractMinigame> minigame) {
  minigames_[type] = std::move(minigame);

  emit SendGameInfo();
}

GameStatus GameModel::GetStatus() const {
  return status_;
}

void GameModel::AddMinigame(MinigameType type) {
  players_by_minigame_[type] = {};

  emit SendGameInfo();
}

int GameModel::GetMinigamesCount() const {
  return players_by_minigame_.size();
}

bool GameModel::IsPlayerBusy(UserId id) {
  return !free_users_ids_.contains(id);
}

void GameModel::DeleteMinigamePlayer(UserId id) {
  if (!IsPlayerBusy(id)) {
    return;
  }
  free_users_ids_.insert(id);
  auto minigame_type = minigame_type_by_player_id_[id];
  players_by_minigame_.erase(minigame_type);
  if (minigames_.contains(minigame_type)) {
    DeleteMinigame(minigame_type);
  }
  emit SendGameInfo();
}

void GameModel::DecreaseProgress() {
  --progress_;

  emit SendGameInfo();
}

const std::unordered_map<MinigameType,
                         std::shared_ptr<AbstractMinigame>>&
                         GameModel::GetAllMinigames() const {
  return minigames_;
}

void GameModel::SetProgress(uint64_t progress) {
  progress_ = progress;
}

int GameModel::GetPlayersCount() const {
  return players_.size();
}
