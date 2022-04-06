#ifndef PAINT_WIDGET_H_
#define PAINT_WIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QPen>

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

  void SetState(bool state);

 private:
  std::vector<std::pair<QPoint, QPoint>> lines_;
  int white_blur_ = 0;
  std::vector<QColor> colors_ =
    {QColor(51, 255, 255, 200), QColor(51, 153, 255, 200),
     QColor(51, 255, 53, 200), QColor(204, 255, 153, 200),
     QColor(255, 204, 229, 200), QColor(204, 153, 255, 200),
     QColor(0, 0, 204, 200), QColor(255, 204, 153, 200)};
  std::vector<Star> stars_;
  bool state_ = 0;
};

#endif //PAINT_WIDGET_H_
