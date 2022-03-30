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
  room_model_.AddUser(user);
}

void RoomController::DeleteUser(UserId id) {
  room_model_.DeleteUser(id);
}

void RoomController::Handle(const proto::Event& event) {
  LogHandling(event);
  switch (event.type()) {
    case proto::Event::kChangeWaitingStatus: {
      UserId user_id = event.sender_id();
      room_model_.ChangeUserWaitingStatus(user_id);
      break;
    }
    default: {}
  }
}
