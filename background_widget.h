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

#include <vector>
#include <iostream>

#include "stars.h"

class BackgroundWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BackgroundWidget(QWidget* parent);
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
  std::vector<Star> stars_;
  std::vector<std::pair<QPointF, QPointF>> lines_;
  bool light_speed_effect_ = false;  // enables effect
  double white_blur_ = 0;  // initial white blur
  double blur_acceleration_ = 1;
  double star_time_acceleration_ = 0.015;
  double max_star_time_interval_ = 5;
  double default_star_interval_;
  int stars_number_ = 1000;
  double max_line_size_ = 2;
  double max_star_size_ = 150;
  int max_shake_ = 10;
  double camera_divergence_ = 8;
  QPointF center_;
  QPointF prev_pos_;
  bool cursor_move_effect_1_ = true;  // enables effect
  bool cursor_move_effect_2_ = false;  // enables effec
};

#endif  // BACKGROUND_WIDGET_H_
