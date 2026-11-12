# Porting the original game: the map (stage 1)

The goal (pinned Nov 12 2026): bring the original Critter Keep (`CritterKeep_original.gba`, 70,912 bytes) into our own
source, so the game builds from C alone, with no binary patching. This file is the map of the original that
the later stages work from.

The tools and files:
- `tools/disasm.py`: the disassembler. It writes `port/orig.lst` (a full listing) and `port/funcs.json`.
  It needs `pip install capstone`.
- `tools/extract_orig.py`: extracts the data into `gen/orig_data.py`. Running `python3 gen/orig_data.py` rebuilds
  the data and checks it is **byte-identical** to the original.
- `tools/lockstep.py`: the comparison harness (see below).

## The ROM, region by region

| Address | Size | What |
|---|---|---|
| `0x08000000-0x080000BF` | 192 B | Cartridge header: Nintendo logo, title `CRITTERKEEP`, code `CKPE`. `patch.py` renames the title to STARFALL. |
| `0x080000C0-0x08000133` | 116 B | ARM startup code (crt0). It sets the IRQ, SVC and user stacks (`0x03007FA0`, `0x03007FE0`, `0x03007F00`), copies `.data` (16 B) to `0x03000000`, clears `.bss` (`0x03000010-0x03000487`), installs the IRQ handler and jumps to `main` (Thumb). |
| `0x08000134-0x08000177` | 68 B | ARM IRQ handler. It acknowledges IE/IF and, on vblank, does `++*(u32*)0x03000480` (the vblank counter `main` waits on). |
| `0x080001AC-0x0800FB19` | 63 KB | Thumb code: 24 functions plus 3 that are never called. About 29,700 instructions, 11 switch tables and the literal pools. |
| `0x0800FB1A-0x0801149F` | 6.5 KB | Read-only data: 133 strings (messages and the pixel art), 98 pointers, palettes, the font and tables. All of it is in `gen/orig_data.py`. |
| `0x080114A0-0x080114AF` | 16 B | `.data`: the initial values for `0x03000000`. They are the RNG seed `0x4567D4D4`, then 120, 120, 60 for the title timers. |
| `0x080114B0-0x08011500` | 80 B | `0xFF` padding. The save-type tag `SRAM_V113` sits just before `.data`, at `0x08011494`. |

The compiler was Clang (Thumb, armv4t) with little optimisation. Every function sets up `r7` as a frame pointer, the
big ones keep their locals in a very large stack frame, and it uses `bl` as a long jump inside a function (so a `bl`
target that isn't a function start is a jump). Switches come in four forms (word, byte, halfword, and halfword via
`adr`), and returns are `pop {rX}; bx rX`.

## The functions

Sizes in bytes. "Calls in" counts the call sites in the original's code; our hooks and `ext.c` also call some of these.

