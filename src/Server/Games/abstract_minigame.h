#pragma once

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/User/user.h"

using RoleId = uint64_t;

class AbstractMinigame : public AbstractController {
  Q_OBJECT

 public:
  AbstractMinigame(
      std::vector<std::shared_ptr<User>>* players,
      uint64_t complexity,
      uint64_t duration);

  ~AbstractMinigame() override = default;

 signals:
  void MinigameEnded(uint64_t score = 0);

 protected:
  virtual void StartMinigame();
  // returns true if minigame is completed
  virtual bool CheckMinigameStatus() = 0;

  virtual void SendResponseMessages() = 0;
  virtual events::EventWrapper GenerateResponseMessage(UserId user_id) = 0;

  void OnTick() override;

  uint64_t duration_;
  uint64_t ticks_{0};

  uint64_t complexity_;

  std::unordered_map<UserId, std::shared_ptr<User>> players_;
  std::unordered_map<UserId, RoleId> role_id_by_player_id_;
  std::unordered_map<RoleId, UserId> player_id_by_role_id_;
};
