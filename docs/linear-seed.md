# Proposed Linear Seed (not imported)

Planning only. No Linear project/team IDs are known or invented, and no tickets have been created. **P0** means prerequisite for the first useful experiment; **Later** means exploratory follow-up. Dependencies refer to titles below. Human-owned core work is explicitly not agent-delegable by default.

## Epic A — Experimental Apparatus

### A1 (P0) Inventory ESP32 boards and verify CSI support
- **Objective / why:** Determine exact targets, module/antenna details, interfaces, and target/version CSI API support before selecting an acquisition design.
- **Dependencies:** None.
- **Deliverable:** Hardware inventory entries with markings/photos or exact identifiers, ESP-IDF version/reference links, selected candidate receiver and sender (or documented gap).
- **Acceptance:** Exact chip/module identifiers recorded; CSI API/configuration is documented for the chosen target and pinned IDF version; unresolved capability claims explicitly listed.
- **Class:** theoretical. **Ownership:** human-owned; agent may gather references/review, not inspect hardware.

### A2 (P0) Establish and record the embedded toolchain
- **Objective / why:** Establish a reproducible build/flash/monitor path for the selected target.
- **Dependencies:** A1.
- **Deliverable:** Human-selected ESP-IDF install/version and a documented successful build/flash/console check using an appropriate official example or minimal probe.
- **Acceptance:** Version, target, commands, and observed successful device identity/console output recorded; no project firmware committed as an assumed implementation.
- **Class:** firmware. **Ownership:** human-owned; agent may explain diagnostics.

### A3 (P0) Produce a minimal genuine CSI capture
- **Objective / why:** Resolve whether chosen hardware, packet source, target build, and receive path produce usable CSI observations.
- **Dependencies:** A1, A2.
- **Deliverable:** Human-built capture that obtains and preserves CSI plus relevant receive metadata and packet identity/sequence where available; record exact setup and raw sample.
- **Acceptance:** Raw bytes originate from actual device callbacks; receive configuration and target are recorded; callback work is bounded; capture can be repeated; missing/invalid fields are not silently interpreted.
- **Class:** firmware. **Ownership:** human-owned core machinery; agents limited to design review and incremental guidance unless specifically authorized.

### A4 (P0) Verify local host-side capture and record integrity
- **Objective / why:** Establish that device output reaches local storage without silent corruption or unaccounted drops.
- **Dependencies:** A3.
- **Deliverable:** Documented local transfer/capture path and one preserved run with record count/checksum or equivalent integrity check.
- **Acceptance:** Transport and framing are described; counts/sequence gaps or known limits are reported; raw record and metadata are recoverable from disk.
- **Class:** host systems. **Ownership:** human-owned for acquisition path; peripheral integrity-check suggestions may be delegated.

### A5 (P0) Visualize capture and validate the acquisition
- **Objective / why:** Inspect whether records are structured, variable, and interpretable before physical inference.
- **Dependencies:** A4.
- **Deliverable:** Local offline plot/table of at least one genuine capture, with axes, units/encoding, metadata, and invalid/missing records handled explicitly.
- **Acceptance:** Visualization is reproducible from preserved raw input; no synthetic data is presented as measured; capture limitations and anomalies are noted.
- **Class:** analysis. **Ownership:** human-owned interpretation; plotting/peripheral tooling may be agent-assisted after approval.

## Epic B — Physical Measurement and Inference

### B1 (P0) Run a controlled stationary baseline
- **Objective / why:** Estimate ordinary within-run and between-run variability against which perturbation can be judged.
- **Dependencies:** A5.
- **Deliverable:** Repeated stationary trials, completed metadata, a preregistered summary statistic, and baseline distribution/uncertainty.
- **Acceptance:** Geometry, channel/traffic, procedure, versions, and controls recorded; trial and missing-packet counts reported; raw data retained; no post-hoc threshold represented as preregistered.
- **Class:** experimental. **Ownership:** human-owned experimental design and interpretation.

### B2 (P0) Test one controlled physical perturbation
- **Objective / why:** Test whether a defined physical change produces a distinguishable response relative to baseline variability.
- **Dependencies:** B1.
- **Deliverable:** Repeatable, timed perturbation protocol (initial suggestion: person crosses marked path), control/sham condition if feasible, and paired raw runs.
- **Acceptance:** Prediction and metric fixed before examining condition comparison; condition labels and timing recorded; packet/timing quality checked; positive, null, and ambiguous outcomes reported.
- **Class:** experimental. **Ownership:** human-owned.

