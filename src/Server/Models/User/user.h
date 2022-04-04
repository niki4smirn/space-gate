#pragma once

#include <cstdint>
#include <QWebSocket>

using UserId = uint64_t;

class User {
 public:
  User(UserId id, const std::shared_ptr<QWebSocket>& socket);

  UserId GetId() const;

  std::shared_ptr<QWebSocket> GetSocket() const;

  enum class WaitingStatus {
    kNone,
    kNotReady,
    kReady
  };

  WaitingStatus GetStatus() const;
  void SetStatus(WaitingStatus new_status);

 private:
  UserId id_;
  std::shared_ptr<QWebSocket> socket_;
  WaitingStatus status_{WaitingStatus::kNone};
};
