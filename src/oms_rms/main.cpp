#include "common/config.hpp"
#include "common/logger.hpp"
#include "rms/rms.hpp"
#include "oms/oms.hpp"
#include <iostream>
#include <cstring>

using namespace std;
using namespace trader;

static void usage() {
  cerr << "Usage: oms_rms --config <file> --symbol <s> --side BUY|SELL --price <p> --qty <q>\n";
}

int main(int argc, char **argv) {
  string cfgPath, symbol, sideStr; double price=0; int qty=0;
  for (int i=1; i<argc; ++i) {
    string a = argv[i];
    if (a=="--config" && i+1<argc) cfgPath = argv[++i];
    else if (a=="--symbol" && i+1<argc) symbol = argv[++i];
    else if (a=="--side" && i+1<argc) sideStr = argv[++i];
    else if (a=="--price" && i+1<argc) price = atof(argv[++i]);
    else if (a=="--qty" && i+1<argc) qty = atoi(argv[++i]);
  }
  if (cfgPath.empty()||symbol.empty()||sideStr.empty()||price<=0||qty<=0) { usage(); return 1; }

  Side side = (sideStr=="BUY"?Side::BUY:Side::SELL);

  Config cfg; Logger log;
  if (!cfg.loadFromIni(cfgPath)) { cerr << "Failed to load config\n"; return 1; }

  Rms rms(cfg);
  Oms oms;

  Order ord{symbol, side, price, qty};
  auto [ok, reason] = rms.validate(ord);
  if (!ok) { cout << "REJECT: " << reason << "\n"; return 0; }
  auto ack = oms.send(ord);
  cout << (ack.accepted?"ACK":"REJECT") << ": " << ack.reason << "\n";
  return 0;
}
