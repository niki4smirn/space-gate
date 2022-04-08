#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "background_widget.h"

#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QMainWindow>
#include <QBasicTimer>
#include <QRadialGradient>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow();

 private:
  void SetupScene();
  void Connect();
  BackgroundWidget* widget_;

};
#endif //  MAINWINDOW_H_
