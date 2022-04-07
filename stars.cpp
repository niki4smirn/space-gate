#include "stars.h"

double Star::time_ = 1;

Star::Star(int width, int height, QColor color) {
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_) + minimum_z_distance_;
  while (y_distance_ * y_distance_view_ + x_distance_ * x_distance_ < center_size_ * center_size_){
    x_distance_ = QRandomGenerator::global()->bounded(width * 2) - width;
    y_distance_ = QRandomGenerator::global()->bounded(height * 2) - height;
  }
  size_ = QRandomGenerator::global()->bounded(max_size_);
  color_ = std::move(color);
  x_distance_view_ = x_distance_;
  y_distance_view_ = y_distance_;
}

double Star::XViewVelocity() const{
  double square_hypotenuse = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  return velocity_ * (x_distance_ / sqrt(square_hypotenuse));
}

double Star::YViewVelocity() const{
  double square_hypotenuse = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  return velocity_ * (y_distance_ / sqrt(square_hypotenuse));
}


void Star::Move() {
  double old_distance = sqrt(x_distance_ * x_distance_ + z_distance_ * z_distance_ + y_distance_ * y_distance_);
  z_distance_ -= velocity_ * time_;
  x_distance_view_ += XViewVelocity() * time_;
  y_distance_view_ += YViewVelocity() * time_;
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
