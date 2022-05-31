#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class WaitingScreen : public QWidget {
  Q_OBJECT

 public:
  explicit WaitingScreen(QWidget* parent = nullptr);

 signals:
  void Leave();

 private:
  QLabel* text_;
  QPushButton* leave_button_;

  QGridLayout* layout_;
};
