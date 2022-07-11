#include "client_view.h"
#include "src/Helpers/logging.h"

ClientView::ClientView() :
    stacked_widget_(new QStackedWidget(this)),
    main_menu_(new ClientMainMenu(this)),
    game_view_(new GameView(this)),
    terminal_minigame_view_(new TerminalMinigameView(this)),
    hole_repair_minigame_view_(new HoleRepairView(this)),
    final_screen_(new FinalScreen(this)),
    input_controller_(new InputController),
    network_problem_widget_(new NetworkProblemWidget(this)) {
  AddWidgets();
  stacked_widget_->setCurrentWidget(main_menu_);
  setCentralWidget(stacked_widget_);
  showFullScreen();
  Connect();
  setMouseTracking(true);
  stacked_widget_->setMouseTracking(true);
  main_menu_->setMouseTracking(true);
  game_view_->setMouseTracking(true);
}

void ClientView::mouseMoveEvent(QMouseEvent* event) {
  main_menu_->SetCenterPos(event->pos());
  network_problem_widget_->SetCenterPos(event->pos());
  input_controller_->MouseMove(event->pos());
}

void ClientView::CloseWindow() {
  close();
}

void ClientView::Connect() {
  connect(main_menu_, &ClientMainMenu::Close, this, &ClientView::CloseWindow);
  connect(main_menu_,
          &ClientMainMenu::ReadyButtonPressed,
          [this]() { emit ReadyButtonPressed(); });
  connect(main_menu_,
          &ClientMainMenu::CreateRoomSignal,
          [this]() { emit CreateRoom(); });
  connect(main_menu_,
          &ClientMainMenu::LeaveRoom,
          [this]() { emit LeaveRoom(); });
  connect(main_menu_,
          &ClientMainMenu::JoinRoomSignal,
          [this](uint64_t room_id) { emit JoinRoom(room_id); });
  connect(main_menu_,
          &ClientMainMenu::StartGame,
          [this]() { emit StartGame(); });
  connect(input_controller_.get(),
          &InputController::KeyEventToServer,
          [this](input::Name key) { emit KeyEventToServer(key); });
  connect(input_controller_.get(),
          &InputController::MouseMoveToServer,
          [this](const QPoint& pos) { emit MouseMoveToServer(pos); });
  connect(input_controller_.get(),
          &InputController::MouseKeyToServer,
          [this](input::Name key) { emit KeyEventToServer(key); });
  connect(game_view_, &GameView::JoinMinigame,
          [this](int minigame_menu_pos) {
    emit JoinMinigame(minigame_menu_pos);
  });
  connect(game_view_, &GameView::LeaveMinigame, [&]() {
    emit LeaveMinigame();
  });
  connect(final_screen_, &FinalScreen::MenuPressed, [this]() {
    stacked_widget_->setCurrentWidget(main_menu_);
    main_menu_->BackToStart();
    emit LeaveRoom();
  });
  connect(final_screen_, &FinalScreen::LobbyPressed, [this]() {
    stacked_widget_->setCurrentWidget(main_menu_);
    main_menu_->BackToLobby();
  });
  connect(network_problem_widget_, &NetworkProblemWidget::Reconnect, [&]() {
    emit Reconnect();
  });
  connect(hole_repair_minigame_view_,
          &HoleRepairView::MousePos,
          [&](QPointF pos) {
            emit HoleRepairMousePos(pos);
          });
  connect(hole_repair_minigame_view_,
          &HoleRepairView::PlatePos,
          [&](QPointF pos) {
            emit HoleRepairPlatePos(pos);
          });
}

void ClientView::AddWidgets() {
  stacked_widget_->addWidget(terminal_minigame_view_);
  stacked_widget_->addWidget(hole_repair_minigame_view_);
  stacked_widget_->addWidget(main_menu_);
  stacked_widget_->addWidget(game_view_);
  stacked_widget_->addWidget(final_screen_);
  stacked_widget_->addWidget(network_problem_widget_);
}

void ClientView::UpdateRoomInfoMenu(
    const server_events::RoomInfo& room_info,
    uint64_t client_id) {
  main_menu_->UpdatePlayersList(room_info);
  main_menu_->UpdateInterface(client_id == room_info.chief_id());
}

