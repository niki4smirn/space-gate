#include "stars.h"

double Star::time_ = 1.5;

Star::Star(QSize size, QColor color, QPointF center) {
  prev_pos_ = center;
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_)
    + minimum_z_distance_;
  while (coords_.x() * coords_.x() + coords_.y() * coords_.y()
    < center_size_ * center_size_) {
    coords_.setX(
      QRandomGenerator::global()->bounded(size.width()) - center.x());
    coords_.setY(
      QRandomGenerator::global()->bounded(size.width()) - center.y());
  }
  size_ = QRandomGenerator::global()->bounded(max_size_);
  color_ = std::move(color);
  view_coords_ = coords_;
}

double Star::XViewVelocity() const {
  double square_hypotenuse =
    coords_.x() * coords_.x() + z_distance_ * z_distance_
      + coords_.y() * coords_.y();
  return velocity_ * (coords_.x() / sqrt(square_hypotenuse));
}

double Star::YViewVelocity() const {
  double square_hypotenuse =
    coords_.x() * coords_.x() + z_distance_ * z_distance_
      + coords_.y() * coords_.y();
  return velocity_ * (coords_.y() / sqrt(square_hypotenuse));
}

void Star::Move() {
  double old_distance = sqrt(
    coords_.x() * coords_.x() + z_distance_ * z_distance_
      + coords_.y() * coords_.y());
  z_distance_ -= velocity_ * time_;
  view_coords_.setX(view_coords_.x() + XViewVelocity() * time_);
  view_coords_.setY(view_coords_.y() + YViewVelocity() * time_);
  size_ = size_ * old_distance
    / sqrt(coords_.x() * coords_.x() + z_distance_ * z_distance_
             + coords_.y() * coords_.y());
}

double Star::GetSize() const {
  return size_;
}
double Star::GetXViewDistance() const {
  return view_coords_.x();
}
double Star::GetYViewDistance() const {
  return view_coords_.y();
}
double Star::GetZDistance() const {
  return z_distance_;
}
QColor Star::GetColor() const {
  return color_;
}
void Star::AddTime(double add) {
  time_ += add;
}
void Star::SetTime(double set) {
  time_ = set;
}
double Star::GetTime() {
  return time_;
}

void Star::MoveCenter(QMouseEvent* event) {
  coords_ += prev_pos_ - event->pos();
  view_coords_ += prev_pos_ - event->pos();
  prev_pos_ = event->pos();
}
QPointF Star::GetViewPoint() const {
  return view_coords_;
}
