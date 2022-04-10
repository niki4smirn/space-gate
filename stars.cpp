#include "stars.h"

double Star::time_ = 1;
double Star::angle_ = 180;

Star::Star(QSize size, QColor color, QPointF center) {
  window_size_ = size;
  prev_pos_ = center;
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_)
    + minimum_z_distance_;
  coords_.setX(
    QRandomGenerator::global()->bounded(size.width()) - center.x());
  coords_.setY(
    QRandomGenerator::global()->bounded(size.width()) - center.y());
  while (coords_.x() * coords_.x() + coords_.y() * coords_.y()
    < center_size_ * center_size_) {
    coords_.setX(
      QRandomGenerator::global()->bounded(size.width()) - center.x());
    coords_.setY(
      QRandomGenerator::global()->bounded(size.width()) - center.y());
  }
  size_ = QRandomGenerator::global()->bounded(max_size_);
  color_ = std::move(color);
}

void Star::Move() {
  double old_distance = sqrt(
    coords_.x() * coords_.x() + z_distance_ * z_distance_
      + coords_.y() * coords_.y());
  z_distance_ -= velocity_ * time_;
  size_ = size_ * old_distance
    / sqrt(coords_.x() * coords_.x() + z_distance_ * z_distance_
             + coords_.y() * coords_.y());
}

double Star::GetSize() const {
  return size_;
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
  prev_pos_ = event->pos();
}

QPointF Star::GetViewPoint() const {
  double distance_xz = sqrt(
    coords_.x() * coords_.x() + z_distance_ * z_distance_);
  double distance_yz = sqrt(
    z_distance_ * z_distance_ + coords_.y() * coords_.y());
  double pntx = window_size_.width() / 2 * (acos(z_distance_ / distance_xz) /  (angle_ / 180 * std::numbers::pi / 8));
  if (coords_.x() < 0){
    pntx *= -1;
  }
  double pnty = window_size_.height() / 2 * (acos(z_distance_ / distance_yz) /  (angle_ / 180 * std::numbers::pi / 8));
  if (coords_.y() < 0){
    pnty *= -1;
  }
  return QPointF(pntx, pnty);
}

double Star::GetAngle() {
  return angle_;
}
