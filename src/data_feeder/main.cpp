#include "common/config.hpp"
#include "common/logger.hpp"
#include "net/tcp_client.hpp"
#include "net/udp_receiver.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>

using namespace std;
using namespace trader;

static void usage() {
  cerr << "Usage: data_feeder --config <file>\n";
}

int main(int argc, char **argv) {
  string cfgPath;
  int duration_sec = 0; // 0 means run indefinitely
  for (int i=1; i<argc; ++i) {
    string a = argv[i];
    if (a == "--config" && i+1<argc) { cfgPath = argv[++i]; }
    else if (a == "--duration" && i+1<argc) { duration_sec = atoi(argv[++i]); }
  }
  if (cfgPath.empty()) { usage(); return 1; }

  Config cfg; Logger log;
  if (!cfg.loadFromIni(cfgPath)) { cerr << "Failed to load config\n"; return 1; }

  string feedMode = cfg.get("market_data", "feed_mode", "Snapshot");
  string streamIp = cfg.get("market_data", "stream_ip", "239.0.0.1");
  int streamPort = cfg.getInt("market_data", "stream_port", 31000);
  string iface = cfg.get("market_data", "iface", "");
  string ssm = cfg.get("market_data", "ssm_source", "");

  string tapHost = cfg.get("tap", "host", "127.0.0.1");
  int tapPort = cfg.getInt("tap", "port", 9000);

  string nnf = cfg.get("ids", "nnf_id", "");
  string broker = cfg.get("ids", "broker_id", "");
  string client = cfg.get("ids", "client_id", "");
  string user = cfg.get("credentials", "username", "");
  string pass = cfg.get("credentials", "password", "");

  log.info("Starting Data Feeder: mode=" + feedMode + ", stream=" + streamIp + ":" + to_string(streamPort) + (ssm.empty()?"":" ssm="+ssm));

  // Connect TAP for control/login (illustrative only)
  TcpClient tap;
  if (tap.connectTo(tapHost, tapPort)) {
    string hello = "LOGIN|" + nnf + "|" + broker + "|" + client + "|" + user + "|" + pass + "\n";
    tap.sendAll(hello.data(), hello.size());
    char rb[256];
    auto n = tap.recvSome(rb, sizeof(rb), 1000);
    if (n > 0) log.info(string("TAP reply: ") + string(rb, rb+n));
  } else {
    log.error("Could not connect to TAP server");
  }

  // Receive market data over UDP; if non-multicast IP is supplied it still works.
  UdpReceiver udp;
  if (!udp.bindAndJoin(streamIp, streamPort, iface, ssm)) {
    log.error("Failed to bind/join UDP stream");
    return 2;
  }
  log.info("Listening for market data packets...");

  vector<char> buf(64*1024);
  auto start = chrono::steady_clock::now();
  auto deadline = (duration_sec > 0) ? (start + chrono::seconds(duration_sec)) : chrono::steady_clock::time_point::max();
  while (true) {
    auto n = udp.recvSome(buf.data(), buf.size(), 1000);
    if (n > 0) {
      // For demo, just print packet size; you can parse TBT/Snapshot later.
      log.info("pkt " + to_string(n) + " bytes");
    }
    if (chrono::steady_clock::now() >= deadline) break;
  }
  log.info("Exiting data feeder");
}
