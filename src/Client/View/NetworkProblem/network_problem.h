#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "src/Client/View/MainMenu/Background/background_widget.h"

class NetworkProblemWidget : public QWidget {
  Q_OBJECT

 public:
  explicit NetworkProblemWidget(QWidget* parent = nullptr);

  void SetCenterPos(const QPoint& pos);

 signals:
  void Reconnect();

 private:
  BackgroundWidget* background_widget_;
  QGridLayout* background_layout_;

  QWidget* interface_;
  QGridLayout* interface_layout_;
  QLabel* text_;
  QPushButton* reconnect_button_;
};
