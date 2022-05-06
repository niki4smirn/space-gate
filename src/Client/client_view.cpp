#include "client_view.h"
ClientView::ClientView() :
    stacked_widget_(new QStackedWidget),
    main_menu_(new ClientMainMenu){
  AddWidgets();
  stacked_widget_->setCurrentWidget(main_menu_);
  setCentralWidget(stacked_widget_);
  showFullScreen();
  Connect();
}

void ClientView::CloseWindow() {
  close();
}

void ClientView::Connect() {
  connect(main_menu_, &ClientMainMenu::Close, this, &ClientView::CloseWindow);
  connect(main_menu_,
          &ClientMainMenu::ReadyButtonPressed,
          [this](){emit ReadyButtonPressed();});
  connect(main_menu_,
          &ClientMainMenu::CreateRoomSignal,
          [this](){emit CreateRoom();});
  connect(main_menu_,
          &ClientMainMenu::LeaveRoom,
          [this](){emit LeaveRoom();});
}

void ClientView::AddWidgets() {
  stacked_widget_->addWidget(main_menu_);
}

void ClientView::MenuUpdatePlayerList(const server_events::RoomInfo& room_info) {
  main_menu_->UpdatePlayerList(room_info);
}
