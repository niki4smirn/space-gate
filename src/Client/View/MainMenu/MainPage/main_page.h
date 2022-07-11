#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "Resources/Fonts/fonts.h"

class MainPage : public QWidget {
  Q_OBJECT

 public:
  MainPage(QWidget* parent = nullptr);

 private:
  void Connect();
  void InterfaceConfigure();
  void SetLayout();

 private:
  QGridLayout* interface_layout_;
  QPushButton* exit_button_;
  QPushButton* play_button_;
  QPushButton* settings_button_;
  QLabel* game_name_;

  signals:
  void CloseGame();
  void Play();
  void Settings();
};
