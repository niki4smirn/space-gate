#ifndef SPACE_GATE_CLIENT_VIEW_CONTROLLER_H_
#define SPACE_GATE_CLIENT_VIEW_CONTROLLER_H_

#include "client_view.h"
#include "client_main_menu.h"
#include "Protobuf/client_events.pb.h"
#include "Protobuf/server_events.pb.h"

class ClientViewController {
 public:
  ClientViewController();
  void RecieveData(const QByteArray& message);
 private:

 private:
  ClientView* view_;
  ClientMainMenu* main_menu_;
};

#endif //  SPACE_GATE_CLIENT_VIEW_CONTROLLER_H_
