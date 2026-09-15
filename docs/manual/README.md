# Project Manual

A living, unambiguous, rebuild-from-scratch manual for this project. Goal:
**someone handed this folder + the source could rebuild the project 1:1, with no
room to interpret it differently.**

| File | Role | Maintained by |
|------|------|---------------|
| NOTEBOOK.md | Live thinking surface — reasoning, options, dead ends, as work happens | model (continuous) |
| BACKLOG.md | Durable to-do board — Now (≤3) / Next (ordered) / Icebox / Done | model |
| ARCHITECTURE.md | How the system is structured and how parts connect | model |
| DECISIONS.md | *Why* it's built this way; alternatives rejected (ADR-style) | model |
| GOTCHAS.md | What doesn't work, constraints, things to keep in mind | model |
| REBUILD.md | Step-by-step runbook to reproduce the project exactly | model |
| CHANGELOG.md | Mechanical log of every edit/command | hook (automatic) |

Workflow: think out loud in **NOTEBOOK.md**; when a thought hardens into a durable
fact, **promote** it — decision → DECISIONS.md, structure → ARCHITECTURE.md,
pitfall → GOTCHAS.md, setup step → REBUILD.md.

Disable for this project: `/manual-off` (or create an empty `.claude/no-manual`).
