# Starfall Meadow (GBA) — notes for Claude Code

Starfall Meadow is a Tamagotchi-style Game Boy Advance game, formerly "Critter Keep". It is built by patching a
C extension (`ext.c`) onto an original GBA ROM (`CritterKeep_original.gba`). The current release is **v8.1**
(Nov 10, 2026). See `README.txt` for the feature history and `MANUAL.md` for the player's manual.

## Working agreement (the owner's standing rules)

- **Ask before each stage.** Propose a plan and wait for approval before building. For anything visual (sprites,
  layouts, new screens), show a **mockup first** (a PNG sheet) and build only after approval.
- **Don't change balance, prices or stats on your own.** Ask first, even for a small tweak. For example, milk
  stays at 10 coins although it now does the same as the apple (8).
- **Ship a test ROM after each stage** (`StarfallMeadow_v8.Nx_test.gba`) with in-game screenshots of what changed.
- **Keep the stat budget:** at most about 8 to 10 things may raise each stat (happiness, hunger, bond, health).
  New features should reward collection or cosmetics rather than add stat boosts.
- **Every decor piece stays in the game** with its animation and message, even when it has no stat effect.

## Build

Toolchain: `clang` (armv4t target), `ld.lld` and `llvm-objcopy` (`llvm-nm` and `llvm-objdump` are used by
`patch.py`), and Python 3 with Pillow (for the art generators and tests).

```
./build.sh            # -> CritterKeep.gba (the playable ROM) and ./emu (the test emulator)
```

Repo layout: the ROM sources (`ext.c`, `*.h`, `link.ld`, `patch.py`, `build.sh`) sit in the root with
`CritterKeep_original.gba`; the art/music generators live in `gen/` (with `gen/bg/` and `gen/picture_src.jpg`).
All paths are relative to the repo (no more `/home/claude/...`). Generators that write headers find the root from
their own location; `bgfx.py` and `tunes.py` write `../*.h`, so run them from inside `gen/`; `picture.py` runs from
the root. Build outputs (`ext.o/.elf/.bin`, `CritterKeep.gba`, `emu`) are git-ignored; the release ROM is committed
as `StarfallMeadow_v<N>.gba`.

**No writable globals:** the extension runs from ROM. Any `static` or global variable that isn't `const` makes
the build fail with "BUILD FAILED: no globals allowed" (`patch.py` reports a writable section). All state lives
in `struct State` (the macro `G`, at `0x0203E000`). Add new fields at the end of `State`. It is about 2 KB; nothing else
in the extension uses the RAM just after it, but check the size if you add a lot.

## Tests

The test emulator: `./emu` (built by `build.sh`). Usage: `emu rom.gba script [sram]`. It writes SRAM back
to the save file, so always copy a save first (the tests do this).

`python3 tests/run_tests.py <sections...>`. There are 175 checks, and every section passed in v8.1. Run them in
batches, each taking up to about 10 minutes:

