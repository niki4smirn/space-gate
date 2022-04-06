#ifndef STARS_H_
#define STARS_H_

#include <QRandomGenerator>
#include <QColor>

#include <iostream>

class Star {
 public:

  Star(int width, int height, QColor color);

  double GetXViewDistance() const;
  double GetYViewDistance() const;
  double GetZDistance() const;
  double GetSize() const;
  QColor GetColor() const;

  static double GetTime();
  static void AddTime(double add);
  static void SetTime(double set);

  void Move();


 private:
  double XViewVeilocity() const;
  double YViewVeilocity() const;

  double velocity_;
  double z_distance_;
  double x_distance_;
  double y_distance_;
  double x_distance_view_;
  double y_distance_view_;
  double size_;
  QColor color_;
  static double time_;
};


#endif //STARS_H_
