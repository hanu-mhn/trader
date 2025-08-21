#include "rms/rms.hpp"
#include <sstream>
#include <vector>

namespace trader {

static std::vector<std::string> splitCsv(const std::string &s) {
  std::vector<std::string> out; std::string cur;
  for (char c: s) { if (c==',') { if(!cur.empty()) out.push_back(cur); cur.clear(); } else cur.push_back(c);} 
  if(!cur.empty()) out.push_back(cur);
  return out;
}

Rms::Rms(const Config &cfg) {
  max_qty_ = cfg.getInt("rms", "max_qty", 0);
  max_notional_ = cfg.getDouble("rms", "max_notional", 0);
  price_band_pct_ = cfg.getDouble("rms", "price_band_pct", 0);
  for (auto &s : splitCsv(cfg.get("rms", "symbols", ""))) symbols_.insert(s);
}

std::pair<bool, std::string> Rms::validate(const Order &ord) const {
  if (symbols_.size() && symbols_.find(ord.symbol) == symbols_.end())
    return {false, "SYMBOL_NOT_ALLOWED"};
  if (ord.qty <= 0) return {false, "BAD_QTY"};
  if (max_qty_ > 0 && ord.qty > max_qty_) return {false, "QTY_LIMIT"};
  double notion = ord.price * ord.qty;
  if (max_notional_ > 0 && notion > max_notional_) return {false, "NOTIONAL_LIMIT"};
  // Price band check around a notional reference of ord.price (demo only)
  double ref = ord.price; // placeholder; replace with LTP or reference price
  double band = ref * (price_band_pct_ / 100.0);
  if (price_band_pct_ > 0 && (ord.price < ref - band || ord.price > ref + band))
    return {false, "PRICE_BAND"};
  return {true, "OK"};
}

}
