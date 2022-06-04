#pragma once

#include "src/Client/InputController/input_names.h"
#include "src/Server/Games/AbstractMinigame/abstract_minigame.h"

class TerminalMinigame final : public AbstractMinigame {
 public:
  explicit TerminalMinigame(const std::vector<std::shared_ptr<User>>& players);

  QString GetControllerName() const override;

  static const uint64_t players_count;

  ~TerminalMinigame() override = default;

 private:
  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

  void StartMinigame() override;
  bool IsCompleted() override;

  void SendResponseMessages() override;
  events::EventWrapper GenerateResponseMessage(UserId user_id) override;

  static constexpr int kDuration = 2000;
  static constexpr int kMaxScore = 5;

  QString current_{};
  QString right_answer_
      {"sudo ship-controller --reload .. ship-controller --fix-all"};

  std::unordered_map<RoleId, std::vector<input::Name>> available_keys_;
  void SendInitialMessages();
  events::EventWrapper GenerateInitialMessage(const UserId id);
  bool CanPlayerUseKey(uint64_t id, input::Name name);
  void HandleKey(input::Name key_name);
};
