#include "net/tcp_client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <poll.h>

namespace trader {

TcpClient::TcpClient(): fd_(-1) {}
TcpClient::~TcpClient() { close(); }

bool TcpClient::connectTo(const std::string &host, int port) {
  close();
  struct addrinfo hints{}; hints.ai_socktype = SOCK_STREAM; hints.ai_family = AF_UNSPEC;
  struct addrinfo *res = nullptr;
  std::string portStr = std::to_string(port);
  if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res) != 0) return false;
  int s = -1;
  for (auto p = res; p; p = p->ai_next) {
    s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (s < 0) continue;
    if (::connect(s, p->ai_addr, p->ai_addrlen) == 0) { fd_ = s; break; }
    ::close(s); s = -1;
  }
  freeaddrinfo(res);
  return fd_ >= 0;
}

ssize_t TcpClient::sendAll(const void *buf, size_t len) {
  const char *p = static_cast<const char *>(buf);
  size_t sent = 0;
  while (sent < len) {
    ssize_t n = ::send(fd_, p + sent, len - sent, 0);
    if (n <= 0) return n;
    sent += (size_t)n;
  }
  return (ssize_t)sent;
}

ssize_t TcpClient::recvSome(void *buf, size_t len, int timeout_ms) {
  struct pollfd pfd{fd_, POLLIN, 0};
  int r = poll(&pfd, 1, timeout_ms);
  if (r <= 0) return r; // 0 timeout, -1 error
  return ::recv(fd_, buf, len, 0);
}

void TcpClient::close() {
  if (fd_ >= 0) { ::close(fd_); fd_ = -1; }
}

}
