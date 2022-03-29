#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#include <QWebSocket>

#include "../User/user.h"
#include "../../Controller/room_controller.h"

class ServerModel {
 public:
  std::weak_ptr<User> GetUserBySocket(QWebSocket* socket) const;
  std::weak_ptr<User> GetUserById(UserId id) const;

  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);
  void AddRoom(const std::shared_ptr<RoomController>& room);
  void DeleteRoom(RoomId id);

  bool ExistsRoom(RoomId id) const;
  bool ExistsUser(UserId id) const;

  UserId GetUnusedUserId() const;
  RoomId GetUnusedRoomId() const;

  void AddUserToRoom(UserId user_id, RoomId room_id);
  void DeleteUserFromRoom(UserId user_id, RoomId room_id);

  bool IsInSomeRoom(UserId id) const;
  RoomId GetRoomIdByUserId(UserId id) const;

 private:
  std::map<UserId, std::shared_ptr<User>> users_;
  std::map<RoomId, std::shared_ptr<RoomController>> rooms_;
  std::unordered_map<UserId, std::optional<RoomId>> room_id_for_user_;
  std::unordered_map<QWebSocket*, UserId> user_id_by_socket_;
};