### B3 (Later) Compare candidate CSI statistics and features
- **Objective / why:** Determine which transparent summaries preserve physical response while representing noise and packet variation.
- **Dependencies:** B1, B2.
- **Deliverable:** Analysis comparing explicitly defined amplitude/complex CSI summaries and simple temporal statistics with units/transformations.
- **Acceptance:** Code/method revision recorded; training/comparison separation stated; feature selection and exclusions documented; no unsupported physical interpretation.
- **Class:** analysis. **Ownership:** human-owned numerical analysis; agents may review or suggest tests.

### B4 (Later) Establish and challenge a baseline detector
- **Objective / why:** Quantify a minimal distinction against a stationary null without equating classification with causal understanding.
- **Dependencies:** B3.
- **Deliverable:** Simple, reproducible detection rule/model and evaluation on held-out runs.
- **Acceptance:** Baseline false-positive behavior reported; split is by run/trial rather than leaking adjacent packets; null result and limitations retained.
- **Class:** analysis. **Ownership:** human-owned model/interpretation.

### B5 (Later) Test generalization and attempt falsification
- **Objective / why:** Learn whether an apparent effect survives plausible changes or collapses under alternate explanations.
- **Dependencies:** B4.
- **Deliverable:** Controlled follow-up over selected geometry/orientation, people/objects, time, or channel conditions plus explicit failure cases.
- **Acceptance:** Each condition and predicted distinguishing observation defined beforehand; changes isolated as far as practical; scope of any generalization claim bounded.
- **Class:** experimental. **Ownership:** human-owned.

## Epic C — Systems Instrumentation

### C1 (Later) Characterize acquisition latency and loss
- **Objective / why:** Determine whether packet scheduling, receive callback delivery, and output introduce gaps or timing variability that compromise measurement claims.
- **Dependencies:** A4.
- **Deliverable:** Defined timestamp/sequence points and repeated capture summary of observed gaps and latency where clock semantics permit.
- **Acceptance:** Clock domains, resolution, and timestamp origin stated; expected/received counts reconciled; instrument overhead and unobservable intervals noted.
- **Class:** host systems. **Ownership:** human-owned acquisition changes; agent may review measurement plan.

### C2 (Later) Instrument host scheduling and buffering
- **Objective / why:** Separate radio-side variation from host-side delay, queue buildup, and storage backpressure.
- **Dependencies:** C1.
- **Deliverable:** Local diagnostics of host arrival, queue/buffer occupancy, write completion, and relevant scheduling/load observations.
- **Acceptance:** Data path stages and units documented; instrumentation overhead considered; no causal attribution based on host-arrival time alone.
- **Class:** host systems. **Ownership:** human-owned low-level systems code.

### C3 (Later) Perform controlled system-load experiments
- **Objective / why:** Test whether host load changes delivery latency/loss while the radio scene and traffic remain controlled.
- **Dependencies:** C2, B1.
- **Deliverable:** Matched idle/load runs, documented load procedure, and stage-specific timing/loss comparison.
- **Acceptance:** Load condition is repeatable and recorded; physical setup/traffic held as constant as practical; queue saturation and loss are reported rather than filtered away.
- **Class:** experimental. **Ownership:** human-owned.

### C4 (Later) Assess end-to-end measurement reliability
- **Objective / why:** Establish which portion of observed variability can be assigned to physical, device/acquisition, transport, or host stages.
- **Dependencies:** B2, C3.
- **Deliverable:** Error/provenance budget and a stage-by-stage reliability assessment with unresolved ambiguity.
- **Acceptance:** Claims trace to measured observables and stated uncertainty; unidentifiable causes remain explicitly unassigned.
- **Class:** analysis. **Ownership:** human-owned interpretation.

### C5 (Later) Synthesize observed failure mechanisms
- **Objective / why:** Consolidate what failed, why it failed, and what experiments distinguish competing mechanisms.
- **Dependencies:** C4, B5.
- **Deliverable:** Technical synthesis linking observations to radio, firmware, transport, and Linux mechanisms with follow-up questions.
- **Acceptance:** Distinguish observations from explanations; cite provenance and negative results; do not overgeneralize beyond tested setup.
- **Class:** theoretical. **Ownership:** human-owned synthesis; agents may review for clarity/citations.

## Dependency path and first milestone

Minimum path: **A1 → A2 → A3 → A4 → A5 → B1 → B2**. A1/A2 may proceed serially because hardware target determines toolchain configuration. B1/B2 must follow validated genuine capture and repeatable data storage. The initial milestone is complete when genuine CSI can be visualized with reproducible metadata for a stationary baseline and one controlled perturbation—not when a detector or product exists.

After milestone, B3–B5 and C1–C5 are exploratory follow-up. Some systems measurements (C1) may be pulled earlier if initial integrity checks expose loss or latency as an immediate blocker. There are no dashboard, database, cloud, machine-learning, multi-user, or deployment issues in this seed.
