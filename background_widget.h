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

class BackgroundWidget : public QWidget {
 Q_OBJECT
 public:
  explicit BackgroundWidget(QWidget* parent);
  void Paint(QPainter* painter) const;
  void Tick();
  void GenerateStars();
  void RemoveStars();

  void SetState(bool state);

  void PaintStars(QPainter* painter) const;
  void PaintLines(QPainter* painter) const;
  void PaintBackground(QPainter* painter) const;
  void PaintBlur(QPainter* painter) const;


 private:
  std::vector<QColor> colors_ =
    {QColor(51, 255, 255, 200), QColor(51, 153, 255, 200),
     QColor(51, 255, 53, 200), QColor(204, 255, 153, 200),
     QColor(255, 204, 229, 200), QColor(204, 153, 255, 200),
     QColor(0, 0, 204, 200), QColor(255, 204, 153, 200)};
  std::vector<Star> stars_;
  std::vector<std::pair<QPoint, QPoint>> lines_;
  bool light_speed_effect_state_ = false;
  double white_blur_ = 0;
  double blur_acceleration_ = 0.5;
  double star_time_acceleration_ = 0.01;
  double max_star_interval_ = 5;
  double default_star_interval_;
  int stars_number_ = 1000;
  double max_line_size_ = 10;
  int width_for_big_stars_ = 3;
  int max_shake_ = 10;

};

#endif //PAINT_WIDGET_H_
