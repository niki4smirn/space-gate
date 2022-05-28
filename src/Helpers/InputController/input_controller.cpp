#include "input_controller.h"

InputController::InputController() {
  timer_.start(constants::kKeyTickTime, this);
}

void InputController::KeyPressed(quint32 key_number) {
  if (!key_names::kNativeCodeToKeyEnum.contains(key_number)) {
    return;
  }
  auto key = key_names::kNativeCodeToKeyEnum.at(key_number);
  if (FindKey(key)) {
    return;
  }
  keys_pressed_.emplace_back(key);
  emit KeyEventToServer(key);
}

void InputController::MousePosStartTracking() {
  is_mouse_tracking_ = true;
}

void InputController::MousePosStopTracking() {
  is_mouse_tracking_ = false;
}

void InputController::MouseMove(const QPoint& pos) {
  if (!is_mouse_tracking_) {
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

bool InputController::FindKey(key_names::keys key) {
  return std::any_of(keys_pressed_.begin(),
                     keys_pressed_.end(),
                     [key](auto key_it) { return key_it.key == key; });
}

void InputController::MouseKeyPressed(const Qt::MouseButton& button) {
  switch (button) {
    case Qt::LeftButton: {
      emit MouseKeyToServer(key_names::kLeftMouseButton);
      break;
    }
    case Qt::RightButton: {
      emit MouseKeyToServer(key_names::kRightMouseButton);
      break;
    }
    default: {}
  }
}

Key::Key(key_names::keys key_pressed) :
  key(key_pressed) {}
