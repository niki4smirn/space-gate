#pragma once

#include <cstdint>

#include "src/Server/Models/User/user.h"
#include "src/Server/Controllers/GameController/game_controller.h"

using RoomId = uint64_t;

class RoomModel : public QObject {
  Q_OBJECT

 public:
  RoomModel(RoomId room_id, const std::shared_ptr<User>& chief);

  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

  bool HasUser(UserId id) const;

  RoomId GetRoomId() const;

  UserId GetChiefId() const;
  void SetChiefId(UserId id);

  bool IsEmpty() const;

  UserId GetRandomUser() const;

  User::WaitingStatus GetUserWaitingStatus(UserId id) const;
  void SetUserWaitingStatus(UserId id, User::WaitingStatus status);

  const std::unordered_map<UserId, std::shared_ptr<User>>& GetUsers() const;
  std::vector<std::shared_ptr<User>> GetVectorOfUsers() const;

  void SetGameController(std::shared_ptr<GameController> controller);
  std::shared_ptr<GameController> GetGameController() const;
  void DeleteGameController();

  bool HasGameController() const;

 signals:
  void SendRoomInfo();

 private:
  RoomId id_;
  UserId chief_id_;
  std::unordered_map<UserId, std::shared_ptr<User>> users_;

  std::shared_ptr<GameController> game_controller_{nullptr};
};
