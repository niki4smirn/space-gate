#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include "QIcon"
#include "QImage"
#include "QLabel"
#include "QMainWindow"
#include "QPainter"
#include <QPicture>
#include "QPixmap"
#include <QPushButton>
#include "QTimer"
#include "QWidget"

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget(QWidget* parent = nullptr);

    ~MainWidget();

private:
    QPushButton* red_button_;
    QPushButton* green_button_;
    QPushButton* blue_button_;
    QPushButton* yellow_button_;
    QPushButton* purple_button_;
    QLabel* red_bulb_red_;
    QLabel* green_bulb_red_;
    QLabel* blue_bulb_red_;
    QLabel* yellow_bulb_red_;
    QLabel* red_bulb_purple_;
    QLabel* green_bulb_purple_;
    QLabel* blue_bulb_purple_;
    QLabel* yellow_bulb_purple_;
    QLabel* red_bulb_green_;
    QLabel* green_bulb_green_;
    QLabel* blue_bulb_green_;
    QLabel* yellow_bulb_green_;
    QLabel* red_bulb_blue_;
    QLabel* green_bulb_blue_;
    QLabel* blue_bulb_blue_;
    QLabel* yellow_bulb_blue_;
    QPainter* painter_;
    QTimer* shining_timer_;
    QTimer* loss_timer_;
    QPixmap main_image_ ;
    QPixmap* images_shining[2];
    QPixmap* images_crack[6];
    int progress_ = 0;
    int max_progress_;
    int player_number_;
    int index_ = 0;


private:
    void SetIcons();
    void SetButtonsGeometry();
    void SetButtonsSize();
    void SetBackground();
    void ButtonClicked();
    void SetBulbsIcons();
    void SetBulbsSize();
    void SetBulbsGeometry();
    void SetTracking();
    void mouseMoveEvent(QMouseEvent* event) override;
    void MiniGameChosen();
    void paintEvent(QPaintEvent *event) override;
    void SetProgress(int progress, int max_progress);
    void SetPlayerNumber(int player_number);
    void BackgroundShines();
    void SetMainAnimations();
    void SetCrackAnimation();
    void Loss();
    void EndGame();
};






#endif // GAME_WIDGET_H
