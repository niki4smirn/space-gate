#include "room_model.h"

RoomModel::RoomModel(RoomId room_id, const std::shared_ptr<User>& host)
    : id_(room_id), host_id_(host->GetId()) {
  users_[host_id_] = host;
}

void RoomModel::AddUser(const std::shared_ptr<User>& user) {
  UserId id = user->GetId();
  users_[id] = user;
  user->SetStatus(User::WaitingStatus::kNotReady);
}

void RoomModel::DeleteUser(UserId id) {
  Q_ASSERT(HasUser(id));
  users_.erase(id);
}

RoomId RoomModel::GetRoomId() const {
  return id_;
}

bool RoomModel::HasUser(UserId id) const {
  return users_.contains(id);
}

UserId RoomModel::GetHostId() const {
  return host_id_;
}

bool RoomModel::IsEmpty() const {
  return users_.empty();
}

void RoomModel::SetHostId(UserId id) {
  host_id_ = id;
}

UserId RoomModel::GetRandomUser() const {
  Q_ASSERT(!IsEmpty());
  return users_.begin()->first;
}

void RoomModel::SetUserWaitingStatus(UserId id, User::WaitingStatus status) {
  Q_ASSERT(HasUser(id));
  users_[id].lock()->SetStatus(status);
}

User::WaitingStatus RoomModel::GetUserWaitingStatus(UserId id) const {
  Q_ASSERT(HasUser(id));
  return users_.at(id).lock()->GetStatus();
}
