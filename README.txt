Starfall Meadow v8.4 - release (Nov 14, 2026)

What is in this package
- StarfallMeadow_v8.4.gba : the game (plays in any GBA emulator; it saves to 32 KB SRAM). Saves from v8.2 carry on
  (decor moves to its new spots by itself).
- MANUAL.md, CLAUDE.md (notes for working on the game), the sources (ext.c, orig*.c, start.s) and the generators in
  gen/, tests/run_tests.py (228 checks), build.sh / mkrom.py / link.ld (./build.sh builds it)

What v8.4 adds over v8.2 (the work went by the name v8.3 while in testing; there was no v8.3 release)
- Built entirely from our own sources: the original Critter Keep now lives in C (orig.c and the lifted
  orig_update.c, orig_render.c, orig_main.c), so the build needs no original ROM and no patching.
- A faster life: an 11-minute day (8 minutes of day, 3 of night) and about a two-hour life. Hunger and happiness
  drain faster, and the growth targets are a third lower to match.
- Weight in pounds on the status card, with a lightest weight for each stage (5 / 10 / 20 / 30 LB).
- Bond is gone. The status card shows HUNGER and HAPPY. Answering a call gives happy +1 (plain petting purrs),
  and a gift has a surprise inside: a seed, coins, a treat or, rarely, a decor piece.
- The room, rearranged into themed spots: the bed (or beanbag) under the window, with the toy box at its foot; the
  fireplace, TV (on a low cabinet), radio (on a side table), arcade or piano under the picture; a new corner for the
  plant or the fishbowl (on its own table); the wall; and the ceiling. Pieces stand against the back wall with soft
  shadows. Nestle needs all five room spots.
- The firefly jar has its own stand, which comes with a critter's first firefly. At dawn the jar floats to the
  window for the release and back.
- The room and the yard glow orange at sunset and lavender at dawn.
- Poops in the room sit on the floor in 2.5D, clear of the decor.
- Night-yard comforts: START and SELECT work out there (BAG, SHOP, CLEAN UP when the yard needs it, JOURNAL), a
  firefly round ends back in the yard, and fireflies drift over the night garden.
- The SELECT menu shows only what works. A skips the rest of a meal or snack.
- Minigames: much smoother (Hop Hop runs at full speed again). A round pays coins by score, and AMAZING! or better
  adds the game's own boost (Pancake Tower hunger +1, Left or Right? happy +1; Hop Hop's weight -1 every round).
  No bonus coins.
- The journal: tabs wrap round (arrows at both ends), new icons for the redesigned decor, corrected descriptions.
- Smaller things: lights off by day makes it unhappy, the sick symbol shows while it sleeps, secret forms wait
  for the 4th grown-up day, new rules for Sunpuff, Glimmer and Zippit, black cuts into and out of the title.
- Switched off for now, with their code kept for later: the bedtime stories and the Stargazer minigame.
- Save version unchanged (15).

----------------------------------------------------------------------------------------------------------------
Starfall Meadow v8.2 - release (Nov 12, 2026)

What is in this package
- StarfallMeadow_v8.2.gba : the game (plays in any GBA emulator; it saves to 32 KB SRAM). Saves from v8.1 carry on.
- MANUAL.md, CLAUDE.md (notes for working on the game), ext.c and the generators in gen/, tests/run_tests.py
  (202 checks), build.sh / patch.py / link.ld (./build.sh builds it)

