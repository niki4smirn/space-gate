#ifndef SRC_CLIENT_CLIENT_VIEW_H_
#define SRC_CLIENT_CLIENT_VIEW_H_

#include "client_main_menu.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>

class ClientView : public QMainWindow {
  Q_OBJECT
 public:
  ClientView(ClientMainMenu* main_menu);

 private:
  void CloseWindow();
  void AddWidgets();
  void Connect();

 private:
  ClientMainMenu* main_menu_;
  QStackedWidget* stacked_widget_;
};

#endif //  SRC_CLIENT_CLIENT_VIEW_H_
