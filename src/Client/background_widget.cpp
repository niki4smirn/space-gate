#include "background_widget.h"

BackgroundWidget::BackgroundWidget(QWidget* parent):
  QWidget(parent){
  default_star_interval_ = Star::GetTime();
  animation_timer_.start(15, this);
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
    QPointF size;
    if (star.GetSize() > max_star_size_) {
      size.setX(max_star_size_);
      size.setY(max_star_size_);
    } else {
      size.setX(star.GetSize());
      size.setY(star.GetSize());
    }
    QColor color = star.GetColor();
    color.setAlpha(255);
    QRadialGradient radialGrad(star.GetViewPoint() + center_,
                               static_cast<int>(size.x() / 2));
    radialGrad.setColorAt(0, color);
    color.setAlpha(0);
    radialGrad.setColorAt(1, color);

    QBrush brush(radialGrad);
    painter->setBrush(brush);
    painter->drawEllipse(star.GetViewPoint() + center_, size.x(), size.y());
  }
}
void BackgroundWidget::PaintLines(QPainter* painter) const {
  for (int i = 0; i < lines_.size(); i++) {
    QPen pen(QColor(255, 255, 255, 255));
    int width;
    if (stars_.at(i).GetSize() < max_line_size_) {
      width = static_cast<int>(stars_.at(i).GetSize());
    } else {
      width = static_cast<int>(max_line_size_);
    }
    pen.setWidth(width);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    int shake = QRandomGenerator::global()->bounded(2 * max_shake_ + 1)
      - max_shake_;
    QPointF shake_pnt(shake, shake);
    painter->drawLine(shake_pnt + lines_.at(i).first,
                      shake_pnt + lines_.at(i).second);
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
    center_ = QPointF(width() / 2, height() / 2);
  }
  for (int i = 0; i < stars_.size(); i++) {
    stars_.at(i).Move();
    if (light_speed_effect_) {
      QPointF pnt = stars_.at(i).GetViewPoint() + center_;
      while (lines_.size() <= i) {
        lines_.push_back({pnt, pnt});
      }
      lines_.at(i).second = pnt;
    } else {
      lines_.clear();
    }
  }

  if (light_speed_effect_) {
    if (white_blur_ < 255) {
      white_blur_ += blur_acceleration_;
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
  RemoveStars();
  GenerateStars();
}

void BackgroundWidget::GenerateStars() {
  if (stars_.size() < stars_number_) {
    for (int i = 0;
         i < QRandomGenerator::global()->bounded(stars_number_ * 0.2);
         i++) {
      uint32_t size = colors_.size();
      stars_.emplace_back(Star(this->size(),
                               colors_.at(QRandomGenerator::global()->
                                 bounded(size)), center_));
    }
  }
}

void BackgroundWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    double pntx = stars_.at(i).GetViewPoint().x();
    double pnty = stars_.at(i).GetViewPoint().y();
    if (pntx > width() - center_.x() || pntx < -center_.x()
      || pnty > height() - center_.y() || pnty < -center_.y()) {
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
    center_ += ((event->pos() - prev_pos_) / camera_divergence_);
    prev_pos_ = event->pos();
  }
  if (cursor_move_effect_2_ && !light_speed_effect_) {
    for (auto& star : stars_) {
      center_ = event->pos();
      star.MoveCenter(event);
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
  SetState(true);
}

void BackgroundWidget::mouseReleaseEvent(QMouseEvent*) {
  SetState(false);
}

void BackgroundWidget::mouseMoveEvent(QMouseEvent* event) {
  SetCenterPos(event);
}
void BackgroundWidget::resizeEvent(QResizeEvent* event) {
  center_ = QPointF(event->size().width() / 2, event->size().height() / 2);
  prev_pos_ = center_;
}
