#include <QCoreApplication>

#include "src/Server/Controllers/ServerController/server_controller.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);

  auto server_controller = std::make_unique<ServerController>();
  LOG << "Server started";

  return QCoreApplication::exec();
}
