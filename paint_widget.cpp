#include "paint_widget.h"

PaintWidget::PaintWidget(QWidget* parent) : QWidget(parent) {

}

void PaintWidget::Paint(QPainter* painter) const {
  QBrush brush(QColor(0, 0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0, 0, this->width(), this->height());

  for (int i = 0; i < stars_.size(); i++) {
    brush.setColor(colors_.at(QRandomGenerator::global()->bounded(2)));
    painter->setBrush(brush);
    painter->drawEllipse(stars_.at(i).GetXViewDistance() + this->width() / 2,
                         stars_.at(i).GetYViewDistance() + this->height() / 2,
                         stars_.at(i).GetSize(),
                         stars_.at(i).GetSize());
  }
}

void PaintWidget::Tick() {
  for (int i = 0; i < stars_.size(); i++) {
    stars_.at(i).Move();
  }
  GenerateStars();
  RemoveStars();
}
void PaintWidget::GenerateStars() {
  if (stars_.size() < 1000) {
    for (int i = 0; i < 100; i++) {
      stars_.push_back(Star(this->width(), this->height()));
    }
  }
}
void PaintWidget::RemoveStars() {
  for (int i = 0; i < stars_.size(); i++) {
    if (stars_.at(i).GetXViewDistance() > this->width()
      || stars_.at(i).GetYViewDistance() > this->height()
      || stars_.at(i).GetZDistance() < 0) {
      stars_.erase(stars_.begin() + i);
    }
  }
}

