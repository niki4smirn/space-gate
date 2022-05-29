#include "game_widget.h"

MainWidget::MainWidget() {
    widget_ = new QWidget;
    setCentralWidget(widget_);
    yellow_button_ = new QPushButton(widget_);
    green_button_ = new QPushButton(widget_);
    purple_button_ = new QPushButton(widget_);
    red_button_ = new QPushButton(widget_);
    blue_button_ = new QPushButton(widget_);
    SetIcons();
    SetGeometry();
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
    blue_button_->setIcon(QIcon(pixmap_bl));
    blue_button_->setIconSize(pixmap_bl.size());
    blue_button_->setFixedSize(pixmap_bl.size());
    blue_button_->setVisible(1);

    QPixmap pixmap_pr(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/purple_button.png");
    purple_button_->setIcon(QIcon(pixmap_pr));
    purple_button_->setIconSize(pixmap_pr.size());
    purple_button_->setFixedSize(pixmap_pr.size());
    purple_button_->setVisible(1);

    QPixmap pixmap_red(":/home/ekkatrina_hottova/SpaceGatePics/Buttons/red_button.png");
    red_button_->setIcon(QIcon(pixmap_red));
    red_button_->setIconSize(pixmap_red.size());
    red_button_->setFixedSize(pixmap_red.size());
    red_button_->setVisible(1);
}

void MainWidget::SetGeometry() {

}



