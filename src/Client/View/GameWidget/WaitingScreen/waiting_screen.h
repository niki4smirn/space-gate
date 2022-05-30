#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class WaitingScreen : public QWidget {
  Q_OBJECT

 public:
  explicit WaitingScreen(QWidget* parent = nullptr);

 signals:
  void Leave();

 private:
  QLabel* text_;
  QPushButton* leave_button_;
};