| Address | Size | Name (ours) | Calls in | What it does | Port effort |
|---|---|---|---|---|---|
| `0x0800F714` | 326 | `main` | none | Sets up the display (Mode 4, BG2) and the IRQ, loads the save, then loops: wait for vblank, rebuild the palette (`0x0800CC94`) and copy 0x80 bytes to `0x05000000`, flip the page, read the keys, then call `game_update` 1–4 times (more on FAST) and `game_render` once. **Patched:** its 7 calls to `game_update` and 2 to `game_render` go to our hooks. | small |
| `0x080001AC` | 7,560 | `game_update` | 7 (main) | **The per-frame game logic.** A 10-way switch on `SCENE` (table `0x08000978`) plus the clock block (skipped by `patch.py` since v8, as `sim_tick` runs the clock), icon actions (switch `0x08001690`), hatch and evolve, messages and the runaway. 4 more switch tables. | **large** |
| `0x08001FB4` | 44,224 | `game_render` | 2 (main) | **All the drawing**, one huge function: a 10-way switch on `SCENE` (`0x08001FDC`) into the title, room, feeding, status card, help, minigames and the end scenes. It has a ~250-byte stack frame and calls the draw helpers about 300 times. 3 switch tables (`0x08006E64`, `0x080070E4`, `0x08000334`). | **largest** |
| `0x0800CC94` | 426 | `build_palette` | 2 | Builds the 256-colour palette for the scene and time of day (it dims at night with the lights off), from the tables at `0x0800FB1C` and `0x0800FB6C`. Switch at `0x0800CD6C` (33 cases). | medium |
| `0x0800CE5C` | 2,086 | `game_init` | 1 (main) | Startup: loads the save (`save_read`), checks the `PET` magic `CKP1` (if missing, it starts fresh with seed `0x5EED`), resets the title state, pre-draws two page buffers (`0x02000000`, `0x02009600`) with `0x0800D690`, fills the third with colour 9 and scatters points with an LCG (`0x035A4E91`). Uses the font and "CRITTER". | medium |
| `0x0800D690` | 4,432 | `prerender` | 2 | Pre-draws a full page into the buffer it's given (`game_init` calls it for the first two buffers), using the pixel art at `0x08010213` and colour 2 fills. Likely the title or the room background (to confirm in stage 2). | medium |
| `0x0800E840` | 16 | `rng_mix` | 1 | `rng ^= x; if (rng < 2) rng = 1`, on `0x03000000`. | trivial |
| `0x0800E854` | 110 | `set_message` | 2 | Copies a string (up to 38 chars) to `MSG_BUF` (`0x030002A4`) and sets `MSG_TIMER = 100`. | trivial |
| `0x0800E8F0` | 798 | `minigame_start` | 1 | Sets up a minigame round ("LEFT/RIGHT: CATCH BERRIES!" and the rest), using the minigame block at `0x030002DC-0x03000358`. | small |
| `0x0800EC74` | 314 | `draw_text` (`O_TEXT`) | 3 | Draws text with the 8x8 font (`0x0800FC50`, 448 B) into `DRAW_TARGET`. | small |
| `0x0800EDB8` | 486 | `draw_specks` | 2 | Something animated over a 240x150 area, timed by `FRAMES>>4` (likely sparkles or stars; to confirm in stage 2). | small |
| `0x0800EFB4` | 188 | `draw_box` (`O_BOX`) | 6 | A filled box with a frame. | trivial |
| `0x0800F070` | 552 | `draw_sprite` (`O_SPRITE`) | 19 | Draws a pixel-art string picture (one char = one pixel, colour from `CMAP` at `0x030003B0`), scaled and flipped. **Patched:** all 19 calls go through `hook_sprite` (critters get the new art). | small |
| `0x0800F2A4` | 204 | `hline` | **256** | Fills one horizontal span of pixels (clipped to 240x160), handling the odd/even byte of Mode 4's 16-bit VRAM. Every shape is made of these. | trivial |
| `0x0800F37C` | 624 | `draw_sprite_row` | 3 | Draws up to four small pictures through `draw_sprite`, picked from the descriptors at `0x0800FFE4` and `0x0801001C` (likely the poops or the hearts; to confirm in stage 2). Used by `game_render`. | small |
| `0x0800F5F8` | 18 | `back_buffer` (`O_BACKBUF`) | 1 | The page not on screen (`0x0600A000` or `0x06000000`, from `0x0300047C`). | trivial |
| `0x0800F614` | 12 | `dma_copy` (`O_DMACOPY`) | 1 | DMA3 copy of a full 240x160 page (`0x040000D4`). **Patched:** a call at `0x080047AE` goes to `hook_bg_copy`. | trivial |
| `0x0800F63C` | 120 | `sound_play` (`O_SOUND`) | 7 | Starts a note sequence on a channel (`SND_PTR/CNT/IDX` at `0x03000288+`); the sound registers are at `0x04000062` and up. | small |
| `0x0800F6C0` | 32 | `save_write` (`O_SAVE`) | 3 | Writes bytes to SRAM (`0x0E000000`), behind the `CK` tag. | trivial |
| `0x0800F6E0` | 50 | `save_read` | 1 | Checks for `CK` at `0x0E000000` and reads the save. | trivial |
| `0x0800F98C` | 76 | `__udiv` | 7 | Unsigned divide (shift and subtract). | library |
| `0x0800F9D8` | 92 | `__umod` | 4 | Unsigned remainder. | library |
| `0x0800FA34` | 108 | `__sdiv` | 2 | Signed divide. | library |
| `0x0800FAA0` | 122 | `__smod` | 11 | Signed remainder. | library |

