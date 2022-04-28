#include "room_model.h"

RoomModel::RoomModel(RoomId room_id, const std::shared_ptr<User>& chief)
    : id_(room_id), chief_id_(chief->GetId()) {
  users_[chief_id_] = chief;
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

UserId RoomModel::GetChiefId() const {
  return chief_id_;
}

bool RoomModel::IsEmpty() const {
  return users_.empty();
}

void RoomModel::SetChiefId(UserId id) {
  chief_id_ = id;
}

UserId RoomModel::GetRandomUser() const {
  Q_ASSERT(!IsEmpty());
  return users_.begin()->first;
}

void RoomModel::SetUserWaitingStatus(UserId id, User::WaitingStatus status) {
  Q_ASSERT(HasUser(id));
  auto user = users_.at(id);
  user->SetStatus(status);
}

User::WaitingStatus RoomModel::GetUserWaitingStatus(UserId id) const {
  Q_ASSERT(HasUser(id));
  auto user = users_.at(id);
  return user->GetStatus();
}

std::unordered_map<UserId, std::shared_ptr<User>> RoomModel::GetUsers() const {
  return users_;
}
