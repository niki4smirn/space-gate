#include <iostream>
#include "game_widget.h"
#include "src/Helpers/logging.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QBrush>

GameWidget::GameWidget(QWidget* parent) : QWidget(parent) {
  if (parent) {
    resize(parent->size());
  }
  yellow_button_ = new QPushButton(this);
  green_button_ = new QPushButton(this);
  purple_button_ = new QPushButton(this);
  red_button_ = new QPushButton(this);
  blue_button_ = new QPushButton(this);
  red_bulb_red_ = new QLabel(this);
  green_bulb_red_ = new QLabel(this);
  blue_bulb_red_ = new QLabel(this);
  yellow_bulb_red_ = new QLabel(this);
  red_bulb_purple_ = new QLabel(this);
  green_bulb_purple_ = new QLabel(this);
  blue_bulb_purple_ = new QLabel(this);
  yellow_bulb_purple_ = new QLabel(this);
  red_bulb_green_ = new QLabel(this);
  green_bulb_green_ = new QLabel(this);
  blue_bulb_green_ = new QLabel(this);
  yellow_bulb_green_ = new QLabel(this);
  red_bulb_blue_ = new QLabel(this);
  green_bulb_blue_ = new QLabel(this);
  blue_bulb_blue_ = new QLabel(this);
  yellow_bulb_blue_ = new QLabel(this);
  loss_timer_ = new QTimer(this);
  loss_timer_->setInterval(500);
  loss_timer_->setSingleShot(0);
  shining_timer_ = new QTimer(this);
  shining_timer_->setInterval(300);
  shining_timer_->setSingleShot(0);
  shining_timer_->start();
  SetButtonsSize();
  SetIcons();
  SetButtonsGeometry();
  SetMainAnimations();
  SetCrackAnimation();
  ButtonClicked();
  SetBulbsSize();
  SetBulbsIcons();
  SetBulbsGeometry();
  SetTracking();
  BackgroundShines();
  Loss();

  // QTimer::singleShot(6000, [&](){
  //   EndGame();
  // });
  // SetProgress(50,100);
}

void GameWidget::SetIcons() {
  QPixmap pixmap_yel(":Buttons/yellow_button.png");
  pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
  yellow_button_->setIcon(QIcon(pixmap_yel));
  yellow_button_->setIconSize(pixmap_yel.size());
  yellow_button_->setVisible(1);

  QPixmap pixmap_gr(":Buttons/green_button.png");
  pixmap_gr = pixmap_gr.scaled(green_button_->size());
  green_button_->setIcon(QIcon(pixmap_gr));
  green_button_->setIconSize(pixmap_gr.size());
  green_button_->setVisible(1);

  QPixmap pixmap_bl(":Buttons/blue_button.png");
  pixmap_bl = pixmap_bl.scaled(blue_button_->size());
  blue_button_->setIcon(QIcon(pixmap_bl));
  blue_button_->setIconSize(pixmap_bl.size());
  blue_button_->setVisible(1);

  QPixmap pixmap_pr(":Buttons/purple_button.png");
  pixmap_pr = pixmap_pr.scaled(purple_button_->size());
  purple_button_->setIcon(QIcon(pixmap_pr));
  purple_button_->setIconSize(QSize(pixmap_pr.size()));
  purple_button_->setVisible(1);

  QPixmap pixmap_red(":Buttons/red_button.png");
  pixmap_red = pixmap_red.scaled(QSize(220, 175), Qt::IgnoreAspectRatio);
  red_button_->setIcon(QIcon(pixmap_red));
  red_button_->setIconSize(pixmap_red.size());
  red_button_->setVisible(1);
}

void GameWidget::SetButtonsSize() {
  double width = QApplication::screens()[0]->size().width();
  double height = QApplication::screens()[0]->size().height();
  blue_button_->setFixedSize(width / 1920 * 380, height / 1080 * 122);
  green_button_->setFixedSize(width / 1920 * 297, height / 1080 * 122);
  red_button_->setFixedSize(width / 1920 * 222, height / 1080 * 177);
  yellow_button_->setFixedSize(width / 1920 * 318, height / 1080 * 132);
  purple_button_->setFixedSize(width / 1920 * 227, height / 1080 * 152);

}

