#include <iostream>
#include "game_widget.h"

#include <QApplication>
#include <QScreen>


MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
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
    SetButtonsSize();
    SetIcons();
    SetButtonsGeometry();
    SetBackground();
    ButtonClicked();
//    setMouseTracking(1);
    SetBulbsSize();
    SetBulbsIcons();
    SetBulbsGeometry();
}

MainWidget::~MainWidget() {

}

void MainWidget::SetIcons() {
    QPixmap pixmap_yel(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/yellow_button.png");
    pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
    yellow_button_->setVisible(1);

    QPixmap pixmap_gr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/green_button.png");
    pixmap_gr = pixmap_gr.scaled(green_button_->size());
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
    green_button_->setVisible(1);

    QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button.png");
    pixmap_bl = pixmap_bl.scaled(blue_button_->size());
    blue_button_->setIcon(QIcon(pixmap_bl));
    blue_button_->setIconSize(pixmap_bl.size());
    blue_button_->setVisible(1);

    QPixmap pixmap_pr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/purple_button.png");
    pixmap_pr = pixmap_pr.scaled(purple_button_->size());
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(QSize(pixmap_pr.size()));
    purple_button_->setVisible(1);

    QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/red_button.png");
    pixmap_red = pixmap_red.scaled(QSize(220, 175), Qt::IgnoreAspectRatio);
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
    red_button_->setVisible(1);
}

void MainWidget::SetButtonsSize() {
    double width = QApplication::screens()[0]->size().width();
    double height = QApplication::screens()[0]->size().height();
    blue_button_->setFixedSize(width / 1920 * 378, height / 1080 * 120);
    green_button_->setFixedSize(width / 1920 * 295, height / 1080 * 120);
    red_button_->setFixedSize(width / 1920 * 220, height / 1080 * 175);
    yellow_button_->setFixedSize(width / 1920 * 316, height / 1080 * 130);
    purple_button_->setFixedSize(width / 1920 * 225, height / 1080 * 150);

}

void MainWidget::SetButtonsGeometry() {
    double width = QApplication::screens()[0]->size().width();
    double height = QApplication::screens()[0]->size().height();

    blue_button_->setGeometry(width * 1410 / 1920, height / 1080 * 930, 1, 1);
    green_button_->setGeometry(width * 68 / 1920, height / 1080 * 930 , 1, 1);
    red_button_->setGeometry(width / 1920 * 472, height / 1080 * 878, 1, 1);
    yellow_button_->setGeometry(width / 1920 * 727, height / 1080 * 810, 1, 1);
    purple_button_->setGeometry(width / 1920 * 1080, height / 1080 * 885, 1, 1);
}

void MainWidget::SetBackground() {
    QPixmap background(":/home/ekkatrina_hottova/SpaceGatePics/empty_display.png");
    background = background.scaled(QApplication::screens()[0]->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);
}

void MainWidget::ButtonClicked() {
    connect(blue_button_, &QPushButton::pressed, this, [=] {
        QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button_clicked.png");
        pixmap_bl = pixmap_bl.scaled(blue_button_->size());
        blue_button_->setIcon(QIcon(pixmap_bl));
        blue_button_->setIconSize(pixmap_bl.size());
    });
    connect(blue_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button.png");
        pixmap_bl = pixmap_bl.scaled(blue_button_->size());
        blue_button_->setIcon(QIcon(pixmap_bl));
        blue_button_->setIconSize(pixmap_bl.size());
    });

    connect(red_button_, &QPushButton::pressed, this, [=] {
        QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/red_button_clicked.png");
        pixmap_red = pixmap_red.scaled(red_button_->size());
        red_button_->setIcon(QIcon(pixmap_red));
        red_button_->setIconSize(pixmap_red.size());
    });
    connect(red_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/red_button.png");
        pixmap_red = pixmap_red.scaled(red_button_->size());
        red_button_->setIcon(QIcon(pixmap_red));
        red_button_->setIconSize(pixmap_red.size());
    });

    connect(green_button_, &QPushButton::pressed, this, [=] {
        QPixmap pixmap_gr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/green_button_clicked.png");
        pixmap_gr = pixmap_gr.scaled(green_button_->size());
        green_button_->setIcon(QIcon(pixmap_gr));
        green_button_->setIconSize(pixmap_gr.size());
    });
    connect(green_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_gr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/green_button.png");
        pixmap_gr = pixmap_gr.scaled(green_button_->size());
        green_button_->setIcon(QIcon(pixmap_gr));
        green_button_->setIconSize(pixmap_gr.size());
    });

    connect(purple_button_, &QPushButton::pressed, this, [=] {
        QPixmap pixmap_pr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/purple_button_clicked.png");
        pixmap_pr = pixmap_pr.scaled(purple_button_->size());
        purple_button_->setIcon(QIcon(pixmap_pr));
        purple_button_->setIconSize(pixmap_pr.size());
    });
    connect(purple_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_pr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/purple_button.png");
        pixmap_pr = pixmap_pr.scaled(purple_button_->size());
        purple_button_->setIcon(QIcon(pixmap_pr));
        purple_button_->setIconSize(pixmap_pr.size());
    });

    connect(yellow_button_, &QPushButton::pressed, this, [=] {
        QPixmap pixmap_yel(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/yellow_button_clicked.png");
        pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
        yellow_button_->setIcon(QIcon(pixmap_yel));
        yellow_button_->setIconSize(pixmap_yel.size());
    });
    connect(yellow_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_yel(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/yellow_button.png");
        pixmap_yel = pixmap_yel.scaled(yellow_button_->size());
        yellow_button_->setIcon(QIcon(pixmap_yel));
        yellow_button_->setIconSize(pixmap_yel.size());
    });
}

