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
  users_[id] = std::make_shared<User>(id, socket);
  user_id_by_socket_[socket.get()] = id;
}

void ServerModel::DeleteUser(UserId id) {
  user_id_by_socket_.erase(GetUserById(id).lock()->GetSocket().get());
  users_.erase(id);
}

bool ServerModel::ExistsUser(UserId id) {
  return users_.contains(id);
}

UserId ServerModel::GetUnusedUserId() const {
  return users_.empty() ? 1 : users_.rbegin()->first + 1;
}
