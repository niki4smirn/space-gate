#include "client_view_controller.h"

ClientViewController::ClientViewController() :
  main_menu_(new ClientMainMenu){
  view_ = new ClientView(main_menu_);

}
void ClientViewController::RecieveData(const QByteArray& message) {
  server_events::ServerEventWrapper data;
  data.ParseFromArray(message, message.size());
  switch (data.type_case()) {
    case server_events::ServerEventWrapper::kRoomInfo:{
      main_menu_->UpdatePlayerList(data.room_info());
      break;
    }
    default: {}
  }

}
