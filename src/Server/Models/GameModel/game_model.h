#pragma once

#include <unordered_map>
#include <vector>
#include <optional>

#include "src/Server/Models/User/user.h"
#include "src/Server/Games/minigames.h"

class GameModel : public QObject {
  Q_OBJECT

 public:
  GameModel() = default;

  const std::unordered_map<UserId, std::shared_ptr<User>>& GetPlayers() const;
  uint64_t GetProgress() const;

  void AddPlayer(std::shared_ptr<User> player);
  void AddPlayerToMinigame(UserId player_id, MinigameId minigame_id);

  AbstractMinigame* GetMinigameById(MinigameId id);

  std::optional<MinigameType> GetMinigameTypeByPlayerId(UserId id) const;

  const
    std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>&
      GetAvailableMinigames() const;

 signals:
  void SendGameInfo();
  void StartMinigame(MinigameType type);

 private:
  uint64_t progress_{0};

  std::unordered_map<UserId, std::shared_ptr<User>> players_;

  std::unordered_map<MinigameType, AbstractMinigame*> minigames_;
  std::unordered_map<UserId, MinigameType> minigame_by_player_id_;
  std::unordered_map<MinigameType, std::vector<std::shared_ptr<User>>>
      available_minigames_;
};
