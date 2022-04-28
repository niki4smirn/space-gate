#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include "client_main_menu.h"

#include <QWidget>

class СlientView : public QWidget {
  Q_OBJECT
 public:
  ClientView();
 private:
  ClientMainMenu* main_menu_;

};

#endif // CLIENT_VIEW_H_
