#pragma once
#include <string>
#include <cstdint>

namespace trader {

enum class Side { BUY, SELL };

struct Order {
  std::string symbol;
  Side side;
  double price;
  int32_t qty;
};

}
