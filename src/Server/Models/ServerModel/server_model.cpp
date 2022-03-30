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
  Q_ASSERT(ExistsUser(id));
  user_id_by_socket_.erase(GetUserById(id).lock()->GetSocket().get());
  room_id_for_user_id_.erase(id);
  users_.erase(id);
}

bool ServerModel::ExistsUser(UserId id) const {
  return users_.contains(id);
}

UserId ServerModel::GetUnusedUserId() const {
  return users_.empty() ? 0 : users_.rbegin()->first + 1;
}

RoomId ServerModel::GetUnusedRoomId() const {
  return rooms_.empty() ? 0 : rooms_.rbegin()->first + 1;
}

void ServerModel::AddRoom(const std::shared_ptr<RoomController>& room) {
  rooms_[room->GetId()] = room;
}

void ServerModel::DeleteRoom(RoomId id) {
  Q_ASSERT(ExistsRoom(id));
  rooms_.erase(id);
}

bool ServerModel::ExistsRoom(RoomId id) const {
  return rooms_.contains(id);
}

void ServerModel::AddUserToRoom(UserId user_id, RoomId room_id) {
  room_id_for_user_id_[user_id] = room_id;

  auto user = users_[user_id];
  auto room = rooms_[room_id];
  room->AddUser(user);
}

void ServerModel::DeleteUserFromRoom(UserId user_id, RoomId room_id) {
  room_id_for_user_id_[user_id] = std::nullopt;

  auto room = rooms_[room_id];
  room->DeleteUser(user_id);
  if (room->IsEmpty()) {
    DeleteRoom(room_id);
  }
}

RoomId ServerModel::GetRoomIdByUserId(UserId id) const {
  Q_ASSERT(IsInSomeRoom(id));
  return room_id_for_user_id_.at(id).value();
}

bool ServerModel::IsInSomeRoom(UserId id) const {
  Q_ASSERT(ExistsUser(id));
  return room_id_for_user_id_.at(id).has_value();
}

std::weak_ptr<RoomController> ServerModel::GetRoomById(RoomId id) const {
  Q_ASSERT(ExistsRoom(id));
  return rooms_.at(id);
}
