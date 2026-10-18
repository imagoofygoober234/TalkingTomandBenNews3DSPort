# Talking Tom & Ben News — Original 3DS development port

**NOT A COMPILED OR TESTED CIA.** Original assets supplied by project owner. No complete original iOS source available.

## Included
- Original converted 400×240 animation frames and original audio (audio not wired to animation events yet).
- Personalized bottom-screen graphic and touchscreen hit regions.
- Mic prototype: hold X for Ben (deeper), Y for Tom (higher), release to replay. Requires working DSP firmware / NDSP.
- Auto news intro; basic fight-to-technical-difficulties transition.
- Latest glowing-console icon artwork. `icon.png` is a reference PNG, not an installed CIA icon.

## Incomplete / limitations
- **Chair-fall sequences are absent** from the inspected `AnimationsTomBenNews` folder; mapped to temporary sequences, not authentic chair falls.
- Random attacks are provisional until clips have been checked against original behavior.
- Facty graphics were NOT supplied; button reserves a touch region but doesn't spawn him yet.
- Sound-to-animation synchronization, background idle compositing, complete runtime, microphone UI integration, and finished user experience are incomplete.
- Current Makefile outputs a `.3dsx` and may need devkitPro fixes; no CIA generated. Nothing tested on a console.

## Build basics
Install devkitPro's 3DS toolchain including `libctru`, 3DS libjpeg port, and CIA build tools. Copy the supplied `sdmc/3ds/tom_ben_news` folder to the SD card. See devkitPro examples for Makefile and icon/banner packaging. Source assets should not be publicly redistributed without appropriate rights.

## Controls
X held: record Ben (deeper); Y held: record Tom (higher); A: replay intro; B: fight; L/R: sample attack; START: quit. Touch controls use the custom bottom-screen sketch; mapping pending more testing.