1. `fidelity`: compares the clock against v7, byte for byte, by day only (on its own, it's slow)
2. `growth secrets care save fast rebal`
3. `games fixes unlocks journal b1 stories health`
4. `garden budget fuzz`
5. `fly`

Other details:
- `CUR_VER = 15` is the save version. `BASE14 = tests/baseline_v81b2.gba` is a save-version-14 build used for
  the save conversion test.
- `budget` checks that busy screens stay under about 280,000 CPU cycles a frame. Text costs about 2k cycles per
  character, so cache static text in `SCRATCH_BG` and copy it each frame (see `garden_words` and `fly_words`).
- The struct offsets used in pokes come from parsing `ext.c`, via `g('field')` and the `OFF` map in
  `run_tests.py`. Add new fields to that map.
- `tests/shots.py` is a screenshot helper. The script command `shot file.png` also works.

## Code map (`ext.c`, about 4,700 lines)

- **Modes** (`G.mode`): `M_NONE, M_MENU (START), M_EXTRAS (SELECT), M_SENDOFF, M_SHOP, M_BAG, M_EAT, M_MINI,
  M_ALBUM, M_SURE, M_DEX (journal), M_NAME, M_STORY, M_GARDEN, M_FLY`.
- **Entry points:**
  - `hook_update`: the per-mode update switch.
  - `hook_render`: the per-mode draw.
  - `hook_bg_copy`: draws the room or yard background with sky and decor animations, plus the firefly lantern.
- **Core:**
  - `sim_tick` runs every 90 frames (36 on FAST), and handles needs, health and the clock.
  - `life_watch` handles mornings, cries and the bed's happy morning.
  - `decor_tick` handles decor use (messages and kind boosts; no happiness since v8.1).
- **Original game addresses:** `PET` at `0x03000028` (the health is `P16(0x16)`, 0–100), `SCENE` at
  `0x03000020`, `MSG_BUF` at `0x030002A4`, `MSG_TIMER` at `0x0300001C`, `FRAMES` at `0x03000018` and the clock
  `PET_CLOCK` at `0x0300004C`.
- **Time:**
  - A day is 600 clock steps; night is steps 480–599.
  - At night the clock advances on only 2 of every 3 ticks, so the night lasts 4.5 minutes, while the critter's
    own counters keep their usual pace.
  - The clock runs in `M_NONE`, `M_EXTRAS`, `M_NAME`, `M_GARDEN` and `M_FLY`. It is paused everywhere else.
- **Display:**
  - The game uses Mode 4 (8-bit bitmap) with page flipping. Draw through `DRAW_TARGET`.
  - `pset` is a read-modify-write of 16 bits, so it is slow.
  - `YARD_BG` holds the pre-rendered room or yard (from `build_bg`), and `SCRATCH_BG` is the cache for static
    text and bars.
- **Palette map:**
  - `0–39`: the `C_*` UI colours. With the lights off at night the original dims these: `C_GOLD` and `C_GREEN`
    go dark; `C_LTEXT`, `C_LILAC` and `C_ORANGE` stay readable.
  - `40–63`: reset by the original every frame, and borrowed by food (`M_EAT`), stories and the garden.
  - `120–143`: the critter.
  - `144–207`: room and yard colours (`BGX`).
  - `208–243`: the decor bands.
  - `244–254`: the garden. `garden_palette_on()` decides when it's loaded.
  - `255`: the ember firefly. In the room, `244–255` carry the firefly colours (`fly_palette`).
- **Save:** magic `CKS8`, version 15, 11 parts (part 10 is the Garden). `More.spare` is used as follows:
  - `[0]`: the night of the last wish (day+1)
  - `[1]`: the night of the last story
  - `[2]`: a story's bond, waiting for the morning
  - `[3]`: the firefly kinds found (bits)
  - `[4]`: fireflies caught in all
  - `[5]`: the best night
  - `[6]`: tonight's marker (day+1)
  - `[7]`: rounds used tonight (bits 0–1), plus tonight's kinds shifted left by 4
- **Generators → headers:**
  - `bgfx.py` → `bgfx.h` (the room and yard pictures, including the tree trunk)
  - `garden_art.py` + `gardengen.py` → `garden.h`
  - `critters.py`, `newforms.py`, `limbs.py` and `critgen.py` → `critters.h`
  - `decor.py` / `decorgen.py` → `decor.h`
  - `icons.py` / `icongen.py` → `icons.h`
  - `baricons.py` → `baricons.h`
  - `title.py` → `title.h`
  - `minis.py` → `minis.h`
  - `tunes.py` → `tunes.h`
  - `picture.py` → `picture.h`
  - `foodart.py` → `foodart.h`

## v8.1 design decisions (already approved and built)

- **Yard:** the three moonflower patches sit at the "B5c" spots: `PATCH_X = {80,132,152}`,
  `PATCH_Y = {110,104,127}`. They are drawn before (behind) the decor, and the critter stands in front of them.
  The garden opens in look mode: A tends (the cursor shows), B puts the cursor away, and B again goes inside.
  SELECT over the night yard opens a menu with only GO INSIDE and JOURNAL.
- **Fireflies:**
  - R in the night yard starts a 30 s round, with 3 rounds a night.
  - There are 12 kinds: 3 common, 2 of 5 uncommon each night, and 4 rare. Moonglow needs a full moon, Rose Heart
    a rose in bloom, Wish Spark a wish made that night, and Starfly about 1 night in 10 with 2+ blooms.
  - Rare kinds are decided at nightfall, so extra rounds can't farm them.
  - Catching gives no coins and no stats.
  - The night's catch shows as a lantern on the windowsill, and it is released at dawn.
  - Once all 12 are found, they dance at the window every night.
- **Shooting stars:** about 1 night in 3, one star at a random step of the night. The message reads
  "A SHOOTING STAR! A: MAKE A WISH", and A still works for about 3 s after the star has crossed.
- **Stories:** one a night.
- **Journal:** in the SELECT menu, not START. The tab strip scrolls (`jt_first`), and there is a FIREFLIES tab.
- **Minigames:**
  - A 50% checkerboard fade over the room or yard during a round (drawn once in `prerender_mini`).
  - The big words are outlined (`textbig_o`), with two offset copies to stay inside the frame budget.
  - Yard games load the garden palette.
- **Rebalance:**
  - Decor no longer raises happiness; the bed keeps its +1 each morning.
  - Milk gives hunger only, and soap only cleans.
  - The mobile, plane, portrait and clock are kind `DECOR` (`KD`), with no care boost; the portrait keeps its
    bond.
  - A happy rise no longer counts as a play point by itself. That fixed a bias where idle play decor pushed every
    critter toward Fluffbud.
- **Fixes:** no name on the status card before naming, and a darker tree trunk with no green stripe.

## Pinned for later (not started)

1. **Hardware sprites** for small effects: the firefly release (a soft glow that shrinks toward the window, using
   the GBA window feature) and the farewell star rising and fading. Limits: one alpha value is shared by every
   blended sprite, and a sprite is either in front of or behind the whole bitmap layer.
2. **The critter walking in front of floor decor** in the room's corners. The idea is to keep its wandering out
   of a corner that has decor in it.
3. **If happiness feels too hard:** fun decor (the TV, arcade and disco ball) could cheer it up only when its
   happiness is empty.

## Shared docs (on claude.ai; Claude Code can't open them, so ask the owner to paste anything needed)

- Instruction booklet (the source of `MANUAL.md`): https://claude.ai/artifact/VAHgTvT8Lm5um1z1qBmqAV
- Stat list (everything that raises each stat, before and after the rebalance): https://claude.ai/artifact/AYZF3adSpk8rKWEVCc3vuf
- The plan doc, with its "Pinned for later" and "Stat rebalance" sections, is a third claude.ai doc.
