#include "sample_minigame.h"

#include "src/Helpers/Constants/constants.h"

const uint64_t SampleMinigame::players_count = 4;

SampleMinigame::SampleMinigame(
    const std::vector<std::shared_ptr<User>>& players)
    : AbstractMinigame(players, kMaxScore, kDuration) {
  StartMinigame();
}

QString SampleMinigame::GetControllerName() const {
  return "Sample Minigame";
}

void SampleMinigame::OnTick() {
  AbstractMinigame::OnTick();

  bool is_completed = IsCompleted();

  if (is_completed) {
    emit MinigameEnded(MinigameType::kSample, max_score_);
  } else if (ticks_ == duration_) {
    emit MinigameEnded(MinigameType::kSample, 0);
  }
}

void SampleMinigame::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);

  auto& action =
      event.client_event().event_to_game().minigame_action().sample_minigame();

  answers_[role_id_by_player_id_[event.client_event().sender_id()]] =
      QString::fromStdString(action.str());
}

void SampleMinigame::StartMinigame() {
  AbstractMinigame::StartMinigame();
}

bool SampleMinigame::IsCompleted() {
  QString temp = "";

  for (RoleId id = 0; id < players_count; ++id) {
    temp += answers_[id];

    if (id < players_count - 1) {
      temp += " ";
    }
  }

  SendResponseMessages();

  if (temp != right_answer_) {
    return false;
  }

  return true;
}

void SampleMinigame::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);

  auto receiver_id = event.server_event().receiver_id();

  auto& receiver = players_[receiver_id];

  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  receiver->GetSocket()->sendBinaryMessage(byte_array);
}

events::EventWrapper SampleMinigame::GenerateResponseMessage(UserId user_id) {
  events::EventWrapper event;

  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_receiver_id(user_id);

  auto* game_response = new minigame_responses::MinigameResponse;
  auto *sample_response = new minigame_responses::SampleResponse;

  for (const auto& [id, answer] : answers_) {
    (*sample_response->mutable_answers())[id] = answer.toStdString();
  }

  sample_response->set_role(
      static_cast<minigame_responses::SampleResponse_Role>(
          role_id_by_player_id_[user_id]));

  game_response->set_remaining_time(duration_ - ticks_);
  game_response->set_allocated_sample_response(sample_response);
  server_event->set_allocated_game_response(game_response);

  event.set_allocated_server_event(server_event);

  return event;
}

void SampleMinigame::SendResponseMessages() {
  if (ticks_ > duration_) {
    return;
  }
  for (const auto& [id, _] : players_) {
    AddEventToSend(GenerateResponseMessage(id));
  }
}
