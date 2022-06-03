#ifndef FINAL_SCREEN_H
#define FINAL_SCREEN_H

#include "src/Client/View/MainMenu/Background/background_widget.h"

#include "QApplication"
#include "QObject"
#include "QWidget"
#include "QLabel"
#include "QScreen"
#include "QGroupBox"
#include "QPushButton"
#include "QGridLayout"
#include "QFont"
#include "QFontDatabase"

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  MainWidget(QWidget* parent = nullptr);

  ~MainWidget();

 private:
  void SetSize();
  void SetName();
  void SetLayout();
  void SetInfo();
 private:
  QGroupBox* buttons_;
  QGridLayout* layout_;
  QPushButton* to_menu_button_;
  QPushButton* to_lobby_button_;
  BackgroundWidget* background_;
  QLabel* info_;
  QLabel* game_name_;
  QFont font_;
};


#endif // FINAL_SCREEN_H
