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
    SetIcons();
    SetButtonsSize();
    SetButtonsGeometry();
    SetBackground();
    ButtonClicked();
}

MainWidget::~MainWidget() {

}

void MainWidget::SetIcons() {
    QPixmap pixmap_yel(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/yellow_button.png");
    yellow_button_->setIcon(QIcon(pixmap_yel));
    yellow_button_->setIconSize(pixmap_yel.size());
    yellow_button_->setFixedSize(pixmap_yel.size());
    yellow_button_->setVisible(1);

    QPixmap pixmap_gr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/green_button.png");
    green_button_->setIcon(QIcon(pixmap_gr));
    green_button_->setIconSize(pixmap_gr.size());
    green_button_->setFixedSize(pixmap_gr.size());
    green_button_->setVisible(1);

    QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button.png");
    pixmap_bl = pixmap_bl.scaled(QSize(378, 120), Qt::IgnoreAspectRatio);
    blue_button_->setIcon(QIcon(pixmap_bl));
    blue_button_->setIconSize(pixmap_bl.size());
    blue_button_->setFixedSize(pixmap_bl.size());
    blue_button_->setVisible(1);

    QPixmap pixmap_pr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/purple_button.png");
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(QSize(pixmap_bl.size()));
    purple_button_->setFixedSize(pixmap_pr.size());
    purple_button_->setVisible(1);

    QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/red_button.png");
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
    red_button_->setFixedSize(pixmap_red.size());
    red_button_->setVisible(1);
}

void MainWidget::SetButtonsSize() {
    blue_button_->setFixedSize(378, 120);
    green_button_->setFixedSize(295, 120);
    red_button_->setFixedSize(220, 175);
    yellow_button_->setFixedSize(316, 130);
}

void MainWidget::SetButtonsGeometry() {
    blue_button_->setFlat(1);
    blue_button_->setGeometry(1410, 930, 100, 200);
    green_button_->setFlat(1);
    green_button_->setGeometry(68, 930, 100, 100);
    red_button_->setFlat(1);
    red_button_->setGeometry(472, 878, 100, 100);
    yellow_button_->setFlat(1);
    yellow_button_->setGeometry(727, 810, 100, 100);
    purple_button_->setGeometry(1080, 885, 100, 100);
    purple_button_->setFixedSize(225, 150);
    purple_button_->setFlat(1);
    std::cerr<<QApplication::screens()[0]->size().height();
    std::cerr<<QApplication::screens()[0]->size().width();
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
        QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button_clicked_2.png");
        pixmap_bl = pixmap_bl.scaled(QSize(378, 120), Qt::IgnoreAspectRatio);
        blue_button_->setIcon(QIcon(pixmap_bl));
        blue_button_->setIconSize(pixmap_bl.size());
        blue_button_->setFixedSize(pixmap_bl.size());
        blue_button_->setVisible(1);
    });
    connect(blue_button_, &QPushButton::released, this, [=] {
        QPixmap pixmap_bl(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/blue_button.png");
        pixmap_bl = pixmap_bl.scaled(QSize(378, 120), Qt::IgnoreAspectRatio);
        blue_button_->setIcon(QIcon(pixmap_bl));
        blue_button_->setIconSize(pixmap_bl.size());
        blue_button_->setFixedSize(pixmap_bl.size());
        blue_button_->setVisible(1);
    });
}


