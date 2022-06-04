#pragma once

#include "QIcon"
#include "QImage"
#include "QLabel"
#include "QMainWindow"
#include "QPainter"
#include <QPicture>
#include "QPixmap"
#include <QPushButton>
#include "QTimer"
#include "QWidget"

class GameWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GameWidget(QWidget* parent = nullptr);

  ~GameWidget() override = default;

  void SetProgress(int progress, int max_progress);
  void SetBulbsCount(int minigame_pos, int count);
  void TurnOffGreen();
  void TurnOffRed();
  void TurnOffPurple();
  void TurnOffBlue();


 signals:
  void JoinMinigame(int minigame_index_pos);
  void LeaveMinigame();

 private:
  void SetIcons();
  void SetButtonsGeometry();
  void SetButtonsSize();
  void ButtonClicked();
  void SetBulbsIcons();
  void SetBulbsSize();
  void SetBulbsGeometry();
  void SetTracking();
  void mouseMoveEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void BackgroundShines();
  void SetMainAnimations();
  void SetCrackAnimation();
  void Loss();
  void EndGame();

  QPushButton* red_button_;
  QPushButton* green_button_;
  QPushButton* blue_button_;
  QPushButton* yellow_button_;
  QPushButton* purple_button_;
  QLabel* red_bulb_red_;
  QLabel* green_bulb_red_;
  QLabel* blue_bulb_red_;
  QLabel* yellow_bulb_red_;
  QLabel* red_bulb_purple_;
  QLabel* green_bulb_purple_;
  QLabel* blue_bulb_purple_;
  QLabel* yellow_bulb_purple_;
  QLabel* red_bulb_green_;
  QLabel* green_bulb_green_;
  QLabel* blue_bulb_green_;
  QLabel* yellow_bulb_green_;
  QLabel* red_bulb_blue_;
  QLabel* green_bulb_blue_;
  QLabel* blue_bulb_blue_;
  QLabel* yellow_bulb_blue_;
  QTimer* shining_timer_;
  QTimer* loss_timer_;
  QPixmap* main_image_;
  QPixmap* images_shining[2];
  QPixmap* images_crack[6];
  int progress_ = 0;
  int max_progress_;
  int index_ = 0;
};
