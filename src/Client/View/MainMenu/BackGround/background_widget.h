#pragma once

#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QRadialGradient>
#include <QBasicTimer>
#include <QResizeEvent>
#include <QWidget>

#include <vector>
#include <list>

#include "star.h"

class BackgroundWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BackgroundWidget(QWidget* parent = nullptr);
  void Paint(QPainter* painter) const;
  void Tick();

  void SetLightEffect(bool state);
  void SetCenterPos(const QPoint& pos);

 private:
  void GenerateStars();
  void RemoveOutOfBoundsStars();
  void PaintStars(QPainter* painter) const;
  void PaintLines(QPainter* painter) const;
  void PaintBackground(QPainter* painter) const;
  void PaintBlur(QPainter* painter) const;

  void timerEvent(QTimerEvent* event) override;
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent *event) override;

  QBasicTimer animation_timer_;

  const std::vector<QColor> colors_ =
    {QColor(51, 255, 255),
     QColor(154, 244, 255),
     QColor(250, 218, 254),
     QColor(196, 251, 239),
     QColor(249, 216, 243),
     QColor(216, 249, 230),
     QColor(51, 153, 255)};
  std::list<Star> stars_;
  std::vector<QLineF> lines_;
  bool light_speed_effect_ = false;  // enables effect
  double white_blur_ = 0;  // initial white blur
  const double blur_acceleration_ = 1;  // 1 - 254, more - faster
  const double star_time_acceleration_ = 0.015;  // 0 - inf, more - faster
  const double max_star_time_interval_ = 5;  // blur_accel - inf, more - longer
  double default_star_interval_;
  const int stars_number_ = 250;  // 0 - inf
  const int generate_stars_number_ = static_cast<int>(stars_number_ * 0.2);
  const double max_line_size_ = 4;  // 0 - inf
  const double max_star_size_ = 250;  // 1 - inf
  const int max_shake_ = 10;  // 0 - inf
  const double camera_divergence_ = 20;  // 1 - inf, more - smaller
  QPointF center_;
  QPointF prev_pos_;
  const bool cursor_move_effect_1_ = true;  // enables effect
  const bool cursor_move_effect_2_ = false;  // enables effect
};

