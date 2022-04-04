#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "paint_widget.h"

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QGraphicsPixmapItem>
#include <QMainWindow>
#include <QBasicTimer>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();
  void SetupScene();
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent* event) override;


 private:
  void Connect();
  QBasicTimer animation_timer_;

  PaintWidget* widget_;


 signals:
};
#endif //MAINWINDOW_H_
