#pragma once

#include <cstdint>

#include "src/Server/Models/User/user.h"

using RoomId = uint64_t;

class RoomModel {
 public:
  RoomModel(RoomId room_id, const std::shared_ptr<User>& host);

  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

  bool HasUser(UserId id) const;

  RoomId GetRoomId() const;

  UserId GetHostId() const;
  void SetHostId(UserId id);

  bool IsEmpty() const;

  UserId GetRandomUser() const;

  User::WaitingStatus GetUserWaitingStatus(UserId id) const;
  void SetUserWaitingStatus(UserId id, User::WaitingStatus status);

 private:
  RoomId id_;
  UserId host_id_;
  std::unordered_map<UserId, std::weak_ptr<User>> users_;
};
