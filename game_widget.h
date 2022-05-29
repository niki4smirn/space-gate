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

class MainWidget : public QMainWindow {
Q_OBJECT
public:
    MainWidget();

    ~MainWidget();

private:
    QWidget* widget_;
    QPushButton* red_button_;
    QPushButton* green_button_;
    QPushButton* blue_button_;
    QPushButton* yellow_button_;
    QPushButton* purple_button_;
    void SetIcons();
    void SetGeometry();
};






#endif // GAME_WIDGET_H
