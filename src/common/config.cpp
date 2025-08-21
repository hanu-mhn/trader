#include "common/config.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

namespace trader {

static inline string trim(const string &s) {
  size_t start = s.find_first_not_of(" \t\r\n");
  if (start == string::npos) return "";
  size_t end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

bool Config::loadFromIni(const string &path) {
  ifstream in(path);
  if (!in) return false;
  string line, section;
  while (getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == ';' || line[0] == '#') continue;
    if (line.front() == '[' && line.back() == ']') {
      section = line.substr(1, line.size()-2);
      continue;
    }
    auto eq = line.find('=');
    if (eq == string::npos) continue;
    string key = trim(line.substr(0, eq));
    string val = trim(line.substr(eq+1));
    data_[section][key] = val;
  }
  return true;
}

string Config::get(const string &section, const string &key, const string &def) const {
  auto it = data_.find(section);
  if (it == data_.end()) return def;
  auto it2 = it->second.find(key);
  if (it2 == it->second.end()) return def;
  return it2->second;
}

int Config::getInt(const string &section, const string &key, int def) const {
  try {
    return stoi(get(section, key, to_string(def)));
  } catch (...) {
    return def;
  }
}

double Config::getDouble(const string &section, const string &key, double def) const {
  try {
    return stod(get(section, key, to_string(def)));
  } catch (...) {
    return def;
  }
}

}
