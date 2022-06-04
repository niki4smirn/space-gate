#include "terminal_minigame.h"

#include <random>

#include "src/Helpers/Constants/constants.h"

const uint64_t TerminalMinigame::players_count =
    helpers::GetMinigamePlayersCountByType(MinigameType::kTerminal);

TerminalMinigame::TerminalMinigame(
    const std::vector<std::shared_ptr<User>>& players)
    : AbstractMinigame(players, kMaxScore, kDuration) {
  StartMinigame();
}

QString TerminalMinigame::GetControllerName() const {
  return "Terminal Minigame";
}

void TerminalMinigame::OnTick() {
  AbstractMinigame::OnTick();

  bool is_completed = IsCompleted();

  if (is_completed) {
    emit MinigameEnded(MinigameType::kTerminal, max_score_);
  } else if (ticks_ == duration_) {
    emit MinigameEnded(MinigameType::kTerminal, 0);
  }
}

void TerminalMinigame::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);

  auto& action =
    event.client_event().event_to_game().minigame_action().terminal_minigame();

  auto key = static_cast<input::Name>(action.key_id());

  if (CanPlayerUseKey(event.client_event().sender_id(), key)) {
    HandleKey(key);
  }
}

void TerminalMinigame::StartMinigame() {
  // TODO(Andrei): Make prettier

  std::vector<input::Name> needed_keys
      {input::Name::kS, input::Name::kU, input::Name::kD, input::Name::kO,
       input::Name::kSpace, input::Name::kH, input::Name::kI, input::Name::kP,
       input::Name::kMinus, input::Name::kC, input::Name::kN, input::Name::kT,
       input::Name::kR, input::Name::kL, input::Name::kE, input::Name::kA,
       input::Name::kApostrophe, input::Name::kF, input::Name::kX,
       input::Name::kBackspace};

  std::shuffle(needed_keys.begin(),
               needed_keys.end(),
               std::mt19937(std::random_device()()));

  int keys_count = needed_keys.size() / 4;
  for (int i = 0; i < needed_keys.size(); ++i) {
    available_keys_[i / keys_count].push_back(needed_keys[i]);
  }

  SendInitialMessages();
  AbstractMinigame::StartMinigame();
}

bool TerminalMinigame::IsCompleted() {
  SendResponseMessages();

  if (current_ != right_answer_) {
    return false;
  }

  return true;
}

void TerminalMinigame::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);

  auto receiver_id = event.server_event().receiver_id();

  auto& receiver = players_[receiver_id];

  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  receiver->GetSocket()->sendBinaryMessage(byte_array);
}

events::EventWrapper TerminalMinigame::GenerateResponseMessage(UserId user_id) {
  events::EventWrapper event;

  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_receiver_id(user_id);

  auto* game_response = new minigame_responses::MinigameResponse;
  auto* terminal_response = new minigame_responses::TerminalResponse;

  terminal_response->set_entered_text(current_.toStdString());

  game_response->set_remaining_time(duration_ - ticks_);
  game_response->set_allocated_terminal_response(terminal_response);
  server_event->set_allocated_game_response(game_response);

  event.set_allocated_server_event(server_event);

  return event;
}

void TerminalMinigame::SendResponseMessages() {
  if (ticks_ > duration_) {
    return;
  }
  for (const auto& [id, _] : players_) {
    AddEventToSend(GenerateResponseMessage(id));
  }
}

void TerminalMinigame::SendInitialMessages() {
  for (const auto& [id, _] : players_) {
    AddEventToSend(GenerateInitialMessage(id));
  }
}

events::EventWrapper TerminalMinigame::GenerateInitialMessage(
    UserId user_id) {
  events::EventWrapper event;

  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_receiver_id(user_id);

  auto* game_response = new minigame_responses::MinigameResponse;
  auto* initial_terminal_response =
      new minigame_responses::InitialTerminalResponse;

  initial_terminal_response->set_text(right_answer_.toStdString());

  for (const auto
        & key_name : available_keys_[role_id_by_player_id_[user_id]]) {
    initial_terminal_response->add_available_buttons_id(
        static_cast<uint32_t>(key_name));
  }

  game_response->set_remaining_time(duration_ - ticks_);
  game_response->set_allocated_initial_terminal_respone(
      initial_terminal_response);
  server_event->set_allocated_game_response(game_response);

  event.set_allocated_server_event(server_event);

  return event;
}

bool TerminalMinigame::CanPlayerUseKey(UserId id, input::Name key_name) {
  const auto& available_keys = available_keys_[role_id_by_player_id_[id]];

  return std::find(available_keys.begin(), available_keys.end(), key_name)
      != available_keys.end();
}

void TerminalMinigame::HandleKey(input::Name key_name) {
  switch (key_name) {
    case input::Name::kBackspace: {
      if (!current_.isEmpty()) {
        current_.remove(current_.size() - 1, 1);
      }

      break;
    }
    case input::Name::kApostrophe: {
      current_ += '&';

      break;
    }
    case input::Name::kMinus: {
      current_ += '-';

      break;
    }
    case input::Name::kSpace: {
      current_ += ' ';

      break;
    }
    default: {
      auto name = input::InputNameToString(key_name).toLower();
      name.remove(0, 1);
      current_ += name;
    }
  }
}
