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

  AbstractMinigame* GetMinigameById(MinigameId id);
  std::optional<MinigameType> GetMinigameTypeByPlayerId(UserId id) const;
  const
    std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>&
      GetAvailableMinigames() const;

  void AddMinigame(MinigameType type);
  void AddCreatedMinigame(MinigameType type, AbstractMinigame* minigame);
  void DeleteMinigame(MinigameType type);

  void AddScore(uint64_t score);

  void AddPlayerToMinigame(UserId player_id, MinigameId minigame_id);
  void MakePlayersBusy(const std::vector<std::shared_ptr<User>>& players);

 signals:
  void SendGameInfo();
  void StartMinigame(MinigameType type);

 private:
  uint64_t progress_{0};
  GameStatus status_{GameStatus::kGoing};

  std::unordered_map<UserId, std::shared_ptr<User>> players_;

  std::unordered_map<MinigameType, AbstractMinigame*> minigames_;
  std::unordered_map<UserId, MinigameType> minigame_by_player_id_;
  std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>
      available_minigames_;
  std::unordered_map<UserId, bool> busy_players_;
};
