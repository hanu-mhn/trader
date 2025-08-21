# Trader Infra (Data Feeder, OMS, RMS) - C++

A minimal, modular scaffold for:
- Market Data Feeder over TCP/UDP (TBT or Snapshot) with TAP server and stream IP/port.
- OMS + RMS for basic risk checks, suitable for local strategy integration and future exchange integration.

This is a starter project intended for colocation-style deployments; real exchange protocols are not included. You can wire actual NSE/BSE adapters later.

## Features
- Configurable via INI: TAP IP/port, Stream IP/port, NNF ID, Broker ID, Client ID, Username, Password, feed mode (TBT/Snapshot), RMS limits.
- TCP client and UDP receiver (supports multicast join).
- Data Feeder logs raw packets to file and stdout.
- OMS applies simple RMS checks (max qty/value, price bands, symbol whitelist) and prints decision.

## Build

Requirements: GCC/Clang with C++17, CMake >= 3.15, Linux.

```bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
```

## Run

1) Copy and edit the example config:
```bash
cp config/example.ini config/local.ini
```

2) Data Feeder (UDP or TCP per config):
```bash
./build/data_feeder --config config/local.ini
```

3) OMS + RMS check an order:
```bash
./build/oms_rms \
  --config config/local.ini \
  --symbol NIFTY24AUGFUT \
  --side BUY \
  --price 22200.50 \
  --qty 50
```

If accepted, you'll see an ACK; otherwise a REJECT with a reason.

## Config
See `config/example.ini` for all fields. Key sections:
- [tap]: TAP server host/port (TCP)
- [market_data]: feed_mode (TBT/Snapshot), stream_ip, stream_port (UDP), iface (optional NIC for multicast)
- [ids]: nnf_id, broker_id, client_id
- [credentials]: username, password
- [rms]: max_qty, max_notional, price_band_pct, symbols (comma-separated)

## Notes
- This repo does not implement real NSE/BSE wire protocols. Use this as a base and add exchange-specific adapters.
- Network permissions/firewall may be needed for multicast. Run with appropriate NIC and routes.

## License
MIT
# trader
