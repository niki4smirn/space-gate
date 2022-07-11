#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "Resources/Fonts/fonts.h"

#include "Resources/Fonts/fonts.h"

class LightEffect : public QWidget {
 Q_OBJECT

 public:
  LightEffect(QWidget* parent = nullptr);

 private:
  QGridLayout* interface_layout_;
  QLabel* game_name_;
};
