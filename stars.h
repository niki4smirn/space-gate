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

  Star(QSize size, QColor color);
  Star(QSize size, QColor color, QPoint center);

  double GetXViewDistance() const;
  double GetYViewDistance() const;
  QPoint GetViewPoint() const;
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
  double x_distance_ = 0;
  double y_distance_ = 0;
  double x_distance_view_;
  double y_distance_view_;
  double size_;
  QColor color_;
  static double time_;

  double velocity_ = 2;
  int center_size_ = 100;
  double minimum_z_distance_ = 200;
  double max_random_z_distance_ = 200;
  double max_size_ = 10;
  QPoint tmp_;
};

#endif // STARS_H_
