#ifndef BACKGROUND_WIDGET_H_
#define BACKGROUND_WIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QPen>
#include <QRadialGradient>
#include <QBasicTimer>
#include <QPainter>
#include <QResizeEvent>

#include <deque>
#include <vector>
#include <iostream>

#include "stars.h"

class BackgroundWidget : public QWidget {
  Q_OBJECT

 public:
  BackgroundWidget(QWidget* parent = nullptr);
  void Paint(QPainter* painter) const;
  void Tick();

  void timerEvent(QTimerEvent* event) override;
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent *event) override;

  void SetState(bool state);
  void SetCenterPos(QMouseEvent* event);

  void PaintStars(QPainter* painter) const;
  void PaintLines(QPainter* painter) const;
  void PaintBackground(QPainter* painter) const;
  void PaintBlur(QPainter* painter) const;

 private:
  void GenerateStars();
  void RemoveStars();
  QBasicTimer animation_timer_;

  std::vector<QColor> colors_ =
    {QColor(51, 255, 255),
     QColor(154, 244, 255),
     QColor(250, 218, 254),
     QColor(196, 251, 239),
     QColor(249, 216, 243),
     QColor(216, 249, 230),
     QColor(51, 153, 255)};
  std::deque<Star> stars_;
  std::vector<std::pair<QPointF, QPointF>> lines_;
  bool light_speed_effect_ = false;  // enables effect
  double white_blur_ = 0;  // initial white blur
  double blur_acceleration_ = 1;  // 1 - 254, more - faster
  double star_time_acceleration_ = 0.015;  // 0 - inf, more - faster
  double max_star_time_interval_ = 5;  // blur_accel - inf, more - longer
  double default_star_interval_;
  int stars_number_ = 250;  // 0 - inf
  double max_line_size_ = 4;  // 0 - inf
  double max_star_size_ = 250;  // 1 - inf
  int max_shake_ = 10;  // 0 - inf
  double camera_divergence_ = 20;  // 1 - inf, more - smaller
  QPointF center_;
  QPointF prev_pos_;
  bool cursor_move_effect_1_ = true;  // enables effect
  bool cursor_move_effect_2_ = false;  // enables effect
};

#endif  // BACKGROUND_WIDGET_H_
