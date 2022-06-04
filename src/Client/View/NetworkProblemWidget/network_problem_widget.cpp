#include <QFontDatabase>
#include <QApplication>
#include <QScreen>
#include "network_problem_widget.h"

NetworkProblemWidget::NetworkProblemWidget(QWidget* parent) :
    QWidget(parent),
    background_widget_(new BackgroundWidget(this)),
    background_layout_(new QGridLayout()),
    interface_(new QWidget(this)),
    interface_layout_(new QGridLayout()),
    text_(new QLabel("Network problems", this)),
    reconnect_button_(new QPushButton("Reconnect", this)) {
  background_widget_->setFixedSize(QApplication::screens()[0]->size().width(),
                                   QApplication::screens()[0]->size().height());
  QString family =
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
          "../Resources/Fonts/Paladins.otf")).at(0);
  QFont font(family);
  font.setPointSize(55);
  font.setBold(true);

  text_->setStyleSheet("QLabel {color : #88bcff; }");
  text_->setFont(font);

  reconnect_button_->setFixedSize(550, 250);
  reconnect_button_->setFont(font);
  reconnect_button_->setStyleSheet(
      "background-color: rgb(136, 247, 255);font-size: 40px;");

  background_layout_->setContentsMargins(0, 0, 0, 0);
  interface_layout_->setSpacing(50);

  interface_layout_->addWidget(text_, 0, 0, Qt::AlignCenter);
  interface_layout_->addWidget(reconnect_button_, 1, 0,
                               Qt::AlignHCenter | Qt::AlignVCenter);

  interface_->setLayout(interface_layout_);

  setMouseTracking(true);
  background_widget_->setMouseTracking(true);
  interface_->setMouseTracking(true);
  text_->setMouseTracking(true);
  reconnect_button_->setMouseTracking(true);

  background_layout_->addWidget(background_widget_, 0, 0);
  background_layout_->addWidget(interface_, 0, 0);
  setLayout(background_layout_);

  connect(reconnect_button_, &QPushButton::clicked, [&]() {
    emit Reconnect();
  });
}

void NetworkProblemWidget::SetCenterPos(const QPoint& pos) {
  background_widget_->SetCenterPos(pos);
}