void GameWidget::SetButtonsGeometry() {
  double width = QApplication::screens()[0]->size().width();
  double height = QApplication::screens()[0]->size().height();

  blue_button_->setGeometry(width * 1410 / 1920, height / 1080 * 930, 1, 1);
  green_button_->setGeometry(width * 68 / 1920, height / 1080 * 930, 1, 1);
  red_button_->setGeometry(width / 1920 * 472, height / 1080 * 878, 1, 1);
  yellow_button_->setGeometry(width / 1920 * 727, height / 1080 * 810, 1, 1);
  purple_button_->setGeometry(width / 1920 * 1080, height / 1080 * 885, 1, 1);
}

void GameWidget::ButtonClicked() {
  connect(green_button_, &QPushButton::pressed, this, [&] {
    QPixmap pixmap_gr(":Buttons/green_button_clicked.png");
    pixmap_gr = pixmap_gr.scaled(green_button_->size());
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
  });
  connect(green_button_, &QPushButton::released, this, [&] {
    QPixmap pixmap_gr(":Buttons/green_button.png");
    pixmap_gr = pixmap_gr.scaled(green_button_->size());
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
    emit JoinMinigame(0);
  });

  connect(red_button_, &QPushButton::pressed, this, [&] {
    QPixmap pixmap_red(":Buttons/red_button_clicked.png");
    pixmap_red = pixmap_red.scaled(red_button_->size());
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
  });
  connect(red_button_, &QPushButton::released, this, [&] {
    QPixmap pixmap_red(":Buttons/red_button.png");
    pixmap_red = pixmap_red.scaled(red_button_->size());
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
    emit JoinMinigame(1);
  });

  connect(purple_button_, &QPushButton::pressed, this, [&] {
    QPixmap pixmap_pr(":Buttons/purple_button_clicked.png");
    pixmap_pr = pixmap_pr.scaled(purple_button_->size());
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(pixmap_pr.size());
  });
  connect(purple_button_, &QPushButton::released, this, [&] {
    QPixmap pixmap_pr(":Buttons/purple_button.png");
    pixmap_pr = pixmap_pr.scaled(purple_button_->size());
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(pixmap_pr.size());
    emit JoinMinigame(2);
  });

  connect(blue_button_, &QPushButton::pressed, this, [&] {
    QPixmap pixmap_bl(":Buttons/blue_button_clicked.png");
    pixmap_bl = pixmap_bl.scaled(blue_button_->size());
    blue_button_->setIcon(QIcon(pixmap_bl));
    blue_button_->setIconSize(pixmap_bl.size());
  });
  connect(blue_button_, &QPushButton::released, this, [&] {
    QPixmap pixmap_bl(":Buttons/blue_button.png");
    pixmap_bl = pixmap_bl.scaled(blue_button_->size());
    blue_button_->setIcon(QIcon(pixmap_bl));
    blue_button_->setIconSize(pixmap_bl.size());
    emit JoinMinigame(3);
  });

  connect(yellow_button_, &QPushButton::pressed, this, [&] {
    QPixmap pixmap_yel(":Buttons/yellow_button_clicked.png");
    pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
  });
  connect(yellow_button_, &QPushButton::released, this, [&] {
    QPixmap pixmap_yel(":Buttons/yellow_button.png");
    pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
  });
}

