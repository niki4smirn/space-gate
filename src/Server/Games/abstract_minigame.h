#pragma once

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/User/user.h"

class AbstractMinigame : public AbstractController {
  Q_OBJECT

 public:
  AbstractMinigame(
      std::vector<std::shared_ptr<User>>* players,
      uint64_t complexity,
      uint64_t duration);

  ~AbstractMinigame() override = default;

 signals:
  void MinigameEnded();

 protected:
  virtual void StartMinigame();
  virtual void EndMinigame() = 0;

  virtual void SendResponseMessages() = 0;
  virtual events::EventWrapper GenerateResponseMessage(UserId user_id) = 0;

  void OnTick() override;

 private:
  uint64_t duration_;
  uint64_t ticks_{0};

  uint64_t complexity_;

  std::unordered_map<UserId, std::shared_ptr<User>> players_;
  std::unordered_map<UserId, uint64_t> roles_ids_;
  std::vector<UserId> players_ids_;
};
