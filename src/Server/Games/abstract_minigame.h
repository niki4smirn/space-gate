#pragma once

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/User/user.h"
#include "src/Helpers/minigames_types.h"

using RoleId = uint64_t;

class AbstractMinigame : public AbstractController {
  Q_OBJECT

 public:
  AbstractMinigame(
      const std::vector<std::shared_ptr<User>>& players,
      uint64_t max_score,
      uint64_t duration);

  ~AbstractMinigame() override = default;

 signals:
  void MinigameEnded(MinigameType type, uint64_t score = 0);

 protected:
  virtual void StartMinigame();
  virtual bool IsCompleted() = 0;

  virtual void SendResponseMessages() = 0;
  virtual events::EventWrapper GenerateResponseMessage(UserId user_id) = 0;

  void OnTick() override;

  uint64_t duration_;
  uint64_t ticks_{0};

  uint64_t max_score_;

  std::unordered_map<UserId, std::shared_ptr<User>> players_;
  std::unordered_map<UserId, RoleId> role_id_by_player_id_;
  std::unordered_map<RoleId, UserId> player_id_by_role_id_;
};
