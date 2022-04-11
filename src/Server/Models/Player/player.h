#pragma once

#include <cstdint>

using PlayerId = uint64_t;

class Player {
 public:
  explicit Player(PlayerId id);

  void SetId(PlayerId id);
  PlayerId GetId() const;

 private:
  PlayerId id_;
};
