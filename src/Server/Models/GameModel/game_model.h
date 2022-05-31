#pragma once

#include <unordered_map>
#include <vector>
#include <optional>

#include "src/Server/Models/User/user.h"
#include "src/Server/Games/minigames.h"

enum class GameStatus {
  kGoing,
  kEnded
};

class GameModel : public QObject {
  Q_OBJECT

 public:
  explicit GameModel(const std::vector<std::shared_ptr<User>>& players);

  int GetMinigamesCount() const;
  uint64_t GetProgress() const;
  GameStatus GetStatus() const;

  const std::unordered_map<UserId, std::shared_ptr<User>>& GetPlayers() const;
  const std::vector<std::shared_ptr<User>>& GetPlayersForMinigame(
      MinigameType type);
  std::vector<UserId> GetFreePlayersIds() const;
  bool IsPlayerBusy(UserId id);


  std::shared_ptr<AbstractMinigame> GetMinigameByType(MinigameType type);
  std::optional<MinigameType> GetMinigameTypeByPlayerId(UserId id) const;
  const
    std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>&
      GetPlayersByMinigame() const;

  void AddMinigame(MinigameType type);
  void AddCreatedMinigame(MinigameType type,
                          std::shared_ptr<AbstractMinigame> minigame);
  void DeleteMinigame(MinigameType type);

  void AddScore(uint64_t score);

  void AddPlayerToMinigameQueue(UserId player_id, MinigameType type);

 signals:
  void SendGameInfo();
  void StartMinigame(MinigameType type);

 private:
  uint64_t progress_{0};
  GameStatus status_{GameStatus::kGoing};

  std::unordered_map<UserId, std::shared_ptr<User>> players_;
  std::set<UserId> free_users_ids_;

  std::unordered_map<MinigameType,
                     std::shared_ptr<AbstractMinigame>> minigames_;
  std::unordered_map<UserId, MinigameType> minigame_by_player_id_;
  std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>
      players_by_minigame_;
};
