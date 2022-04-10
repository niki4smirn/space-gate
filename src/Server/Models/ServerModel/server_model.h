#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#include <QWebSocket>

#include "src/Server/Models/User/user.h"
#include "src/Server/Controllers/room_controller.h"

class ServerModel {
 public:
  std::shared_ptr<User> GetUserBySocket(QWebSocket* socket) const;
  std::shared_ptr<User> GetUserById(UserId id) const;

  std::shared_ptr<RoomController> GetRoomById(RoomId id) const;

  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);
  void AddRoom(const std::shared_ptr<RoomController>& room);
  void DeleteRoom(RoomId id);

  bool ExistsRoom(RoomId id) const;
  bool ExistsUser(UserId id) const;

  UserId GetUnusedUserId() const;
  RoomId GetUnusedRoomId() const;

  void AddUserToRoom(UserId user_id, RoomId room_id);
  void DeleteUserFromRoom(UserId user_id);

  bool IsInSomeRoom(UserId id) const;
  std::shared_ptr<RoomController> GetRoomByUserId(UserId id) const;

 private:
  std::map<UserId, std::shared_ptr<User>> users_;
  std::map<RoomId, std::shared_ptr<RoomController>> rooms_;
  std::unordered_map<UserId, std::optional<RoomId>> room_id_for_user_id_;
  std::unordered_map<QWebSocket*, UserId> user_id_by_socket_;
};