What v8.2 adds over v8.1
- Hardware sprites (the game's first): small glowing effects drawn by the GBA itself over the picture.
- The firefly release: in the morning the window swings open, the cork pops, and the night's fireflies float out
  as soft glowing lights, shrinking and fading into the sky (the GBA's window keeps them inside the open window);
  then the window shuts.
- The farewell star: a light gathers on the critter as it rises, stays sharp through the white-out, glides into
  its place in the night sky and then gently breathes and turns. A sad goodbye leaves a small, faint star.
- The jar on the windowsill glows softly at night (no more specks flying round it).
- v8.2b fixes and changes (Nov 12):
  - The yard keeps its own mess: the critter poops outside too, and CLEAN works there.
  - Moonflowers keep their colours on every yard screen (menus, feeding, petting).
  - Save and quit in the yard: CONTINUE goes back to the yard. LIGHTS outside says "YOU'RE OUTSIDE!".
  - The pinwheel, sprinkler and frisbee moved off the front moonflower patch.
  - Plant a seed, then water it: a watering can pours onto the patch and the soil darkens.
  - Rare moonflowers: Moon Lily 1 in 3, Starbloom 1 in 2, Twinglow 1 in 5 (Aurora still 1 in 20).
  - Fireflies: 20-second rounds, a quicker jar, and a faintly glowing firefly can be scooped. The totals moved
    to the journal's RECORDS tab.
  - A shorter life: about 8 days as a grown-up (was 12); the shop still opens all its levels.
  - The journal's moonflowers stand on their soil.
- v8.2c (Nov 12):
  - A new runaway scene: at dawn it packs its bindle, looks back at the house one last time, walks out across the
    meadow and finds a new home by the stream, helping a wild one of its own kind look after two little Bliplets; that night its star's place stays
    empty. New painted meadow and stream (gen/meadow.py).
  - Poops in the yard sit at free spots (never on the house, decor, toys or flowers), smaller further back.
  - The journal's RECORDS tab scrolls; the moonflower pictures stand on their soil.
- No changes to prices; the stat changes above were asked for. Save version unchanged (15).
- Tools: gen/sprfx.py (sprite art -> sprfx.h), tools/mgba_shot.c (screenshots on the real mGBA core), and 11 new
  `sprites` tests (186 checks in all).

----------------------------------------------------------------------------------------------------------------
Starfall Meadow v8.1 - release (Nov 10, 2026)

What is in this package
- StarfallMeadow_v8.1.gba : the game (plays in any GBA emulator; it saves to 32 KB SRAM)
- MANUAL.md          : the instruction booklet (the same text as the shared booklet doc)
- ext.c              : the extension patched onto the original ROM (all of the game logic and screens)
- *.py               : the art, music and table generators (see the notes below for each one)
- tests/run_tests.py : the test suite, 175 checks (run it with the test emulator, test_emulator.c)
- build.sh / patch.py / link.ld : build the ROM (paths assume /home/claude/ck and /home/claude/rom/orig.gba)
To continue work later: upload this zip in the chat.

What v8.1 adds over v8
- Riddles: every secret form and rare moonflower has a riddle in the journal until found (answers at the back
  of the manual). The journal gets a COLLECTION of every item, clearer bonus wording and shorter tab names.
- Wishing stars: about one night in three a single shooting star crosses the sky; "A SHOOTING STAR! A: MAKE A
  WISH", and A works for about 3 s after it has crossed (+5 coins).
- Bedtime stories: three six-page stories of the meadow, one a night (+1 bond in the morning).
- The moonflower garden (gardengen.py / garden_art.py -> garden.h): three patches spread through the yard behind
  the decor, nine kinds of bloom with care-picked colours and four rare forms, ten night visitors. It opens just
  for looking; A tends, B puts the cursor away. The clock keeps running out there.
- Fireflies: from the night yard, R starts a 30 s round (three a night). Twelve kinds to find for the journal's
  FIREFLIES field guide; the night's catch glows on the windowsill and flies home in the morning; all twelve
  found, they dance at the window every night. No coins or stats: a collection.
- Nights: 4.5 minutes (the clock moves 2 steps in 3 at night; the critter's own needs keep their pace).
- Journal: in the SELECT menu now (also over the night yard); the tab strip scrolls.
- Health: explained in the manual; the critter says when it looks weak.
- Minigames: the room or yard is faded behind a round; big words outlined; yard flowers keep their colours.
- Stat rebalance: decor no longer raises happiness (the bed's happy morning stays), milk and soap no longer do,
  the mobile, plane, portrait and clock are DECOR (just to look at), and a happy rise no longer counts as play by
  itself. About 8-10 ways now raise each stat (see the stat list doc).
- Fixes: no name on the status card before naming; a darker tree trunk with no green stripe.
- Save version 15 (CKS8); older saves convert on first load.

Pinned for later (see the plan doc): hardware sprites for small effects (firefly release, farewell star), and
keeping the critter's wandering out of decorated room corners.

----------------------------------------------------------------------------------------------------------------
Starfall Meadow v8 - release (Nov 9, 2026)
(formerly Critter Keep; renamed as the final change before release)

What is in this package
- StarfallMeadow_v8.gba : the game (plays in any GBA emulator; it saves to 32 KB SRAM)
- MANUAL.md          : the instruction booklet (the same text as the shared booklet doc)
- ext.c              : the extension patched onto the original ROM (all of v8's game logic and screens)
- *.py               : the art, music and table generators (see the notes below for each one)
- tests/run_tests.py : the test suite (run it with the test emulator, test_emulator.c)
- build.sh / patch.py / link.ld : build the ROM (paths assume /home/claude/ck and /home/claude/rom/orig.gba)

What v8 adds over v7
- The name: STARFALL MEADOW, with a new title logo (gen/title.py) and header title STARFALL; the game code stays CKPE.
- Care and growth: five kinds of care, a teen stage, grown-ups chosen by the care they got most, one secret form each.
- Art: redrawn critters with a boy/girl touch, a star-seed egg, a painted title, room, yard and decor.
- The world: decor and toys tied to kinds of care; each critter opens its own shop as it grows.
- Four minigames (Pancake Tower, Hop Hop, Left or Right?, Stargazer); coins by rating (2 / 8 / 15).
- Naming at hatch, a hidden health stat, hardware fades, mosaic and zoom.
- The journal (night sky, field guide, records), one farewell where every critter rises to become a star.
- Save version 13 (two copies at 0x1000 / 0x2000); older saves convert on first load.

Development notes (oldest first)
Source package notes from v7:
- CritterKeep_original.gba : the original ROM the extension is patched onto
- ext.c      : the extension (menus, yard, shop, bag, decor, sky, secret forms, cries, music, life cycle, minigames)
- tunes.py   : music composer script -> generates tunes.h (note tables for the wave channel)
- link.ld    : linker script (code placed at 0x08020000)
- patch.py   : patches the original ROM's hooks/tables and appends the extension
- build.sh   : clang + ld.lld build (paths assume /home/claude/ck and /home/claude/rom/orig.gba)
- test_emulator.c : small GBA emulator used for testing
To continue work later: upload this zip in the chat.
- picture.py : wall picture converter (picture_src.jpg -> picture.h)
- preview.py : renders the soundtrack to WAV/MP3 the way the GBA plays it
- bgfx.py : builds the upgraded room/yard backgrounds (bg/ -> bgfx.h)
- critters.py : draws the 13 critter forms x 6 poses (idle, blink, happy, sleep, eat, sad) at any size
- critgen.py : renders them at 16/32/48/64 px with per-form palettes -> critters.h
- decor.py / decorgen.py : shaded decor pieces and yard toys, each on its own 12-colour palette band -> decor.h
- icons.py / icongen.py : shop icons (shared 112-colour palette, loaded while the shop is open) and the 20px food art -> icons.h
- baricons.py : the 8 menu icons at full 24x24 plus the status hearts and sick skull -> baricons.h (bgfx.py bakes the idle icons into the bars)
- title.py : the painted title screen and the farewell "star world" (shared 112-colour palette, 4 twinkling entries) -> title.h
- minis.py : stars, berries, rocks, arrows, balloons and the bindle used by the minigames and farewells -> minis.h
- volume.py : soft-depth shading with contour lines between body parts, used by critters.py

Limbs and animation (v7, step 5)
- limbs.py adds arms and legs to the critter forms; critters.py now has 14 poses:
  idle, blink, happy, sleep, eat, sad, walk1, walk2, breathe, sleep2, love, wave1, wave2, stretch.
  Run critgen.py to rebuild critters.h.
- ext.c picks the pose: breathing when idle, stepping while the game moves the critter,
  waving when it calls you, heart eyes when praised, a stretch and yawn when it wakes up,
  waving goodbye in the farewell scenes, and walking/running in the catch and hop games.
- Facing: critgen.py also stores a 48px copy of every pose facing right (CR_PIXOFF[..][4]). It is drawn
  lit from the top-right and then mirrored, so the light stays top-left whichever way the critter faces.

Growth tree, bestiary, title (v7, step 6)
- newforms.py (exec'd by critters.py) draws the new forms: BLOOMLE/THORNLE (teens), OAKHART, DUNEROSE,
  THORNHOG, SMOGWOOL, GUNKBILL and PETALAMB (forms 13..20). Forms 0..12 keep their numbers so old albums still work.
- ext.c: care is sampled from baby to teen (Dex block, SRAM 0x0E000280). When the original grows a Sproutle up,
  it becomes a teen picked by care (7 in 10 good checks = BLOOMLE), and after two days grows into a grown-up:
  good side OAKHART (10+ min outside) / DUNEROSE (PLANT inside + FLOWERS or GARDEN outside) / FLUFFBUD (4+ games) / FRILLO;
  less-care side THORNHOG (sad a lot) / SMOGWOOL (lived in a mess) / GUNKBILL. Secret forms: good side SUNPUFF, ZIPPIT,
  GLIMMER, NESTLE, STARWING; less-care side PETALAMB (a SMOGWOOL kept clean for two days). Grown-ups from older saves
  keep the old rules (including CHONKLE, whose code stays).
- Bestiary (title screen and pause menu), boy/girl per egg, egg rocks gently, fuller title scene (title.py).

v8 rework, phase 1: foundation
- The game clock now runs in the extension (ext.c, "GAME CLOCK (v8)": sim_frame / sim_tick / sim_grow).
  It is a faithful port of the original's once-a-tick simulation (hunger, happiness, poop, calls, sleep and
  morning, sickness, health, running away), using the same record fields, timers, odds and random numbers, so
  the original's screens and buttons keep working. patch.py turns the original's own clock block into a branch
  past it (0x080002a6: b 0x0800096c).
- Growing up is decided here too: baby -> child after 240 steps, child -> teen after 600 (its side picked by care),
  teen -> grown-up after two days, now with the original's growing-up scene ("X GREW INTO A Y!"). Grown-ups from
  before v7 keep the original's second growth step.
- FAST mode is 2.5x (a clock step every 36 frames instead of 90); timers counted in frames use fastdiv().
- One save: Ext, Life, Deco, Album and Dex are written as one versioned block ("CKS8", version 8) with a checksum,
  alternating between two copies at SRAM 0x0E000400 and 0x0E000600. The newest valid copy loads; a v7 save
  (five separate blocks) is converted the first time it loads.
- Forms are one table (FORMS[]: name, side, stage, cry, bestiary text).
- tests/run_tests.py: regression tests run in the test emulator - clock fidelity against tests/baseline_v7.gba
  (byte-for-byte), every growth outcome, Petalamb, save conversion and torn-save fallback, FAST timing, fuzz runs
  and the frame budget. Usage: python3 tests/run_tests.py [fidelity growth secrets save fast fuzz budget]
- thornhog_secret_mock.py: mockup of three options for Thornhog's secret form (not in the game yet).

v8 rework, phase 2: care and growth
- One care record (ext.c "Care"): every care check (15 s, 6 on FAST) looks at FOOD (hungry or overfed = missing),
  PLAY (1 or fewer HAPPY hearts = missing), CLEAN (poop = missing), REST (sick, or lights on while asleep = missing)
  and OUTSIDE (yard time, plus garden points: a PLANT inside, FLOWERS or a GARDEN outside). Two windows: growing up
  (picks the teen and the grown-up) and grown-up (secret form and farewell). It replaces the old dex/life counters.
- Teen: 7 in 10 all-round good checks = BLOOMLE, as before.
- Grown-up: good side = whichever is furthest past its target (yard 40 checks / garden 120 points / play 16 points:
  +1 each time it gets happier, +4 a minigame) -> OAKHART / DUNEROSE / FLUFFBUD, none reached = FRILLO.
  Less-care side = the need missed most (play vs 30%, clean vs 25%, food vs 25% of checks) -> THORNHOG / SMOGWOOL / GUNKBILL.
- Secrets, one per grown-up: OAKHART->SUNPUFF (60 yard checks), DUNEROSE->GLIMMER (120 garden points + full bond),
  FLUFFBUD->ZIPPIT (6 WHAT A STAR!), FRILLO->NESTLE (4 room decor spots), any good-care->STARWING (9 in 10 good),
  SMOGWOOL->PETALAMB, GUNKBILL->LILYBILL, THORNHOG->JAMBLE (that need not missed for a minute at a time, two days).
- New forms 21 LILYBILL and 22 JAMBLE (newforms.py; rebuild critters.h with critgen.py).
- Status card: the corner shows the care it gets most (heart: OUTSIDE / PLANTS / PLAY / BALANCED) and the need it
  misses most (! FOOD / PLAY / CLEAN / REST). Bestiary has 20 forms.
- Save version 9 adds the care record as a sixth part; version 8 saves (phase 1) and v7 saves convert on load.

v8 rework, phase 3: roster art
- v8forms.py (exec'd by critters.py after newforms.py) redraws EGG (star-seed), FLUFFBUD, FRILLO, ZIPPIT, NESTLE,
  SUNPUFF, GLIMMER and STARWING (the original star, updated), and adds the boy/girl touches: the boy touch is baked
  into each form, the girl version is an extra form NAME~G rendered after the 23 base forms (critters.h CR_GIRL maps
  a form to its girl version; ext.c pet_variant() picks it). Girls cry a little higher, boys a little lower.
  Mockups: roster_v8_mock.py, gender_mock.py.

v8 rework, phase 4: world
- Every item belongs to one kind of care (ITEM_KIND in ext.c: FOOD, PLAY, CLEAN, REST, OUTSIDE), shown in the shop
  next to its name and in the bag in front of what an item does. Using a piece of decor or a yard toy boosts its kind
  (decor_boost): PLAY counts as play care, REST restores health, CLEAN resets the messy-room sickness timer, FOOD gives
  a HUNGER heart, OUTSIDE adds garden points.
- At night the room window shows a star for each critter that rose to the sky (crit_stars, drawn in sky colours so it
  stays bright: gold good care, amber less care, moon-white secret forms; the newest twinkles).

v8 rework, phase 5: activities and economy
- PLAY opens a game picker (ext.c PH_PICK; UP/DOWN, A plays, B back; it opens on the last game played). Four games,
  each with a kind of care (GAME_KIND) and a good-round boost: PANCAKE TOWER (food, +1 HUNGER; replaces BERRY CATCH),
  HOP HOP (play; the only game that trims weight), LEFT OR RIGHT? (play, +1 extra HAPPY) and STARGAZER (rest, +20
  health). Ratings: RATE1/RATE2, perfect scores: PERFECT. Two clean games (BUBBLE BATH, DUST BUNNIES) were tried and
  dropped after playtests.
- PANCAKE TOWER: slider over the stack, overhang trimmed into falling chunks (eaten on the critter's side), within
  2 px = PERFECT (no trim), 15 pancakes max, best stack in Keep.best (per life).
- STARGAZER: Simon-style, 4 stars on the D-pad, tune length 2..6 over 5 rounds, 4 s to answer; night sky prerendered.
- New tune MINI_STARS (tunes.py, appended to the tune list).
- Status card: HEALTH bar (0-100) under BOND (card_health).
- Shop: each critter unlocks its own shop (bits in Keep, wiped by new_unlock_seed at every new egg). Each growth level
  opens UNLOCK_ADD pieces (4,4,5,5,5,4 = 27) from those not left out this life (left_out: 6 of 33).
- Save version 10 adds Keep as a seventh part; a v9 save's unlocks are rebuilt from its old per-life order.
- tests: games, health and unlocks added (baseline_v8p4.gba = the phase 4 ROM, for the v9 -> v10 conversion).
- Mockups: phase5_mock.py, food_mock.py, tidy_mock.py, clean2_mock.py (clean-game ideas, not used).

v8 phase 5 fixes (after the playtest)
- Eating: the bag foods and the FEED icon's MEAL/SNACK are drawn from 28-36 px art with 24 colours each (gen/foodart.py
  -> foodart.h), loaded into palette 40-63 while eating (dimmed at night). The original blacks those out every vblank,
  so hook_update puts them back straight away. patch.py blanks the original's 8x8 MEAL/SNACK eating art.
- Music keeps time by timer 3 (16.78 MHz / 1024) instead of by frames drawn, so slow screens no longer stretch notes.
- Health: the status card bar is gone; health still drains/recovers as before (hidden).
- Naming: the hatch box asks LET'S GIVE IT A NAME!; A opens the naming screen (M_NAME, 6 letters, a suggested name
  from NAME_IDEAS, SELECT = another). Its background is built a piece per frame so no frame runs long.
  The name lives in Names.cur and is patched into RAM_NAMES for the original's own texts; the star album keeps each
  critter's name (Names.alb). Save version 11 adds Names as an eighth part.
- Room: the sunbeam/moonbeam (gen/bgfx.py) is the window opening slid down and right, so it starts at the glass.

v8 phase 6, chunk 1 (speed-up, title, transitions)
- Speed-up: the full-screen copies (O_DMACOPY) were already DMA3, so there was nothing to gain there. The critter's
  palette writes now happen at the start of the frame (flush_crit_palette, called first in hook_update).
- Hardware effects (ext.c "HARDWARE EFFECTS"): fx_fade (BLDCNT brighten/darken + BLDY), fx_mosaic (MOSAIC + BG2CNT
  bit 6), fx_shake (BG2X/BG2Y). fx_watch starts them on changes: title fade-in, into the game, room <-> yard, entering
  or leaving the games/album/journal, lights off/on (eased), hatch (white flash, mosaic, shake), evolving (long white
  flash, mosaic). fx_tick clears the registers when idle; the original never touches them.
- Title: up to 12 past critters shine as stars in their own colours (FORM_STAR, TSTAR), twinkling; a shooting star
  every 7 s; a glowing star-seed for the first few seconds. Menu: CONTINUE / NEW SEED / JOURNAL (JOURNAL opens the
  bestiary until the journal arrives in chunk 2).
- test_emulator.c renders BG2 affine, mosaic and brighten/darken now.
- PANCAKE TOWER no longer shows a BEST counter (a full tower is 14, so it never moved); Keep.best is still kept.

v8 phase 6, chunk 2 (the journal and the bigger save)
- Save version 12 adds Journal as a ninth part and moves the two copies to 0x1000 / 0x2000 (4 KB each; the ROM is SRAM_V,
  32 KB). load_all reads the new copies first, then an older save from 0x400 / 0x600; journal_from_album turns an older
  star album into the first stars (gender and days not known).
- Journal: a Story (name, form, gender, ending, days together, care it got most) for each of the last 40 farewells
  (journal_end, from start_sendoff); a critter that runs away (stage 8) is counted in CRITTERS RAISED but leaves no star.
  Records: raised, longest life, minigame rounds (GAMES PLAYED replaces the best pancake stack), coins earned (add_coins),
  and how often each kind of care was loved most.
- The journal screen is M_DEX with three tabs (G.jtab, L/R): NIGHT SKY (10 stars a page, joined in order, bright /
  normal / dim for wonderful / happy / sad; LEFT/RIGHT picks a star and shows its story), FIELD GUIDE (the bestiary;
  A turns a met form's page between its lore and HOW IT GROWS, with its secret's name once found) and RECORDS.
  Title JOURNAL opens the night sky (field guide if there are no stars yet); the START menu's JOURNAL opens the field
  guide. The title's SELECT star album is gone; the title sky now shows the journal's latest 12 stars.
- NEW SEED keeps the journal ("YOUR JOURNAL IS KEPT").
- Journal frames use dma_fill and quick_box (about 180k-250k cycles a frame).
- tests: journal (v11 album -> stars, farewell story, runaway, tabs); baseline_v8p6c1.gba is the last v11 ROM.

v8 phase 6, chunk 3 (farewells and the star-seed story)
- RECORDS: WONDERFUL FAREWELLS (Journal.wonders, every wonderful ending ever) replaces LONGEST LIFE, which was about
  15-16 days for every critter that reached its farewell. Save version 13: a v12 save counts it from its stars.
- One farewell for every form (rise_draw / rise_fx), except SUNPUFF, CHONKLE, ZIPPIT, GLIMMER and NESTLE, which keep
  their scenes (so_unified). It glows (a see-through halo), a beam of starlight comes down and it rises; from t=400
  the hardware mosaic (G.mos_fix) and a white-out (BLDY) build up; at t=560 the star world appears, the white fades
  and BG2's affine scale (G.zoom_t, BG2PA/PD from 0xA0 to 0x100) zooms out from its new star at (120,58), with the
  journal's earlier stars around it. Wording for wonderful / happy / sad; a sad goodbye rises at night, faint.
  The lights are switched on for the farewell (the original dims the low colours with them off). The yard and sky
  colours for it are set in farewell_palette, at the start of the frame.
- A critter that runs away (health ran out, scene 9) gets our own scene instead of the original's home star:
  runaway_draw, a rainy night, it walks off, IT LEFT NO STAR BEHIND, PRESS START FOR A NEW SEED.
- Story text: A STAR-SEED FELL IN THE MEADOW!, A: NEW SEED, help pages (rises to become a star; runs away with no
  star), the summary card (WONDERFUL IN A ROW), the window stars come from the journal (sad ones faint).
- The title's old star count and SELECT: ALBUM line are gone.
- tests: journal adds the v12 -> v13 count and the farewell's mosaic / white-out / zoom registers.

v8 phase 7, balance (chunk 1, hardware sprites, was dropped: the room and yard already run inside every frame)
- Care: a care check counts hunger and happiness as missing only at 0, when the critter calls for them (they needed
  2+ before, so a player who answered every call within 15 s scored about 50%). Modelled on the care rules: answering
  within 15 s now scores about 90%, 30 s about 85%, 1 minute about 73%, 1.5 minutes about 61%.
- Coins: a perfect round pays about a coin for every second it takes (GAME_COINS): PANCAKE TOWER 20, HOP HOP 15,
  LEFT OR RIGHT? 15, STARGAZER 30; the picker shows it (PERFECT: N COINS).
- HOP HOP: a round is HOP_N = 12 hurdles and berries (it ends when the last has gone by), so clearing them all is a
  perfect 12; the top bar shows how many are LEFT. Ratings 9 / 5.
- test_emulator.c: 'carebot' (fixes whatever shows, after a delay) and 'cyc' reports the slowest frame; 'auto' plays
  HOP HOP's berries too. tests: care at 1 counts as good; HOP HOP perfect round pays 15.
- Coins (revised): every game pays by the round's rating (RATING_COINS): BETTER LUCK 2, NICE 8, AMAZING 15.
  The picker shows NICE: 8   AMAZING: 15.

v8 phase 7, sound
- New jingles (Note sequences on the square channels): SEQ_JOURNAL when the journal opens, SEQ_BEAM when the
  farewell's beam comes down (t=220), SEQ_NEWSTAR / SEQ_FAINTSTAR when its star appears (sad goodbyes get the faint
  one), SEQ_RUNAWAY when a runaway turns and walks off. Music and sound-effect volumes are unchanged.

v8 phase 7, text pass (every line reviewed in the "every in-game line" doc)
- Help: START: MENU (SPEED, SOUND, JOURNAL); ONCE IT'S A CHILD, IT CAN GO OUT.
- Field guide: the EGG has a how-it-grows line (FALLS WITH A SHOOTING STAR ON CLEAR NIGHTS); THORNHOG's lore rewrapped.
- Farewells: CHONKLE ends UP AND UP IT GOES, ALL THE WAY TO THE STARS!; NESTLE ends AND A NEW STAR SHINES RIGHT ABOVE IT.

v8.1, chunk A (playtest fixes)
- Coins by score, per game (COIN_PANCAKE / COIN_HOP / COIN_GUESS / COIN_STARS), and a result for each score
  (RES_*): BETTER LUCK, NICE!, AMAZING!, PERFECT! (the top score; 4 stars on the results). The picker shows the
  TOP PRIZE. Bonuses: NICE! or better gives the game's boost, AMAZING! or better +1 bond and counts for ZIPPIT.
- HOP HOP: a rock that touches the critter while it still flashes from a hit is a miss, so standing still scores 0.
- Age in years: the status card's " DAY"/" DAYS" are repointed to " YEAR"/" YEARS" (patch.py); the farewell summary
  shows AGE, the journal's stories give years (both from the status card's age counter, PET+0x18); help page 5.
- Status card: the ! names what went wrong in the last day or so (HUNGRY, UNHAPPY, MESSY, SICK, LIGHTS ON), from
  More.rn / More.rmiss, kept per in-game day. Save version 14 adds More as a tenth part (also the collection and
  the night's extras, for chunk B).
- A yuck face (pose 'yuck' in critters.py: squeezed eyes and a tongue) for medicine, sick or not.
