# Backlog

> The project's durable to-do board — continuity of intent across sessions: any
> future session reads this and knows what's next and why.
>
> - **Capture rule:** any idea, bug, or "later" that surfaces mid-session gets a
>   line here THE MOMENT it's deferred. One line: what + enough why to act on it cold.
> - **Now** = hard limit 3. Finish or demote before pulling more in.
> - **Next** is ordered — top item is what a fresh session picks up first.
> - **Done**: move the line here with the date. **Icebox**: someday/maybe, prune freely.

## Now (max 3)

## Next (ordered)
- Begin the Caelestia Windows renderer/input spike from `docs/research/caelestia-windows-port-plan.md`: pin the source, capture the reference, retain blob rendering and prove the connected top dashboard reveal on Windows.
- After visual acceptance, port the connected bar/popouts, volume OSD and session drawer; wire real Windows controls; complete remaining shell surfaces and desktop-restoration tests.

## Icebox

## Done
- [2026-09-15] Disabled Seelen startup task and stopped app/service; verified zero Seelen processes and native taskbar visible. Retained installation/configuration and prior backups.
- [2026-09-15] Delivered source-backed Caelestia Windows port plan with pinned upstream commit, proposed Qt/C++ architecture, module map, visual acceptance gates and research log.
- [2026-09-15] Closed Seelen theming and WMI/cold-start investigation follow-ups because user rejected Seelen as the visual foundation; these tasks were not implemented.
- [2026-09-15] User confirmed that physically clicking the top-right power button opens the large power menu; automated click failure does not reproduce with the user's mouse.
- [2026-09-15] Started Seelen UI 2.8.4.0 dock/toolbar trial, backed up legacy configuration, corrected duplicate toolbar aliases, added power-menu plugin, and disabled wallpaper/task-switcher replacements while keeping tiling off. Verified visible dock and toolbar and power overlay through CLI.