Never called (they don't need porting):
- `0x0800F628` (20 B): reads the keys.
- `0x0800F884` (264 B): memcpy/memset library code.
- `0x080014B8` (84 B): dead code inside `game_update`.

**Totals:** about 52 KB of the 60 KB of code is `game_update` (7.5 KB) and `game_render` (44 KB). Everything else
together is about 8.5 KB, which stage 2 ports.

## Where we hook into it today (`patch.py`)

- `main`: `game_update` at `0x0800F7DC/E6/F0/FA`, `0x0800F804/0A/10` → `hook_update`. `game_render` at
  `0x0800F752` and `0x0800F814` → `hook_render`.
- `0x08007012` (the room critter draw) → `hook_room_critter`. 18 other `draw_sprite` calls → `hook_sprite`.
- `0x080047AE` (the room background copy) → `hook_bg_copy`.
- Literals redirected to RAM copies that `ext.c` keeps up to date:
  - the critter sprite table, at `0x08002D5C`, `0x080031C4` and `0x08007118` → `0x0203D000`
  - the names table, at `0x08001BAC`, `0x08003B90`, `0x080074E0`, `0x08001BC8` and `0x08007A00` → `0x0203D040` (and `+0x28`)
- `0x080002A6`: the clock block in `game_update` is skipped (`b 0x0800096C`).
- The food art literals (`0x08009AE0`, `0x080074C4`) and " DAY"/" DAYS" → our versions.

Each of these becomes an ordinary call or table in our code once the function around it is ported. That retires the
workarounds noted in `CLAUDE.md`: re-ORing `DISPCNT`, the poop swap and hiding, drawing "LB" over "70G", and watching
the original's timers.

## The original's RAM (IWRAM, `0x03000000`)

| Address | What | Our name in `ext.c` |
|---|---|---|
| `0x03000000` | RNG state | `ORIG_RNG` |
| `0x03000004-0C` | Title timers (120, 120, 60) | (reset in `ext.c`) |
| `0x03000010` | Keys last frame | `KEYS_PREV` |
| `0x03000018` | Frame counter | `FRAMES` |
| `0x0300001C` | Message timer | `MSG_TIMER` |
| `0x03000020` | Scene: 0 title, 1 room, 3 eating and the other animations, 4 status card, 5 hatching/naming, 7 help, 9 runaway (the others are still to name) | `SCENE` |
| `0x03000024` | Tick counter | `TICK_CNT` |
| `0x03000028-73` | The `PET` record (magic `CKP1`; stage +4, hunger +6, happy +7, bond +8, weight +9, sick +10, poop +11, asleep +12, lights +13, call +14, mistakes +0x0F/+0x10, meds +0x11, fast +0x12, sound +0x13, best +0x14, health +0x16, counters +0x1C/+0x20, the clock +0x24, the hunger/happy/poop timers +0x28..+0x2E, PET's cooldown +0x40, ...) | `PET`, `P_*` |
| `0x03000074` | Where drawing goes | `DRAW_TARGET` |
| `0x03000078-27F` | Palette work area | |
| `0x03000280-87` | Title: cursor, confirm | `TITLE_CUR` |
| `0x03000288-97` | Sound channels: pointers, counts, indexes; save counter (`0x294`) | `SND_*`, `SAVE_CNT` |
| `0x03000298-A3` | The current animation: kind, time, argument | `ANIM_*` |
| `0x030002A4-CB` | Message text | `MSG_BUF` |
| `0x030002CC-D7` | Runaway timer, the "new egg?" confirm | `GONE_T`, `TITLE_CONFIRM` |
| `0x030002D8` | Icon bar cursor | `ICON_CUR` |
| `0x030002DC-380` | Minigame state | |
| `0x03000381-AB` | Naming text | |
| `0x030003AC` | Help page | `HELP_PAGE` |
| `0x030003B0-42F` | Colour map for the pixel-art letters | `CMAP` |
| `0x03000430-47B` | Save work area | |
| `0x0300047C-487` | The page shown, the vblank count, frame flags | |
| `0x03007000-7FFF` | Stacks | |

EWRAM (`0x02000000`): four 38,400-byte page buffers at `0x02000000`, `0x02009600`, `0x02012C00` and `0x0201C200`
(`YARD_BG` and `SCRATCH_BG` among them). `ext.c` keeps its tables at `0x0203D000` and `State` at `0x0203E000`.

**Stage 2 note:** ported code must keep these exact addresses (as a fixed-address struct, like `G`) until the whole
game is ported, so the original's remaining code and our new code share the same state.

## The comparison harness (`tools/lockstep.py`)

```
python3 tools/lockstep.py A.gba B.gba script.txt save.sav [--frames=N] [--stack]
```

It runs both ROMs in the test emulator on the same script and save. The emulator's new `trail` command records,
every frame:
- a hash of each 256-byte block of IWRAM and each 4 KB block of EWRAM
- the shown page, in 16-row bands
- both palettes

At the first frame where they differ, it re-runs both to that frame, dumps RAM and the screen, and prints the
differing bytes by name (the RAM map above) and the number of differing pixels. The stack is left out unless you pass
`--stack`. A script line `TRAIL n` records n frames at that point, so the inputs can sit between recordings.

It takes about 2 seconds for 600 frames. A build checked against itself is identical. Against the v8.2 release, it
finds the first change, in `ext.c`'s tables, on the first frame.

**How each later stage uses it:** port a function, build, and run lockstep against the build from before the port
over a set of scripts that reach the code (title, room, feeding, every icon, minigames, status, help, hatch, evolve,
the runaway, a night, the farewell). It must stay identical. The stack area is expected to differ, and so are `ext.c`'s
own tables if our code moved.

## Stage 2: the helpers (done)

All 22 called helpers now run from our C (`orig.c`, `orig.h`), linked at `0x08012000`, in the gap before `ext.c`, so
`ext.c` doesn't move. Two hooks in `patch.py` connect them:
- **Call sites:** `redirect()` points every call the original's own code makes to a ported function at ours. There are
  319 of them.
- **Entries:** `tramp()` overwrites the original function's first 16–18 bytes with a jump to ours (`bx pc` / ARM
  `ldr ip, [pc]; bx ip` / address), so other callers reach the port too: `ext.c`'s `O_*` calls and the hooks.

What each one turned out to be (names in `orig.c`):

| Original | Ours | Notes |
|---|---|---|
| `0x0800E840` | `o_rng_mix` | |
| `0x0800F5F8` | `o_back_buffer` | |
| `0x0800F614` | `o_dma_copy` | Too small for an entry jump (12 B); its one caller in the original is redirected, and `hook_bg_copy` replaces the other. |
| `0x0800F6C0`, `0x0800F6E0` | `o_save_write`, `o_save_read` | |
| `0x0800F2A4` | `o_hline` | **VRAM must be written 16 bits at a time:** the compiler turned a high-byte update into a byte store, and on the GBA a byte store to VRAM writes both pixels. The edge pixels use `volatile u16`. |
| `0x0800EFB4` | `o_draw_box` | The message box. |
| `0x0800F98C`, `F9D8`, `FA34`, `FAA0` | `o_udiv`, `o_udivmod`, `o_sdiv`, `o_sdivmod` | The standard ARM divide routines (the ...mod ones return the remainder in `r1`). Dividing by 0 gives all ones, with the dividend as the remainder. Checked against the original with unicorn (`tools/orig_call.py`). |
| `0x0800E854` | `o_set_message` | Also restarts the message chime and switches an awake critter to its talking animation (scene 3). |
| `0x0800EC74` | `o_draw_text` | |
| `0x0800F63C` | `o_sound_play` | |
| `0x0800F070` | `o_draw_sprite` | Has a fast path (even x and scale, fully on screen). |
| `0x0800F37C` | `o_draw_hearts` | The status card's 4-heart meter. |
| `0x0800EDB8` | `o_draw_stars` | 40 twinkling stars (a golden-ratio hash), only drawn on the sky colour. |
| `0x0800CC94` | `o_build_palette` | The lights-off dimming (7/32) and the night tint. |
| `0x0800E8F0` | `o_minigame_start` | BERRY CATCH. The original's hint text is an inlined copy of `draw_text`. |
| `0x0800CE5C` | `o_game_init` | Its "CRITTER" / "KEEP" title letters are four more inlined `draw_text` calls (scale 3). |
| `0x0800D690` | `o_prerender` | It draws the room by day or night and depends on nothing else, so it is kept as the two pictures it draws (`tools/extract_rooms.py` → `gen/orig_rooms.py` → `orig_rooms.h`, 7.3 KB run-length coded), plus the 19 colour-map entries it sets. Its last 8 drawings, the original critter pictures in two rows, go through `draw_sprite`, which the game hooks (`ext.c`'s `hook_sprite` gives critters their new art). So they are recorded as calls, with the colour map at each, and made at run time through the hook. The extracted pictures match the unpatched original byte for byte. |

**Hooked calls stay hooked:** the original's own `draw_sprite` calls (all of them, including those in the heart meter and
`prerender`) go to `ext.c`'s `hook_sprite`. The port calls `hook_sprite` at the same places. Missing that shows up as
different critter or heart pixels.

**Checking:** `tools/port_check.py REF.gba` runs every script in `port/scripts` against 5 saves. The scripts cover the
room, menus, help, minigames, a new egg, the runaway, the title, and 20,000-frame random-input runs; together they reach
every scene, 0–9. The ported build must be **identical** to the reference: all RAM except the stack, the screen, both
palettes, the display and sound registers, and OAM, on every frame. The emulator runs in turbo mode for this (`turbo
1`: no frame runs out of CPU time), so a faster port stays in step with the original frame for frame. The frame-budget
tests check speed separately.

Still read from the original ROM: the ported helpers' data (the font, the palettes, the heart pictures, two tunes,
three strings), at their old addresses (`orig.h`). It moves into our source with the rest of the data in stage 5, when
nothing else reads it there.

## The plan from here

1. **Done (stage 1):** this map, the listing, the data extracted byte-identical, and the harness.
2. **Done (stage 2), see above. Port the helpers:** `rng_mix`, `set_message`, `hline`, `draw_box`, `draw_text`, `draw_sprite`,
   `draw_specks`, `draw_sprite_row`, the sound player, save read/write, the palette builder, `back_buffer`, `dma_copy`,
   the divides, then `game_init`, `minigame_start` and `prerender`. About 8.5 KB of code.

   The original's own calls to these get redirected to ours, with `patch.py` growing a `bl` redirect per call site. Each
   function is checked with lockstep. The data they use moves from the ROM to `gen/orig_data.py` → `orig_data.h`.
3. **Port `game_update`:** scene by scene, following its switch. While it's half done, a scene not yet ported jumps back
   into the original's code for that case.
4. **Port `game_render`:** scene by scene the same way. It's the biggest job, mostly long, straight-line drawing code.
5. **Our own startup:** crt0, the IRQ, `main`, the header. Then `patch.py` and `CritterKeep_original.gba` drop out of
   the build.
