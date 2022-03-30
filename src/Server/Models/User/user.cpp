#include "user.h"

User::User(UserId id, const std::shared_ptr<QWebSocket>& socket)
    : id_(id), socket_(socket) {}

UserId User::GetId() const {
  return id_;
}

std::shared_ptr<QWebSocket> User::GetSocket() const {
  return socket_;
}
