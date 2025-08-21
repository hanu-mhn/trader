#include "common/logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

namespace trader {

static string nowStr() {
  using namespace std::chrono;
  auto t = system_clock::to_time_t(system_clock::now());
  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
  return string(buf);
}

Logger::Logger(const string &file) {
  if (!file.empty()) file_.open(file, ios::app);
}

void Logger::log(const char *level, const string &msg) {
  lock_guard<mutex> lk(mu_);
  string line = nowStr() + string(" ") + level + ": " + msg + "\n";
  cerr << line;
  if (file_.is_open()) file_ << line;
}

void Logger::info(const string &msg) { log("INFO", msg); }
void Logger::error(const string &msg) { log("ERROR", msg); }

}