void GameWidget::SetBulbsIcons() {
  QPixmap pixmap_yel(":Bulbs/yellow_bulb_off.png");
  pixmap_yel = pixmap_yel.scaled(yellow_bulb_red_->size());
  yellow_bulb_red_->setPixmap(pixmap_yel);
  yellow_bulb_purple_->setPixmap(pixmap_yel);
  yellow_bulb_green_->setPixmap(pixmap_yel);
  yellow_bulb_blue_->setPixmap(pixmap_yel);

  QPixmap pixmap_bl(":Bulbs/blue_bulb_off.png");
  pixmap_bl = pixmap_bl.scaled(blue_bulb_red_->size());
  blue_bulb_red_->setPixmap(pixmap_bl);
  blue_bulb_purple_->setPixmap(pixmap_bl);
  blue_bulb_green_->setPixmap(pixmap_bl);
  blue_bulb_blue_->setPixmap(pixmap_bl);

  QPixmap pixmap_gr(":Bulbs/green_bulb_off.png");
  pixmap_gr = pixmap_gr.scaled(green_bulb_red_->size());
  green_bulb_red_->setPixmap(pixmap_gr);
  green_bulb_purple_->setPixmap(pixmap_gr);
  green_bulb_green_->setPixmap(pixmap_gr);
  green_bulb_blue_->setPixmap(pixmap_gr);

  QPixmap pixmap_red(":Bulbs/red_bulb_off.png");
  pixmap_red = pixmap_red.scaled(red_bulb_red_->size());
  red_bulb_red_->setPixmap(pixmap_red);
  red_bulb_purple_->setPixmap(pixmap_red);
  red_bulb_green_->setPixmap(pixmap_red);
  red_bulb_blue_->setPixmap(pixmap_red);
}

void GameWidget::SetBulbsSize() {
  double width = QApplication::screens()[0]->size().width();
  double height = QApplication::screens()[0]->size().height();

  yellow_bulb_red_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  red_bulb_red_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  blue_bulb_red_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  green_bulb_red_->setFixedSize(width / 1920 * 41, height / 1080 * 33);

  yellow_bulb_purple_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  red_bulb_purple_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  blue_bulb_purple_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  green_bulb_purple_->setFixedSize(width / 1920 * 41, height / 1080 * 33);

  yellow_bulb_green_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  red_bulb_green_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  blue_bulb_green_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  green_bulb_green_->setFixedSize(width / 1920 * 41, height / 1080 * 33);

  yellow_bulb_blue_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  red_bulb_blue_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  blue_bulb_blue_->setFixedSize(width / 1920 * 41, height / 1080 * 33);
  green_bulb_blue_->setFixedSize(width / 1920 * 41, height / 1080 * 33);

}

void GameWidget::SetBulbsGeometry() {
  double width = QApplication::screens()[0]->size().width();
  double height = QApplication::screens()[0]->size().height();

  yellow_bulb_red_->setGeometry(width / 1920 * 487, height / 1080 * 824, 1, 1);
  green_bulb_red_->setGeometry(width / 1920 * 532, height / 1080 * 824, 1, 1);
  blue_bulb_red_->setGeometry(width / 1920 * 577, height / 1080 * 824, 1, 1);
  red_bulb_red_->setGeometry(width / 1920 * 622, height / 1080 * 824, 1, 1);

  yellow_bulb_purple_->setGeometry(width / 1920 * 1095,
                                   height / 1080 * 846,
                                   1,
                                   1);
  green_bulb_purple_->setGeometry(width / 1920 * 1147,
                                  height / 1080 * 846,
                                  1,
                                  1);
  blue_bulb_purple_->setGeometry(width / 1920 * 1200,
                                 height / 1080 * 846,
                                 1,
                                 1);
  red_bulb_purple_->setGeometry(width / 1920 * 1252, height / 1080 * 846, 1, 1);

  yellow_bulb_green_->setGeometry(width / 1920 * 82, height / 1080 * 891, 1, 1);
  green_bulb_green_->setGeometry(width / 1920 * 142, height / 1080 * 891, 1, 1);
  blue_bulb_green_->setGeometry(width / 1920 * 202, height / 1080 * 891, 1, 1);
  red_bulb_green_->setGeometry(width / 1920 * 263, height / 1080 * 891, 1, 1);

  yellow_bulb_blue_->setGeometry(width / 1920 * 1575,
                                 height / 1080 * 891,
                                 1,
                                 1);
  green_bulb_blue_->setGeometry(width / 1920 * 1627, height / 1080 * 891, 1, 1);
  blue_bulb_blue_->setGeometry(width / 1920 * 1680, height / 1080 * 891, 1, 1);
  red_bulb_blue_->setGeometry(width / 1920 * 1733, height / 1080 * 891, 1, 1);
}

