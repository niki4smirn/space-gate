#include <QApplication>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.show();
  window.setWindowTitle("SpaceGate");

  return QApplication::exec();
}
