#include "game_controller.h"

#include <QRandomGenerator64>

#include "src/Helpers/Constants/constants.h"

GameController::GameController(
    const std::vector<std::shared_ptr<User>>& players)
  : model_(players) {
  connect(&model_, &GameModel::SendGameInfo,
          this, &GameController::SendGameInfoEvent);
  connect(&model_, &GameModel::StartMinigame,
          this, &GameController::StartMinigameEvent);

  StartTicking();
}

QString GameController::GetControllerName() const {
  return "Game";
}

void GameController::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);

  const auto& client_event = event.client_event();
  const auto& game_event = client_event.event_to_game();

  switch (game_event.type_case()) {
    case client_events::EventToGame::kJoinMinigame: {
      if (!model_.IsPlayerBusy(client_event.sender_id())) {
        model_.AddPlayerToMinigame(client_event.sender_id(),
                                   game_event.join_minigame().minigame_id());
      }

      break;
    }
    case client_events::EventToGame::kMinigameAction: {
      AbstractMinigame* minigame =
          model_.GetMinigameById(game_event.minigame_action().minigame_id());

      minigame->AddEventToHandle(event);
    }
    default: {}
  }
}

void GameController::OnTick() {
  ++ticks_;

  if (ticks_ == constants::kMinigamesAddingFrequency
      && model_.GetMinigamesCount() < constants::kMinigamesCount) {
    MinigameType type =
        static_cast<MinigameType>(
            QRandomGenerator64::global()->bounded(1,
                                                  constants::kMinigamesCount));

    model_.AddMinigame(type);

    ticks_ = 0;
  }
}

void GameController::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);

  auto receiver_id = event.server_event().receiver_id();

  auto& receiver = model_.GetPlayers().at(receiver_id);

  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  receiver->GetSocket()->sendBinaryMessage(byte_array);
}

void GameController::SendGameInfoEvent() {
  switch (model_.GetStatus()) {
    case GameStatus::kGoing: {
      for (const auto& player_id : model_.GetFreePlayersIds()) {
        AddEventToSend(GetGameInfo(player_id));
      }

      break;
    }
    case GameStatus::kEnded: {
      emit GameEnded(model_.GetProgress());

      break;
    }
    default: {}
  }
}

void GameController::StartMinigameEvent(MinigameType type) {
  AbstractMinigame* minigame;
  auto& players = model_.GetPlayersForMinigame(type);

  model_.MakePlayersBusy(players);

  switch (type) {
    case MinigameType::kSample: {
      minigame = new SampleMinigame(players);
      break;
    }
    default: {}
  }

  model_.AddCreatedMinigame(type, minigame);

  connect(minigame,
          &AbstractMinigame::MinigameEnded,
          this,
          &GameController::MinigameEndedEvent);
}

void GameController::MinigameEndedEvent(MinigameType type, uint64_t score) {
  model_.AddScore(score);
  model_.DeleteMinigame(type);
}

events::EventWrapper GameController::GetGameInfo(UserId player_id) const {
  auto* game_info = new server_events::GameInfo();

  game_info->set_progress(model_.GetProgress());

  for (const auto& [minigame_type, players] : model_.GetPlayersByMinigame()) {
    auto* minigame_info = game_info->add_minigames_info();
    minigame_info->set_id(static_cast<MinigameId>(minigame_type));
    minigame_info->set_num_of_joined(players.size());
  }

  auto joined_game = model_.GetMinigameTypeByPlayerId(player_id);
  if (joined_game.has_value()) {
    game_info->set_joined_minigame(
        static_cast<MinigameId>(joined_game.value()));
  }

  auto* server_event = new server_events::ServerEventWrapper();
  server_event->set_allocated_game_info(game_info);
  server_event->set_receiver_id(player_id);

  events::EventWrapper event;
  event.set_allocated_server_event(server_event);

  return event;
}
