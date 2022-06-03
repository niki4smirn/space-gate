#pragma once

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class MainGameWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MainGameWidget(QWidget* parent = nullptr);

 signals:
  void JoinMinigame(int minigame_index);

 private:
  QPushButton* button1_;
  QPushButton* button2_;

  QGridLayout* layout_;
};
