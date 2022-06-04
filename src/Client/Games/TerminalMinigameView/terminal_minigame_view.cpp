#include "terminal_minigame_view.h"

#include <QFontDatabase>

#include "src/Client/InputController/input_names.h"

TerminalMinigameView::TerminalMinigameView(QWidget* parent)
    : QWidget(parent),
      static_text_(new QLabel(this)),
      needed_text_(new QLabel(this)),
      entered_text_(new QLabel(this)),
      available_keys_(new QLabel(this)),
      layout_(new QGridLayout(this)) {
  QString family =
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
          "../Resources/Fonts/Fairfax.ttf")).at(0);
  auto font = QFont(family);
  font.setPointSize(36);

  static_text_->setStyleSheet("QLabel {color : #20C20E; }");
  static_text_->setFont(font);
  needed_text_->setStyleSheet("QLabel {color : lightgray; }");
  needed_text_->setFont(font);
  entered_text_->setStyleSheet("QLabel {color : #20C20E; }");
  entered_text_->setFont(font);
  available_keys_->setStyleSheet("QLabel {color : #20C20E; }");
  available_keys_->setFont(font);

  layout_->addWidget(static_text_, 0, 0, Qt::AlignBottom | Qt::AlignLeft);
  layout_->addWidget(needed_text_, 1, 0, Qt::AlignBottom | Qt::AlignLeft);
  layout_->addWidget(entered_text_, 1, 0, Qt::AlignBottom | Qt::AlignLeft);
  layout_->addWidget(available_keys_, 2, 0, Qt::AlignBottom | Qt::AlignLeft);

  layout_->setRowStretch(0, 1);
  layout_->setRowStretch(1, 1);
  layout_->setRowStretch(2, 12);

  setLayout(layout_);
  repaint();
}

void TerminalMinigameView::PaintBackground(QPainter* painter) {
  painter->save();

  auto width = size().width();
  auto height = size().width();

  painter->setBrush(QColorConstants::Black);
  painter->setPen(QColorConstants::Black);
  painter->drawRect(0, 0, width, height);

  painter->setBrush(QColor(32, 194, 14));
  painter->drawRect(0, 0, width * (remaining_time_ * 1.0 / total_time_), 10);

  painter->restore();
}

void TerminalMinigameView::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  PaintBackground(&painter);
}

void TerminalMinigameView::InitializeView(
    const minigame_responses::MinigameResponse& response) {
  auto& initial_response = response.initial_terminal_respone();
  total_time_ = response.remaining_time();
  remaining_time_ = total_time_;

  needed_text_->setText(
      text_prefix_ + QString::fromStdString(initial_response.text()));

  QString available_keys = "Available keys: ";

  for (const auto& id : initial_response.available_buttons_id()) {
    QString temp = input::InputNameToString(static_cast<input::Name>(id));
    temp.remove(0, 1);
    available_keys += "\"" + temp + "\",";
  }
  available_keys.remove(available_keys.size() - 1, 1);

  available_keys_->setText(available_keys);

  repaint();
}

void TerminalMinigameView::UpdateView(
    const minigame_responses::MinigameResponse& response) {
  remaining_time_ = response.remaining_time();

  auto& game_response = response.terminal_response();
  entered_text_->setText(
      text_prefix_ + QString::fromStdString(game_response.entered_text()));

  repaint();
}
