#include "game_controller.h"

GameController::GameController() {
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
      model_.AddPlayerToMinigame(client_event.sender_id(),
                                 game_event.join_minigame().minigame_id());

      break;
    }
    case client_events::EventToGame::kMinigameAction: {
      AbstractMinigame* minigame =
          model_.GetMinigameById(game_event.minigame_action().id());

      minigame->AddEventToHandle(event);
    }
    default: {}
  }
}

void GameController::OnTick() {}

void GameController::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);

  // TODO(Andrei): implement
}

void GameController::SendGameInfoEvent() {
  // TODO(Andrei): implement
}

void GameController::StartMinigameEvent() {
  // TODO(Andrei): implement
}
