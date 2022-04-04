#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    widget_(new PaintWidget(this))
    {
  //widget_->resize(100, 100);
  resize(1000, 700);
  SetupScene();
  animation_timer_.start(50, this);
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
  //connect(this, &MainWindow::resize_signal, this, &MainWindow::Resize);
}
void MainWindow::timerEvent(QTimerEvent* event) {
  if (event->timerId() == animation_timer_.timerId()) {
    QObject::timerEvent(event);
    widget_->Tick();
    repaint();
  }
}
