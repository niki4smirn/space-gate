#ifndef PAINT_WIDGET_H_
#define PAINT_WIDGET_H_

#include "controller.h"
#include <QWidget>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>

class PaintWidget : public QWidget {
 Q_OBJECT
 public:
  explicit PaintWidget(QWidget* parent);

 private:

};

#endif //PAINT_WIDGET_H_
