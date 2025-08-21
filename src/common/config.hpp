#pragma once
#include <string>
#include <unordered_map>
#include <optional>

namespace trader {

class Config {
public:
  bool loadFromIni(const std::string &path);

  std::string get(const std::string &section, const std::string &key, const std::string &def="") const;
  int getInt(const std::string &section, const std::string &key, int def=0) const;
  double getDouble(const std::string &section, const std::string &key, double def=0.0) const;

private:
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data_;
};

}
