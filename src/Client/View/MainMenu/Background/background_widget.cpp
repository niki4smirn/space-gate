#include "background_widget.h"
#include <iostream>
#include <utility>

BackgroundWidget::BackgroundWidget(QWidget* parent) :
    QWidget(parent) {
  default_star_interval_ = Star::GetTime();
  animation_timer_.start(15, this);
  setMouseTracking(true);
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
  for (const auto& item : stars_) {
    int star_diameter;
    if (item.star.GetSize() > max_star_size_) {
      star_diameter = static_cast<int>(max_star_size_);
    } else {
      star_diameter = static_cast<int>(item.star.GetSize());
    }
    QColor color = item.star.GetColor();
    color.setAlpha(255);
    QRadialGradient radialGrad(item.star.GetViewPoint() + center_,
                               star_diameter / 2);
    radialGrad.setColorAt(0, color);
    color.setAlpha(0);
    radialGrad.setColorAt(1, color);

    QBrush brush(radialGrad);
    painter->setBrush(brush);
    painter->drawEllipse(item.star.GetViewPoint() + center_,
                         star_diameter,
                         star_diameter);
  }
}
void BackgroundWidget::PaintLines(QPainter* painter) const {
  for (const auto& item : stars_) {
    QPen pen(Qt::white);
    int width = static_cast<int>(std::min(item.star.GetSize(),
                                          max_line_size_));
    pen.setWidth(width);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    int shake = QRandomGenerator::global()->bounded(2 * max_shake_ + 1)
        - max_shake_;
    QPointF shake_pnt(shake, shake);
    painter->drawLine(shake_pnt + item.line.p1(),
                      shake_pnt + item.line.p2());
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
  for (auto& item : stars_) {
    item.star.Move();
  }

  if (light_speed_effect_) {
    AddLines();
    white_blur_ += blur_acceleration_;
    if (white_blur_ > 255) {
      white_blur_ = 255;
    }
    if (Star::GetTime() < max_star_time_interval_) {
      Star::AddTime(star_time_acceleration_);
    }
  } else {
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
  std::vector<decltype(stars_.begin())> stars_to_remove;
  for (auto item = stars_.begin(); item != stars_.end(); ++item) {
    auto star_point = item->star.GetViewPoint();
    auto normalized_star_point = (star_point + center_).toPoint();
    if (geometry().contains(normalized_star_point)) {
      continue;
    }
    stars_to_remove.push_back(item);
  }
  for (const auto& it : stars_to_remove) {
    stars_.erase(it);
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
    for (auto& item : stars_) {
      item.star.MoveCenter(posf);
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

void BackgroundWidget::mouseMoveEvent(QMouseEvent* event) {
  SetCenterPos(event->pos());
}

void BackgroundWidget::resizeEvent(QResizeEvent* event) {
  center_ = QPointF(event->size().width() / 2., event->size().height() / 2.);
  prev_pos_ = center_;
}

void BackgroundWidget::AddLines() {
  QPointF cmp(0, 0);
  for (auto& item : stars_) {
    QPointF pnt = item.star.GetViewPoint() + center_;
    if (item.line.p1() == cmp) {
      item.line.setP1(pnt);
    }
    item.line.setP2(pnt);
  }
}

StarAndLine::StarAndLine(QSize window_size, QColor color, QPointF center) :
  star(Star(window_size, std::move(color), center)),
  line(QLineF(0, 0, 0, 0)) {}
