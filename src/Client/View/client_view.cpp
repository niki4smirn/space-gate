#include "client_view.h"
#include "src/Helpers/logging.h"

ClientView::ClientView() :
    stacked_widget_(new QStackedWidget(this)),
    main_menu_(new ClientMainMenu(this)),
    input_controller_(new InputController) {
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
  main_menu_->SetCenterPos(event->pos());
  input_controller_->MouseMove(event->pos());
}

void ClientView::CloseWindow() {
  close();
}

void ClientView::Connect() {
  connect(main_menu_, &ClientMainMenu::Close, this, &ClientView::CloseWindow);
  connect(main_menu_,
          &ClientMainMenu::ReadyButtonPressed,
          [this]() { emit ReadyButtonPressed(); });
  connect(main_menu_,
          &ClientMainMenu::CreateRoomSignal,
          [this]() { emit CreateRoom(); });
  connect(main_menu_,
          &ClientMainMenu::LeaveRoom,
          [this]() { emit LeaveRoom(); });
  connect(main_menu_,
          &ClientMainMenu::JoinRoomSignal,
          [this](uint64_t room_id) { emit JoinRoom(room_id); });
  connect(input_controller_.get(),
          &InputController::KeyEventToServer,
          [this](key_names::keys key) { emit KeyEventToServer(key); });
  connect(input_controller_.get(),
          &InputController::MouseMoveToServer,
          [this](const QPoint& pos) { emit MouseMoveToServer(pos); });
  connect(input_controller_.get(),
          &InputController::MouseKeyToServer,
          [this](key_names::keys key) { emit KeyEventToServer(key); });
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

void ClientView::keyPressEvent(QKeyEvent* event) {
  QWidget::keyPressEvent(event);
  input_controller_->KeyPressed(event->nativeScanCode());
}

void ClientView::mousePressEvent(QMouseEvent* event) {
  input_controller_->MousePosStartTracking();
  input_controller_->MouseKeyPressed(event->button());
}

void ClientView::mouseReleaseEvent(QMouseEvent* event) {
  input_controller_->MousePosStopTracking();
}

