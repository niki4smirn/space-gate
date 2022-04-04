#ifndef STARS_H_
#define STARS_H_

#include <QRandomGenerator>

#include <iostream>

class Star {
 public:

  Star(int width);
  double GetTangentialVeilocity() const;
  double GetLineOfSightVeilocity();
  int GetTangentialDistance() const;
  int GetRadialDistance() const;
  int GetSize() const;
  void Move();

 private:
  int velocity_;
  int radial_distance_;
  int tangential_distance_view_;
  int tangential_distance_;
  double size_;
  int time_;
};

#endif //STARS_H_
