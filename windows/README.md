# Caelestia Windows: first renderer/input test

This is a diagnostic prototype, not the finished Caelestia desktop. It uses the
upstream blob renderer, Settings/Config modules, animation component and M3Shapes.
The panel text, palette, font and 340 px content height are temporary test fixtures.

## Run this machine's packaged test

Open `E:/Customization/build/first-test/bin/caelestia-first-test.exe`.
Keep the entire `build/first-test` directory together: the executable needs its
DLLs and QML directories. No startup task is installed.

- Hover the top centre to open the panel; move away to retract it.
- Escape dismisses it until the pointer leaves the panel area.
- Click **Close test**, or press **Ctrl+Alt+Q**, to exit.
- Normal Windows window positioning and the native taskbar stay in use.

Default: D3D11, primary monitor's available work area, 8 ms cursor polling.
Avoid launching multiple copies. A second overlay cannot register the exit key.

| Optional argument | Purpose |
|---|---|
| `--preview` | Ordinary 1280×720 window instead of a desktop overlay |
| `--smoke-test` | Automated load/render/exclusion/open/close test; exits with status |
| `--input-target` | Separate 480×280 test window; clicking logs `INPUT_TARGET_CLICK n` |
| `--tight-mask` | Compare Qt's rendering/input mask with cursor-based transparency |
| `--padded-mask` | Same mask with 20 px padding around the drawer |
| `--vulkan` | Comparison backend; **desktop transparency failed on this PC** |

`first-test.log`, `input-target.log` and `smoke-open.png` are written in the package
root. Logs are replaced when their corresponding mode starts. Smoke exit codes:
2 log-open failure; 3 Vulkan initialization; 4 QML load; 5 exit-hotkey conflict;
6 click-through API failure; 7 opening/render/exclusion/capture/warning gate;
8 closing/warning gate; 9 non-excluded control failed to merge.

## Verified on 2026-09-15

- CMake 4.4.2; Visual Studio Build Tools 18, MSVC 14.51.36231 (compiler
  19.51.36252); Windows SDK 10.0.26100.0; Qt 6.11.2; Vulkan SDK 1.4.357.0.
- `ctest --test-dir build/input-tests -C Release --output-on-failure`: 1/1 passed;
  executable prints `PASS: 16 input-policy checks`.
- `windows/tests/scene-smoke.ps1`: QML load, animated open/close, rendered frame and
  clean exit passed without runtime warnings on D3D11.
- Shader gap control: excluded alpha 27; non-excluded alpha 255 at (305,550).
- Native UI flow: top hover → panel → Close test exited; Ctrl+Alt+Q exited;
  separate-process button received a click while the overlay remained running.

## Limitations and next gate

An immediate synthetic move-and-click missed once. Separating hover and click
worked; the 8 ms polling transition is not a production-ready input guarantee.
The two mask alternatives are available but have not been compared visually.
Coverage percentage, Linux pixel/motion parity, multiple monitors, changed DPI,
fullscreen suppression and crash recovery are unverified. Vulkan desktop
composition displayed black instead of transparent even with 8 alpha bits;
D3D11 passed that live check.

No real dashboard widgets, bar controls, volume, power, launcher, notifications,
wallpaper, tray or tiling integration is implemented. The diagnostic TEST strip
only supplies the border/bar geometry. Linux reference recordings, scheme and
fonts remain required before claiming 1:1 appearance.

## Build

Run from the repository root in PowerShell. The Qt and Vulkan SDK installations
below are external build prerequisites, not checked into the fork. Dependency
setup and checksum evidence are in `docs/manual/REBUILD.md` and `NOTEBOOK.md`.

```powershell
$env:PATH = 'E:/VulkanSDK/1.4.357.0/Bin;C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64;' + $env:PATH
$env:VULKAN_SDK = 'E:/VulkanSDK/1.4.357.0'
cmake -S . -B build/windows -G 'Visual Studio 18 2026' -A x64 '-DVERSION=0.1.0' '-DENABLE_MODULES=plugin;windows' '-DCMAKE_PREFIX_PATH=E:/Qt' '-DINSTALL_QMLDIR=qml'
cmake --build build/windows --config Release --parallel 6
cmake --install build/windows --config Release --prefix E:/Customization/build/first-test
E:/Qt/bin/windeployqt.exe --release --no-translations --qmldir windows/qml --qmlimport build/first-test/qml build/first-test/bin/caelestia-first-test.exe
cmake -S windows/tests -B build/input-tests -G 'Visual Studio 18 2026' -A x64
cmake --build build/input-tests --config Release
ctest --test-dir build/input-tests -C Release --output-on-failure
./windows/tests/scene-smoke.ps1
```

Stop at any nonzero command exit. Always run configure before build: Windows
copies prefixed headers during configure. FetchContent pins M3Shapes to
`32ad9ce328bb77ed349b40a3be10ee9ea610b8ab`; it reuses sources through a Windows CMake
adapter. The existing local checkout can be selected using
`-DFETCHCONTENT_SOURCE_DIR_M3SHAPES_SOURCE=E:/Customization/build/vendor/m3shapes`.

The initial Qt extraction omitted translations; windeployqt reports missing
`translations/catalogs.json` even with `--no-translations`. English diagnostic
content runs. This is not a tested multilingual distribution.
