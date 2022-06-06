#pragma once

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScreen>

#include "src/Helpers/Constants/constants.h"
#include "Protobuf/minigame_responses.pb.h"

enum PlayerType {
  kNavigator,
  kMechanic,
};

class HoleRepairView : public QWidget {
 Q_OBJECT

 public:
  explicit HoleRepairView(QWidget* parent = nullptr);
  void InitializeView(const minigame_responses::MinigameResponse& response);
  void UpdateView(const minigame_responses::MinigameResponse& response);

 private:
  void PaintHoles(QPainter* painter);
  void PaintPlates(QPainter* painter);
  void PaintBackground(QPainter* painter);
  void PaintCursor(QPainter* painter);
  void PaintAvailablePlatesNumber(QPainter* painter);
  void PaintTimeBar(QPainter* painter);
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  std::vector<QPointF> plates_;
  std::vector<QPointF> holes_;
  int available_plates_;
  QPointF cursor_pos_;
  PlayerType player_;
  QSize screen_size_;
  double hole_size_ = screen_size_.width() / 39;
  double cursor_size_ = screen_size_.width() / 96;
  double bar_width_ = screen_size_.height() / 80;
  double text_size_ = screen_size_.height() / 40;
  int time_;

 signals:
  void PlatePos(QPointF pos);
  void MousePos(QPointF pos);
};
