#include "input_controller.h"

InputController::InputController(QWidget* parent) {
  timer_.start(constants::kKeyTickTime, this);
}

void InputController::KeyPressed(quint32 key_number) {
  if (!key_names::kNativeCodeToKeyName.contains(key_number)) {
    return;
  }
  auto key = key_names::kNativeCodeToKeyName.at(key_number);
  if (FindKey(key)) {
    return;
  }
  keys_pressed_.emplace_back(key);
  emit KeyEventToServer(key);
}

void InputController::MousePosStartTracking() {
  track_mouse_ = true;
}

void InputController::MousePosStopTracking() {
  track_mouse_ = false;
}

void InputController::MouseMove(const QPoint& pos) {
  if (!track_mouse_) {
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
    key.time += constants::kKeyTickTime;
  }
}

void InputController::RemoveKeys() {
  std::vector<decltype(keys_pressed_.begin())> keys_to_remove_;
  for (auto key = keys_pressed_.begin(); key != keys_pressed_.end(); ++key) {
    if (key->time > hold_time_) {
      keys_to_remove_.push_back(key);
    }
  }
  for (const auto& it : keys_to_remove_) {
    keys_pressed_.erase(it);
  }
}

bool InputController::FindKey(const std::string& key) {
  return std::any_of(keys_pressed_.begin(),
                     keys_pressed_.end(),
                     [key](auto key_it) { return key_it.key == key; });
}

Key::Key(std::string pressed_key) {
  key = pressed_key;
  time = 0;
}
