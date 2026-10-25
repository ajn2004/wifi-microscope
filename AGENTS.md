# Scientific Engineering Protocol

This repository is an experimental laboratory for learning embedded systems, signal processing, physical measurement, and Linux internals. It is not a SaaS product. These rules apply to Luna, OpenCode, Devin, and other coding agents.

## Scientific reasoning

1. Before proposing an experiment, state the uncertainty it is intended to resolve.
2. Label theoretical predictions separately from empirical observations.
3. Prefer analytical reasoning before expensive or time-consuming experiments.
4. State competing hypotheses and identify observations that would distinguish them.
5. Never fabricate measurements, results, device capabilities, or verification.
6. Preserve negative results, anomalies, and unexplained behavior in the record.
7. Treat provenance, units, timestamps, and uncertainty as first-class measurement requirements.

## Human ownership of implementation

The human researcher retains primary ownership of ESP32 firmware and acquisition logic; low-level C/C++ and Linux systems code; numerical analysis and signal-processing algorithms; experimental design; and debugging or interpretation of unexpected behavior.

Agents may offer architecture discussion, references, code review, test proposals, diagnostics, and peripheral tooling. **Do not autonomously implement core experimental machinery unless specifically authorized for that task.** When asked to implement a core component, default to explaining its mechanisms, interfaces, tradeoffs, and incremental implementation steps rather than generating the entire solution.

## Engineering practices

- Prefer minimal, understandable code over abstraction-heavy frameworks.
- Test deterministic software behavior.
- Keep raw observations distinct from processed data.
- Record acquisition configuration and relevant software versions with every dataset.
- Make procedures reproducible; prefer local operation and local storage.
- Avoid unnecessary distributed systems, agent orchestration, and dependencies.
- Document important architectural decisions.
- Never modify production or unrelated home-server services without explicit approval.
- Always utilize Jujutsu for local version control adding in the description message any changes made to the repo
- Use the github cli to push changes to the remote repo

## Agent workflow

For any nontrivial task:

1. Inspect existing code and documentation.
2. Explain the problem and relevant system mechanism.
3. State assumptions and uncertainties.
4. Propose a minimal change.
5. Identify how correctness will be tested.
6. Implement only within the explicitly approved scope.
7. Report what was verified and what remains unverified.

A successful contribution should improve the researcher's understanding, not merely increase repository throughput.
