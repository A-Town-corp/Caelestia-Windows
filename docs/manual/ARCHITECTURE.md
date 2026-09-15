# Architecture

## Current state after trial rejection (2026-09-15)
- Seelen app and service are stopped; `\Seelen\Seelen UI Service` scheduled task is Disabled. The native Windows taskbar is visible. Seelen files remain installed.
- The intended system is a Qt Quick/QML and C++20 Windows host retaining Caelestia's shared blob renderer, tokens and drawer composition. Proposed modules and source mapping are in `docs/research/caelestia-windows-port-plan.md`; implementation has not begun.

## Historical desktop trial (2026-09-15; superseded)
- This workspace contains operational documentation, not a custom shell implementation.
- Installed Seelen UI 2.8.4.0 provides the dock (`@seelen/weg`), top toolbar (`@seelen/fancy-toolbar`) and power overlay (`@seelen/power-menu`). The toolbar plugin `@seelen/tb-power-menu` triggers the overlay.
- User settings live under `%APPDATA%\com.seelen.seelen-ui`; the program and service binaries live under `C:\Program Files\WindowsApps\Seelen.SeelenUI_2.8.4.0_x64__p6yyn03m1894e`.
- Windows continues to position application windows. Seelen's window manager, launcher, wallpaper manager and task switcher are disabled.
- The Seelen service monitors app availability and manages native-taskbar integration. Disabling it temporarily was diagnostic; it is not the desired steady-state setup.

> Precise enough to redraw the system from this file alone. Use **exact** names
> and paths — no "a service", name it. No vague words.

## System overview
_One paragraph + a component list or diagram._

## Components
<!-- Repeat per component:
### <name>   (path: <dir or file>)
- **Responsibility:** the single thing it owns.
- **Inputs / outputs:** exact shapes, where they come from / go to.
- **Key public symbols:** functions/classes/endpoints (exact names).
- **Depends on:** ... · **Depended on by:** ...
-->

## Data model / schemas
_Tables, types, message shapes — exact field names + types + constraints._

## Data / control flow
_Walk one representative operation end-to-end, step by step._

## External dependencies & integrations
| Dependency | Exact version | Why it's used | Where |
|------------|---------------|---------------|-------|
