#include "stars.h"
Star::Star(int width) {
  velocity_ = 5;
  radial_distance_ = 500;
  tangential_distance_ = QRandomGenerator::global()->bounded(width) + 40;
  size_ = QRandomGenerator::global()->bounded(5);
  time_ = 1;
  tangential_distance_view_ = tangential_distance_;
}
double Star::GetTangentialVeilocity() const{
  double square_hypotenyze = tangential_distance_ * tangential_distance_ + radial_distance_ * radial_distance_;
  if (square_hypotenyze > 0) {
    return velocity_ * (tangential_distance_ / sqrt(square_hypotenyze));
  } else{
    return 0;
  }
}


// double Star::GetLineOfSightVeilocity() {
//   double square_hypotenyze = tangential_distance_ * tangential_distance_ + radial_distance_ * radial_distance_;
//   if (square_hypotenyze != 0) {
//     return velocity_ * (radial_distance_ / sqrt(square_hypotenyze));
//   } else{
//     return 0;
//   }
// }
void Star::Move() {
  int old_radial_distance = radial_distance_;

  radial_distance_ -= velocity_ * time_;
  tangential_distance_view_ += GetTangentialVeilocity() * time_;

  std::cout << old_radial_distance << " " << old_radial_distance << "\n";
  size_ = size_ * old_radial_distance / radial_distance_;

  //std::cout << size_ << "\n";
}
int Star::GetTangentialDistance() const{
  //std::cout << tangential_distance_view_ << "\n";
  return tangential_distance_view_;
}
int Star::GetSize() const{
  return size_;
}
int Star::GetRadialDistance() const {
  return radial_distance_;
}

