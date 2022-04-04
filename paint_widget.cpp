#include "paint_widget.h"


PaintWidget::PaintWidget(QWidget* parent) : QWidget(parent) {

}

void PaintWidget::Paint(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0, 0, this->width(), this->height());

  brush.setColor(colors_.at(QRandomGenerator::global()->bounded(1)));
  painter->setBrush(brush);
  painter->drawEllipse(coord_, 50, 50, 50);

  for (int i = 0; i < stars_.size(); i++) {
    brush.setColor(colors_.at(QRandomGenerator::global()->bounded(1)));
    painter->setBrush(brush);
    painter->drawEllipse(stars_.at(i).GetTangentialDistance(), 200, stars_.at(i).GetSize(), stars_.at(i).GetSize());
  }
}

void PaintWidget::Tick() {
  for (int i = 0; i < stars_.size(); i++){
    stars_.at(i).Move();
  }
  GenerateStars();
  RemoveStars();
  coord_ += 5;
}
void PaintWidget::GenerateStars() {
  if (stars_.size() < 50) {
    for (int i = 0; i < 1; i++) {
      stars_.push_back(Star(this->width()));
    }
  }
}
void PaintWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    if (stars_.at(i).GetTangentialDistance() > this->width() || stars_.at(i).GetRadialDistance() < 0) {
      stars_.erase(stars_.begin() + i);
    }
  }
}

