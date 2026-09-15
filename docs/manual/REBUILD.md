# Rebuild Runbook

## Current state — Seelen disabled, port not implemented
The trial procedure below is historical; do not run it to resume current work. Follow `docs/research/caelestia-windows-port-plan.md` for the first implementation milestone.

Disable commands executed on 2026-09-15:
```powershell
Disable-ScheduledTask -TaskName 'Seelen UI Service' -TaskPath '\Seelen\'
& 'C:/Program Files/WindowsApps/Seelen.SeelenUI_2.8.4.0_x64__p6yyn03m1894e/slu-service.exe' stop
Get-Process seelen-ui -ErrorAction SilentlyContinue | Stop-Process
```
Verification: task state `Disabled`; count of `Get-Process seelen-ui,slu-service -ErrorAction SilentlyContinue` results is zero. Desktop screenshot showed the native Windows taskbar and original wallpaper. Nothing was uninstalled, and backups remain at the path below.

## 2026-09-15 — Desktop trial procedure
1. Host: Windows 11 Pro 10.0.26200; installed package `Seelen.SeelenUI` version `2.8.4.0`.
2. Baseline backup: `C:\Users\Fonem\AppData\Local\CodexBackups\Seelen\20260915-171715`. Contains `settings.json`, `toolbar_items.yml`, `seelenweg_items_v2.yml`, and toolbar state snapshots before migration and power-button replacement.
3. Read `%APPDATA%\com.seelen.seelen-ui\settings.json`. Keep `byWidget["@seelen/window-manager"].enabled` false and `byWidget["@seelen/launcher"].enabled` false.
4. Launch with PowerShell: `Start-Process explorer.exe -ArgumentList 'shell:AppsFolder\Seelen.SeelenUI_p6yyn03m1894e!App' -WindowStyle Hidden`.
5. In Seelen UI Settings, disable Wallpaper Manager and Task Switcher using each page's `Enable this widget` toggle, then Save. This preserves the existing wallpaper and Windows task switching.
6. Keep `@seelen/weg` and `@seelen/fancy-toolbar` enabled. In active toolbar state, replace `@default/power` with `@seelen/tb-power-menu` and relaunch. See GOTCHAS.md for the duplicate-alias migration.
7. To open settings, run `& 'C:/Program Files/WindowsApps/Seelen.SeelenUI_2.8.4.0_x64__p6yyn03m1894e/slu.exe' settings`.
8. Verify the visible dock and top toolbar. Open the power menu, then dismiss by clicking the empty backdrop; do not invoke shutdown, restart, sign-out, lock or sleep during verification.
9. Native power-menu trigger: `& 'C:/Program Files/WindowsApps/Seelen.SeelenUI_2.8.4.0_x64__p6yyn03m1894e/slu.exe' widget trigger '@seelen/power-menu'`. Command was checked against installed CLI help.
10. Read current-session output in `%LOCALAPPDATA%\com.seelen.seelen-ui\logs\Seelen UI.log`; search for dock/toolbar `status changed to: Ready` and record remaining errors separately.

> Hand this + the source to a competent stranger; they reproduce the project
> exactly. Every step must be literally runnable. No "configure as appropriate."

## 0. Prerequisites
- **OS / platform:**
- **Runtime / SDK versions (exact):**
- **Global tools (exact versions):**

## 1. Get the code
_Repo URL, branch, how to clone._

## 2. Environment variables
| Name | Purpose | Example (redacted) | Required? |
|------|---------|--------------------|-----------|

## 3. Install dependencies
_Exact commands._

## 4. External services / accounts
_Databases, APIs, keys — what to provision and how._

## 5. Build
_Exact commands._

## 6. Run (development)
_Exact commands + expected URL/port/output._

## 7. Run (production) / Deploy
_Exact steps._

## 8. Verify it works
_Exact command + the exact expected output that proves success._
