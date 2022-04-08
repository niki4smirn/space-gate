#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    widget_(new BackgroundWidget(this)) {

  resize(1000, 700);
  SetupScene();
  Connect();
}

void MainWindow::SetupScene() {
  setCentralWidget(widget_);
}

void MainWindow::Connect() {

}
