# Hardware Inventory and CSI Feasibility

## Reported available (not yet physically inspected)

- ESP32 boards (exact chip/module/revision and antenna unknown).
- Linux desktop and laptop computers.
- Ubuntu home server.
- Two Android phones.
- Home Wi-Fi network.

This is user-reported inventory, not independently verified capability. No service on the home server is in scope for modification.

## Still to verify

For each ESP32: exact SoC and board/module marking, board revision, antenna type/connector, USB-to-UART or native USB interface, flash size, and which boards can be dedicated as sender/receiver. Verify available serial adapters/cables and a stable mount/tape measure for repeatable geometry. Record access to a known-good ESP-IDF installation and its version. Do not infer these from the generic “ESP32” label.

## CSI/API findings (documentation review; board behavior untested)

Espressif's maintained ESP-IDF Wi-Fi vendor-features guide documents CSI as per-subcarrier channel frequency response and exposes `wifi_csi_info_t` data/receive-control metadata. The documented enable sequence is build option `CONFIG_ESP_WIFI_CSI_ENABLED`, `esp_wifi_set_csi_rx_cb()`, `esp_wifi_set_csi_config()`, then `esp_wifi_set_csi()`. The receive callback executes in the Wi-Fi task: avoid lengthy work there and hand off data to a queue/task. The guide notes a disconnected station receives no packets and suggests promiscuous/sniffer mode when broader CSI reception is needed. CSI contents/length vary by packet and PHY configuration; ESP32 documentation calls out a hardware limitation where the first four bytes can be invalid (`first_word_invalid`). Check the target- and IDF-version-specific API before interpreting bytes.

Espressif's official `esp-csi` project README states CSI support across ESP32, ESP32-S2, ESP32-C3, ESP32-S3, ESP32-C5, ESP32-C6, and ESP32-C61 families, and includes sender/receiver get-started examples. This is family-level guidance, not proof every board/module/release and desired packet path behaves identically. In particular, C6 documentation exposes the CSI configuration API but its page omits some format details present in ESP32 documentation. Verify the chosen target's versioned reference and run a minimal capture before choosing hardware.

### Feasible acquisition paths

1. **Two-device path (preferred investigation):** one ESP32 sends known/repeatable Wi-Fi traffic; the second receives and reports CSI. Espressif examples include sender/receiver demonstrations. Confirm packet reception, peer identity, CSI callback delivery, and lawful/non-disruptive channel use on the actual target. Do not assume a router is required.
2. **Router/AP as transmitter:** an ESP32 can receive CSI from traffic exchanged with an AP/router, subject to the actual packet direction, driver behavior, and network traffic. More environmental/network confounders; not the preferred controlled pair.
3. **Local output interface:** board USB serial/UART or another verified local link can carry records to Linux. Determine capability and sustainable throughput empirically. Keep acquisition callback bounded and account for loss/backpressure.

No experimental firmware has been written. No claim is made yet about supported boards in the available inventory, stable phase, timestamp accuracy, packet rate, loss rate, or suitability of the home AP.

## Optional equipment

Not currently a blocker: tripod/fixture and measured tape for repeatable geometry; spare serial/USB cables; an RF-shielded or controlled test space; a Wi-Fi monitor-capable adapter or spectrum analyzer for independent diagnostics; temperature logging. Acquire only when a specific uncertainty requires it.

## Actual blockers

Before first capture, identify at least one CSI-capable board and its target/version support, establish the toolchain and programming/console interface, and choose a repeatable packet source. Before claiming controlled pair measurements, identify two usable devices or document an alternative source. These are verification tasks, not established shortages.

## Official references

- [ESP-IDF Wi-Fi Vendor Features (ESP32, latest)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi-driver/wifi-vendor-features.html) — CSI representation and callback/configuration guidance.
- [ESP-IDF Wi-Fi Vendor Features (ESP32-C3)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/wifi-driver/wifi-vendor-features.html)
- [ESP-IDF Wi-Fi Vendor Features (ESP32-C6)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-guides/wifi-driver/wifi-vendor-features.html)
- [Espressif `esp-csi` examples and README](https://github.com/espressif/esp-csi) — family-level support statement, receiver/sender examples and documented acquisition approaches.

Documentation URLs using `latest` can change. Pin the ESP-IDF version and use its matching target reference once hardware is selected.
