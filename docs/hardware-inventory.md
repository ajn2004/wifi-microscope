# Hardware Inventory and CSI Feasibility

## Connected-device observations — 2026-10-24

The current Linux execution environment sees two USB serial adapters:

| Host path | USB adapter | USB VID:PID | Stable path | Access |
|---|---|---|---|---|
| `/dev/ttyUSB0` | Silicon Labs CP2102 USB-to-UART bridge (`cp210x`) | `10c4:ea60` | `/dev/serial/by-path/pci-0000:00:14.0-usb-0:11.1:1.0-port0` | Not accessible to current user |
| `/dev/ttyUSB1` | Silicon Labs CP2102 USB-to-UART bridge (`cp210x`) | `10c4:ea60` | `/dev/serial/by-path/pci-0000:00:14.0-usb-0:10.2:1.0-port0` | Not accessible to current user |

`lsusb` showed two CP210x devices. Both tty nodes are owned by `root:uucp` with mode `0660`; the current user is not in `uucp`. No by-id distinction is available: both adapters report the same generic CP2102 identity and adapter serial string. The distinct by-path entries establish two separate USB interfaces, **not** that they are independently connected ESP32 boards. A CP2102 is a USB-to-UART bridge, not an ESP32 identification.

**Not observed:** ESP32 target/family/revision, module and board markings, flash vendor/capacity, antenna, whether both adapters terminate at ESP32 boards, or whether any USB interface is native USB. No esptool query was run because serial access is denied; no hardware flash operation was attempted. USB and serial identities above are inventory data, not proof of board identity.

To resolve the blocker without changing host configuration: on the workstation with the boards, run `id`, `ls -l /dev/ttyUSB* /dev/ttyACM*`, `ls -l /dev/serial/by-id/ /dev/serial/by-path/`, and `udevadm info --query=property --name=/dev/ttyUSB0` (repeat for the other path). If permitted by the local operator, use the existing serial-port access method and run `uvx --from esptool esptool --port <stable-path> chip-id` followed by `flash-id` for each board. These are identification/read-only queries; do not use write/erase commands. If the boards cannot be identified over serial, provide clear photographs of the module/board markings and antenna area. Do not add udev rules or change group membership without authorization.

## CSI compatibility — documentation evidence, target pending

Espressif's maintained [`esp-csi`](https://github.com/espressif/esp-csi) README states family-level CSI support for ESP32, ESP32-S2, ESP32-C3, ESP32-S3, ESP32-C5, ESP32-C6, and ESP32-C61. It provides `examples/get-started/csi_recv` and `csi_send`, plus `csi_recv_router`. This does not identify these boards or establish identical APIs/data formats across targets.

The official [ESP-IDF Wi-Fi vendor-features guide for ESP32](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/wifi-driver/wifi-vendor-features.html) and [Wi-Fi API reference](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html) document the CSI APIs and `wifi_csi_info_t`. The documented pattern is CSI enabled in configuration (`CONFIG_ESP_WIFI_CSI_ENABLED`), register `esp_wifi_set_csi_rx_cb()`, set `esp_wifi_set_csi_config()`, then enable using `esp_wifi_set_csi()`. The callback is in the Wi-Fi task and should be bounded. Captured CSI is packet/PHY dependent and includes receive-control metadata; it is not inherently calibrated phase. Consult the matching target/release documentation before building.

ESP-IDF v5.5.1's [ESP32-C3 Wi-Fi guide](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c3/api-guides/wifi.html) specifically notes that `wifi_csi_info_t.first_word_invalid` indicates invalid first four CSI bytes due to a hardware limitation. This demonstrates why the exact target matters; it is not evidence that either connected board is a C3. Exact ESP-IDF target/release API availability, operating-mode and packet-path behavior, and usable CSI metadata remain unverified for these boards.

## Initial experiment recommendation (conditional)

If identification confirms two ESP32 devices supported by the same pinned ESP-IDF release, use one as a controlled packet sender and the other as CSI receiver, following the official get-started `csi_send` / `csi_recv` examples. Start with the direct two-device arrangement if their documented packet path permits reception; the maintained project also describes router-assisted methods, so do not assume an AP is either mandatory or unnecessary until the target-specific example is checked. Use the available UART/USB-serial host path only after confirming board mapping and console behavior; retain raw CSI and driver metadata before any transformation.

No transmitter/receiver assignment is made yet: there is no evidence distinguishing the boards or their antenna/module configurations. No ESP-IDF release is selected because target selection is a prerequisite to validating its versioned API and examples. Do not install the toolchain until this is resolved.

## Remaining blockers and uncertainties

- Determine whether the two CP2102 paths correspond to the two reported physical ESP32 boards.
- Obtain serial access through an already authorized mechanism; no host permission changes were made.
- Read each chip family/variant, revision, and flash identification with esptool; match each to physical module/board markings and antenna configuration.
- Verify each identified target against version-specific CSI APIs, packet reception mode, and official sender/receiver example support; then select a common ESP-IDF release.
- Confirm repeatable packet source, receiver assignment, host acquisition path and throughput in the first capture. CSI phase calibration, timing, packet loss and measurement repeatability remain experimental questions.

## References

- [Espressif `esp-csi`](https://github.com/espressif/esp-csi) — maintained family-level support statement and sender/receiver/router examples.
- [ESP-IDF Wi-Fi Vendor Features (ESP32, stable)](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/wifi-driver/wifi-vendor-features.html) — CSI configuration and interpretation.
- [ESP-IDF Wi-Fi API Reference (ESP32, stable)](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html) — API declarations and CSI metadata types.
- [ESP-IDF Wi-Fi Guide (ESP32-C3 v5.5.1)](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c3/api-guides/wifi.html) — target-specific `first_word_invalid` limitation.

Version selection is deferred until both actual targets are identified; `stable` documentation can move and is not itself a release pin.
