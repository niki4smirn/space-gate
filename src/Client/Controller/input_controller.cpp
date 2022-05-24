#include "input_controller.h"

InputController::InputController(QWidget* parent) {}

void InputController::KeyPressed(quint32 key_number) {
  auto key = KeyNames::kNativeCodeToKeyName.at(key_number).toStdString();
  if (keys_pressed_.contains(key)){
    return;
  }
  keys_pressed_.insert(key);
}

void InputController::KeyReleased(quint32 key_number) {
  auto key = KeyNames::kNativeCodeToKeyName.at(key_number).toStdString();
  if (!keys_pressed_.contains(key)){
    return;
  }
  keys_pressed_.erase(key);
  emit KeyEventToServer(key);
}
#include <iostream>
void InputController::MousePosStartTracking() {
  mouse_pressed_ = true;
}

void InputController::MousePosStopTracking() {
  mouse_pressed_ = false;
}

void InputController::MouseMove(const QPoint& pos) {
  if (!mouse_pressed_){
    return;
  }
  emit MouseMoveToServer(pos);
}
