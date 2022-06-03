#ifndef FINAL_SCREEN_H
#define FINAL_SCREEN_H

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
  FinalScreen(QWidget* parent = nullptr);

  ~FinalScreen();

  void SetResult(bool victory);

 private:
  void SetSize();
  void SetName();
  void SetLayout();
  void SetInfo();
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
  bool victory_;

  signals:
  void MenuPressed();
  void LobbyPressed();
};


#endif // FINAL_SCREEN_H
