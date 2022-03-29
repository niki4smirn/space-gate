#pragma once

#include <memory>
#include <unordered_map>

#include <QWebSocket>

#include "../User/user.h"

class ServerModel {
 public:
  std::weak_ptr<User> GetUserBySocket(QWebSocket* socket) const;
  std::weak_ptr<User> GetUserById(UserId id) const;

  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

  bool ExistsUser(UserId id);

  UserId GetUnusedUserId() const;

 private:
  std::map<UserId, std::shared_ptr<User>> users_;
  std::unordered_map<QWebSocket*, UserId> user_id_by_socket_;
};


