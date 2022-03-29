#include "server_model.h"

std::weak_ptr<User> ServerModel::GetUserBySocket(QWebSocket* socket) const {
  auto user_it = user_id_by_socket_.find(socket);
  if (user_it == user_id_by_socket_.end()) {
    return {};
  }
  return users_.at(user_it->second);
}

std::weak_ptr<User> ServerModel::GetUserById(UserId id) const {
  auto user_it = users_.find(id);
  if (user_it == users_.end()) {
    return {};
  }
  return user_it->second;
}

void ServerModel::AddUser(const std::shared_ptr<User>& user) {
  UserId id = GetUnusedUserId();
  auto socket = user->GetSocket();
  users_[id] = user;
  user_id_by_socket_[socket.get()] = id;
}

void ServerModel::DeleteUser(UserId id) {
  user_id_by_socket_.erase(GetUserById(id).lock()->GetSocket().get());
  users_.erase(id);
}

bool ServerModel::ExistsUser(UserId id) const {
  return users_.contains(id);
}

UserId ServerModel::GetUnusedUserId() const {
  return users_.empty() ? 1 : users_.rbegin()->first + 1;
}

RoomId ServerModel::GetUnusedRoomId() const {
  return rooms_.empty() ? 1 : rooms_.rbegin()->first + 1;
}

void ServerModel::AddRoom(const std::shared_ptr<RoomController>& room) {
  rooms_[room->GetId()] = room;
}

void ServerModel::DeleteRoom(RoomId id) {
  rooms_.erase(id);
}

bool ServerModel::ExistsRoom(RoomId id) const {
  return rooms_.contains(id);
}

void ServerModel::AddUserToRoom(UserId user_id, RoomId room_id) {
  room_id_for_user_[user_id] = room_id;

  auto user = users_[user_id];
  auto room = rooms_[room_id];
  room->AddUser(user);
}

void ServerModel::DeleteUserFromRoom(UserId user_id, RoomId room_id) {
  room_id_for_user_[user_id] = std::nullopt;

  auto room = rooms_[room_id];
  room->DeleteUser(user_id);
}

RoomId ServerModel::GetRoomIdByUserId(UserId id) const {
  return room_id_for_user_.at(id).value();
}

bool ServerModel::IsInSomeRoom(UserId id) const {
  return room_id_for_user_.at(id).has_value();
}
