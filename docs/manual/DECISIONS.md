# Decisions (ADR log)

## 2026-09-15 — Port Caelestia's connected scene to Windows
**Status:** accepted direction; implementation architecture proposed pending renderer spike.
**Decision:** Disable Seelen and plan a Qt Quick/QML + C++20 Windows adaptation of `caelestia-dots/shell@0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0`. Preserve the shared blob geometry, deformation, panel origins and animation tokens. Keep Windows window management.
**Why:** User requires the all-around Caelestia look and motion, not a similar palette or macOS-like dock. Upstream `modules/drawers/ContentWindow.qml` connects panels through `Caelestia.Blobs`.
**Alternatives rejected:** Seelen styling does not meet the visual target. Rebuilding separate HTML cards risks losing shared geometry. Porting all of Quickshell expands scope beyond the required Windows host. The community Windows port's final dashboard-only scope omits requested surfaces.
**Consequences:** First milestone must demonstrate the actual connected top-edge scene on Windows. Full parity remains unproven until the renderer/input spike, reference capture and Windows integration tests pass. See `docs/research/caelestia-windows-port-plan.md`.

## 2026-09-15 — Preserve Windows window behavior
**Status:** Seelen trial superseded by the Caelestia port decision above; preserving Windows window behavior remains required.
**Decision:** Trial Seelen UI 2.8.4.0 with `byWidget["@seelen/window-manager"].enabled = false`. Scope is dock, toolbar and power menu.
**Why:** User explicitly rejected Hyprland-style window tiling and authorized trying Seelen UI.
**Alternatives rejected:** GlazeWM and Linux/Hyprland change the window workflow the user wants to preserve. The community Caelestia Windows port documents a dashboard-only scope.
**Consequences:** Stock Seelen UI is the initial trial; a closer Caelestia theme remains follow-up work.

> The *why*. One entry per meaningful choice. Newest on top. State the rule, the
> exact reason, and what you rejected and why. Never use "some/a few/properly/etc."

<!-- Template:
## <YYYY-MM-DD> — <decision title>
**Status:** accepted | superseded by <#/date>
**Context:** the forces — constraints, requirements, what we knew at the time.
**Decision:** what we do, stated as a rule, with exact names/values/versions.
**Why:** the concrete reasoning.
**Alternatives rejected:** each option considered + the specific reason it lost.
**Consequences:** trade-offs accepted, follow-ups, what this locks us into.
-->
