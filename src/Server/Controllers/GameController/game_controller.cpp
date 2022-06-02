#include "game_controller.h"

#include "src/Helpers/Constants/constants.h"

GameController::GameController(
    const std::vector<std::shared_ptr<User>>& players)
  : model_(players) {
  connect(&model_, &GameModel::SendGameInfo,
          this, &GameController::SendGameInfoEvent);
  connect(&model_, &GameModel::StartMinigame,
          this, &GameController::StartMinigameEvent);

  // just let it be :)
  model_.SetProgress(constants::kScoreToFinish - 1);

  QTimer::singleShot(constants::kStartAnimationDuration, [&]() {
    StartTicking();
  });
}

QString GameController::GetControllerName() const {
  return "Game";
}

void GameController::Handle(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kHandle);

  const auto& client_event = event.client_event();
  const auto& game_event = client_event.event_to_game();

  UserId user_id = client_event.sender_id();

  switch (game_event.type_case()) {
    case client_events::EventToGame::kJoinMinigame: {
      if (!model_.IsPlayerBusy(client_event.sender_id())) {
        MinigameId minigame_id = game_event.join_minigame().minigame_id();
        model_.AddPlayerToMinigameQueue(client_event.sender_id(),
                                        static_cast<MinigameType>(minigame_id));
      }

      break;
    }
    case client_events::EventToGame::kMinigameAction: {
      MinigameId minigame_id = game_event.join_minigame().minigame_id();
      auto minigame =
          model_.GetMinigameByType(static_cast<MinigameType>(minigame_id));

      minigame->AddEventToHandle(event);

      break;
    }
    case client_events::EventToGame::kLeaveMinigame: {
      model_.DeleteMinigamePlayer(user_id);
      break;
    }
    default: {}
  }
}

void GameController::OnTick() {
  ++ticks_;

  if (ticks_ % constants::kMinigamesAddingTickFrequency == 0
      && model_.GetMinigamesCount() < constants::kMaxMinigamesCount) {
    auto range = helpers::Range(MinigameType::kSample, MinigameType::kLast);
    auto type = helpers::GetRandomInRange<MinigameType>(range);

    model_.AddMinigame(type);
  }

  if (ticks_ % constants::kGameDecreaseTickFrequency == 0) {
    model_.DecreaseProgress();
  }

  auto progress = model_.GetProgress();
  if (progress == 0 || progress >= constants::kScoreToFinish) {
    FinishGame(progress);
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
  std::shared_ptr<AbstractMinigame> minigame;
  auto& players = model_.GetPlayersForMinigame(type);

  switch (type) {
    case MinigameType::kSample: {
      minigame = std::make_shared<SampleMinigame>(players);
      break;
    }
    default: {}
  }

  model_.AddCreatedMinigame(type, minigame);

  connect(minigame.get(),
          &AbstractMinigame::MinigameEnded,
          this,
          &GameController::MinigameEndedEvent);
}

void GameController::MinigameEndedEvent(MinigameType type, uint64_t score) {
  disconnect(model_.GetMinigameByType(type).get(),
          &AbstractMinigame::MinigameEnded,
          this,
          &GameController::MinigameEndedEvent);

  SendMinigameEndedEvent(type, score);

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

void GameController::FinishGame(uint64_t score) {
  auto minigames = model_.GetAllMinigames();
  for (const auto& [minigame_type, minigame] : minigames) {
    MinigameEndedEvent(minigame_type, 0);
  }
  emit GameEnded(score);
}

void GameController::SendMinigameEndedEvent(MinigameType type, uint64_t score) {
  events::EventWrapper event;

  auto* server_event = new server_events::ServerEventWrapper;

  auto* game_response = new minigame_responses::MinigameResponse;
  minigame_responses::MinigameResponse::Result result;
  if (score == 0) {
    result = minigame_responses::MinigameResponse::kFailed;
  } else {
    result = minigame_responses::MinigameResponse::kCompleted;
  }
  game_response->set_result(result);

  server_event->set_allocated_game_response(game_response);

  event.set_allocated_server_event(server_event);

  const auto& players = model_.GetPlayersByMinigame().at(type);
  for (const auto& player : players) {
    server_event->set_receiver_id(player->GetId());
    AddEventToSend(event);
  }
}
