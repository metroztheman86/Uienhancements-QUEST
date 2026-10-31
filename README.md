# UIEnhancements — Quest (Beat Saber 1.40.8_7379)

Quest port of the UI background-panel mod. Targets **Beat Saber Quest 1.40.8 / build 7379**.

## What it does

Adds customizable semi-transparent background panels (with optional borders) behind:

- Song list (`LevelCollectionViewController`)
- Gameplay setup (`GameplaySetupViewController`)
- Level selection (`LevelSelectionNavigationController`)
- Leaderboard (`PlatformLeaderboardViewController`)

Settings live at:

```
/sdcard/ModData/com.beatgames.beatsaber/Configs/UIEnhancements.json
```

Global alpha/color/border + per-screen overrides (when “Selective” is enabled for that screen).

## Dependencies (pinned for 1.40.8)

| Package            | Range        |
|--------------------|--------------|
| beatsaber-hook     | ^6.4.2       |
| bs-cordl           | 4008.*       |
| bsml               | ^0.4.55      |
| config-utils       | ^1.4.0       |
| custom-types       | ^0.18.3      |
| paper2_scotland2   | ^4.6.4       |
| scotland2          | ^0.1.6       |

These match other published 1.40.8 mods (BeatTogether, Vivify, BSML 0.4.55, etc.).

## Build with GitHub Actions (recommended)

1. Create a GitHub repository and push the contents of this folder to the root.
2. Go to **Actions → Build Quest mod (1.40.8) → Run workflow**.
3. Download the `UIEnhancements-Quest-1.40.8` artifact and extract `UIEnhancements.qmod`.
4. Install the QMOD with **ModsBeforeFriday (MBF)** (or QuestPatcher) on a 1.40.8_7379 install.

## Local build

Requirements:

- Android NDK r27+ (set `ANDROID_NDK_HOME` or put the path in `ndkpath.txt`)
- CMake, Ninja
- Rust/Cargo + `qpm-cli` (`cargo install qpm-cli --locked`)
- PowerShell (or use the bash helper on Linux/macOS)

```powershell
qpm restore
pwsh ./scripts/build.ps1
pwsh ./scripts/createqmod.ps1
```

Linux/macOS alternative:

```bash
export ANDROID_NDK_HOME=/path/to/ndk
./scripts/build-quest.sh
# then package manually or adapt createqmod.ps1
```

## Important notes

- This is a **port scaffold with correct 1.40.8 dependency pins and modern scotland2 macros**. It has **not** been runtime-tested on a Quest headset.
- After a successful compile you **must** test in-game: the four `DidActivate` hooks, BSML panel layout, and sibling ordering are runtime-sensitive.
- If a hook signature or BSML API changes, the only places that need updating are the `ATTACH_HOOK` macros and the `Construct` helper in `src/main.cpp`.
- Do not distribute a binary as “confirmed working” until you have loaded it on 1.40.8_7379 and verified the panels appear on the four screens.

## Credits

- Original PC mod concept / layout numbers: zjavierf
- Quest scaffolding & 1.40.8 dependency lock: this port
