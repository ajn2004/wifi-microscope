# Hardware Inventory and CSI Feasibility

## Verified SoC and interface facts

The researcher reports identifying two separate Espressif ESP32-D0WD-V3 SoCs, each revision v3.1, 4 MB flash, and a 40 MHz crystal. Each board connects through a Silicon Labs CP2102 USB-to-UART bridge and is accessible as `/dev/ttyUSB0` or `/dev/ttyUSB1`. These are researcher-reported observations; this session did not query either chip or inspect test logs.

| Board | SoC | Revision | Flash | Crystal | Host interface |
|---|---|---|---|---|---|
| A | ESP32-D0WD-V3 | v3.1 | 4 MB (reported) | 40 MHz (reported) | CP2102 USB-UART; port assignment must be checked before use |
| B | ESP32-D0WD-V3 | v3.1 | 4 MB (reported) | 40 MHz (reported) | CP2102 USB-UART; port assignment must be checked before use |

**Identity boundary:** SoC identity and target family are reported established. Exact development-board and module markings, flash manufacturer, antenna type/location, and which stable USB path maps to which physical board have not been verified. Do not infer module antenna or RF characteristics from the SoC. No MAC addresses, USB serial numbers, or workstation-specific paths are recorded here.

## CSI feasibility

ESP-IDF v5.5.6's [ESP32 Wi-Fi feature list](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-guides/wifi.html#esp32-wi-fi-feature-list) explicitly includes CSI. The version-pinned [`esp-csi` sender](https://github.com/espressif/esp-csi/tree/master/examples/get-started/csi_send) and [receiver](https://github.com/espressif/esp-csi/tree/master/examples/get-started/csi_recv) examples provide an ESP-NOW controlled-packet path. Their current `master` sources are examples, not pinned to an ESP-IDF release; check out a compatible project revision and build for `esp32` before adopting settings.

The v5.5.6 [`esp_wifi.h` API reference](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/network/esp_wifi.html) exposes `esp_wifi_set_csi_rx_cb()`, `esp_wifi_set_csi_config()`, and `esp_wifi_set_csi()`. CSI output and metadata depend on PHY and configuration; raw CSI is not calibrated phase. Do not assume subcarrier layout or phase comparability across targets or runs.

## Proposed first arrangement (pending confirmation)

Use one board to send sequenced ESP-NOW packets and the other in station mode to receive CSI; roles can be swapped after verifying which serial port controls each board. This path is demonstrated by Espressif's `csi_send`/`csi_recv` pair and does not inherently require an access point. The current sender broadcasts ESP-NOW data on a configured channel. Confirm actual packet reception and CSI callback delivery on our boards before treating this as established.

## Remaining uncertainties

- Board/module part numbers, PCB-vs-external antenna, antenna orientation, and RF path.
- Port-to-board mapping and assignment of experimental roles.
- Exact ESP-NOW payload reception behavior, channel/rate/bandwidth configuration, and CSI length/format on the boards.
- UART throughput/loss with CSI output; callback buffering and record transport remain to be designed and measured.
- CSI phase calibration and repeatability are not implied by successful CSI reception.

## References

- [ESP-IDF v5.5.6 ESP32 Wi-Fi guide](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-guides/wifi.html)
- [ESP-IDF v5.5.6 Wi-Fi API reference](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/network/esp_wifi.html)
- [ESP-IDF v5.5.6 Wi-Fi API header](https://github.com/espressif/esp-idf/blob/v5.5.6/components/esp_wifi/include/esp_wifi.h)
- [Espressif esp-csi repository](https://github.com/espressif/esp-csi)
