#include "star.h"

double Star::time_ = 1;
double Star::angle_x_ = 60;
double Star::angle_y_ = 60;

Star::Star(QSize window_size, QColor color, QPointF center) :
    window_size_(window_size),
    prev_pos_(center){
  z_distance_ = QRandomGenerator::global()->bounded(max_random_z_distance_ + 1)
    + minimum_z_distance_;

  double max_x = z_distance_ * std::tan(ToDeg(angle_x_ / 2));
  double max_y = z_distance_ * std::tan(ToDeg(angle_y_ / 2));

  coords_ = {QRandomGenerator::global()->bounded(2 * max_x) - max_x,
    QRandomGenerator::global()->bounded(2 * max_y) - max_y};

  size_ = QRandomGenerator::global()->bounded(max_size_ + 1);
  color_ = std::move(color);
}

void Star::Move() {
  double old_distance = Length();
  z_distance_ -= velocity_ * time_;
  size_ = size_ * old_distance / Length();
}

double Star::GetSize() const {
  return size_;
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

void Star::MoveCenter(QPointF& pos) {
  coords_ += (prev_pos_ - pos);
  prev_pos_ = pos;
}

QPointF Star::GetViewPoint() const {
  double pntx = window_size_.width() / 2. * (atan(coords_.x() / z_distance_)
    / (ToDeg(angle_x_ / 2)));
  double pnty = window_size_.height() / 2. * (atan(coords_.y() / z_distance_)
    / (ToDeg(angle_y_ / 2)));
  return {pntx, pnty};
}
double Star::ToDeg(double angle) const{
  return angle * std::numbers::pi / 180;
}
double Star::Length() const {
  return sqrt(coords_.x() * coords_.x() + z_distance_ * z_distance_
          + coords_.y() * coords_.y());
}
