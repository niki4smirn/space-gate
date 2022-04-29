#include "client_view.h"
ClientView::ClientView() :
  main_menu_(new ClientMainMenu),
  stacked_widget_(new QStackedWidget)
  {
  stacked_widget_->addWidget(main_menu_);
  stacked_widget_->setCurrentWidget(main_menu_);
  setCentralWidget(stacked_widget_);
  showFullScreen();
  //connect(main_menu_, &ClientMainMenu::Close, background_, &BackgroundWidget::SetState);

}
void ClientView::CloseWindow() {

}
