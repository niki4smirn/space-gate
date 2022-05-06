#pragma once

#include <QRandomGenerator>
#include <QMouseEvent>
#include <QColor>
#include <QSize>
#include <QPoint>

#include <iostream>
#include <math.h>

class Star {
 public:
  Star(QSize window_size, QColor color, QPointF center);

  QPointF GetViewPoint() const;
  double GetSize() const;
  QColor GetColor() const;

  static double GetTime();
  static void AddTime(double add);
  static void SetTime(double set);

  void Move();
  void MoveCenter(QMouseEvent* event);
  bool Remove(QPointF coords, QPointF center);

 private:
  double z_distance_;
  double size_;
  QSize window_size_;
  QColor color_;
  static double time_;
  static double angle_x_;
  static double angle_y_;
  QPointF prev_pos_;
  QPointF coords_;

  double velocity_ = 2;
  double minimum_z_distance_ = 200;
  double max_random_z_distance_ = 1000;
  double max_size_ = 6;
};
