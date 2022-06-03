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
#include "QHBoxLayout"
#include "QVBoxLayout"

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  MainWidget(QWidget* parent = nullptr);

  ~MainWidget();
 private:
  BackgroundWidget* background_;
  QLabel* info_;
};


#endif // FINAL_SCREEN_H
