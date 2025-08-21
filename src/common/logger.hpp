#pragma once
#include <string>
#include <mutex>
#include <fstream>

namespace trader {

class Logger {
public:
  explicit Logger(const std::string &file="");
  void info(const std::string &msg);
  void error(const std::string &msg);

private:
  void log(const char *level, const std::string &msg);
  std::mutex mu_;
  std::ofstream file_;
};

}
