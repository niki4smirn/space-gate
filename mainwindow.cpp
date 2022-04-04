#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    widget_(new PaintWidget(this)),
    {

  resize(1000, 700);
}


void MainWindow::SetupScene() {
  setCentralWidget(widget_);
}

void MainWindow::paintEvent(QPaintEvent*) {
  QPainter painter(this);

}

void MainWindow::Connect() {
  //connect(this, &MainWindow::resize_signal, this, &MainWindow::Resize);
}
