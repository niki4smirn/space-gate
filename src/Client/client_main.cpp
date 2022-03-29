#include <QApplication>
#include <QUrl>

#include "Controller/client_controller.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // TODO: remove hardcoded url
  QUrl server_url("ws://localhost:1337");
  auto* client_controller = new ClientController(server_url);
  return QApplication::exec();
}
