#ifndef CLIENT_MAIN_MENU_H_
#define CLIENT_MAIN_MENU_H_

#include "background_widget.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QListWidget>

class ClientMainMenu : public QWidget {
  Q_OBJECT
 public:
  ClientMainMenu();
  void Play();
  void Connect();
 private:
  BackgroundWidget* background_;
  QGridLayout* interface_layout_;
  QGridLayout* background_layout_;
  QListWidget* player_list_;
  QPushButton* settings_;
  QPushButton* exit_;
  QPushButton* play_;
  QPushButton* start_game_;
  QPushButton* create_room_;
  QPushButton* join_room_;
  QLabel* game_name_;
  QWidget* interface_;

  signals:
    void Start(bool state);
};

#endif // CLIENT_MAIN_MENU_H_
