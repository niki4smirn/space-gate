#include "paint_widget.h"

PaintWidget::PaintWidget(QWidget* parent) : QWidget(parent) {

}

void PaintWidget::Paint(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0, 0, this->width(), this->height());

  for (int i = 0; i < stars_.size(); i++) {
    brush.setColor(stars_.at(i).GetColor());
    painter->setBrush(brush);
    painter->drawEllipse(stars_.at(i).GetXViewDistance() + this->width() / 2,
                         stars_.at(i).GetYViewDistance() + this->height() / 2,
                         stars_.at(i).GetSize(),
                         stars_.at(i).GetSize());
  }
  if (state_) {
    for (int i = 0; i < lines_.size(); i++){
      QPen pen(QColor(255, 255, 255, 255));
      pen.setWidth(stars_.at(i).GetSize());
      painter->setPen(pen);
      int shake = QRandomGenerator::global()->bounded(20) - 10;
      painter->drawLine(shake + lines_.at(i).first.x(), shake + lines_.at(i).first.y(), shake + lines_.at(i).second.x(), shake + lines_.at(i).second.y());
    }
  }
}

void PaintWidget::Tick() {
  for (int i = 0; i < stars_.size(); i++) {
    stars_.at(i).Move();
    if (state_) {
      while (lines_.size() <= i) {
        lines_.push_back(std::make_pair(QPoint(stars_.at(i).GetXViewDistance() + this->width() / 2,
                                               stars_.at(i).GetYViewDistance() + this->height() / 2),
                                        QPoint(stars_.at(i).GetXViewDistance() + this->width() / 2,
                                               stars_.at(i).GetYViewDistance() + this->height() / 2)));
      }
      lines_.at(i).second = QPoint(stars_.at(i).GetXViewDistance() + this->width() / 2,
                                     stars_.at(i).GetYViewDistance() + this->height() / 2);
    } else {
      lines_.clear();
    }
  }
  if (state_ && Star::GetTime() < 5) {
    Star::AddTime(0.01);
  } else if (Star::GetTime() > 1) {
    Star::AddTime(-0.01);
  }
  GenerateStars();
  RemoveStars();
}
void PaintWidget::GenerateStars() {
  if (stars_.size() < 10000) {
    for (int i = 0; i < QRandomGenerator::global()->bounded(2000); i++) {
      int size = colors_.size();
      stars_.push_back(Star(this->width(),
                            this->height(),
                            colors_.at(QRandomGenerator::global()->bounded(size))));
    }
  }
}
void PaintWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    if (stars_.at(i).GetXViewDistance() > this->width() / 2
      || stars_.at(i).GetXViewDistance() < -this->width() / 2
      || stars_.at(i).GetYViewDistance() > this->height() / 2
      || stars_.at(i).GetYViewDistance() < -this->height() / 2
      || stars_.at(i).GetZDistance() < 0) {
      stars_.erase(stars_.begin() + i);
      if (state_) {
        lines_.erase(lines_.begin() + i);
      }
    }
  }
}
void PaintWidget::SetState(bool state) {
  state_ = state;
}

