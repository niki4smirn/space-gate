#pragma once

#include "client_main_menu.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>

class ClientView : public QMainWindow {
  Q_OBJECT
 public:
  ClientView();
  void MenuUpdatePlayerList();

 private:
  void CloseWindow();
  void AddWidgets();
  void Connect();

 private:
  ClientMainMenu* main_menu_;
  QStackedWidget* stacked_widget_;

  signals:
  void ReadyButtonPressed();
};
