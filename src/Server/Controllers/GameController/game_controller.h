#pragma once

#include <QString>

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/GameModel/game_model.h"

class GameController : public AbstractController {
  Q_OBJECT

 public:
  explicit GameController(const std::vector<std::shared_ptr<User>>& players);

  QString GetControllerName() const override;

 signals:
  void GameEnded(uint64_t score);

 private:
  void SendGameInfoEvent();
  void SendStartMinigameEvent();
  void StartMinigameEvent(MinigameType type);
  void MinigameEndedEvent(MinigameType type, uint64_t score);
  events::EventWrapper GetGameInfo(UserId player_id) const;
  void SendMinigameEndedEvent(MinigameType type, uint64_t score);

  void FinishGame(uint64_t score);

  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

  uint64_t ticks_{0};
  GameModel model_;
};
