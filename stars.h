#ifndef STARS_H_
#define STARS_H_

#include <QRandomGenerator>
#include <QMouseEvent>
#include <QColor>
#include <QSize>
#include <QPoint>

#include <iostream>

class Star {
 public:
  Star(QSize size, QColor color, QPointF center);

  double GetXViewDistance() const;
  double GetYViewDistance() const;
  QPointF GetViewPoint() const;
  double GetZDistance() const;
  double GetSize() const;
  QColor GetColor() const;

  static double GetTime();
  static void AddTime(double add);
  static void SetTime(double set);

  void Move();
  void MoveCenter(QMouseEvent* event);

 private:
  double XViewVelocity() const;
  double YViewVelocity() const;

  double z_distance_;
  double size_;
  QColor color_;
  static double time_;
  QPointF prev_pos_;
  QPointF coords_ = QPointF(0, 0);
  QPointF view_coords_;

  double velocity_ = 2;
  int center_size_ = 100;
  double minimum_z_distance_ = 200;
  double max_random_z_distance_ = 200;
  double max_size_ = 10;
};

#endif  // STARS_H_
