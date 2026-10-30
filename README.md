# Wi-Fi Radio Microscope

An experimental laboratory for investigating Wi-Fi channel state information (CSI), physical signal propagation, measurement reliability, and Linux system effects using ESP32 devices and local computers.

The goal is hands-on understanding—not a product launch. Research questions and falsifiable hypotheses are in [the research charter](docs/research-charter.md); working conventions are in [AGENTS.md](AGENTS.md).

## Current state

The repository has ESP-IDF projects for a Wi-Fi frame transmitter and CSI receiver. They are an early device-to-device firmware path; a host capture tool, GUI, calibrated measurement workflow, and reproducible experiment are not yet complete. See [firmware quick start](firmware/README.md) to build and flash the boards.

## Repository map

- `firmware/` — ESP-IDF transmitter and CSI receiver projects (see its README).
- `host/` — future local capture and systems instrumentation.
- `analysis/` — future reproducible analysis.
- `experiments/` — experiment records and protocols.
- `data/` — local raw and derived data; generated data is ignored by Git.
- `docs/` — research, architecture, method, hardware, and backlog planning.

Start with [hardware inventory](docs/hardware-inventory.md), [experimental method](docs/experimental-method.md), and [architecture](docs/architecture.md). No Linear issues have been created; the proposed seed is [docs/linear-seed.md](docs/linear-seed.md).
