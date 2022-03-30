#pragma once

#include <cstdint>

#include "../User/user.h"

using RoomId = uint64_t;

enum class WaitingStatus {
  kNone,
  kNotReady,
  kReady
};

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

  WaitingStatus GetUserWaitingStatus(UserId id) const;
  void SetUserWaitingStatus(UserId id, WaitingStatus status);

 private:
  RoomId id_;
  UserId host_id_;
  std::unordered_map<UserId, std::weak_ptr<User>> users_list_;
  std::unordered_map<UserId, WaitingStatus> waiting_status_for_user_id_;
};
