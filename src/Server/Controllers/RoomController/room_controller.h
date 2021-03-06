#pragma once

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/RoomModel/room_model.h"

class RoomController : public AbstractController {
  Q_OBJECT

 public:
  RoomController(RoomId room_id, const std::shared_ptr<User>& chief);
  QString GetControllerName() const override;

  RoomId GetId() const;
  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

  bool IsEmpty() const;
  bool IsInGame() const;

  int GetPlayersCount() const;

  void SendEventToGame(const events::EventWrapper& event);

 signals:
  void SendRoomsList();

 protected:
  void OnTick() override;

  void Send(const events::EventWrapper& event) override;
  void Handle(const events::EventWrapper& event) override;

 private:
  void SendRoomInfoEvent();
  void SendStartGameEvent();
  bool IsEverybodyReady();
  void SendEveryUser(events::EventWrapper event) const;
  void GameEndedEvent(uint64_t score);
  void UnreadyRoomPlayers();

  RoomModel room_model_;
};
