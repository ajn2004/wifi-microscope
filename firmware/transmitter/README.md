# Transmitter

ESP-IDF project that sends a broadcast 802.11 probe-request frame on Wi-Fi channel 6 every 100 ms. It logs its station MAC at startup; configure that address in the receiver's `TX_MAC` before using the pair.

For ESP-IDF activation, build, flash, and monitor commands, follow the [firmware quick start](../README.md). The transmitter has no sequence-numbered payload, so it currently supports basic packet/CSI path checks rather than packet-loss accounting.
