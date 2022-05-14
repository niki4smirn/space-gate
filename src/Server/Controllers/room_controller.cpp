#include "room_controller.h"

#include "src/Server/Models/User/user.h"

RoomController::RoomController(
    RoomId room_id,
    const std::shared_ptr<User>& chief)
    : room_model_(room_id, chief) {
  connect(&room_model_, &RoomModel::SendRoomInfo,
          this, &RoomController::SendRoomInfoEvent);
  StartTicking();
}

QString RoomController::GetControllerName() const {
  return "Room";
}

void RoomController::OnTick() {}

void RoomController::Send(const events::EventWrapper& event) {
  switch (event.type_case()) {
    case events::EventWrapper::kServerEvent: {
      LogEvent(event, logging::Type::kSend);
      for (auto [_, user_ptr] : room_model_.GetUsers()) {
        auto serialized = event.SerializeAsString();
        QByteArray byte_array(serialized.data(), serialized.size());
        user_ptr->GetSocket()->sendBinaryMessage(byte_array);
      }
      break;
    }
    default: {}
  }
}

RoomId RoomController::GetId() const {
  return room_model_.GetRoomId();
}

void RoomController::AddUser(const std::shared_ptr<User>& user) {
  if (room_model_.IsEmpty()) {
    room_model_.SetChiefId(user->GetId());
  }
  room_model_.AddUser(user);
}

void RoomController::DeleteUser(UserId id) {
  room_model_.DeleteUser(id);
  if (!room_model_.IsEmpty() && room_model_.GetChiefId() == id) {
    room_model_.SetChiefId(room_model_.GetRandomUser());
  }
}

void RoomController::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);
  const auto& client_event = event.client_event();
  const auto& room_event = client_event.event_to_room();
  switch (room_event.type_case()) {
    case client_events::EventToRoom::kChangeWaitingStatus: {
      UserId user_id = client_event.sender_id();
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

void RoomController::SendRoomInfoEvent() {
  auto* room_info = new server_events::RoomInfo;
  room_info->set_room_id(room_model_.GetRoomId());
  room_info->set_chief_id(room_model_.GetChiefId());

  for (auto [user_id, user_ptr] : room_model_.GetUsers()) {
    auto* proto_user = room_info->add_users();
    auto* str = new std::string{std::to_string(user_id)};
    proto_user->set_allocated_nickname(str);
    switch (user_ptr->GetStatus()) {
      case User::WaitingStatus::kNotReady: {
        proto_user->set_is_ready(server_events::RoomUser::kNotReady);
        break;
      }
      case User::WaitingStatus::kReady: {
        proto_user->set_is_ready(server_events::RoomUser::kReady);
        break;
      }
      default: {}
    }
  }

  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_allocated_room_info(room_info);

  events::EventWrapper event;
  event.set_allocated_server_event(server_event);
  AddEventToSend(event);
}
