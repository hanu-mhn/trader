#pragma once
#include "common/config.hpp"
#include "oms/order_types.hpp"
#include <string>
#include <unordered_set>

namespace trader {

class Rms {
public:
  explicit Rms(const Config &cfg);
  std::pair<bool, std::string> validate(const Order &ord) const;

private:
  int max_qty_;
  double max_notional_;
  double price_band_pct_;
  std::unordered_set<std::string> symbols_;
};

}
