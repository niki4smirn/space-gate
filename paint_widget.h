#ifndef PAINT_WIDGET_H_
#define PAINT_WIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>

#include <vector>
#include <iostream>

#include "stars.h"

class PaintWidget : public QWidget {
 Q_OBJECT
 public:
  explicit PaintWidget(QWidget* parent);
  void Paint(QPainter* painter) const;
  void Tick();
  void GenerateStars();
  void RemoveStars();

 private:
  std::vector<QColor> colors_ = {QColor(51, 255, 255, 200), QColor(51, 153, 255, 200)};
  std::vector<Star> stars_;
};

#endif //PAINT_WIDGET_H_
