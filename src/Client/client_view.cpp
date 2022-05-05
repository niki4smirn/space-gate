#include "client_view.h"
ClientView::ClientView(ClientMainMenu* main_menu) :
    stacked_widget_(new QStackedWidget) {
  main_menu_ = main_menu;
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
}

void ClientView::AddWidgets() {
  stacked_widget_->addWidget(main_menu_);
}
