#include "stars.h"

double Star::time_ = 1;

Star::Star(QSize size, QColor color) {
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_)
    + minimum_z_distance_;
  while (y_distance_ * y_distance_view_ + x_distance_ * x_distance_
    < center_size_ * center_size_) {
    x_distance_ = QRandomGenerator::global()->bounded(size.width())
      - size.width() / 2;
    y_distance_ = QRandomGenerator::global()->bounded(size.height())
      - size.height() / 2;
  }
  size_ = QRandomGenerator::global()->bounded(max_size_);
  color_ = std::move(color);
  x_distance_view_ = x_distance_;
  y_distance_view_ = y_distance_;
}

Star::Star(QSize size, QColor color, QPoint center) {
  tmp_ = center;
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_)
    + minimum_z_distance_;
  while (y_distance_ * y_distance_view_ + x_distance_ * x_distance_
    < center_size_ * center_size_) {
    x_distance_ = QRandomGenerator::global()->bounded(size.width())
      - center.x();
    y_distance_ = QRandomGenerator::global()->bounded(size.height())
      - center.y();
  }
  size_ = QRandomGenerator::global()->bounded(max_size_);
  color_ = std::move(color);
  x_distance_view_ = x_distance_;
  y_distance_view_ = y_distance_;
}

double Star::XViewVelocity() const {
  double
    square_hypotenuse = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  return velocity_ * (x_distance_ / sqrt(square_hypotenuse));
}

double Star::YViewVelocity() const {
  double
    square_hypotenuse = x_distance_ * x_distance_ + z_distance_ * z_distance_;
  return velocity_ * (y_distance_ / sqrt(square_hypotenuse));
}

void Star::Move() {
  double old_distance = sqrt(
    x_distance_ * x_distance_ + z_distance_ * z_distance_
      + y_distance_ * y_distance_);
  z_distance_ -= velocity_ * time_;
  x_distance_view_ += XViewVelocity() * time_;
  y_distance_view_ += YViewVelocity() * time_;
  size_ = size_ * old_distance / sqrt(
    x_distance_ * x_distance_ + z_distance_ * z_distance_
      + y_distance_ * y_distance_);

}

double Star::GetSize() const {
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
  x_distance_ += tmp_.x() - event->pos().x();
  y_distance_ += tmp_.y() - event->pos().y();
  x_distance_view_ += tmp_.x() - event->pos().x();
  y_distance_view_ += tmp_.y() - event->pos().y();
  tmp_ = event->pos();
}
QPoint Star::GetViewPoint() const {
  return QPoint(GetXViewDistance(), GetYViewDistance());
}
