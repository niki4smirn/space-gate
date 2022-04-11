#ifndef STARS_H_
#define STARS_H_

#include <QRandomGenerator>
#include <QMouseEvent>
#include <QColor>
#include <QSize>
#include <QPoint>

#include <iostream>
#include <math.h>

class Star {
 public:
  Star(QSize size, QColor color, QPointF center);

  QPointF GetViewPoint() const;
  double GetSize() const;
  QColor GetColor() const;

  static double GetTime();
  static double GetAngle();
  static void AddTime(double add);
  static void SetTime(double set);

  void Move();
  void MoveCenter(QMouseEvent* event);

 private:

  double z_distance_;
  double size_;
  QSize window_size_;
  QColor color_;
  static double time_;
  static double angle_;
  QPointF prev_pos_;
  QPointF coords_;

  double velocity_ = 2;
  int center_size_ = 20;
  double minimum_z_distance_ = 200;
  double max_random_z_distance_ = 200;
  double max_size_ = 10;
};

#endif  // STARS_H_
