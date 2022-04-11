#include "player.h"

Player::Player(PlayerId id) : id_(id) {}

PlayerId Player::GetId() const {
  return id_;
}

void Player::SetId(PlayerId id) {
  id_ = id;
}
