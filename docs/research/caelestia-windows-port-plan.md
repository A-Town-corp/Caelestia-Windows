# Caelestia Windows port plan

Date: 2026-09-15. Status: planned; no port implementation or build exists yet.

## Outcome and design contract

Build Caelestia's connected desktop shell on Windows 11 while preserving Windows application positioning, resizing, snapping and Alt-Tab behavior. The user rejected the Seelen trial because its dock and detached surfaces looked like macOS. Seelen is disabled; it is not the foundation for this port.

The target is the upstream geometry, typography, icons, colours, panel contents, opening origins, deformation, easing and dismissal behavior. Reuse source instead of interpreting screenshots into a new design. A generic dock, separate floating cards, or native Windows-styled power dialog does not satisfy the target.

Pin upstream to `caelestia-dots/shell@0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0`. The reference Windows community port is `Krozzii/caelestia-windows@d2976b305040254774790df44ff40aa97c7bbe53`; use it only as a candidate backend reference after review. Its final build explicitly excludes the launcher, taskbar, power actions and connected full shell. [Upstream](https://github.com/caelestia-dots/shell/tree/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0), [community scope](https://github.com/Krozzii/caelestia-windows/blob/d2976b305040254774790df44ff40aa97c7bbe53/PORTING.md)

**Confidence:** the upstream architecture and missing Windows host are directly evidenced by source. Reusing the renderer on Windows is the proposed engineering approach, not a proven build. Pixel-identical output, Windows flyout suppression and complete tray/notification parity remain unproven.

## Reference layout: do not guess the edges

The pinned source positions the dashboard at the top centre, launcher at the bottom centre, bar along the left, and volume/session panels on the right. The user called the top reveal a dock; implement the actual dashboard reveal rather than inserting a macOS-style application dock. Preserve the pinned arrangement initially. If the user's reference video shows another revision or configuration, resolve that difference before declaring visual acceptance. No screenshot or video was supplied or captured as an authoritative visual baseline in this research pass. [Panels.qml](https://github.com/caelestia-dots/shell/blob/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0/modules/drawers/Panels.qml)

## Architecture decision

Use **Qt Quick/QML with a C++20 Windows host**. QML is Qt's declarative interface language. Keep portable upstream QML and native Caelestia modules; replace Linux-dependent imports through explicit Windows adapters. Do not attempt to run the full Linux shell through WSL or port all of Quickshell as a prerequisite.

Caelestia already renders its border and panel backgrounds together through `BlobGroup`, `BlobRect` and `BlobInvertedRect`. `ContentWindow.qml` applies corresponding deformation matrices to the panel contents. This shared rendering surface is the essential visual behavior. Qt Quick supports Direct3D rendering on Windows, making reuse plausible. [ContentWindow.qml](https://github.com/caelestia-dots/shell/blob/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0/modules/drawers/ContentWindow.qml), [Qt scene graph](https://doc.qt.io/qt-6/qtquick-visualcanvas-scenegraph.html)

Proposed structure; these files do not exist yet:

| Area | Proposed path | Responsibility |
|---|---|---|
| Pinned source | `vendor/caelestia-shell/` | Upstream checkout and notices; record every patch separately |
| Build root | `CMakeLists.txt`, `CMakePresets.json` | Windows targets, explicit Qt/toolchain paths, shader compilation |
| Application | `src/main.cpp` | Non-elevated startup, single instance, teardown |
| Display host | `src/windows/PanelHost.*` | Transparent Qt Quick surface per monitor; per-monitor scaling; z-order and focus |
| Input regions | `src/windows/InputRegionController.*` | Interactive shapes follow visible panels; desktop remains clickable outside them |
| Desktop integration | `src/windows/DesktopIntegration.*` | Bar work area, fullscreen detection and taskbar restoration |
| Service contracts | `src/platform/` | QML-facing audio, media, power, system and network interfaces |
| Windows providers | `src/windows/services/` | Windows implementations, event subscriptions and capability reporting |
| Adapted scene | `qml/` | Ported drawers, bar and platform bindings; retain upstream components where feasible |
| Reference evidence | `tests/reference/` | Version/config/font manifest, reference captures and timed interaction scenarios |
| Distribution | `packaging/` | Runtime Qt deployment, notices, startup opt-in and restoration checks |

Start with a per-monitor shared scene because upstream uses one `ContentWindow` per screen. Prototype input masking and transparency before committing to a full-screen surface: Qt/Windows hit testing must not create an invisible overlay that intercepts ordinary applications. If a native host limitation requires multiple surfaces, the joins and motion still have to meet the same visual reference; changing the design is not an acceptable workaround. [Drawers.qml](https://github.com/caelestia-dots/shell/blob/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0/modules/drawers/Drawers.qml), [Regions.qml](https://github.com/caelestia-dots/shell/blob/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0/modules/drawers/Regions.qml)

## Port map

| Surface/system | Reuse | Windows work and acceptance condition |
|---|---|---|
| Connected border and panel backgrounds | `plugin/src/Caelestia/Blobs/`, drawer composition | Compile shader variants for Windows; preserve unions, rounding, smoothing and deformation without visible seams |
| Animation and visual tokens | `components/Anim.qml`, `Caelestia.Config`, colour/font assets | Keep actual curves, durations, font axes and dimensions; remove Linux path assumptions |
| Top dashboard | `modules/dashboard/` | Replace profile, metrics, weather and media providers; preserve top-edge reveal and tab resizing |
| Bar and attached menus | `modules/bar/`, `modules/bar/popouts/` | Replace Hyprland focus/window data, tray and menu providers; preserve menu attachment and movement |
| Volume popup and on-screen display (OSD) | `modules/osd/`, `AudioPopout.qml` | Core Audio master volume/mute, endpoint-change notifications and feedback from hardware keys; no polling-only fake slider |
| Power/session panel | `modules/session/` | Bind explicit Windows power/session operations; preserve upstream drawer geometry and dismissal |
| Media | Dashboard media QML | Global System Media Transport Controls sessions, metadata, artwork and commands; distinguish unavailable player support |
| Launcher | `modules/launcher/` | Index and launch Windows apps/shortcuts; preserve lower-edge reveal, search and keyboard navigation |
| Network/Bluetooth/brightness | Corresponding panel content | Capability-based Windows providers; external-monitor brightness is a separate hardware compatibility test |
| Notifications | Notification/sidebar QML | Windows notification-listener capability and user access; do not promise silent access or complete action parity |
| Workspaces | Preserve visual treatment where meaningful | No Hyprland tiling implementation. Resolve which Windows desktop information can be represented honestly; no fake switching controls |
| Secure lock/sign-in | No Linux lock-screen backend | Invoke Windows lock when requested; replacing Windows secure sign-in is outside the shell port |

Windows service API anchors: [Core Audio volume](https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume), [media sessions](https://learn.microsoft.com/en-us/uwp/api/windows.media.control.globalsystemmediatransportcontrolssessionmanager?view=winrt-26100), [notification listener](https://learn.microsoft.com/en-us/windows/apps/develop/notifications/app-notifications/notification-listener). Power, network, brightness and tray method signatures must be checked against the selected Windows SDK during implementation; they were not fully audited in this plan.

## Implementation sequence and gates

### 1. Capture the reference and prove the renderer

- Check out the pinned source. Inventory its dependency graph, assets and notices before modifying or redistributing them.
- Establish a runnable reference session or maintainer reference recording for the pinned configuration. Record actual fonts, palette, dimensions, animation values and panel origins in `tests/reference/manifest.json`.
- Build the smallest Windows scene containing the real blob renderer, border and top dashboard wrapper. Fixture data is permitted only in this explicitly labelled prototype.
- Test the Windows shader pipeline immediately: upstream `qt_add_shaders` currently specifies `NOHLSL NOMSL`. HLSL is the shader language used for Direct3D; remove the exclusion in the Windows build and verify the shader output and rendering. Do not assume an unchanged Linux shader package will work. [Blob build file](https://github.com/caelestia-dots/shell/blob/0f1435a2f5f0c6ad25a2858f282eee1a0453d8b0/plugin/src/Caelestia/Blobs/CMakeLists.txt)
- Prove click-through outside the panel, pointer travel across the edge trigger, interruptible open/close, Escape/outside-click dismissal and no accidental application activation.

**Gate:** an actual Windows executable shows the authentic connected top-edge reveal and matches a recorded upstream interaction. User judges that visible milestone before broadening the UI. A static screenshot or standalone rounded card does not pass.

### 2. Complete the connected shell structure

- Port the bar, attached popouts, right-side OSD and session drawer into the same scene.
- Retain the upstream `ScreenState` relationships, panel offsets, clipping and stacking when drawers overlap.
- Replace Quickshell screen/window/focus primitives with Windows host contracts; do not retain imports that only appear to work because a branch never runs.
- Integrate desktop work-area reservation via the Windows appbar mechanism where required. Floating drawers must not repeatedly resize application windows during animation. [Windows appbars](https://learn.microsoft.com/en-us/windows/win32/shell/application-desktop-toolbars)
- Keep Explorer running. Add native-taskbar concealment only after a tested normal-exit and crash-recovery route exists. Initial visual tests can run with the native taskbar present.

**Gate:** dashboard, bar menus, volume panel and power panel all use the same source-derived shape/motion system; normal window dragging, sizing and Alt-Tab still behave like Windows.

### 3. Connect real controls and complete the remaining panels

- Implement Core Audio before decorative extras. Validate volume changes in both directions: Windows controls update the panel, and the panel updates the intended endpoint.
- Connect the power actions through an injected command interface. Automated tests use a recording implementation; shutdown/restart/logout are not exercised on this active workstation as routine tests.
- Connect media, metrics, application launching, then network/Bluetooth/brightness and notifications according to capability.
- Add the remaining launcher, utilities, notification and settings surfaces to the same fidelity checklist. These remain required follow-on milestones for an all-around shell, not silently omitted features.
- Resolve the native Windows volume-flyout coexistence problem in a dedicated experiment. Access to volume events does not prove we can suppress Microsoft's flyout. If duplication cannot be eliminated through an acceptable mechanism, report that parity gap before calling the port 1:1.

**Gate:** user-facing controls perform their stated Windows actions; unavailable capabilities have explicit states, not fabricated values or inert buttons. All requested visual surfaces have implementation and verification entries.

### 4. Stabilize, compare and package

- Capture closed, opening, fully open, reversing and closing states. Compare upstream and Windows at matching scale, palette, fonts and content. Compare silhouettes and attachment points separately from font-antialiasing differences.
- Exercise rapid pointer entry/exit, panel switching during motion, display scale changes, multiple monitors, monitor removal, fullscreen apps, sleep/resume and Explorer restart.
- Add tests for audio event subscriptions, device removal, provider errors, scene state transitions and cleanup. Drive the real desktop UI as well as automated tests; synthetic mouse input alone failed to characterize Seelen earlier in this session.
- Measure frame times on the actual display refresh rate and hidden-state resource use; record results rather than inventing performance claims.
- Produce a self-contained package with pinned Qt/runtime/toolchain versions and retained license notices. Startup remains opt-in. Verify quit/crash restores desktop access and native-taskbar behavior.

**Gate:** no missing module is labelled complete; every accepted parity gap is documented. Build, test, visual captures and restoration evidence accompany the release candidate.

## Toolchain and scope constraints

Proposed Qt baseline: 6.11.2, subject to the shader/compile spike. The Qt documentation examined identifies 6.11.2, and Qt 6.11 lists Windows 11 x64 with MSVC 2022 as supported. Select the matching compiler toolset rather than assuming any installed Visual Studio release is supported. [Qt Windows support](https://doc.qt.io/qt-6/windows.html)

Local inspection found Visual Studio Build Tools 2026 version `18.8.12023.21`, Git `2.55.0.windows.3`, and no `C:\Qt`. `cmake`, `ninja`, `qmake`, `qtpaths` and `cl` were not found on the current PATH; this does not prove they are absent elsewhere. No compiler component inventory or Qt installation was performed. Exact CMake, Ninja, Windows SDK and MSVC component versions must be selected and pinned in the build spike. Do not present this planning document as a runnable build runbook.

This is a multi-stage native development project. A reliable effort estimate follows the renderer/input spike and import inventory. The first useful result is the faithful live top-edge scene; a broad but visually approximate shell is not a substitute.

## Research Log

| Round | Queries / direct reads actually run | Tools | New sources | Added |
|---|---|---|---|---|
| 1 | Both repository HEADs; upstream recursive tree; seven upstream files: `shell.qml`, dashboard/bar-popout/OSD/session wrappers, `StyledWindow.qml`, root `CMakeLists.txt` | `git ls-remote`, GitHub REST/raw through PowerShell; web reader attempted first | Upstream tree and seven source files | Pinned versions, drawer origins, Quickshell/Wayland boundary and C++20 build |
| 2 | Upstream tree again; community tree; Qt scene graph, Core Audio and appbar docs; upstream `Drawers.qml`, `ContentWindow.qml`, `Panels.qml`, `Anim.qml`; community `PORTING.md` and `windows/CMakeLists.txt` | GitHub REST/raw through PowerShell; MCP `fetch_content` | Community tree, six source files, three documentation pages | Shared blob scene, deformation, platform APIs and limits of existing port |
| 3 | Blob/config/service build files and `Regions.qml`; Qt Windows, media-session and notification-listener docs | GitHub raw through PowerShell; MCP `fetch_content` | Four source files, three documentation pages | NOHLSL issue, native hit regions, supported compiler family and notification consent boundary |

Totals: 3 rounds; 0 search-engine queries; 2 repository HEAD lookups; 25 distinct URLs successfully fetched (17 source files, 2 repository trees, 6 documentation pages). All 17 source files were read in full; 2 documentation pages were read in full and 4 as relevant excerpts. Tree output was filtered for relevant paths. The plan cites 12 distinct source/documentation pages plus the pinned repository tree link.

Dead ends: the web reader returned `Cache miss` for all seven first-round pinned raw-file requests; PowerShell direct reads succeeded for those same URLs. The local PATH lookup returned no build-tool commands and its compound command exited 1; follow-up `vswhere` identified the installed Build Tools installation. No Qt build, shader conversion, Windows notification access, native flyout suppression or upstream Linux visual capture was tested. Three rounds converge on a bounded architecture plan; remaining claims require implementation experiments.
