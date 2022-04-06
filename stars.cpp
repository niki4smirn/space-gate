#include "stars.h"

double Star::time_ = 1;

Star::Star(int width, int height, QColor color) {
  velocity_ = 2;
  z_distance_ = QRandomGenerator::global()->bounded(500) + 200;
  x_distance_ = 0;
  y_distance_ = 0;
  while (y_distance_ * y_distance_view_ + x_distance_ * x_distance_ < 100 * 100){
    x_distance_ = QRandomGenerator::global()->bounded(width * 2) - width;
    y_distance_ = QRandomGenerator::global()->bounded(height * 2) - height;
  }
  size_ = QRandomGenerator::global()->bounded(10);
  color_ = color;
  x_distance_view_ = x_distance_;
  y_distance_view_ = y_distance_;
}
double Star::XViewVeilocity() const{
  double square_hypotenyze = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  if (square_hypotenyze > 0) {
    return velocity_ * (x_distance_ / sqrt(square_hypotenyze));
  } else{
    return 0;
  }
}

double Star::YViewVeilocity() const{
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
  x_distance_view_ += XViewVeilocity() * time_;
  y_distance_view_ += YViewVeilocity() * time_;

  size_ = size_ * old_distance / sqrt(x_distance_ * x_distance_ + z_distance_ * z_distance_ + y_distance_ * y_distance_);

}

double Star::GetSize() const{
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
QColor Star::GetColor() const{
  return color_;
}
void Star::AddTime(double add) {
  time_ += add;
}
void Star::SetTime(double set) {
  time_ = set;
}
double Star::GetTime(){
  return time_;
}
