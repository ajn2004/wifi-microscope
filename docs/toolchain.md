# ESP-IDF Toolchain Bring-up

## Verified setup (researcher-reported)

The researcher reports Arch Linux, ESP-IDF v5.5.6 at `~/esp/esp-idf-v5.5.6`, target `esp32`, and a successful official `hello_world` build, flash, and serial-monitor run on both boards. This session independently sourced that ESP-IDF installation and observed `idf.py --version` report `ESP-IDF v5.5.6`; no build/monitor logs were available, so the smoke-test successes remain researcher-reported. The example is not project firmware and should not be copied into `firmware/`.

## Activate and build the smoke test

In each new shell, source ESP-IDF's activation script:

```bash
source "$HOME/esp/esp-idf-v5.5.6/export.sh"
idf.py --version
```

Reproduce the reported smoke-test build from ESP-IDF's bundled example:

```bash
cd "$IDF_PATH/examples/get-started/hello_world"
idf.py set-target esp32
idf.py build
```

`set-target` selects/configures this example's build target. The target reported for both boards is `esp32`.

## Flash and monitor

First check which physical board is connected to each port; `/dev/ttyUSB0` and `/dev/ttyUSB1` are not intrinsically board identities. Close any existing monitor before opening another. With the intended port attached:

```bash
cd "$IDF_PATH/examples/get-started/hello_world"
idf.py -p /dev/ttyUSB0 flash monitor
```

Use `/dev/ttyUSB1` for the other attached board, after confirming the mapping. The researcher reports this flash/monitor procedure succeeded on both boards. Flash writes firmware; do not run it against an unintended port. Exit the monitor with `Ctrl+]`.

Expected `hello_world` behavior: a `Hello world!` message, a countdown, then a software restart and repeat. This is the example's expected behavior, not console output independently observed in this workspace.

For future project firmware, use the same sourced environment, select `esp32`, build from that project's directory, and specify the confirmed serial port for flashing/monitoring. Keep this smoke-test example out of the experimental firmware tree.

## Hardware assumptions still open

The reported SoCs are ESP32-D0WD-V3 revision v3.1 with 4 MB flash and 40 MHz crystals, via CP2102 USB-UART. Exact board/module markings, antenna configuration, and persistent port-to-board mapping remain unverified; see [hardware inventory](hardware-inventory.md).
