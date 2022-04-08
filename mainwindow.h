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
  void SetupScene();
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent* event) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  void Connect();
  QBasicTimer animation_timer_;

  BackgroundWidget* widget_;

 signals:
  void MousePressed(bool state);
  void MouseReleased(bool state);
  void MouseMove(QMouseEvent* event);
};
#endif //  MAINWINDOW_H_
