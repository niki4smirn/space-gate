#include "stars.h"

Star::Star(int width, int height) {
  velocity_ = 2;
  z_distance_ = 500;
  x_distance_ = QRandomGenerator::global()->bounded(width * 2) - width + 40;
  y_distance_ = QRandomGenerator::global()->bounded(height * 2) - height  + 40;
  size_ = QRandomGenerator::global()->bounded(10);
  time_ = 1;
  x_distance_view_ = x_distance_;
  y_distance_view_ = y_distance_;
}
double Star::GetXViewVeilocity() const{
  double square_hypotenyze = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  if (square_hypotenyze > 0) {
    return velocity_ * (x_distance_ / sqrt(square_hypotenyze));
  } else{
    return 0;
  }
}

double Star::GetYViewVeilocity() const{
  double square_hypotenyze = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  if (square_hypotenyze > 0) {
    return velocity_ * (y_distance_ / sqrt(square_hypotenyze));
  } else{
    return 0;
  }
}


void Star::Move() {
  int old_distance = sqrt(x_distance_ * x_distance_ + z_distance_ * z_distance_ + y_distance_ * y_distance_);
  z_distance_ -= velocity_ * time_;
  x_distance_view_ += GetXViewVeilocity() * time_;
  y_distance_view_ += GetYViewVeilocity() * time_;

  size_ = size_ * old_distance / sqrt(x_distance_ * x_distance_ + z_distance_ * z_distance_ + y_distance_ * y_distance_);

}

int Star::GetSize() const{
  return size_;
}
double Star::GetXViewDistance() const {
  return x_distance_view_;
}
double Star::GetYViewDistance() const {
  return y_distance_view_;
}
double Star::GetZDistance() const {
  return z_distance_;
}
