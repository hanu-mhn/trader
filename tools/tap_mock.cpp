// Simple TAP mock server: accepts TCP, prints received, sends OK
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main(int argc, char** argv){
  int port = 9000;
  if (argc>1) port = atoi(argv[1]);
  int s = socket(AF_INET, SOCK_STREAM, 0);
  int reuse=1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port); addr.sin_addr.s_addr=INADDR_ANY;
  bind(s, (sockaddr*)&addr, sizeof(addr));
  listen(s, 8);
  std::cerr << "TAP mock listening on " << port << "\n";
  while (true){
    int c = accept(s, nullptr, nullptr);
    if (c<0) continue;
    char buf[1024]; int n = recv(c, buf, sizeof(buf), 0);
    if (n>0) {
      std::string msg(buf, buf+n);
      std::cerr << "recv: " << msg << "\n";
      std::string ok = "OK\n";
      send(c, ok.data(), ok.size(), 0);
    }
    close(c);
  }
}
