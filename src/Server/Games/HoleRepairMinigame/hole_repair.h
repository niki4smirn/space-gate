#pragma once

#include "src/Server/Games/AbstractMinigame/abstract_minigame.h"
#include "src/Helpers/Constants/minigames_settings.h"

#include <QPointF>
#include <QRandomGenerator>

class HoleRepair : public AbstractMinigame {
 public:
  explicit HoleRepair(const std::vector<std::shared_ptr<User>>& players);
  ~HoleRepair() override = default;

  QString GetControllerName() const override;

 private:
  void ReceiveMousePos(QPointF pos);
  void ReceivePlatePos(QPointF pos);
  void SendInitialResponse();
  void GenerateHoles();
  bool Check();
  double Length(QPointF pnt1, QPointF pnt2);

  void OnTick() override;
  bool IsCompleted() override;
  void StartMinigame() override;
  void SendResponseMessages() override;
  events::EventWrapper GenerateResponseMessage(UserId user_id) override;
  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

 private:
  std::vector<QPointF> plates_;
  std::vector<QPointF> holes_;
};
