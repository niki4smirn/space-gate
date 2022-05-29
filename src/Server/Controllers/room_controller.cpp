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
      const auto& users = room_model_.GetUsers();
      if (!users.empty()) {
        LogEvent(event, logging::Type::kSend);
      }
      SendEveryUser(event);
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
  UserId user_id = client_event.sender_id();
  switch (room_event.type_case()) {
    case client_events::EventToRoom::kChangeWaitingStatus: {
      auto current_status = room_model_.GetUserWaitingStatus(user_id);
      room_model_.SetUserWaitingStatus(user_id,
                                       User::InverseStatus(current_status));
      break;
    }
    case client_events::EventToRoom::kStartGame: {
      if (user_id != room_model_.GetChiefId() || !IsEverybodyReady()) {
        break;
      }
      SendStartGameEvent();
      auto controller =
          std::make_shared<GameController>(room_model_.GetVectorOfUsers());
      connect(controller.get(), &GameController::GameEnded,
              this, &RoomController::GameEndedEvent);

      room_model_.SetGameController(std::move(controller));
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
    auto proto_user_status =
        static_cast<server_events::RoomUser::Status>(user_ptr->GetStatus());
    proto_user->set_ready_status(proto_user_status);
  }

  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_allocated_room_info(room_info);

  events::EventWrapper event;
  event.set_allocated_server_event(server_event);
  AddEventToSend(event);
}

bool RoomController::IsEverybodyReady() {
  const auto& users = room_model_.GetUsers();
  return std::all_of(users.begin(), users.end(), [](const auto& user) {
    return user.second->GetStatus() == User::WaitingStatus::kReady;
  });
}

void RoomController::SendStartGameEvent() {
  auto* start_game_event = new server_events::StartGame;

  auto* event_wrapper = new server_events::ServerEventWrapper;
  event_wrapper->set_allocated_start_game(start_game_event);

  events::EventWrapper start_event;
  start_event.set_allocated_server_event(event_wrapper);
  AddEventToSend(start_event);
}

void RoomController::SendEveryUser(events::EventWrapper event) const {
  const auto& users = room_model_.GetUsers();
  for (const auto& [user_id, user_ptr]: users) {
    event.mutable_server_event()->set_receiver_id(user_id);
    auto serialized = event.SerializeAsString();
    QByteArray byte_array(serialized.data(), serialized.size());
    user_ptr->GetSocket()->sendBinaryMessage(byte_array);
  }
}

void RoomController::GameEndedEvent(uint64_t score) {
  auto* game_result = new server_events::GameResult();
  game_result->set_score(score);

  auto* server_event = new server_events::ServerEventWrapper();
  server_event->set_allocated_game_result(game_result);

  events::EventWrapper event;
  event.set_allocated_server_event(server_event);

  AddEventToSend(event);

  room_model_.DeleteGameController();
}

void RoomController::SendEventToGame(const events::EventWrapper& event) {
  room_model_.GetGameController()->AddEventToHandle(event);
}
