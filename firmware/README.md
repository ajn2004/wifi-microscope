# Firmware quick start

Two ESP-IDF projects are present: [`transmitter/`](transmitter/README.md) sends Wi-Fi probe-request frames on channel 6, and [`reciever/`](reciever/) receives frames from the configured transmitter MAC and prints CSI records over USB serial. The directory name `reciever` is misspelled; use it as written. These are experimental firmware, not a calibrated instrument. See [hardware inventory](../docs/hardware-inventory.md) for what is known and still unverified.

## Requirements

- Two ESP32 boards attached over USB (reported as `/dev/ttyUSB0` and `/dev/ttyUSB1`; confirm which port is which board before flashing).
- ESP-IDF v5.5.6 with its tools installed. In a new shell, activate it:

  ```bash
  source "$HOME/esp/esp-idf-v5.5.6/export.sh"
  idf.py --version
  ```

  The expected version is `ESP-IDF v5.5.6`. See [toolchain setup](../docs/toolchain.md) for the initial ESP-IDF smoke test.

## Build and flash

The reported boards have 4 MB flash, but both checked-in project configurations select 2 MB. In each project, run `idf.py menuconfig` and set **Serial flasher config → Flash size** to the verified device size before building. The reported hardware size is not independently verified; see the inventory. Flashing writes the selected firmware to the selected device, so verify the port before running the command.

```bash
cd firmware/transmitter
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

Exit the serial monitor with `Ctrl+]`. Note the transmitter MAC printed in its log. In a second terminal (or after closing the first monitor), set that address in `reciever/main/receiver.c` as `TX_MAC`; the receiver filters both frames and CSI by this address. Then build and flash the receiver:

```bash
cd firmware/reciever
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py -p /dev/ttyUSB1 flash monitor
```

Swap `/dev/ttyUSB0` and `/dev/ttyUSB1` as needed after identifying the physical board. Both projects use channel 6; the receiver should print lines beginning `CSI,` when matching packets produce CSI. A record contains receiver timestamp, RSSI, channel, signal mode, rate, first-word-invalid flag, byte length, and hex CSI bytes. ESP-IDF logs may appear on the same serial stream.

## Current limits

The receiver MAC must match the transmitter's runtime MAC. The current transmitter sends a probe request every 100 ms and does not include a sequence counter. Receiver CSI records are printed as text/hex; no host capture program or GUI workflow is provided. Queue overflow/loss accounting and CSI calibration are not implemented, and successful output does not establish measurement repeatability. See [transmitter notes](transmitter/README.md), [host status](../host/README.md), and [experimental method](../docs/experimental-method.md).
