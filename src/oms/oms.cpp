#include "oms/oms.hpp"

namespace trader {

OmsAck Oms::send(const Order &ord) {
  // Placeholder; real implementation would serialize to exchange
  (void)ord;
  return {true, "ACK"};
}

}
