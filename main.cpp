#include "game_widget.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget window;
    window.show();
    return a.exec();
}