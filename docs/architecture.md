# Architecture (provisional)

## Boundaries

The intended investigation has four separable stages: (1) a transmitting peer and ESP32 CSI receiver; (2) a local host capture path; (3) preserved raw observations plus per-run metadata; (4) offline analysis/visualization. Host-side scheduling and buffering instrumentation can later be added as a separately observed path. Keep enough identifiers and timestamps to correlate stages without conflating radio receive time and host arrival time. The researcher reports two ESP32-D0WD-V3 revision v3.1 boards and ESP-IDF v5.5.6/`esp32`; exact development-board/module and antenna remain unknown. See the [hardware inventory](hardware-inventory.md) and [toolchain procedure](toolchain.md).

## First milestone scope

One supported ESP32 receiver, a controlled packet source (the second ESP32 or another verified source), local capture, and offline visualization. Start with a simple transport justified by board capability and measured data volume. CP2102 UART is the current candidate host link, pending port mapping and throughput/loss checks. Preserve driver-provided raw CSI and receive metadata before transformation. The first implementation exercise and unresolved assumptions are in the [DAL-237 plan](dal-237-plan.md).

## Deferred decisions

Exact board/target, IDF release, sender/receiver roles, packet-generation method, serial/USB/network transport, record schema, clock alignment, and analysis language depend on hardware inspection and initial experiments. Capture decisions and rationale in experiment records or a short decision note when resolved. No cloud, database, dashboard service, multi-user system, container, or application framework is warranted currently.
