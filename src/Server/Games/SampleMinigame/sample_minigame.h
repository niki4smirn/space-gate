#pragma once

#include "src/Server/Games/abstract_minigame.h"

class SampleMinigame final : public AbstractMinigame {
 public:
  explicit SampleMinigame(std::vector<std::shared_ptr<User>>* players);

  QString GetControllerName() const override;

  static const uint64_t players_count;

 private:
  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

  void StartMinigame() override;
  bool CheckMinigameStatus() override;

  void SendResponseMessages() override;
  events::EventWrapper GenerateResponseMessage(UserId user_id) override;

  QString right_answer_{"Sample Minigame Test Message"};
  std::unordered_map<RoleId, QString> answers_;
};