void MainWidget::SetBulbsIcons() {
    QPixmap pixmap_yel(":/home/ekkatrina_hottova/SpaceGatePics/Bulbs/yellow_bulb_off.png");
    pixmap_yel = pixmap_yel.scaled(yellow_bulb_red_->size());
    yellow_bulb_red_->setPixmap(pixmap_yel);
    yellow_bulb_purple_->setPixmap(pixmap_yel);
    yellow_bulb_green_->setPixmap(pixmap_yel);
    yellow_bulb_blue_->setPixmap(pixmap_yel);

    QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Bulbs/blue_bulb_off.png");
    pixmap_bl = pixmap_bl.scaled(blue_bulb_red_->size());
    blue_bulb_red_->setPixmap(pixmap_bl);
    blue_bulb_purple_->setPixmap(pixmap_bl);
    blue_bulb_green_->setPixmap(pixmap_bl);
    blue_bulb_blue_->setPixmap(pixmap_bl);

    QPixmap pixmap_gr(":/home/ekkatrina_hottova/SpaceGatePics/Bulbs/green_bulb_off.png");
    pixmap_gr = pixmap_gr.scaled(green_bulb_red_->size());
    green_bulb_red_->setPixmap(pixmap_gr);
    green_bulb_purple_->setPixmap(pixmap_gr);
    green_bulb_green_->setPixmap(pixmap_gr);
    green_bulb_blue_->setPixmap(pixmap_gr);

    QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Bulbs/red_bulb_off.png");
    pixmap_red = pixmap_red.scaled(red_bulb_red_->size());
    red_bulb_red_->setPixmap(pixmap_red);
    red_bulb_purple_->setPixmap(pixmap_red);
    red_bulb_green_->setPixmap(pixmap_red);
    red_bulb_blue_->setPixmap(pixmap_red);
}

void MainWidget::SetBulbsSize() {
    double width = QApplication::screens()[0]->size().width() / 1920 * 40;
    double height = QApplication::screens()[0]->size().height() / 1080 * 31;

    yellow_bulb_red_->setFixedSize(width, height);
    red_bulb_red_->setFixedSize(width, height);
    blue_bulb_red_->setFixedSize(width, height);
    green_bulb_red_->setFixedSize(width, height);

    yellow_bulb_purple_->setFixedSize(width, height);
    red_bulb_purple_->setFixedSize(width, height);
    blue_bulb_purple_->setFixedSize(width, height);
    green_bulb_purple_->setFixedSize(width, height);

    yellow_bulb_green_->setFixedSize(width, height);
    red_bulb_green_->setFixedSize(width, height);
    blue_bulb_green_->setFixedSize(width, height);
    green_bulb_green_->setFixedSize(width, height);

    yellow_bulb_blue_->setFixedSize(width, height);
    red_bulb_blue_->setFixedSize(width, height);
    blue_bulb_blue_->setFixedSize(width, height);
    green_bulb_blue_->setFixedSize(width, height);

}

void MainWidget::SetBulbsGeometry() {
    double width = QApplication::screens()[0]->size().width();
    double height = QApplication::screens()[0]->size().height();

    yellow_bulb_red_->setGeometry(width / 1920 * 487, height / 1080 * 825, 1, 1);
    green_bulb_red_->setGeometry(width / 1920 * 532, height / 1080 * 825, 1, 1);
    blue_bulb_red_->setGeometry(width / 1920 * 577, height / 1080 * 825, 1, 1);
    red_bulb_red_->setGeometry(width / 1920 * 622, height / 1080 * 825, 1, 1);

    yellow_bulb_purple_->setGeometry(width / 1920 * 1095, height / 1080 * 847, 1, 1);
    green_bulb_purple_->setGeometry(width / 1920 * 1147, height / 1080 * 847, 1, 1);
    blue_bulb_purple_->setGeometry(width / 1920 * 1200, height / 1080 * 847, 1, 1);
    red_bulb_purple_->setGeometry(width / 1920 * 1252, height / 1080 * 847, 1, 1);

    yellow_bulb_green_->setGeometry(width / 1920 * 82, height / 1080 * 892, 1, 1);
    green_bulb_green_->setGeometry(width / 1920 * 142, height / 1080 * 892, 1, 1);
    blue_bulb_green_->setGeometry(width / 1920 * 202, height / 1080 * 892, 1, 1);
    red_bulb_green_->setGeometry(width / 1920 * 263, height / 1080 * 892, 1, 1);

    yellow_bulb_blue_->setGeometry(width / 1920 * 1575, height / 1080 * 892, 1, 1);
    green_bulb_blue_->setGeometry(width / 1920 * 1627, height / 1080 * 892, 1, 1);
    blue_bulb_blue_->setGeometry(width / 1920 * 1680, height / 1080 * 892, 1, 1);
    red_bulb_blue_->setGeometry(width / 1920 * 1733, height / 1080 * 892, 1, 1);


}


//void MainWidget::mouseMoveEvent(QMouseEvent *event) {
//    QWidget::mouseMoveEvent(event);
//    if (blue_button_->is) {
//        std::cerr<<"lol ";
//    } else {
//        std::cerr<<"cringe ";
//    }
//}








