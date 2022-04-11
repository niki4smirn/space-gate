#pragma once

#include <QString>

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/GameModel/game_model.h"

class GameController : public AbstractController {
 public:
  GameController();

  QString GetControllerName() const override;

 private:
  void OnTick() override;

  void Send(const proto::Event& event) override;
  void Handle(const proto::Event& event) override;

  GameModel model_;
};
