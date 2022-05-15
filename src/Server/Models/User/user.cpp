#include "user.h"

User::User(UserId id, const std::shared_ptr<QWebSocket>& socket)
    : id_(id), socket_(socket) {}

UserId User::GetId() const {
  return id_;
}

std::shared_ptr<QWebSocket> User::GetSocket() const {
  return socket_;
}

User::WaitingStatus User::GetStatus() const {
  return status_;
}

void User::SetStatus(User::WaitingStatus new_status) {
  status_ = new_status;
}

User::WaitingStatus User::InverseStatus(User::WaitingStatus status) {
  switch (status) {
    case WaitingStatus::kNotReady: {
      return WaitingStatus::kReady;
    }
    case WaitingStatus::kReady: {
      return WaitingStatus::kNotReady;
    }
    default: {
      return WaitingStatus::kNone;
    }
  }
}