void GameWidget::mouseMoveEvent(QMouseEvent* event) {
  double rx = cursor().pos().rx();
  double ry = cursor().pos().ry();

  double yel_left = yellow_button_->pos().rx();
  double yel_right = yel_left + yellow_button_->size().width();
  double yel_top = yellow_button_->pos().ry();
  double yel_bottom = yel_top + yellow_button_->size().height();

  double blue_left = blue_button_->pos().rx();
  double blue_right = blue_left + blue_button_->size().width();
  double blue_top = blue_button_->pos().ry();
  double blue_bottom = blue_top + blue_button_->size().height();

  double purple_left = purple_button_->pos().rx();
  double purple_right = purple_left + purple_button_->size().width();
  double purple_top = purple_button_->pos().ry();
  double purple_bottom = purple_top + purple_button_->size().height();

  double green_left = green_button_->pos().rx();
  double green_right = green_left + green_button_->size().width();
  double green_top = green_button_->pos().ry();
  double green_bottom = green_top + green_button_->size().height();

  double red_left = red_button_->pos().rx();
  double red_right = red_left + red_button_->size().width();
  double red_top = red_button_->pos().ry();
  double red_bottom = red_top + red_button_->size().height();

  if ((rx >= yel_left && rx <= yel_right)
      && (ry >= yel_top && ry <= yel_bottom)) {
    QPixmap pixmap_yel(":Buttons/yellow_button_pointed.png");
    pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
  } else {
    QPixmap pixmap_yel(":Buttons/yellow_button.png");
    pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
  }

  if ((rx >= blue_left && rx <= blue_right)
      && (ry >= blue_top && ry <= blue_bottom)) {
    QPixmap pixmap_blue(":Buttons/blue_button_pointed.png");
    pixmap_blue = pixmap_blue.scaled(blue_button_->size());
    blue_button_->setIcon(QIcon(pixmap_blue));
    blue_button_->setIconSize(pixmap_blue.size());
  } else {
    QPixmap pixmap_blue(":Buttons/blue_button.png");
    pixmap_blue = pixmap_blue.scaled(blue_button_->size());
    blue_button_->setIcon(QIcon(pixmap_blue));
    blue_button_->setIconSize(pixmap_blue.size());
  }

  if ((rx >= purple_left && rx <= purple_right)
      && (ry >= purple_top && ry <= purple_bottom)) {
    QPixmap pixmap_pr(":Buttons/purple_button_pointed.png");
    pixmap_pr = pixmap_pr.scaled(purple_button_->size());
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(pixmap_pr.size());
  } else {
    QPixmap pixmap_pr(":Buttons/purple_button.png");
    pixmap_pr = pixmap_pr.scaled(purple_button_->size());
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(pixmap_pr.size());
  }

  if ((rx >= green_left && rx <= green_right)
      && (ry >= green_top && ry <= green_bottom)) {
    QPixmap pixmap_gr(":Buttons/green_button_pointed.png");
    pixmap_gr = pixmap_gr.scaled(green_button_->size());
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
  } else {
    QPixmap pixmap_gr(":Buttons/green_button.png");
    pixmap_gr = pixmap_gr.scaled(green_button_->size());
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
  }

  if ((rx >= red_left && rx <= red_right)
      && (ry >= red_top && ry <= red_bottom)) {
    QPixmap pixmap_red(":Buttons/red_button_pointed.png");
    pixmap_red = pixmap_red.scaled(red_button_->size());
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
  } else {
    QPixmap pixmap_red(":Buttons/red_button.png");
    pixmap_red = pixmap_red.scaled(red_button_->size());
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
  }
}

void GameWidget::SetTracking() {
  setMouseTracking(1);
  yellow_button_->setMouseTracking(1);
  blue_button_->setMouseTracking(1);
  green_button_->setMouseTracking(1);
  red_button_->setMouseTracking(1);
  purple_button_->setMouseTracking(1);
}

void GameWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  QPixmap background = *main_image_;
  background = background.scaled(QApplication::screens()[0]->size(),
                                 Qt::IgnoreAspectRatio);
  QBrush brush(Qt::TexturePattern, background);
  painter.setBrush(brush);
  painter.drawRect(QApplication::desktop()->rect());
  if (progress_ != 0) {
    double width = QApplication::screens()[0]->size().width();
    double height = QApplication::screens()[0]->size().height();

    double start_pos_x = width / 1920 * 700;
    double end_pos_x = width / 1920 * 1130;
    double point_length = (end_pos_x - start_pos_x) / max_progress_;
    painter.setPen(QPen(Qt::green, 68));
    painter.drawLine(start_pos_x,
                       height / 1080 * 86,
                       start_pos_x + point_length * progress_,
                       height / 1080 * 86);
  }
}

