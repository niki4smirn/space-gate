#pragma once

#include <unordered_map>

#include "src/Server/Models/Player/player.h"

class GameModel {
 public:
  GameModel() = default;

 private:
  std::unordered_map<PlayerId, Player> players_;
};
