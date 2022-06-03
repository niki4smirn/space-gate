#include "final_screen.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  if (parent) {
    resize(parent->size());
  }
  background_ = new BackgroundWidget(this);
  background_->setFixedSize(QApplication::screens()[0]->size().width(),
                            QApplication::screens()[0]->size().height());
  to_lobby_button_ = new QPushButton(this);
  to_menu_button_ = new QPushButton(this);
  info_ = new QLabel("Important info", this);
  info_->setFixedSize(250, 250);
  layout_ = new QGridLayout(this);
  layout_->addWidget(to_menu_button_, 1000, 200);
  layout_->addWidget(to_lobby_button_, 1000, 800);
  layout_->addWidget(info_, 200, 800);
  this->setLayout(layout_);
}

MainWidget::~MainWidget() {

}