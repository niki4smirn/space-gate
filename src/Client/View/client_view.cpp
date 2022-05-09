#include "client_view.h"
#include "src/Helpers/logging.h"

ClientView::ClientView() :
    stacked_widget_(new QStackedWidget(this)),
    main_menu_(new ClientMainMenu(this)) {
  AddWidgets();
  stacked_widget_->setCurrentWidget(main_menu_);
  setCentralWidget(stacked_widget_);
  showFullScreen();
  Connect();
  setMouseTracking(true);
  stacked_widget_->setMouseTracking(true);
  main_menu_->setMouseTracking(true);
}

void ClientView::mouseMoveEvent(QMouseEvent* event) {
  LOG << event->pos();
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
  connect(main_menu_,
          &ClientMainMenu::JoinRoomSignal,
          [this](uint64_t room_id){emit JoinRoom(room_id);});
}

void ClientView::AddWidgets() {
  stacked_widget_->addWidget(main_menu_);
}

void ClientView::MenuUpdatePlayerList(
    const server_events::RoomInfo& room_info) {
  main_menu_->UpdatePlayerList(room_info);
}

void ClientView::MenuUpdateRoomList(const server_events::RoomsList& room_list) {
  main_menu_->UpdateRoomList(room_list);
}

