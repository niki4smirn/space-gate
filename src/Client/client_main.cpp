#include <QApplication>
#include <QUrl>

#include "src/Helpers/Constants/constants.h"
#include "Controller/client_controller.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QString url_str = "ws://";
  if (argc > 1) {
    url_str += argv[1];
  } else {
    url_str += constants::kDefaultServerIp;
  }
  url_str += ":" + QString::number(constants::kDefaultPort);
  QUrl server_url(url_str);
  auto client_controller = std::make_unique<ClientController>(server_url);
  return QApplication::exec();
}
