#include "room_controller.h"

RoomController::RoomController(
    RoomId room_id,
    const std::shared_ptr<User>& host)
    : room_model_(room_id, host) {
  StartTicking();
}

QString RoomController::GetControllerName() const {
  return "Room";
}

void RoomController::OnTick() {}

void RoomController::Send(const proto::Event& event) {}

RoomId RoomController::GetId() const {
  return room_model_.GetRoomId();
}

void RoomController::AddUser(const std::shared_ptr<User>& user) {
  if (room_model_.IsEmpty()) {
    room_model_.SetHostId(user->GetId());
  }
  room_model_.AddUser(user);
}

void RoomController::DeleteUser(UserId id) {
  room_model_.DeleteUser(id);
  if (!room_model_.IsEmpty() && room_model_.GetHostId() == id) {
    room_model_.SetHostId(room_model_.GetRandomUser());
  }
}

void RoomController::Handle(const proto::Event& event) {
  LogHandling(event);
  switch (event.type()) {
    case proto::Event::kChangeWaitingStatus: {
      UserId user_id = event.sender_id();
      auto current_status = room_model_.GetUserWaitingStatus(user_id);
      User::WaitingStatus new_status{User::WaitingStatus::kNone};
      switch (current_status) {
        case User::WaitingStatus::kNotReady: {
          new_status = User::WaitingStatus::kReady;
          break;
        }
        case User::WaitingStatus::kReady: {
          new_status = User::WaitingStatus::kNotReady;
          break;
        }
        default: {}
      }
      room_model_.SetUserWaitingStatus(user_id, new_status);
      break;
    }
    default: {}
  }
}

bool RoomController::IsEmpty() const {
  return room_model_.IsEmpty();
}
