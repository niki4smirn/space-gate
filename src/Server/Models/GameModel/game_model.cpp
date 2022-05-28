#include "game_model.h"

#include "Constants/constants.h"
#include "src/Helpers/helpers.h"

GameModel::GameModel(const std::vector<std::shared_ptr<User>>& players) {
  for (const auto& player : players) {
    players_[player->GetId()] = player;
  }

  emit SendGameInfo();
}

const std::unordered_map<UserId,
                         std::shared_ptr<User>>& GameModel::GetPlayers() const {
  return players_;
}

AbstractMinigame* GameModel::GetMinigameById(MinigameId id) {
  auto type = static_cast<MinigameType>(id);

  return minigames_[type];
}

void GameModel::AddPlayerToMinigame(UserId player_id,
                                    MinigameId minigame_id) {
  auto type = static_cast<MinigameType>(minigame_id);

  players_by_minigame_[type].push_back(players_[player_id]);
  minigame_by_player_id_[player_id] = type;

  if (players_by_minigame_[type].size()
      == helpers::GetMinigamePlayersCountByType(type)) {
    emit StartMinigame(type);
  }

  emit SendGameInfo();
}

const
  std::unordered_map
    <MinigameType, std::vector<std::shared_ptr<User>>>&
      GameModel::GetPlayersByMinigame() const {
  return players_by_minigame_;
}

std::optional<MinigameType> GameModel::GetMinigameTypeByPlayerId(
    UserId id) const {
  if (!minigame_by_player_id_.contains(id)) {
    return std::nullopt;
  }

  return minigame_by_player_id_.at(id);
}

uint64_t GameModel::GetProgress() const {
  return progress_;
}

const std::vector<std::shared_ptr<User>>& GameModel::GetPlayersForMinigame(
    MinigameType type) {
  return players_by_minigame_[type];
}

void GameModel::MakePlayersBusy(
    const std::vector<std::shared_ptr<User>>& players) {
  for (const auto& player : players) {
    is_busy_[player->GetId()] = true;
  }
}

void GameModel::AddScore(uint64_t score) {
  progress_ += score;

  if (progress_ >= constants::kScoreToFinish) {
    status_ = GameStatus::kEnded;
  }
}

std::vector<UserId> GameModel::GetFreePlayersIds() const {
  std::vector<UserId> result;

  for (const auto& [id, is_busy] : is_busy_) {
    if (!is_busy) {
      result.push_back(id);
    }
  }

  return result;
}

void GameModel::DeleteMinigame(MinigameType type) {
  delete minigames_[type];
  minigames_.erase(type);

  for (const auto& player : players_by_minigame_[type]) {
    auto player_id = player->GetId();
    minigame_by_player_id_.erase(player_id);
    is_busy_[player_id] = false;
  }

  players_by_minigame_.erase(type);

  emit SendGameInfo();
}

void GameModel::AddCreatedMinigame(
    MinigameType type, AbstractMinigame* minigame) {
  minigames_[type] = minigame;
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
  return is_busy_[id];
}
