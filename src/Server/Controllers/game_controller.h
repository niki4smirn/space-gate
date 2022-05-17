#pragma once

#include <QString>

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/GameModel/game_model.h"

class GameController : public AbstractController {
 public:
  GameController();

  QString GetControllerName() const override;

 private:
  void SendGameInfoEvent();
  void StartMinigameEvent();

  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

  GameModel model_;
};
