#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    widget_(new BackgroundWidget(this))
    {
  // widget_->resize(100, 100);
  setMouseTracking(true);
  this->setMouseTracking(true);
  widget_->setMouseTracking(true);
  resize(1000, 700);
  SetupScene();
  Connect();
  animation_timer_.start(5, this);
}


void MainWindow::SetupScene() {
  setCentralWidget(widget_);
  repaint();
}

void MainWindow::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  widget_->Paint(&painter);
}

void MainWindow::Connect() {
  connect(this, &MainWindow::MousePressed,
          widget_, &BackgroundWidget::SetState);
  connect(this, &MainWindow::MouseReleased,
          widget_, &BackgroundWidget::SetState);
  connect(this, &MainWindow::MouseMove,
          widget_, &BackgroundWidget::SetCenterPos);
}
void MainWindow::timerEvent(QTimerEvent* event) {
  if (event->timerId() == animation_timer_.timerId()) {
    QObject::timerEvent(event);
    widget_->Tick();
    repaint();
  }
}
void MainWindow::mousePressEvent(QMouseEvent*) {
  emit MousePressed(true);
}
void MainWindow::mouseReleaseEvent(QMouseEvent*) {
  emit MouseReleased(false);
}
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  emit MouseMove(event);
}
