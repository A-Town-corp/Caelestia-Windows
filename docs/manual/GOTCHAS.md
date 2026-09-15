# Gotchas & Constraints

## Actual Caelestia visual target
- Seelen's working controls did not satisfy the user's visual requirements. Do not return to a generic dock/theme approach.
- Upstream `ContentWindow.qml` uses shared `Caelestia.Blobs` shapes and deformation matrices; disconnected rounded panels cannot substitute for this behavior.
- Upstream blob shaders use `NOHLSL`; shader conversion must be proven for the Windows renderer.
- The top reveal is the dashboard in the pinned revision; launcher is bottom-centred, bar is left, session/OSD are right. Confirm the reference version/configuration before claiming a 1:1 match.
- Exact native volume-flyout suppression, complete notification/tray parity and renderer/input behavior are unverified; record those gaps rather than claiming full equivalence.

## Synthetic toolbar clicks
- Windows MCP clicks on the power icon did not open the overlay in this session, but the user confirmed physical clicks do. The CLI trigger also opens it. Do not treat synthetic click failure alone as proof of a broken power button.

## Seelen UI 2.8.4 migration and startup (2026-09-15)
- Active toolbar state is `%APPDATA%\com.seelen.seelen-ui\data\seelen-fancy-toolbar\state.yml`, not the legacy root `toolbar_items.yml`.
- Old inline items contained both `onClick: null` and `onClickV2: null`. Version 2.8.4 treats these as aliases for one field; the duplicate produces `SerdeYaml(Error("left: data did not match any variant of untagged enum ToolbarItem2", line: 3, column: 3))`. Remove the redundant null alias after backing up the file.
- Editing toolbar state on disk did not immediately change the running toolbar. Relaunch was required in this session.
- `@default/power` shows battery/plug status. `@seelen/tb-power-menu` opens the actual power menu; its installed definition is `static/widgets/power-menu/toolbar-plugin.yml`.
- Initial launch repeatedly exited with code 1. Stopping `slu-service.exe` with its supported `stop` command let the same UI process finish initialization; the service was then resumed. This is an observed workaround, not a proven permanent startup fix.
- Direct executable launches can inherit administrator privileges. Launch the installed package through Explorer using `shell:AppsFolder\Seelen.SeelenUI_p6yyn03m1894e!App`.
- During testing, the log also reported `WMI(HResultError { hres: -2147217396 })` and missing old pinned-shortcut paths. These are separate from the corrected toolbar deserialization error; do not claim an error-free application.

> What doesn't work and why, non-obvious constraints, and things to keep in mind.
> Often the most valuable part of a rebuild guide — it prevents repeated dead ends.

<!-- Template:
## <topic>
**What happens:** the symptom exactly — error text, observed behavior.
**Why:** root cause.
**Fix / workaround:** exact steps, commands, or config.
**Keep in mind:** the durable lesson or constraint.
-->