void ClientView::UpdateRoomsListMenu(
    const server_events::RoomsList& room_list) {
  main_menu_->UpdateRoomsList(room_list);
}

void ClientView::PlayStartEffect() {
  main_menu_->PlayStartEffect();
  QTimer::singleShot(constants::kStartAnimationDuration, [&]() {
    OpenGame();
  });
}

void ClientView::keyPressEvent(QKeyEvent* event) {
  QWidget::keyPressEvent(event);
  input_controller_->KeyPressed(event->nativeScanCode());
}

void ClientView::mousePressEvent(QMouseEvent* event) {
  input_controller_->MousePosStartTracking();
  input_controller_->MouseKeyPressed(event->button());
}

void ClientView::mouseReleaseEvent(QMouseEvent* event) {
  input_controller_->MousePosStopTracking();
}

void ClientView::OpenGame() {
  stacked_widget_->setCurrentWidget(game_view_);
}

void ClientView::UpdateProgress(uint64_t progress) {
  game_view_->SetProgress(progress, constants::kScoreToFinish);
}

void ClientView::UpdateMinigameBulbs(
    int minigame_pos, int waiting_count) {
  switch (minigame_pos) {
    case 0: {
      game_view_->TurnOffGreen();
      break;
  }
    case 1: {
      game_view_->TurnOffRed();
      break;
    }
    case 2: {
      game_view_->TurnOffPurple();
      break;
    }
    case 3: {
      game_view_->TurnOffBlue();
      break;
    }
    default: {}
  }
  game_view_->SetBulbsCount(minigame_pos, waiting_count);
}

void ClientView::UpdateMinigame(
    const minigame_responses::MinigameResponse& response) {

  switch (response.type_case()) {
    case minigame_responses::MinigameResponse::kInitialTerminalRespone: {
      if (stacked_widget_->currentWidget() != terminal_minigame_view_) {
        stacked_widget_->setCurrentWidget(terminal_minigame_view_);
      }

      terminal_minigame_view_->InitializeView(response);

      break;
    }
    case minigame_responses::MinigameResponse::kTerminalResponse: {
      if (stacked_widget_->currentWidget() != terminal_minigame_view_) {
        stacked_widget_->setCurrentWidget(terminal_minigame_view_);
      }

      terminal_minigame_view_->UpdateView(response);

      break;
    }
    case minigame_responses::MinigameResponse::kInitialHoleRepairResponse: {
      if (stacked_widget_->currentWidget() != hole_repair_minigame_view_) {
        stacked_widget_->setCurrentWidget(hole_repair_minigame_view_);
      }

      hole_repair_minigame_view_->InitializeView(response);

      break;
    }
    case minigame_responses::MinigameResponse::kHoleRepairResponse: {
      if (stacked_widget_->currentWidget() != hole_repair_minigame_view_) {
        stacked_widget_->setCurrentWidget(hole_repair_minigame_view_);
      }

      hole_repair_minigame_view_->UpdateView(response);

      break;
    }
    case minigame_responses::MinigameResponse::kResult: {
      stacked_widget_->setCurrentWidget(game_view_);
      break;
    }
    default: {}
  }
}

std::optional<MinigameType> ClientView::IsMinigameStarted() {
  if (stacked_widget_->currentWidget() == terminal_minigame_view_) {
    return MinigameType::kTerminal;
  }

  if (stacked_widget_->currentWidget() == hole_repair_minigame_view_) {
    return MinigameType::kHoleRepair;
  }

  return std::nullopt;
}

void ClientView::ShowNetworkProblemWidget() {
  stacked_widget_->setCurrentWidget(network_problem_widget_);
}

void ClientView::ShowMainMenu() {
  stacked_widget_->setCurrentWidget(main_menu_);
  main_menu_->BackToStart();
}

void ClientView::ShowFinalScreen(bool is_win) {
  auto call_final_screen = [&]() {
    final_screen_->SetResult(is_win);
    stacked_widget_->setCurrentWidget(final_screen_);
  };
  if (is_win) {
    call_final_screen();
  } else {
    game_view_->EndGame();
    QTimer::singleShot(2500, call_final_screen);
  }
}

void ClientView::ResetAllBulbs() {
  game_view_->TurnOffGreen();
  game_view_->TurnOffRed();
  game_view_->TurnOffPurple();
  game_view_->TurnOffBlue();
}
