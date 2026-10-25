# Research Charter

## Primary question

Can a controlled pair of ESP32 devices capture wireless channel measurements that reliably distinguish physical environmental changes from ordinary measurement variability?

## Secondary questions

- How do multipath interference and device geometry influence measured CSI?
- Which statistics and signal-processing methods reveal meaningful physical changes?
- How do packet loss, scheduling, buffering, and acquisition delays influence measurement validity?
- Can effects originating in the physical channel be distinguished from effects originating in the computing system?

## Hypotheses to test

These are proposed, falsifiable predictions, not findings:

- **H1 — Repeatability:** With devices, channel, traffic, and surroundings held as constant as practical, repeated stationary runs produce a within-condition distribution of CSI-derived summaries that can be characterized and bounded.
- **H2 — Perturbation:** A specified, repeatable physical perturbation (initially a person crossing a marked path) changes at least one preregistered CSI summary more than ordinary stationary run-to-run variability. Failure to separate the distributions under the stated conditions counts against this hypothesis.
- **H3 — Geometry dependence:** Changing a measured device position/orientation changes the baseline or perturbation response. If it does not, the tested geometry change has no demonstrated effect at the chosen measurement sensitivity.
- **H4 — System-path integrity:** Deliberately increasing host load or acquisition buffering changes observed delivery latency and/or missing-record rate; sequence/timestamp diagnostics can distinguish those effects from a change in the radio channel. If they cannot, end-to-end inferences remain confounded.

## Observables

Record received CSI bytes and their documented interpretation; packet/frame metadata available from the driver (e.g. RSSI, receive timestamp, channel/bandwidth and receive-control fields when supported); sender identity and packet sequence where available; expected, received, duplicate, and missing packet counts; device and host timestamps with clock source and resolution; host arrival time, queue/buffer state, and write completion where instrumented; device placement, orientation, separation, channel, traffic and environmental conditions. State units and definitions. CSI availability/format is target- and packet-dependent; do not assume uniform subcarrier count or calibrated phase.

## Preliminary confounders

Uncontrolled people or objects; moving doors/furniture; device placement and antenna orientation; multipath and reflections; neighboring Wi-Fi traffic, channel changes, rate adaptation, retries and power save; packet type, transmitter and timing; device temperature, clock drift, calibration and firmware/IDF revisions; CSI callback workload and buffer overflow; USB/serial throughput and host scheduling/storage delays; clock-domain offset; selection of packets/metrics after looking at outcomes. Log, control, or explicitly bound these where possible.

## Success criteria

Success is evidence quality, not feature completion. First milestone: capture verified CSI from identified supported hardware; retain enough per-run configuration and provenance; visualize the captured observations with labels/units; and execute a stationary baseline plus one controlled perturbation with raw data preserved. A claim of physical discrimination requires preregistered summaries, repeated baseline and perturbation trials, a stated comparison/uncertainty method, and checks that packet loss/timing or obvious environmental differences do not explain the result. No universal threshold is specified in advance: choose and justify it before inspecting the comparison outcomes. A null or ambiguous result is a valid result.

## Limitations

Commodity ESP32 CSI is receiver/driver-reported channel estimation, not a calibrated general-purpose RF instrument. Hardware/firmware behavior, packet mix, channel, antenna, and receive path constrain what can be inferred. A detectable difference does not by itself establish cause, generalization, human sensing capability, or physical units such as absolute distance. Findings initially apply only to the tested apparatus, location, protocol, and procedure. This work does not claim clinical, security, or production-grade sensing validity.
