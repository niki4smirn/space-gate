#include "room_model.h"

RoomModel::RoomModel(RoomId room_id, const std::shared_ptr<User>& host)
    : id_(room_id), host_id_(host->GetId()) {
  users_list_[host_id_] = host;
}

void RoomModel::AddUser(const std::shared_ptr<User>& user) {
  users_list_[user->GetId()] = user;
}

void RoomModel::DeleteUser(UserId id) {
  users_list_.erase(id);
}

RoomId RoomModel::GetRoomId() const {
  return id_;
}

bool RoomModel::HasUser(UserId id) const {
  return users_list_.contains(id);
}

UserId RoomModel::GetHostId() const {
  return host_id_;
}

bool RoomModel::IsEmpty() const {
  return users_list_.empty();
}

void RoomModel::SetHostId(UserId id) {
  host_id_ = id;
}

UserId RoomModel::GetRandomUser() const {
  return users_list_.begin()->first;
}

void RoomModel::ChangeUserWaitingStatus(UserId id) {
  WaitingStatus& cur_status = waiting_status_for_user_id_[id];
  if (cur_status == WaitingStatus::kReady) {
    cur_status = WaitingStatus::kNotReady;
  } else if (cur_status == WaitingStatus::kNotReady) {
    cur_status = WaitingStatus::kReady;
  }
}
