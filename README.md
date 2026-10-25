# Wi-Fi Radio Microscope

An experimental laboratory for investigating Wi-Fi channel state information (CSI), physical signal propagation, measurement reliability, and Linux system effects using ESP32 devices and local computers.

The goal is hands-on understanding—not a product launch. Research questions and falsifiable hypotheses are in [the research charter](docs/research-charter.md); working conventions are in [AGENTS.md](AGENTS.md).

## Initial milestone

Capture and visualize genuine CSI observations from supported ESP32 hardware, with enough provenance to reproduce a stationary baseline and one controlled perturbation. No instrument implementation exists yet.

## Repository map

- `firmware/` — future embedded acquisition work; human-owned core machinery.
- `host/` — future local capture and systems instrumentation.
- `analysis/` — future reproducible analysis.
- `experiments/` — experiment records and protocols.
- `data/` — local raw and derived data; generated data is ignored by Git.
- `docs/` — research, architecture, method, hardware, and backlog planning.

Start with [hardware inventory](docs/hardware-inventory.md), [experimental method](docs/experimental-method.md), and [architecture](docs/architecture.md). No Linear issues have been created; the proposed seed is [docs/linear-seed.md](docs/linear-seed.md).
