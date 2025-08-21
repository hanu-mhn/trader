// UDP burst sender to a given IP:port, optionally multicast
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char** argv){
  if (argc < 4) { std::cerr << "usage: udp_send <ip> <port> <count>\n"; return 1; }
  std::string ip = argv[1]; int port = atoi(argv[2]); int count = atoi(argv[3]);
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port); inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
  std::string payload = "HelloMD";
  for (int i=0;i<count;++i){ sendto(s, payload.data(), payload.size(), 0, (sockaddr*)&addr, sizeof(addr)); usleep(10000); }
  close(s);
  return 0;
}
