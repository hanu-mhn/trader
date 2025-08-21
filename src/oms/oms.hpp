#pragma once
#include "oms/order_types.hpp"
#include <string>

namespace trader {

struct OmsAck {
  bool accepted;
  std::string reason;
};

class Oms {
public:
  Oms() = default;
  OmsAck send(const Order &ord);
};

}
