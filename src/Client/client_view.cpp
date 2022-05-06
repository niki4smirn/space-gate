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
          [=](){emit ReadyButtonPressed();});
}

void ClientView::AddWidgets() {
  stacked_widget_->addWidget(main_menu_);
}

void ClientView::MenuUpdatePlayerList() {

}
