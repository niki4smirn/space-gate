#include <QApplication>
//#include <QUrl>

//#include "Constants/constants.h"
//#include "Controller/client_controller.h"
#include "client_view.h"

int main(int argc, char* argv[]) {
   QApplication app(argc, argv);
  // QString url_str = "ws://";
  // if (argc > 1) {
  //   url_str += argv[1];
  // } else {
  //   url_str += constants::kDefaultServerIp;
  // }
  // url_str += ":" + QString::number(constants::kDefaultPort);
  // QUrl server_url(url_str);
  // auto* client_controller = new ClientController(server_url);
  ClientView window;
  return QApplication::exec();
}
