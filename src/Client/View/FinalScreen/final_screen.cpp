#include "final_screen.h"

FinalScreen::FinalScreen(QWidget* parent) : QWidget(parent) {
  if (parent) {
    resize(parent->size());
  }
  background_ = new BackgroundWidget(this);
  to_lobby_button_ = new QPushButton(this);
  to_menu_button_ = new QPushButton(this);
  info_ = new QLabel("Important info", this);
  game_name_ = new QLabel(this);
  layout_ = new QGridLayout(this);
  SetName();
  SetSize();
  SetLayout();
  SetInfo();
  Connect();
}

FinalScreen::~FinalScreen() {

}

void FinalScreen::SetSize() {
  background_->setFixedSize(QApplication::screens()[0]->size().width(),
                            QApplication::screens()[0]->size().height());

  to_menu_button_->setFixedSize(350, 200);
  to_menu_button_->setText("Menu");
  to_menu_button_->setFont(font_);
  to_menu_button_->setStyleSheet("background-color: rgb(136, 247, 255);font-size: 40px;");

  to_lobby_button_->setFixedSize(350, 200);
  to_lobby_button_->setText("Lobby");
  to_lobby_button_->setFont(font_);
  to_lobby_button_->setStyleSheet("background-color: rgb(136, 247, 255);font-size: 40px;");
}

void FinalScreen::SetName() {
  QString family =
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
          "../Resources/Fonts/Paladins.otf")).at(0);
  font_ =  QFont(family);
  game_name_->setVisible(1);
  game_name_->setText("Spacegate");
  game_name_->setStyleSheet("QLabel {color : #88bcff; }");
  font_.setPointSize(105);
  font_.setBold(true);
  game_name_->setFont(font_);
}

void FinalScreen::SetLayout() {
  layout_->addWidget(game_name_, 0, 0, 1, 2,Qt::AlignHCenter);
  layout_->addWidget(to_menu_button_, 1, 0,Qt::AlignRight | Qt::AlignVCenter);
  layout_->addWidget(to_lobby_button_, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
  layout_->addWidget(info_, 2, 0, 1, 2,
                     Qt::AlignHCenter | Qt::AlignVCenter);
  this->setLayout(layout_);
}

void FinalScreen::SetInfo() {
  info_->setVisible(1);
  if (victory_ == 1) {
    info_->setText("It did was a success");
  } else {
    info_->setText("It didn't was a success");
  }
  info_->setStyleSheet("QLabel {color : #88bcff; }");
  font_.setPointSize(40);
  font_.setBold(0);
  info_->setFont(font_);
}

void FinalScreen::MenuPressedEvent() {
  emit MenuPressed();
}

void FinalScreen::LobbyPressedEvent() {
  emit LobbyPressed();
}

void FinalScreen::Connect() {
  connect(to_menu_button_, &QPushButton::clicked,
          this, &FinalScreen::MenuPressedEvent);
  connect(to_lobby_button_, &QPushButton::clicked,
          this, &FinalScreen::LobbyPressedEvent);
}