void GameWidget::SetProgress(int progress, int max_progress) {
  progress_ = progress;
  max_progress_ = max_progress;
  repaint();
}

void GameWidget::BackgroundShines() {
  connect(shining_timer_, &QTimer::timeout, this, [&] {
    index_ = (index_ + 1) % 2;
    main_image_ = images_shining[index_];

    repaint();
  });
}

void GameWidget::SetCrackAnimation() {
  images_crack[0] = new QPixmap(":Background/background_crack_1.png");
  images_crack[1] = new QPixmap(":Background/background_crack_2.png");
  images_crack[2] = new QPixmap(":Background/background_crack_3.png");
  images_crack[3] = new QPixmap(":Background/background_crack_4.png");
  images_crack[4] = new QPixmap(":Background/background_crack_5.png");
}

void GameWidget::SetMainAnimations() {
  images_shining[0] = new QPixmap(":Background/background_shines.png");
  images_shining[1] = new QPixmap(":Background/display.png");
  main_image_ = images_shining[0];
}

void GameWidget::Loss() {
  connect(loss_timer_, &QTimer::timeout, this, [&] {
    index_ = (index_ + 1) % 5;
    main_image_ = images_crack[index_];

    repaint();
    if (index_ == 4) {
      loss_timer_->stop();
    }
  });
}

void GameWidget::EndGame() {
  index_ = 0;
  shining_timer_->stop();
  blue_button_->pressed();
  red_button_->pressed();
  green_button_->pressed();
  purple_button_->pressed();
  yellow_button_->pressed();
  setMouseTracking(0);
  loss_timer_->start();
  QPixmap pixmap_yellow(":Bulbs/yellow_bulb_on.png");
  pixmap_yellow = pixmap_yellow.scaled(yellow_bulb_red_->size());
  yellow_bulb_red_->setPixmap(pixmap_yellow);
  yellow_bulb_green_->setPixmap(pixmap_yellow);
  yellow_bulb_blue_->setPixmap(pixmap_yellow);
  yellow_bulb_purple_->setPixmap(pixmap_yellow);
  QPixmap pixmap_gr(":Bulbs/green_bulb_on.png");
  pixmap_gr = pixmap_gr.scaled(green_bulb_red_->size());
  green_bulb_red_->setPixmap(pixmap_gr);
  green_bulb_green_->setPixmap(pixmap_gr);
  green_bulb_blue_->setPixmap(pixmap_gr);
  green_bulb_purple_->setPixmap(pixmap_gr);
  QPixmap pixmap_bl(":Bulbs/blue_bulb_on.png");
  pixmap_bl = pixmap_bl.scaled(blue_bulb_red_->size());
  blue_bulb_red_->setPixmap(pixmap_bl);
  blue_bulb_green_->setPixmap(pixmap_bl);
  blue_bulb_blue_->setPixmap(pixmap_bl);
  blue_bulb_purple_->setPixmap(pixmap_bl);
  QPixmap pixmap_red(":Bulbs/red_bulb_on.png");
  pixmap_red = pixmap_red.scaled(red_bulb_red_->size());
  red_bulb_red_->setPixmap(pixmap_red);
  red_bulb_green_->setPixmap(pixmap_red);
  red_bulb_blue_->setPixmap(pixmap_red);
  red_bulb_purple_->setPixmap(pixmap_red);
}

