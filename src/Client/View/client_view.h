#pragma once

#include "src/Client/View/MainMenu/client_main_menu.h"
#include "src/Client/InputController/input_controller.h"
#include "src/Server/Models/RoomModel/room_model.h"
#include "src/Client/View/FinalScreen/final_screen.h"
// #include "src/Client/View/GameWidget/game_widget.h"
#include "src/Client/View/FinalGameWidget/game_widget.h"
#include "src/Client/Games/TerminalMinigameView/terminal_minigame_view.h"
#include "src/Client/Games/HoleRepairMinigameView/hole_repair_view.h"
#include "src/Client/View/NetworkProblemWidget/network_problem_widget.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>

class ClientView : public QMainWindow {
  Q_OBJECT

 public:
  ClientView();
  void UpdateRoomInfoMenu(const server_events::RoomInfo& room_info,
                          uint64_t client_id);
  void UpdateRoomsListMenu(const server_events::RoomsList& room_list);
  void PlayStartEffect();
  void OpenGame();
  void UpdateProgress(uint64_t progress);
  void UpdateMinigameBulbs(int minigame_pos, int waiting_count);
  std::optional<MinigameType> IsMinigameStarted();
  void UpdateMinigame(const minigame_responses::MinigameResponse& response);
  void ResetAllBulbs();

  void ShowNetworkProblemWidget();
  void ShowMainMenu();
  void ShowFinalScreen(bool is_win);

 private:
  void CloseWindow();
  void AddWidgets();
  void Connect();
  void mouseMoveEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  std::unique_ptr<InputController> input_controller_;
  ClientMainMenu* main_menu_;
  GameWidget* game_widget_;
  FinalScreen* final_screen_;
  NetworkProblemWidget* network_problem_widget_;
  TerminalMinigameView* terminal_minigame_view_;
  HoleRepairView* hole_repair_minigame_view_;
  QStackedWidget* stacked_widget_;

 signals:
  void ReadyButtonPressed();
  void CreateRoom();
  void LeaveRoom();
  void JoinRoom(uint64_t room_id);
  void StartGame();
  void KeyEventToServer(input::Name key);
  void MouseMoveToServer(const QPoint& pos);
  void JoinMinigame(int minigame_index);
  void LeaveMinigame();
  void Reconnect();
  void HoleRepairPlatePos(QPointF pos);
  void HoleRepairMousePos(QPointF pos);
};
