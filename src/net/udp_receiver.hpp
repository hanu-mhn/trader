#pragma once
#include <string>
#include <vector>

namespace trader {

class UdpReceiver {
public:
  UdpReceiver();
  ~UdpReceiver();
  // If stream_ip is multicast, joins group on iface (optional local addr). Returns true on success.
  // If ssm_source is non-empty, uses source-specific multicast join.
  bool bindAndJoin(const std::string &stream_ip, int port, const std::string &iface="", const std::string &ssm_source="");
  ssize_t recvSome(void *buf, size_t len, int timeout_ms);
  void close();

private:
  int fd_;
  bool is_multicast_;
};

}
