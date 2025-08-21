#include "net/udp_receiver.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>

namespace trader {

UdpReceiver::UdpReceiver(): fd_(-1), is_multicast_(false) {}
UdpReceiver::~UdpReceiver() { close(); }

bool UdpReceiver::bindAndJoin(const std::string &stream_ip, int port, const std::string &iface, const std::string &ssm_source) {
  close();
  fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd_ < 0) return false;

  int reuse = 1;
  setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  int rcvbuf = 4*1024*1024; // 4MB
  setsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));

  sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (::bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) return false;

  in_addr maddr{}; maddr.s_addr = inet_addr(stream_ip.c_str());
  is_multicast_ = IN_MULTICAST(ntohl(maddr.s_addr));
  if (is_multicast_) {
    if (!ssm_source.empty()) {
      // Source-specific multicast using IP_ADD_SOURCE_MEMBERSHIP
      struct ip_mreq_source mreqs{};
      mreqs.imr_multiaddr = maddr;
      mreqs.imr_interface.s_addr = iface.empty() ? INADDR_ANY : inet_addr(iface.c_str());
      mreqs.imr_sourceaddr.s_addr = inet_addr(ssm_source.c_str());
      if (setsockopt(fd_, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, &mreqs, sizeof(mreqs)) < 0) return false;
    } else {
      ip_mreqn mreq{};
      mreq.imr_multiaddr = maddr;
      mreq.imr_address.s_addr = iface.empty() ? INADDR_ANY : inet_addr(iface.c_str());
      mreq.imr_ifindex = 0;
      if (setsockopt(fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) return false;
    }
  }
  return true;
}

ssize_t UdpReceiver::recvSome(void *buf, size_t len, int timeout_ms) {
  struct pollfd pfd{fd_, POLLIN, 0};
  int r = poll(&pfd, 1, timeout_ms);
  if (r <= 0) return r; // 0 timeout, -1 error
  return ::recv(fd_, buf, len, 0);
}

void UdpReceiver::close() {
  if (fd_ >= 0) {
    if (is_multicast_) { /* best-effort leave to kernel on close */ }
    ::close(fd_); fd_ = -1; is_multicast_ = false;
  }
}

}
