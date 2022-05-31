#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include "QIcon"
#include "QImage"
#include "QLabel"
#include "QMainWindow"
#include <QPicture>
#include "QPixmap"
#include <QPushButton>
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
    void SetIcons();
    void SetButtonsGeometry();
    void SetButtonsSize();
    void SetBackground();
    void ButtonClicked();
    void SetBulbsIcons();
    void SetBulbsSize();
    void SetBulbsGeometry();
//    void mouseMoveEvent(QMouseEvent* event) override;
};






#endif // GAME_WIDGET_H
