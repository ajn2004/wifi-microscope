# Experimental Method

## Required run record

Every experiment plan and run should record:

- Research question, competing hypotheses, and predicted outcomes.
- Apparatus: exact board/module identifiers, antenna configuration, placement and geometry.
- Firmware, ESP-IDF, host OS/kernel, and relevant tool/software versions or revisions.
- Independent and dependent variables, controls, and plausible confounders.
- Measurement procedure, timing, trial count, exclusions, and deviations.
- Raw-data location and metadata; analysis code/version and transformations.
- Results, uncertainty, negative results, interpretation, and remaining questions.

Separate predictions (written before observing the result) from observations and interpretation. Keep raw data unchanged; document all cleaning, filtering, or exclusions.

## Lightweight machine-readable metadata

Use one UTF-8 YAML file named `metadata.yaml` beside each run's raw data. YAML is a human-editable convention, not a requirement to add a parser or dependency. Suggested minimum:

```yaml
schema_version: 1
run_id: "2026-01-01T120000Z_stationary_01" # choose a unique UTC-based ID
started_at_utc: "2026-01-01T12:00:00Z"
question: "..."
hypotheses: ["H1"]
condition: "stationary baseline"
apparatus:
  receiver_board: "unknown until inspected"
  sender: "..."
  geometry_m: {separation: null}
acquisition:
  firmware_revision: "..."
  esp_idf_version: "..."
  channel: null
  bandwidth_mhz: null
  packet_source: "..."
host:
  os_kernel: "..."
raw_data: "raw/observations.bin"
clock_notes: "clock domain and timestamp source"
variables: {independent: [], dependent: []}
controls: []
confounders: []
procedure: "..."
analysis: {method: "...", revision: "..."}
deviations: []
result_summary: "..."
uncertainty: "..."
interpretation: "..."
```

This is a template, not an assertion that example values are measured. Use explicit units in field names or values; prefer UTC ISO-8601 for wall-clock times and preserve device/host monotonic timestamps separately with their clock source. Null means unknown/not recorded, not zero.
