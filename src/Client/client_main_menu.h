#ifndef CLIENT_MAIN_MENU_H_
#define CLIENT_MAIN_MENU_H_

#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>

class ClientMainMenu : public QWidget {
  Q_OBJECT
 public:
  ClientMainMenu();
 private:
  QGridLayout* main_layout_;
  QListWidget* player_list_;
  QPushButton* settings_;
  QPushButton* play_;
  QPushButton* start_game_;
  QPushButton* create_room_;
  QPushButton* join_room_;

};

#endif // CLIENT_MAIN_MENU_H_
