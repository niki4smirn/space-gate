#include "background_widget.h"
#include <iostream>

BackgroundWidget::BackgroundWidget(QWidget* parent) :
    QWidget(parent) {
  default_star_interval_ = Star::GetTime();
  animation_timer_.start(155, this);
  setMouseTracking(true);
  this->setMouseTracking(true);
}

void BackgroundWidget::Paint(QPainter* painter) const {
  PaintBackground(painter);
  if (light_speed_effect_) {
    PaintLines(painter);
  }
  PaintStars(painter);
  if (light_speed_effect_) {
    PaintBlur(painter);
  }
}

void BackgroundWidget::PaintStars(QPainter* painter) const {
  QPen pen(QColor(0, 0, 0, 0));
  painter->setPen(pen);
  for (const auto& star : stars_) {
    QSize size;
    if (star.GetSize() > max_star_size_) {
      size.setWidth(static_cast<int>(max_star_size_));
      size.setHeight(static_cast<int>(max_star_size_));
    } else {
      size.setWidth(static_cast<int>(star.GetSize()));
      size.setHeight(static_cast<int>(star.GetSize()));
    }
    QColor color = star.GetColor();
    color.setAlpha(255);
    QRadialGradient radialGrad(star.GetViewPoint() + center_,
                               static_cast<int>(size.width() / 2));
    radialGrad.setColorAt(0, color);
    color.setAlpha(0);
    radialGrad.setColorAt(1, color);

    QBrush brush(radialGrad);
    painter->setBrush(brush);
    painter->drawEllipse(star.GetViewPoint() + center_,
                         size.width(),
                         size.height());
  }
}
void BackgroundWidget::PaintLines(QPainter* painter) const {
  auto star = stars_.begin();
  for (int i = 0; i < lines_.size(); i++) {
    QPen pen(Qt::white);
    int width = static_cast<int>(std::min(star->GetSize(),
                                          max_line_size_));
    pen.setWidth(width);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    int shake = QRandomGenerator::global()->bounded(2 * max_shake_ + 1)
        - max_shake_;
    QPointF shake_pnt(shake, shake);
    painter->drawLine(shake_pnt + lines_.at(i).p1(),
                      shake_pnt + lines_.at(i).p2());
    ++star;
  }
}

void BackgroundWidget::PaintBackground(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(geometry());
}

void BackgroundWidget::PaintBlur(QPainter* painter) const {
  QBrush brush(QColor(255, 255, 255,
                      static_cast<int>(white_blur_)));
  painter->setBrush(brush);
  painter->drawRect(geometry());
}

void BackgroundWidget::Tick() {
  if (!cursor_move_effect_1_ && !cursor_move_effect_2_) {
    center_ = QPointF(width() / 2., height() / 2.);
  }
  for (auto& star: stars_) {
    star.Move();
  }

  if (light_speed_effect_) {
    if (lines_.size() > stars_.size()) {
      lines_.erase(lines_.begin() + stars_.size(), lines_.end());
    }
    int i = 0;
    for (auto star = stars_.begin(); star != stars_.end(); ++star) {
      QPointF pnt = star->GetViewPoint() + center_;
      //std::cout << lines_.size() << " " << i << "\n";
      if (lines_.size() <= i) {
        lines_.emplace_back(pnt, pnt);
      } else {
        //lines_.at(i).setP1(QPointF(0, 0));
        lines_.at(i).setP2(pnt);
      }
      i++;
    }
    white_blur_ += blur_acceleration_;
    if (white_blur_ > 255) {
      white_blur_ = 255;
    }
    if (Star::GetTime() < max_star_time_interval_) {
      Star::AddTime(star_time_acceleration_);
    }
  } else {
    lines_.clear();
    white_blur_ = 1;
    if (Star::GetTime() > default_star_interval_) {
      Star::AddTime(-star_time_acceleration_);
    }
  }
  RemoveOutOfBoundsStars();
  GenerateStars();
}

void BackgroundWidget::GenerateStars() {
  if (stars_.size() >= stars_number_) {
    return;
  }
  int gen_star_num = QRandomGenerator::global()->
      bounded(generate_stars_number_);
  for (int i = 0; i < gen_star_num; i++) {
    uint32_t size = colors_.size();
    uint32_t gen_color = QRandomGenerator::global()->bounded(size);
    stars_.emplace_back(this->size(),
                        colors_.at(gen_color), center_);
  }
}

void BackgroundWidget::RemoveOutOfBoundsStars() {
  auto line = lines_.begin();
  std::vector<std::list<Star>::iterator> stars_to_remove;
  std::vector<std::vector<QLineF>::iterator> lines_to_remove;
  for (auto star = stars_.begin(); star != stars_.end(); ++star) {
    auto star_point = star->GetViewPoint();
    auto normalized_star_point = (star_point + center_).toPoint();
    if (geometry().contains(normalized_star_point)) {
      ++line;
      continue;
    }
    stars_to_remove.push_back(star);
    if (light_speed_effect_) {
      lines_to_remove.push_back(line);
    }
    ++line;
  }
  for (const auto& it : stars_to_remove) {
    stars_.erase(it);
  }
  for (auto it : lines_to_remove) {
    lines_.erase(it);
  }
}

void BackgroundWidget::SetLightEffect(bool state) {
  light_speed_effect_ = state;
}

void BackgroundWidget::SetCenterPos(const QPoint& pos) {
  QPointF posf(pos.x(), pos.y());
  if (cursor_move_effect_1_ && !light_speed_effect_) {
    center_ += ((posf - prev_pos_) / camera_divergence_);
    prev_pos_ = posf;
  }
  if (cursor_move_effect_2_ && !light_speed_effect_) {
    center_ = posf;
    for (auto& star: stars_) {
      star.MoveCenter(posf);
    }
  }
}

void BackgroundWidget::timerEvent(QTimerEvent* event) {
  if (event->timerId() == animation_timer_.timerId()) {
    QObject::timerEvent(event);
    Tick();
    repaint();
  }
}

void BackgroundWidget::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  Paint(&painter);
}

void BackgroundWidget::mousePressEvent(QMouseEvent*) {
  SetLightEffect(true);
}

void BackgroundWidget::mouseReleaseEvent(QMouseEvent*) {
  SetLightEffect(false);
}

void BackgroundWidget::mouseMoveEvent(QMouseEvent* event) {
  SetCenterPos(event->pos());
}
void BackgroundWidget::resizeEvent(QResizeEvent* event) {
  center_ = QPointF(event->size().width() / 2., event->size().height() / 2.);
  prev_pos_ = center_;
}
