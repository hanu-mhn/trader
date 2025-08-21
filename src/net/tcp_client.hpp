#pragma once
#include <string>
#include <vector>

namespace trader {

class TcpClient {
public:
  TcpClient();
  ~TcpClient();
  bool connectTo(const std::string &host, int port);
  ssize_t sendAll(const void *buf, size_t len);
  ssize_t recvSome(void *buf, size_t len, int timeout_ms);
  void close();
  int fd() const { return fd_; }

private:
  int fd_;
};

}
