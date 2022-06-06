#include "hole_repair_view.h"
#include "src/Helpers/logging.h"
#include "src/Helpers/Constants/minigames_settings.h"

using namespace hole_repair_settings;

HoleRepairView::HoleRepairView(QWidget* parent) :
    QWidget(parent),
    screen_size_(qApp->screens()[0]->size()) {

  setMouseTracking(true);
}

void HoleRepairView::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  PaintBackground(&painter);
  switch (player_) {
    case kMechanic: {
      PaintPlates(&painter);
      PaintTimeBar(&painter);
      break;
    }
    case kNavigator: {
      PaintHoles(&painter);
      PaintPlates(&painter);
      PaintAvailablePlatesNumber(&painter);
      PaintTimeBar(&painter);
      PaintCursor(&painter);
      break;
    }
  }
}

void HoleRepairView::PaintHoles(QPainter* painter) {
  QPixmap hole(":HoleRepair/hole.png");
  hole = hole.scaled(hole_size_, hole_size_);
  for (int i = 0; i < holes_.size(); i++) {
    QPointF rel_pnt = holes_.at(i);
    QPointF pnt
        (1. * rel_pnt.x() / kRelativeCoords
             * screen_size_.width()
             - hole.width() / 2,
         1. * rel_pnt.y() / kRelativeCoords
             * screen_size_.height()
             - hole.height() / 2);
    painter->drawPixmap(pnt, hole);
  }
}

void HoleRepairView::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);
  if (player_ == kNavigator) {
    repaint();
    return;
  }
  if (available_plates_ <= 0) {
    return;
  }
  QPointF rel_pnt(1. * event->pos().x() / screen_size_.width()
                      * kRelativeCoords,
                  1. * event->pos().y() / screen_size_.height()
                      * kRelativeCoords);
  emit PlatePos(rel_pnt);
  plates_.emplace_back(rel_pnt);
  available_plates_--;
  repaint();
}

void HoleRepairView::mouseMoveEvent(QMouseEvent* event) {
  QWidget::mouseMoveEvent(event);
  if (player_ == kNavigator) {
    repaint();
    return;
  }
  QPointF rel_pnt(1. * event->pos().x() / screen_size_.width()
                      * kRelativeCoords,
                  1. * event->pos().y() / screen_size_.height()
                      * kRelativeCoords);
  emit MousePos(rel_pnt);
  repaint();
}

void HoleRepairView::PaintPlates(QPainter* painter) {
  QPixmap cover(":HoleRepair/plate.png");
  cover = cover.scaled(hole_size_, hole_size_);
  for (int i = 0; i < plates_.size(); i++) {
    QPointF pnt(1. * plates_.at(i).x() / kRelativeCoords
                    * screen_size_.width() - cover.width() / 2,
                1. * plates_.at(i).y() / kRelativeCoords
                    * screen_size_.height() - cover.height() / 2);
    painter->drawPixmap(pnt, cover);
  }
}

void HoleRepairView::PaintBackground(QPainter* painter) {
  QPixmap background(":HoleRepair/background.png");
  background = background.scaled(
      background.width() * screen_size_.width()
          / background.height(), screen_size_.height());
  QBrush brush(Qt::TexturePattern, background);
  painter->setBrush(brush);
  painter->drawRect(QRectF(0, 0, screen_size_.width(), screen_size_.height()));
}

void HoleRepairView::PaintCursor(QPainter* painter) {
  QPixmap cursor(":HoleRepair/cursor.png");
  cursor = cursor.scaled(cursor_size_, cursor_size_);
  painter->drawPixmap(cursor_pos_, cursor);
}

void HoleRepairView::InitializeView(const minigame_responses::MinigameResponse&
response) {
  player_ =
      static_cast<PlayerType>(response.initial_hole_repair_response().role());
  available_plates_ = response.initial_hole_repair_response().plates();
  for (int i = 0;
       i < response.initial_hole_repair_response().holes().points_size(); i++) {
    auto pnt = response.initial_hole_repair_response().holes().points(i);
    holes_.emplace_back(pnt.x(), pnt.y());
  }
  repaint();
}

void HoleRepairView::UpdateView(const minigame_responses::MinigameResponse&
response) {
  switch (response.hole_repair_response().message_case()) {
    case minigame_responses::HoleRepairResponse::kMousePos: {
      auto pnt = response.hole_repair_response().mouse_pos();
      cursor_pos_ =
          QPointF(1. * pnt.x() / kRelativeCoords * screen_size_.width(),
                  1. * pnt.y() / kRelativeCoords * screen_size_.height());
      break;
    }
    case minigame_responses::HoleRepairResponse::kPlatePos: {
      auto pnt = response.hole_repair_response().plate_pos();
      plates_.emplace_back(pnt.x(), pnt.y());
      available_plates_--;
      break;
    }
  }
  time_ = response.remaining_time();
  repaint();
}

void HoleRepairView::PaintAvailablePlatesNumber(QPainter* painter) {
  QFont font = painter->font();
  font.setPixelSize(20);
  painter->setFont(font);
  painter->drawText(1. * screen_size_.width() / 2 - text_size_ / 2,
                    1. * screen_size_.height() / 2 - text_size_ / 2,
                    QString(available_plates_));
}

void HoleRepairView::PaintTimeBar(QPainter* painter) {
  QBrush brush(QColor(0, 0, 255));
  painter->setBrush(brush);
  painter->drawRect(0,
                    0,
                    1. * time_ / kGameDuration * screen_size_.width(),
                    bar_width_);
}
