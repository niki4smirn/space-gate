#include <QCoreApplication>

#include "Controller/server_controller.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);

  auto server_controller = std::make_unique<ServerController>();
  qInfo() << "Server started";

  return QCoreApplication::exec();
}
