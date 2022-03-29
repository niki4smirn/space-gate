#pragma once

#include <cstdint>
#include <QWebSocket>

using UserId = uint64_t;

class User {
 public:
  User(UserId id, std::shared_ptr<QWebSocket> socket);

  UserId GetId() const;

  std::shared_ptr<QWebSocket> GetSocket() const;

 private:
  UserId id_;
  std::shared_ptr<QWebSocket> socket_;
};
