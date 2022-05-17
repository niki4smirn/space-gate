#pragma once

#include "src/Server/Games/abstract_minigame.h"

class SampleMinigame : public AbstractMinigame {
 public:
  explicit SampleMinigame(std::vector<std::shared_ptr<User>>* players);

  QString GetControllerName() const override;

  static const uint64_t players_count;

  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

  void StartMinigame() override;
  void EndMinigame() override;

  void SendResponseMessages() override;
  events::EventWrapper GenerateResponseMessage(UserId user_id) override;

 private:
};
