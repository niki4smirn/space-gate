#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include "client_main_menu.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>

class ClientView : public QMainWindow {
  Q_OBJECT
 public:
  ClientView();

 private:
  ClientMainMenu* main_menu_;
  QStackedWidget* stacked_widget_;
};

#endif // CLIENT_VIEW_H_
