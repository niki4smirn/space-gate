#include "final_screen.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  if (parent) {
    resize(parent->size());
  }
  background_ = new BackgroundWidget(this);
  background_->setFixedSize(QApplication::screens()[0]->size().width(),
                            QApplication::screens()[0]->size().height());
}

MainWidget::~MainWidget() {

}