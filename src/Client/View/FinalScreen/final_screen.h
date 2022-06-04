#pragma once

#include "src/Client/View/MainMenu/Background/background_widget.h"

#include "QApplication"
#include "QObject"
#include "QWidget"
#include "QLabel"
#include "QScreen"
#include "QPushButton"
#include "QGridLayout"
#include "QFont"
#include "QFontDatabase"

class FinalScreen : public QWidget {
  Q_OBJECT

 public:
  explicit FinalScreen(QWidget* parent = nullptr);

  ~FinalScreen() override = default;

  void SetResult(bool victory);

 private:
  void SetSize();
  void SetName();
  void SetLayout();
  void UpdateInfo();
  void MenuPressedEvent();
  void LobbyPressedEvent();
  void Connect();

 private:
  QGridLayout* layout_;
  QPushButton* to_menu_button_;
  QPushButton* to_lobby_button_;
  BackgroundWidget* background_;
  QLabel* info_;
  QLabel* game_name_;
  QFont font_;
  bool victory_{false};

 signals:
  void MenuPressed();
  void LobbyPressed();
};
