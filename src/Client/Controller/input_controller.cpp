#include "input_controller.h"

InputController::InputController(QWidget* parent) {
  timer_.start(tick_, this);
}

void InputController::KeyPressed(quint32 key_number) {
  auto key = KeyNames::kNativeCodeToKeyName.at(key_number);
  if (FindKey(key)) {
    return;
  }
  keys_pressed_.emplace_back(Key(key));
  emit KeyEventToServer(key);
}

void InputController::MousePosStartTracking() {
  mouse_pressed_ = true;
}

void InputController::MousePosStopTracking() {
  mouse_pressed_ = false;
}

void InputController::MouseMove(const QPoint& pos) {
  if (!mouse_pressed_) {
    return;
  }
  emit MouseMoveToServer(pos);
}

void InputController::timerEvent(QTimerEvent* event) {
  QObject::timerEvent(event);
  RemoveKeys();
  AddTime();
}

void InputController::AddTime() {
  for (auto& key : keys_pressed_) {
    key.time += 15;
  }
}

void InputController::RemoveKeys() {
  std::vector<decltype(keys_pressed_.begin())> keys_to_remove_;
  for (auto key = keys_pressed_.begin(); key != keys_pressed_.end(); ++key) {
    if (key->time > hold_time_) {
      keys_to_remove_.push_back(key);
    }
  }
  for (const auto& it: keys_to_remove_) {
    keys_pressed_.erase(it);
  }
}
bool InputController::FindKey(const std::string& key) {
  for (const auto& key_it : keys_pressed_) {
    if (key_it.key == key) {
      return true;
    }
  }
  return false;
}

Key::Key(std::string pressed_key) {
  key = pressed_key;
  time = 0;
}
