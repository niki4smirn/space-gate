#include "background_widget.h"

BackgroundWidget::BackgroundWidget(QWidget* parent) : QWidget(parent) {
  default_star_interval_ = Star::GetTime();
}

void BackgroundWidget::Paint(QPainter* painter) const {
  PaintBackground(painter);
  PaintStars(painter);
  if (light_speed_effect_) {
    PaintLines(painter);
    PaintBlur(painter);
  }
}

void BackgroundWidget::PaintStars(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  for (const auto& star: stars_) {
    brush.setColor(star.GetColor());
    painter->setBrush(brush);
    int size;
    if (star.GetSize() > 20) {
      size = 20;
    } else {
      size = static_cast<int>(star.GetSize());
    }
    painter->drawEllipse(
      static_cast<int>(star.GetXViewDistance()) + center_.x(),
      static_cast<int>(star.GetYViewDistance()) + center_.y(),
      size, size);
  }
}
void BackgroundWidget::PaintLines(QPainter* painter) const {
  for (int i = 0; i < lines_.size(); i++) {
    QPen pen(QColor(255, 255, 255, 255));
    if (stars_.at(i).GetSize() < max_line_size_) {
      pen.setWidth(static_cast<int>(stars_.at(i).GetSize()));
    } else {
      pen.setWidth(width_for_big_stars_);
    }
    painter->setPen(pen);
    int
      shake = QRandomGenerator::global()->bounded(2 * max_shake_) - max_shake_;
    painter->drawLine(shake + lines_.at(i).first.x(),
                      shake + lines_.at(i).first.y(),
                      shake + lines_.at(i).second.x(),
                      shake + lines_.at(i).second.y());
  }
}

void BackgroundWidget::PaintBackground(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0, 0, width(), height());
}

void BackgroundWidget::PaintBlur(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  brush.setColor(QColor(255, 255, 255,
                        static_cast<int>(white_blur_)));
  painter->setBrush(brush);
  painter->drawRect(0, 0, width(), height());
}

void BackgroundWidget::Tick() {
  if (!cursor_move_effect_1_ && !cursor_move_effect_2_) {
    center_ = QPoint(width() / 2, height() / 2);
  }
  for (int i = 0; i < stars_.size(); i++) {
    stars_.at(i).Move();
    if (light_speed_effect_) {
      int pntx =
        static_cast<int>(stars_.at(i).GetXViewDistance()) + center_.x();
      int pnty =
        static_cast<int>(stars_.at(i).GetYViewDistance()) + center_.y();
      while (lines_.size() <= i) {
        lines_.push_back({QPoint(pntx, pnty), QPoint(pntx, pnty)});
      }
      lines_.at(i).second =
        QPoint(pntx, pnty);
    } else {
      lines_.clear();
    }
  }

  if (light_speed_effect_) {
    if (white_blur_ < 255) {
      white_blur_ += blur_acceleration_;
    }
    if (Star::GetTime() < max_star_interval_) {
      Star::AddTime(star_time_acceleration_);
    }
  } else {
    white_blur_ = 1;
    if (Star::GetTime() > default_star_interval_) {
      Star::AddTime(-star_time_acceleration_);
    }
  }
  RemoveStars();
  GenerateStars();
}

void BackgroundWidget::GenerateStars() {
  if (stars_.size() < stars_number_) {
    for (int i = 0;
         i < QRandomGenerator::global()->bounded(stars_number_ * 0.4); i++) {
      uint32_t size = colors_.size();
      stars_.emplace_back(Star(this->size(),
                               colors_.at(QRandomGenerator::global()->
                                 bounded(size)), center_));
    }
  }
}

void BackgroundWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    int pntx = static_cast<int>(stars_.at(i).GetXViewDistance());
    int pnty = static_cast<int>(stars_.at(i).GetYViewDistance());
    if (pntx > width() - center_.x() || pntx < -center_.x()
      || pnty > height() - center_.y() || pnty < -center_.y()
      || static_cast<int>(stars_.at(i).GetZDistance()) < 0) {
      stars_.erase(stars_.begin() + i);
      if (light_speed_effect_) {
        lines_.erase(lines_.begin() + i);
      }
    }
  }
}

void BackgroundWidget::SetState(bool state) {
  light_speed_effect_ = state;
}

void BackgroundWidget::SetCenterPos(QMouseEvent* event) {
  if (cursor_move_effect_1_ && !light_speed_effect_) {
    center_ = event->pos();
  }
  if (cursor_move_effect_2_ && !light_speed_effect_) {
    for (auto& star: stars_) {
      center_ = event->pos();
      star.MoveCenter(event);
    }
  }
}