void GameWidget::SetBulbsCount(int minigame_pos, int count) {
  switch (minigame_pos) {
    case 0: {
      switch (count) {
        case 4: {
          QPixmap pixmap_red(":Bulbs/red_bulb_on.png");
          pixmap_red = pixmap_red.scaled(red_bulb_green_->size());
          red_bulb_green_->setPixmap(pixmap_red);
        }
        case 3: {
          QPixmap pixmap_bl(":Bulbs/blue_bulb_on.png");
          pixmap_bl = pixmap_bl.scaled(blue_bulb_green_->size());
          blue_bulb_green_->setPixmap(pixmap_bl);
        }
        case 2: {
          QPixmap pixmap_gr(":Bulbs/green_bulb_on.png");
          pixmap_gr = pixmap_gr.scaled(green_bulb_green_->size());
          green_bulb_green_->setPixmap(pixmap_gr);
        }
        case 1: {
          QPixmap pixmap_yellow(":Bulbs/yellow_bulb_on.png");
          pixmap_yellow = pixmap_yellow.scaled(yellow_bulb_green_->size());
          yellow_bulb_green_->setPixmap(pixmap_yellow);
        }
        default: {}
      }
      break;
    }
    case 1: {
      switch (count) {
        case 4: {
          QPixmap pixmap_red(":Bulbs/red_bulb_on.png");
          pixmap_red = pixmap_red.scaled(red_bulb_red_->size());
          red_bulb_red_->setPixmap(pixmap_red);
        }
        case 3: {
          QPixmap pixmap_bl(":Bulbs/blue_bulb_on.png");
          pixmap_bl = pixmap_bl.scaled(blue_bulb_red_->size());
          blue_bulb_red_->setPixmap(pixmap_bl);
        }
        case 2: {
          QPixmap pixmap_gr(":Bulbs/green_bulb_on.png");
          pixmap_gr = pixmap_gr.scaled(green_bulb_red_->size());
          green_bulb_red_->setPixmap(pixmap_gr);
        }
        case 1: {
          QPixmap pixmap_yellow(":Bulbs/yellow_bulb_on.png");
          pixmap_yellow = pixmap_yellow.scaled(yellow_bulb_red_->size());
          yellow_bulb_red_->setPixmap(pixmap_yellow);
        }
        default: {}
      }
      break;
    }
    case 2: {
      switch (count) {
        case 4: {
          QPixmap pixmap_red(":Bulbs/red_bulb_on.png");
          pixmap_red = pixmap_red.scaled(red_bulb_purple_->size());
          red_bulb_purple_->setPixmap(pixmap_red);
        }
        case 3: {
          QPixmap pixmap_bl(":Bulbs/blue_bulb_on.png");
          pixmap_bl = pixmap_bl.scaled(blue_bulb_purple_->size());
          blue_bulb_purple_->setPixmap(pixmap_bl);
        }
        case 2: {
          QPixmap pixmap_gr(":Bulbs/green_bulb_on.png");
          pixmap_gr = pixmap_gr.scaled(green_bulb_purple_->size());
          green_bulb_purple_->setPixmap(pixmap_gr);
        }
        case 1: {
          QPixmap pixmap_yellow(":Bulbs/yellow_bulb_on.png");
          pixmap_yellow = pixmap_yellow.scaled(yellow_bulb_purple_->size());
          yellow_bulb_purple_->setPixmap(pixmap_yellow);
        }
        default: {}
      }
      break;
    }
    case 3: {
      switch (count) {
        case 4: {
          QPixmap pixmap_red(":Bulbs/red_bulb_on.png");
          pixmap_red = pixmap_red.scaled(red_bulb_blue_->size());
          red_bulb_blue_->setPixmap(pixmap_red);
        }
        case 3: {
          QPixmap pixmap_bl(":Bulbs/blue_bulb_on.png");
          pixmap_bl = pixmap_bl.scaled(blue_bulb_blue_->size());
          blue_bulb_blue_->setPixmap(pixmap_bl);
        }
        case 2: {
          QPixmap pixmap_gr(":Bulbs/green_bulb_on.png");
          pixmap_gr = pixmap_gr.scaled(green_bulb_blue_->size());
          green_bulb_blue_->setPixmap(pixmap_gr);
        }
        case 1: {
          QPixmap pixmap_yellow(":Bulbs/yellow_bulb_on.png");
          pixmap_yellow = pixmap_yellow.scaled(yellow_bulb_blue_->size());
          yellow_bulb_blue_->setPixmap(pixmap_yellow);
        }
        default: {}
      }
      break;
    }
    default: {}
  }
}
