# DAL-237: Minimal CSI Capture Plan

## Question and success criterion

Can these two ESP32-D0WD-V3 boards produce a genuine complex CSI record for a known, controlled transmission? Success is a preserved record tied to a documented sender packet sequence, receiver configuration, receive metadata, and software/target versions—not merely a callback/log line.

## Reference mechanisms (ESP-IDF v5.5.6, target `esp32`)

- Espressif's [`csi_send`](https://github.com/espressif/esp-csi/tree/master/examples/get-started/csi_send) initializes Wi-Fi station mode on a selected channel, initializes ESP-NOW, then periodically broadcasts a 32-bit incrementing count. Its example configures channel, bandwidth and peer PHY rate. Keep those settings fixed and record them. The example source is on `master`; verify its compatibility with IDF v5.5.6 rather than assuming current `master` is a matching release.
- [`csi_recv`](https://github.com/espressif/esp-csi/tree/master/examples/get-started/csi_recv) starts station-mode Wi-Fi on a matching channel, initializes ESP-NOW, enables promiscuous reception, configures CSI, registers `esp_wifi_set_csi_rx_cb()`, and enables CSI with `esp_wifi_set_csi(true)`. For ESP32, its CSI configuration uses legacy/HT LTF enables and merge/filter/scale options. CSI must be enabled in menuconfig (`CONFIG_ESP_WIFI_CSI_ENABLED`). The v5.5.6 guide notes station receives no packets while disconnected and suggests promiscuous mode for more CSI; test this exact path with ESP-NOW.
- The [v5.5.6 ESP32 Wi-Fi guide, CSI section](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-guides/wifi.html#esp32-wi-fi-channel-state-information) and [Wi-Fi API reference](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/network/esp_wifi.html) are the authoritative target/release sources. CSI APIs require Wi-Fi initialized; follow the guide's exact required mode and ordering and check every `esp_err_t` result.

`esp_wifi_set_csi_rx_cb(cb, ctx)` registers the callback and context; driver invokes it for CSI-bearing received packets. The callback executes in the Wi-Fi task. In the installed v5.5.6 ESP32 header, `wifi_csi_info_t` includes `rx_ctrl`, source/destination MACs, `first_word_invalid`, `buf`/`len`, packet header/payload and `payload_len`, and `rx_seq`. IDF documents that the CSI data memory is deallocated after callback return. Copy any CSI bytes and needed metadata into application-owned storage before returning; never retain or free driver pointers.

CSI is not a string. For ESP32 v5.5.6, each subcarrier response is two signed bytes: imaginary first, then real. The buffer can concatenate LLTF, HT-LTF, and STBC HT-LTF; presence, subcarrier indices and total length depend on packet PHY, channel width and CSI configuration. `first_word_invalid` means its first four bytes are invalid due to a hardware limitation. Preserve raw bytes, `len`, receive-control PHY/channel metadata, and this flag; decode using the guide's layout only after inspecting a real record. These driver values are not calibrated amplitude/phase.

## Smallest proposed architecture

Two independent IDF applications, built for `esp32`:

- `firmware/csi_sender/`: minimal sender derived conceptually from `csi_send`; ESP-NOW payload contains an incrementing sequence counter and is transmitted at a fixed, recorded interval on the fixed channel.
- `firmware/csi_receiver/`: station-mode Wi-Fi + ESP-NOW initialization; CSI config, callback registration, CSI enablement; short callback copies the CSI bytes and metadata into a bounded queue; a normal FreeRTOS task formats/writes records to UART. Keep ISR/task ownership clear; the CSI callback is not an ISR, but it shares the Wi-Fi task's latency budget. Do no serial printing, blocking, allocation, complex parsing, or long calculations there. Queue overflow must be counted and surfaced rather than silently dropping observations.
- Host: capture receiver UART with a simple serial terminal/log command initially, preserving the unmodified records and a separate run note. Compare receiver records against sender sequence progression and report missing/duplicate/out-of-order sequence counts. UART is the simplest initial host link because each board already has a CP2102 bridge; its mapping, baud, throughput and loss behavior still need measurement.

The receiver should first filter to the known sender and packet type/sequence where possible. CSI callback triggers for qualifying received Wi-Fi packets, not necessarily only the application's ESP-NOW payload; validate payload parsing and whether the example's sequence field offset applies to the actual ESP-NOW frame. Avoid publishing real MAC addresses: use a private local mapping or pseudonymous labels in shared records.

## Minimal provenance per record/run

Retain CSI raw bytes and byte count; receiver-local timestamp and its units/source; sender identity as a private/pseudonymous label; sender sequence from payload; RSSI, channel, packet length, rate/PHY format, bandwidth and other available `rx_ctrl` fields; `first_word_invalid`; receiver and sender firmware revisions/build configuration; ESP-IDF version and target; channel, bandwidth, protocol/rate, packet payload and interval; host arrival time; queue drops and serial framing/errors; board/module/antenna description when known; geometry/orientation and run start/end. Log unavailable metadata as unavailable, not inferred. Sender and receiver clocks are not synchronized: keep their timestamp domains distinct.

## Incremental human implementation sequence

1. Researcher copies/adapts the official sender example into a small sender project; build and verify monotonically increasing sequence payloads on serial. No receiver yet.
2. Researcher writes receiver initialization and enables CSI with a callback that only checks null pointers, increments a callback count, and reports a count outside the callback. Confirm callback on received packets before adding parsing.
3. During the first paired run, researcher copies CSI bytes and minimum metadata into a bounded queue, then outputs them from a separate task. Capture a short host log and verify nonempty CSI, stable byte lengths/pattern, matching packet sequence and measured drop counts.
4. Inspect one raw record against the documented ESP32 I/Q representation; only then define any decoding and preserve the original bytes.

Expected: both firmware instances start on the same channel; sender reports its fixed packet interval/sequence progress; receiver reports callbacks only when packets arrive and records source/receive metadata plus nonempty CSI. No callback or empty records: verify matching channel, sender/receiver startup order, ESP-NOW reception, CSI Kconfig and API return values. Records without sender sequence: inspect received packet payload/type rather than assuming a fixed byte offset. UART loss: lower packet rate and compare sequence gaps before changing transport.

## Hardware questions to verify

Confirm port-to-board mapping; exact dev-board/module and antenna; same legal 2.4 GHz channel on both devices; matching channel width/PHY settings accepted by this ESP32 target; CSI Kconfig and API support in the actual project; which ESP-NOW packets invoke CSI and where the sequence resides; CSI byte count/layout and first-word handling; host UART baud/throughput and loss. Start with a conservative packet rate and default/HT20-like settings consistent with the exact example and local regulations; do not transplant the example's channel/rate blindly.

## References

- [esp-csi repository and examples](https://github.com/espressif/esp-csi)
- [ESP-IDF v5.5.6 ESP32 CSI guide](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-guides/wifi.html#esp32-wi-fi-channel-state-information)
- [ESP-IDF v5.5.6 Wi-Fi API](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/network/esp_wifi.html)
- [ESP-IDF v5.5.6 ESP32 CSI type definition](https://github.com/espressif/esp-idf/blob/v5.5.6/components/esp_wifi/include/local/esp_wifi_types_native.h)
- [ESP-IDF v5.5.6 ESP-NOW API](https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/network/esp_now.html)
