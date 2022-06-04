#pragma once

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QGridLayout>

#include "Protobuf/minigame_responses.pb.h"

class TerminalMinigameView : public QWidget{
  Q_OBJECT
 public:
  explicit TerminalMinigameView(QWidget* parent = nullptr);

  void InitializeView(const minigame_responses::MinigameResponse& response);
  void UpdateView(const minigame_responses::MinigameResponse& response);

 private:
  QLabel* static_text_;
  QLabel* needed_text_;
  QLabel* entered_text_;
  QLabel* available_keys_;

  QGridLayout* layout_;

  const QString text_prefix_ = "user@spaceship:~/ ";

  uint64_t total_time_ = 1;
  uint64_t remaining_time_ = 1;

  void PaintBackground(QPainter* painter);
  void paintEvent(QPaintEvent* event) override;
};
