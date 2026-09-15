# Notebook (working log)

> Live thinking surface. Lay out reasoning, options, plans, hypotheses, open
> questions, and dead ends here **as you work** — before and during, not after.
> This is a tool to think with, not a report. Newest entries on top.
>
> When a thought hardens into a durable fact, PROMOTE it:
> decision → DECISIONS.md · structure → ARCHITECTURE.md · pitfall → GOTCHAS.md ·
> setup step → REBUILD.md.

## Open questions / TODO
- [ ] (none yet)

## Log
### 2026-09-15 — Seelen rejected; actual port planned
- User explicitly requested disabling Seelen and planning the actual Caelestia port. The target includes seamless edge reveals, attached menus and matching volume/power motion; a macOS-style dock is unacceptable.
- Ran `Disable-ScheduledTask -TaskName 'Seelen UI Service' -TaskPath '\Seelen\'`; output: `Seelen UI Service Disabled`. Settings UI showed Run on startup off. Ran supported service `stop`, then stopped only `seelen-ui`; process count became zero. Screenshot confirmed original wallpaper/native taskbar and no Seelen bar/dock.
- Research pinned upstream `0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0` and community reference `d2976b305040254774790df44ff40aa97c7bbe53`. Key finding: the visual join is a shared `BlobGroup` scene, not independent popup backgrounds. Keep QML/renderer and replace platform host/services.
- Build risk: upstream blob shader configuration disables HLSL; Windows graphics output must be verified first. Input regions/focus and native volume-flyout coexistence also require experiments. No port code or build exists.
- Full plan and auditable three-round log: `docs/research/caelestia-windows-port-plan.md`. No planning-file precedent existed. Seelen follow-ups closed as rejected direction; first port implementation milestone captured in backlog.
### 2026-09-15 — User acceptance of power-button behavior
- User answered `Yes, it opens` when asked whether physically clicking the top-right power icon opens the large power menu. Power-button behavior is verified by user interaction; the failed synthetic clicks are an automation limitation in this session.
- Final settings check printed dock and toolbar `True`; window manager, task switcher, wallpaper manager and launcher `False`. UI PID 35236 and service PID 32300 remained unchanged from the final successful package launch.
- Trial is running. Remaining work is visual customization after user feedback and investigation of the non-blocking WMI log error/cold-start reliability.
### 2026-09-15 — Trial verification and remaining limits
- GUI flow: launched existing Seelen UI, dismissed welcome prompt, disabled Wallpaper Manager and Task Switcher, saved both, and confirmed the original wallpaper returned. Window Manager remained false throughout.
- Fixed inline toolbar alias duplication and replaced the plug/battery toolbar item with `@seelen/tb-power-menu` using the installed plugin definition.
- Final non-elevated package launch through Explorer started PID 35236 at 17:30:16; service PID 32300 at 17:30:19. At 17:30:21 the dock logged `status changed to: Ready`; at 17:30:38 the start-menu scan logged `Loaded 480 start menu items`. This launch survived the previous restart interval with the service enabled.
- Power-menu CLI trigger opens the six-action overlay. Clicking empty backdrop dismisses it. No power/session action was invoked. Automated toolbar clicks did not open it; user was asked to verify physical clicks to distinguish an automation limitation from an app defect.
- Outstanding: a WMI error remains in the final launch log. Do not claim zero errors or a fully themed Caelestia result. No custom CSS or application code was written.
### 2026-09-15 — Toolbar migration diagnosis and experiment
- Exact failure: `SerdeYaml(Error("left: data did not match any variant of untagged enum ToolbarItem2", line: 3, column: 3))`.
- Seelen 2.8.4 source `libs/core/src/state/placeholder.rs` declares `onClickV2` as an alias of `onClick`. Both keys occur in each old inline separator, causing duplicate-field deserialization failure. `src/background/state/application/toolbar_items.rs` reads the migrated `data/seelen-fancy-toolbar/state.yml`.
- Experiment: remove only the two redundant null `onClickV2` keys from the active toolbar state. Keep `onClick`, item order, and all other values. Original copied to the backup as `toolbar-state-before-migration.yml`.
- Verify through the app's file watcher/relaunch and visible toolbar; do not change unrelated settings to mask this error.
## 2026-09-15 — Seelen UI trial
- User wants a coordinated dock, toolbar and power menu inspired by Caelestia while preserving normal Windows window behavior. Tiling is explicitly excluded.
- Seelen UI 2.8.4.0 is already installed on Windows 11 Pro build 26200. Existing settings enable the dock and toolbar and disable `@seelen/window-manager` and `@seelen/launcher`.
- Plan: back up the existing settings and item lists, launch the installed application, verify dock/toolbar/power flyout, and record the trial. Custom Caelestia styling follows user feedback on the trial.
<!-- ## <YYYY-MM-DD> <topic>
What I'm trying · why · options considered · what I picked and why · what's blocked. -->
