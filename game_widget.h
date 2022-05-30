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
    void SetIcons();
    void SetButtonsGeometry();
    void SetButtonsSize();
    void SetBackground();
    void ButtonClicked();
    void ButtonHovered();
};






#endif // GAME_WIDGET_H
