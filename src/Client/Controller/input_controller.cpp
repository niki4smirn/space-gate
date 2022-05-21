#include "input_controller.h"

InputController::InputController(QWidget* parent) {}

void InputController::KeyPressed(quint32 key_number) {
  keys_pressed_.insert(KeyNames::kNativeCodeToKeyName.at(key_number).toStdString());
  if (keys_pressed_.size() != prev_size_) {
    emit KeyEventToServer(keys_pressed_);
    prev_size_ = keys_pressed_.size();
  }
}

void InputController::KeyRelease(quint32 key_number) {
  keys_pressed_.erase(KeyNames::kNativeCodeToKeyName.at(key_number).toStdString());
}
