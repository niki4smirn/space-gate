#include "background_widget.h"

BackgroundWidget::BackgroundWidget(QWidget* parent) : QWidget(parent) {
  default_star_interval_ = Star::GetTime();
}

void BackgroundWidget::Paint(QPainter* painter) const {
  PaintBackground(painter);
  PaintStars(painter);
  if (light_speed_effect_state_) {
    PaintLines(painter);
    PaintBlur(painter);
  }
}

void BackgroundWidget::PaintStars(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  for (const auto& star: stars_) {
    brush.setColor(star.GetColor());
    painter->setBrush(brush);
    painter->drawEllipse(
      static_cast<int>(star.GetXViewDistance()) + this->width() / 2,
      static_cast<int>(star.GetYViewDistance()) + this->height() / 2,
      static_cast<int>(star.GetSize()),
      static_cast<int>(star.GetSize()));
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
    int shake = QRandomGenerator::global()->bounded(2 * max_shake_) - max_shake_;
    painter->drawLine(shake + lines_.at(i).first.x(),
                      shake + lines_.at(i).first.y(),
                      shake + lines_.at(i).second.x(),
                      shake + lines_.at(i).second.y());
  }
}

void BackgroundWidget::PaintBackground(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0, 0, this->width(), this->height());
}

void BackgroundWidget::PaintBlur(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  brush.setColor(QColor(255, 255, 255, static_cast<int>(white_blur_)));
  painter->setBrush(brush);
  painter->drawRect(0, 0, this->width(), this->height());
}

void BackgroundWidget::Tick() {
  for (int i = 0; i < stars_.size(); i++) {
    stars_.at(i).Move();
    if (light_speed_effect_state_) {
      while (lines_.size() <= i) {
        lines_.emplace_back(std::make_pair(QPoint(
                                             static_cast<int>(stars_.at(i).GetXViewDistance()) + this->width() / 2,
                                             static_cast<int>(stars_.at(i).GetYViewDistance())
                                               + this->height() / 2),
                                           QPoint(
                                             static_cast<int>(stars_.at(i).GetXViewDistance())
                                               + this->width() / 2,
                                             static_cast<int>(stars_.at(i).GetYViewDistance())
                                               + this->height() / 2)));
      }
      lines_.at(i).second =
        QPoint(
          static_cast<int>(stars_.at(i).GetXViewDistance()) + this->width() / 2,
          static_cast<int>(stars_.at(i).GetYViewDistance())
            + this->height() / 2);
    } else {
      lines_.clear();
    }
  }

  if (light_speed_effect_state_) {
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
      stars_.emplace_back(Star(this->width(),
                               this->height(),
                               colors_.at(QRandomGenerator::global()->bounded(
                                 size))));
    }
  }
}

void BackgroundWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    if (static_cast<int>(stars_.at(i).GetXViewDistance()) > this->width() / 2
      || static_cast<int>(stars_.at(i).GetXViewDistance()) < -this->width() / 2
      || static_cast<int>(stars_.at(i).GetYViewDistance()) > this->height() / 2
      || static_cast<int>(stars_.at(i).GetYViewDistance()) < -this->height() / 2
      || static_cast<int>(stars_.at(i).GetZDistance()) < 0) {
      stars_.erase(stars_.begin() + i);
      if (light_speed_effect_state_) {
        lines_.erase(lines_.begin() + i);
      }
    }
  }
}

void BackgroundWidget::SetState(bool state) {
  light_speed_effect_state_ = state;
}
