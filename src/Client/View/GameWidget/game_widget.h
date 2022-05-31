#pragma once

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include "src/Client/View/GameWidget/MainGameWidget/main_game_widget.h"
#include "src/Client/View/GameWidget/WaitingScreen/waiting_screen.h"

class GameWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GameWidget(QWidget* parent = nullptr);

 signals:
  void JoinMinigame(int minigame_index);
  void LeaveMinigame();

 private:
  void OpenMainGameWidget();
  void OpenWaitingScreen();

  QStackedWidget* stacked_widget_;
  MainGameWidget* main_game_widget_;
  WaitingScreen* waiting_screen_;
};
