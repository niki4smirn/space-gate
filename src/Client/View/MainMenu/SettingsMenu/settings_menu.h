#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>


class SettingsMenu : public QWidget {
 Q_OBJECT

 public:
  SettingsMenu(QWidget* parent = nullptr);

 private:
  void Connect();
  void InterfaceConfigure();
  void SetLayout();

 private:
  QGridLayout* interface_layout_;
  QLabel* game_name_;
  QLabel* nothing_here_;
  QPushButton* back_button_;

 signals:
  void Back();
};
