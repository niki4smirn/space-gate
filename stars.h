#ifndef STARS_H_
#define STARS_H_

#include <QRandomGenerator>

#include <iostream>

class Star {
 public:

  Star(int width, int height);

  double GetXViewDistance() const;
  double GetYViewDistance() const;
  double GetZDistance() const;

  int GetSize() const;
  void Move();

 private:
  double GetXViewVeilocity() const;
  double GetYViewVeilocity() const;

  double velocity_;
  double z_distance_;
  double x_distance_;
  double y_distance_;
  double x_distance_view_;
  double y_distance_view_;
  double size_;
  int time_;
};

#endif //STARS_H_
