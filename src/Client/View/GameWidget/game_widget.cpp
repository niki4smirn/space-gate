#include "game_widget.h"

GameWidget::GameWidget(QWidget* parent) :
    QWidget(parent),
    stacked_widget_(new QStackedWidget(this)),
    main_game_widget_(new MainGameWidget(this)),
    waiting_screen_(new WaitingScreen(this)) {
  stacked_widget_->addWidget(main_game_widget_);
  stacked_widget_->addWidget(waiting_screen_);
  stacked_widget_->setCurrentWidget(main_game_widget_);

  connect(main_game_widget_, &MainGameWidget::JoinMinigame,
          [&](int minigame_index) {
    OpenWaitingScreen();
    emit JoinMinigame(minigame_index);
  });

  connect(waiting_screen_, &WaitingScreen::Leave, [&]() {
    OpenMainGameWidget();
    emit LeaveMinigame();
  });
}

void GameWidget::OpenMainGameWidget() {
  stacked_widget_->setCurrentWidget(main_game_widget_);
}

void GameWidget::OpenWaitingScreen() {
  stacked_widget_->setCurrentWidget(waiting_screen_);
}
