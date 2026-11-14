#!/usr/bin/env python3
"""Critter Keep regression tests (v8 phase 1).

Runs the ROM headless in the test emulator and checks:
  fidelity  - the game clock port matches v7 byte for byte (needs baseline_v7.gba)
  growth    - every teen -> grown-up outcome, and the old pre-v7 rules
  secrets   - a Smogwool kept clean becomes a Petalamb
  save      - a v7 save converts, reloads from the new block, and survives a torn write
  fast      - FAST mode runs the clock at 2.5x
  fuzz      - long random-input runs from several saves never crash
  budget    - room and title stay inside the frame budget

usage: python3 run_tests.py [test ...]
"""
import os, re, struct, subprocess, sys, shutil, tempfile

CK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EMU = os.environ.get('SFM_EMU') or f'{CK}/emu'
ROM = f'{CK}/CritterKeep.gba'
BASE = f'{CK}/tests/baseline_v7.gba' if os.path.exists(f'{CK}/tests/baseline_v7.gba') else f'{CK}/baseline_v7.gba'
BASE8 = f'{CK}/tests/baseline_v8p1.gba'
BASE11 = f'{CK}/tests/baseline_v8p6c1.gba'   # the last version-11 ROM (phase 6, chunk 1)
BASE12 = f'{CK}/tests/baseline_v8p6c2.gba'   # the version-12 ROM (phase 6, chunk 2)
BASE13 = f'{CK}/tests/baseline_v8final.gba'   # the v8 release (save version 13)
BASE14 = f'{CK}/tests/baseline_v81b2.gba'     # v8.1 B1 (save version 14)
CUR_VER = 15
SAVE_AT = (0x1000, 0x2000)                    # v12: the save's two copies
def save_ver(d): return max(d[a + 4] if d[a:a + 4] == b'CKS8' else 0 for a in SAVE_AT)
SAVES = f'{CK}/tests/saves'
TMP = tempfile.mkdtemp(prefix='cktest_')
PET = 0x03000028

# ---------------------------------------------------------------- struct offsets, straight from ext.c
def offsets():
    fields = {'ext': 'State,ext', 'life': 'State,life', 'deco': 'State,deco', 'album': 'State,album', 'dex': 'State,dex',
              'check_special': 'State,check_special', 'save_seq': 'State,save_seq', 'last_clock': 'State,last_clock',
              'e_day': 'Ext,day', 'e_special': 'Ext,special', 'e_adult_day': 'Ext,adult_day', 'e_coins': 'Ext,coins',
              'd_phase': 'Dex,phase', 'd_side': 'Dex,side', 'd_adult': 'Dex,adult', 'd_teen_day': 'Dex,teen_day',
              'd_mess_day': 'Dex,mess_day', 'd_games': 'Dex,games', 'd_yard': 'Dex,yard', 'd_c_good': 'Dex,c_good',
              'd_c_bad': 'Dex,c_bad', 'd_c_sad': 'Dex,c_sad', 'd_c_mess': 'Dex,c_mess', 'k_slot': 'Deco,slot',
              'care': 'State,care', 'c_grow': 'Care,grow', 'c_adult': 'Care,adult', 'c_miss_day': 'Care,miss_day',
              'w_n': 'CareWin,n', 'w_good': 'CareWin,good', 'w_bad': 'CareWin,bad', 'w_miss': 'CareWin,miss',
              'w_yard': 'CareWin,yard', 'w_garden': 'CareWin,garden', 'w_play': 'CareWin,play', 'e_stars': 'Ext,stars',
              'pick': 'State,pick', 'tune_pos': 'State,tune_pos', 'mode': 'State,mode', 'phase': 'State,phase', 'game': 'State,game', 'score': 'State,score',
              'timer': 'State,timer', 'px': 'State,px', 'py': 'State,py', 'spawn': 'State,spawn', 'lastv': 'State,lastv',
              'obj': 'State,obj', 'st_seq': 'State,st_seq', 'tune': 'State,tune', 'rating': 'State,rating', 'pk_w': 'State,pk_w',
              'keep': 'State,keep', 'k_un0': 'Keep,un0', 'k_un1': 'Keep,un1', 'k_best': 'Keep,best', 'k_ok': 'Keep,ok',
              'd_unlock': 'Deco,unlock_lvl', 'names': 'State,names', 'nm': 'State,nm', 'want_name': 'State,want_name', 'nm_x': 'State,nm_x', 'd_owned': 'Deco,owned', 'd_pad': 'Deco,pad', 'e_toys': 'Ext,toys', 'd_vit': 'Deco,vit_day',
              'jr': 'State,jr', 'jtab': 'State,jtab', 'jstar': 'State,jstar', 'jcare': 'State,jcare', 'j_s': 'Journal,s', 'j_coins': 'Journal,coins', 'j_games': 'Journal,games', 'so_pend': 'State,sendoff_pending', 'retired': 'Life,retired', 'l_pad': 'Life,pad', 'more': 'State,more', 'm_rn': 'More,rn', 'm_rmiss': 'More,rmiss', 'jitem': 'State,jitem', 'm_found0': 'More,found0', 'm_found_ok': 'More,found_ok', 'm_wishes': 'More,wishes', 'wish_t': 'State,wish_t', 'wish_cd': 'State,wish_cd', 'm_spare': 'More,spare', 'e_inv': 'Ext,inv', 'dex_cur': 'State,dex_cur', 'stock': 'State,stock', 'st_cur': 'State,st_cur', 'st_page': 'State,st_page', 'm_stories': 'More,stories', 'st_pad': 'State,st_pad', 'rng': 'State,rng', 'gdn': 'State,gdn', 'gcur': 'State,gcur', 'g_found': 'Garden,found', 'g_seeds': 'Garden,seeds', 'g_vis_n': 'Garden,vis_n', 'g_vis': 'Garden,vis', 'g_vis_day': 'Garden,vis_day', 'e_day': 'Ext,day', 'gpad': 'State,gpad', 'gdetail': 'State,gdetail', 'bg_state': 'State,bg_state', 'weak_warned': 'State,weak_warned', 'outside': 'State,outside', 'gpad2': 'State,gpad2', 'dex_from': 'State,dex_from', 'gmsg': 'State,gmsg', 'ex_yard': 'State,ex_yard', 'jt_first': 'State,jt_first', 'fly': 'State,fly', 'fly_t': 'State,fly_t', 'fly_kinds': 'State,fly_kinds', 'fly_night': 'State,fly_night', 'fly_rel': 'State,fly_rel', 'fly_n': 'State,fly_n', 'fly_state': 'State,fly_state', 'fly_card': 'State,fly_card', 'jx': 'State,jx', 'jy': 'State,jy', 'jfly': 'State,jfly', 'f_x': 'Fly,x', 'f_y': 'Fly,y', 'f_k': 'Fly,k', 'f_t': 'Fly,t', 'f_live': 'Fly,live', 'spr_n': 'State,spr_n', 'spr_live': 'State,spr_live', 'spr_win': 'State,spr_win', 'win': 'State,win', 'm_pad0': 'More,pad0', 'm_pad1': 'More,pad1', 'gwater': 'State,gwater', 'e_adult': 'Ext,adult_day', 'gcur': 'State,gcur', 'gone_t': 'State,gone_t', 'jrec': 'State,jrec', 'sky_t': 'State,sky_t', 'spr_shown': 'State,spr_shown', 'room_cx': 'State,room_cx', 'room_cy': 'State,room_cy', 'ex_first': 'State,ex_first', 'cur': 'State,cur', 'jar_t': 'State,jar_t', 'jar_back': 'State,jar_back', 'st_baked': 'State,st_baked'}
    src = f'#include "{CK}/ext.c"\nconst unsigned TEST_OFFS[] = {{ ' + ', '.join(f'__builtin_offsetof({v})' for v in fields.values()) + ' };\n'
    c, o, b = f'{TMP}/offs.c', f'{TMP}/offs.o', f'{TMP}/offs.bin'
    open(c, 'w').write(src)
    subprocess.run(['clang', '--target=armv4t-none-eabi', '-mthumb', '-O2', '-ffreestanding', '-fno-builtin', '-nostdlib',
                    '-Wno-everything', '-c', c, '-o', o], check=True)
    sym = [l.split() for l in subprocess.run(['llvm-objdump', '-t', o], capture_output=True, text=True).stdout.splitlines() if l.endswith(' TEST_OFFS')][0]
    subprocess.run(['llvm-objcopy', '-O', 'binary', '--only-section=.rodata', o, b], check=True)
    d = open(b, 'rb').read(); at = int(sym[0], 16)
    vals = struct.unpack(f'<{len(fields)}I', d[at:at + 4 * len(fields)])
    return dict(zip(fields, vals))

OFF = offsets()
G = 0x0203E000
def g(field, sub=None):
    a = G + OFF[field]
    if sub: a += OFF[sub]
    return a
K_FOOD, K_PLAY, K_CLEAN, K_REST = range(4)
def cw(win, field, k=0):   # address of a care-window field: win 'c_grow' or 'c_adult'
    return G + OFF['care'] + OFF[win] + OFF[field] + 2 * k
def care_set(win, **kv):    # poke a care window: n, good, bad, yard, garden, play, miss=(k, v)
    out = []
    for f, v in kv.items():
        if f == 'miss': out += p16(cw(win, 'w_miss', v[0]), v[1])
        else: out += p16(cw(win, 'w_' + f), v)
    return out

# ---------------------------------------------------------------- emulator helpers
def run(rom, script, sav, timeout=900):
    sp = f'{TMP}/script.txt'
    open(sp, 'w').write('\n'.join(script) + '\n')
    r = subprocess.run([EMU, rom, sp, sav], capture_output=True, text=True, timeout=timeout)
    out = r.stdout + r.stderr
    if 'undefined' in out or 'weird region' in out:
        raise AssertionError('emulator fault:\n' + out[-800:])
    return out

def peeks(out):
    """'aaaaaaaa: xx xx ..' lines -> list of (addr, bytes)"""
    res = []
    for l in out.splitlines():
        m = re.match(r'([0-9a-f]{8}):((?: [0-9a-f]{2})+)$', l.strip())
        if m: res.append((int(m.group(1), 16), bytes(int(x, 16) for x in m.group(2).split())))
    return res

def copy_save(name):
    dst = f'{TMP}/{name}.sav'
    shutil.copy(f'{SAVES}/{name}.sav', dst)
    return dst

def p8(a, v): return f'poke8 {a:08x} {v:x}'
def p32(a, v): return f'poke32 {a:08x} {v:x}'
def p16(a, v): return [p8(a, v & 0xFF), p8(a + 1, (v >> 8) & 0xFF)]
BOOT = ['wait 60', 'press A', 'waitscene 1', 'wait 10']

results = []
def check(name, cond, detail=''):
    results.append((name, bool(cond), detail))
    print(('  PASS ' if cond else '  FAIL ') + name + ('' if cond else '  ' + detail))

# ---------------------------------------------------------------- tests
def t_fidelity():
    """v8.3: the clock is faster (a tick every second, an 11-minute day), so the old check that it matched v7 step for
    step is retired. What must not change is how fast care is needed in real time: a grown-up still loses a hunger
    heart every 72 s and a happy heart every 78 s, a baby a hunger heart every 30 s (v8.3: 60% faster)."""
    def frames(save, idx, cnt, rate_s):
        sc = BOOT + DAY + [p8(PET + 12, 0), p8(PET + 6, 4), p8(PET + 7, 4), *p16(PET + cnt, 0), 'scene', f'waitb {PET + idx:08x} 3', 'scene']
        f = [int(m) for m in re.findall(r'frame (\d+) scene', run(ROM, sc, copy_save(save)))]
        return (f[1] - f[0]) / 60.0 if len(f) == 2 else -1
    for save, idx, cnt, want, what in (('adult', 6, 0x28, 72, 'a grown-up loses a hunger heart every 72 s'),
                                       ('adult', 7, 0x2A, 78, 'a grown-up loses a happy heart every 78 s'),
                                       ('hatched', 6, 0x28, 30, 'a baby loses a hunger heart every 30 s')):
        s = frames(save, idx, cnt, want)
        check(f'pace: {what} (v8.3: 60% faster): {s:.0f} s', want - 2 <= s <= want + 2, f'{s:.1f} s')

def grow(side, setup_extra, expect, note):
    """A teen two days old, awake in its room by day: the next clock step grows it up."""
    sav = copy_save('teen')
    day = 9
    sc = ['wait 30',
          p8(g('dex', 'd_phase'), 2), p8(g('dex', 'd_side'), side), p8(g('dex', 'd_adult'), 0),
          p8(g('dex', 'd_teen_day'), day - 2), p8(g('dex', 'd_yard'), 0), p8(g('dex', 'd_games'), 0),
          *care_set('c_grow', n=100, good=50, bad=50, yard=0, garden=0, play=0), *[x for k in range(4) for x in care_set('c_grow', miss=(k, 0))],
          *p16(g('ext', 'e_day'), day), p8(g('ext', 'e_special'), 0),
          *[p8(g('deco', 'k_slot') + i, 0xFF) for i in range(6)],
          *setup_extra,
          'press A', 'waitscene 1', 'wait 5',
          p8(PET + 4, 3), p8(PET + 0xC, 0), p32(PET + 0x24, 450), p8(PET + 6, 4), p8(PET + 7, 4), *p16(PET + 0x16, 100),
          'waitscene 3', 'waitscene 5', 'peek 03000340 96', 'press A', 'waitscene 1', 'wait 2',
          f'peek {PET + 4:08x} 1', f'peek {g("dex", "d_phase"):08x} 1', f'peek {g("dex", "d_adult"):08x} 1']
    out = run(ROM, sc, sav)
    pk = peeks(out)
    stage, phase, adult = pk[1][1][0], pk[2][1][0], pk[3][1][0]
    msg = ' '.join(x.decode(errors='replace') for x in pk[0][1].split(b'\0') if x.strip())
    check(f'growth: {note} -> form {expect}', stage == 5 and phase == 3 and adult == expect,
          f'stage {stage} phase {phase} adult {adult} msg "{msg}"')
    return msg

def t_growth():
    msg = grow(1, care_set('c_grow', yard=45), 15, 'good care, 10+ minutes outside = OAKHART')
    check('growth: the scene names the grown-up', 'OAKHART' in msg, msg)
    grow(1, care_set('c_grow', garden=130), 16, 'good care, around plants = DUNEROSE')
    grow(1, care_set('c_grow', play=20), 3, 'good care, lots of play = FLUFFBUD')
    grow(1, care_set('c_grow', yard=45, play=30), 3, 'good care, play beats yard time (1.9x vs 1.1x its target) = FLUFFBUD')
    grow(1, care_set('c_grow', yard=20, garden=50, play=6), 4, 'good care, nothing reaches its target = FRILLO')
    grow(2, care_set('c_grow', miss=(K_PLAY, 40)), 17, 'less care, sad a lot = THORNHOG')
    grow(2, care_set('c_grow', miss=(K_CLEAN, 30)), 18, 'less care, lived in a mess = SMOGWOOL')
    grow(2, care_set('c_grow', miss=(K_FOOD, 30)), 19, 'less care, went hungry = GUNKBILL')
    grow(2, care_set('c_grow', miss=(K_FOOD, 40)) + care_set('c_grow', miss=(K_CLEAN, 20)), 19, 'less care, hunger missed most = GUNKBILL')
    # a teen younger than two days does not grow up
    sav = copy_save('teen')
    sc = ['wait 30', p8(g('dex', 'd_phase'), 2), p8(g('dex', 'd_side'), 1), p8(g('dex', 'd_adult'), 0), p8(g('dex', 'd_teen_day'), 8),
          *p16(g('ext', 'e_day'), 9), 'press A', 'waitscene 1', 'wait 5', p8(PET + 4, 3), p8(PET + 0xC, 0), p32(PET + 0x24, 450),
          'wait 400', f'peek {PET + 4:08x} 1', 'peek 03000020 1']
    pk = peeks(run(ROM, sc, sav))
    check('growth: a one-day-old teen stays a teen', pk[0][1][0] == 3 and pk[1][1][0] == 1, str(pk))
    # child -> teen after a day, picking its side by care
    for good, form_side in ((80, 1), (20, 2)):
        sav = copy_save('teen')
        sc = ['wait 30', p8(g('dex', 'd_phase'), 1), *care_set('c_grow', n=100, good=good, bad=100 - good),
              'press A', 'waitscene 1', 'wait 5', p8(PET + 4, 2), p32(PET + 0x20, 599), p8(PET + 0xC, 0), p32(PET + 0x24, 450),
              'waitscene 3', 'waitscene 5', 'peek 03000340 96', 'press A', 'waitscene 1', 'wait 2', f'peek {PET + 4:08x} 1', f'peek {g("dex", "d_phase"):08x} 2']
        pk = peeks(run(ROM, sc, sav))
        msg = ' '.join(x.decode(errors='replace') for x in pk[0][1].split(b'\0') if x.strip()); pk = pk[1:]
        teen = 'BLOOMLE' if form_side == 1 else 'THORNLE'
        check(f'growth: child with {good}% good care -> {teen}', pk[0][1][0] == 3 and pk[1][1][0] == 2 and pk[1][1][1] == form_side and teen in msg,
              f'{pk[:2]} "{msg}"')
    # pre-v7 grown-ups keep the original's second growth step
    sav = copy_save('teen')
    sc = ['wait 30', p8(g('dex', 'd_phase'), 0), 'press A', 'waitscene 1', 'wait 5',
          p8(PET + 4, 4), p32(PET + 0x20, 900), p8(PET + 0xF, 0), p8(PET + 0xC, 0), p32(PET + 0x24, 450),
          'waitscene 3', 'waitscene 5', 'press A', 'waitscene 1', 'wait 2', f'peek {PET + 4:08x} 1']
    pk = peeks(run(ROM, sc, sav))
    check('growth: an old (pre-v7) stage-4 grown-up still grows into stage 6', pk[0][1][0] == 6, str(pk))

def secret(adult, side, setup, expect_sf, expect_name, note, back=3):
    sav = copy_save('teen'); day = 9
    sc = ['wait 30', p8(g('dex', 'd_phase'), 3), p8(g('dex', 'd_side'), side), p8(g('dex', 'd_adult'), adult),
          *p16(g('ext', 'e_day'), day), *p16(g('ext', 'e_adult_day'), day - back), p8(g('ext', 'e_special'), 0), p8(g('ext', 'e_stars'), 0),
          *[p8(g('deco', 'k_slot') + i, 0xFF) for i in range(6)],
          *[p8(G + OFF['care'] + OFF['c_miss_day'] + k, day) for k in range(4)],
          *care_set('c_adult', n=40, good=20, bad=20, yard=0, garden=0, play=0),
          *setup,
          'press A', 'waitscene 1', 'wait 5', p8(PET + 4, 5), p8(PET + 0xC, 0), p8(PET + 0xB, 0), p8(PET + 6, 4), p8(PET + 7, 4), p32(PET + 0x24, 450),
          p8(g('check_special'), 1), 'wait 20', f'peek {g("ext", "e_special"):08x} 1', 'peek 030002a4 30']
    pk = peeks(run(ROM, sc, sav))
    msg = pk[1][1].split(b'\0')[0].decode(errors='replace')
    check(f'secrets: {note}', pk[0][1][0] == expect_sf and (not expect_name or expect_name in msg), f'special {pk[0][1][0]} "{msg}"')

def t_secrets():
    md = G + OFF['care'] + OFF['c_miss_day']
    YD, WG, P1 = g('dex', 'd_yard'), g('dex', 'd_games'), g('more', 'm_pad1')
    secret(15, 1, [p8(YD, 3)], 1, 'SUNPUFF', 'v8.3: an OAKHART out in the morning sun on 3 days becomes a SUNPUFF')
    secret(15, 1, [p8(YD, 3)], 0, '', 'v8.3: but not before its 4th grown-up day (on its 3rd it stays)', back=2)
    secret(15, 1, [p8(YD, 2), *care_set('c_adult', yard=80)], 0, '', 'v8.3: an OAKHART with 2 sunny mornings stays (time outside alone no longer counts)')
    secret(16, 1, [p8(P1, 4)], 4, 'GLIMMER', 'v8.3: a DUNEROSE with 8 fireflies in its lantern in one night becomes a GLIMMER')
    secret(16, 1, [p8(P1, 0), *care_set('c_adult', garden=120), p8(PET + 8, 4)], 0, '', 'v8.3: a DUNEROSE with only a garden and full bond stays')
    secret(3, 1, [p8(WG, 2)], 3, 'ZIPPIT', 'v8.3: a FLUFFBUD that wished on 2 shooting stars becomes a ZIPPIT')
    secret(3, 1, [p8(WG, 1), p8(g('ext', 'e_stars'), 6)], 0, '', 'v8.3: a FLUFFBUD with 1 wish (and 6 WHAT A STAR! games) stays')
    grown = [p8(g('dex', 'd_phase'), 3), p8(g('ext', 'e_special'), 0), p8(YD, 0), p8(WG, 0), p8(g('life', 'l_pad'), 0), p8(P1, 0)]
    # in the yard early in the morning: counted once that day, with a message
    sc = BOOT + DAY + grown + ['press SELECT', 'wait 6', 'press A', 'wait 40', p32(0x0300004C, 60), 'wait 30',
                               *[x for _ in range(16) for x in ('wait 30', f'peek {YD:08x} 1', 'peek 030002a4 32')], 'wait 300', f'peek {YD:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    ms = [(v[2 * i][0], bytes(v[2 * i + 1]).split(b'\0')[0].decode('ascii', 'replace')) for i in range(16)]
    seen = [m for n, m in ms if n == 1]
    check('secrets: v8.3: a grown-up in the yard in the early morning counts a sunny morning (once a day), when the message box is free',
          ms[-1][0] == 1 and v[-1][0] == 1 and seen and 'MORNING SUN' in seen[0], str(ms[::3]))
    sc = BOOT + DAY + grown + ['press SELECT', 'wait 6', 'press A', 'wait 40', p32(0x0300004C, 200), 'wait 300', f'peek {YD:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('secrets: v8.3: later in the day it does not count', v[0][0] == 0, str(v[0][0]))
    # a wish counts for ZIPPIT
    sc = BOOT + grown + ['poke32 0300004c 1f4', 'wait 5', *p16(g('wish_cd'), 20), 'wait 40', 'press A', 'wait 6', f'peek {WG:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('secrets: v8.3: a grown-up\'s wish on a shooting star is counted', v[0][0] == 1, str(v[0][0]))
    # (v8.3: a full room is all five spots: the window, the picture, the wall, the ceiling and the corner)
    full = [p8(g('deco', 'k_slot') + i, it) for i, it in enumerate((14, 35, 18, 23))]
    secret(4, 1, full + [p8(g('deco', 'd_pad'), 15)], 5, 'NESTLE', 'a FRILLO in a full room (all five spots) becomes a NESTLE')
    secret(4, 1, full + [p8(g('deco', 'd_pad'), 0xFF)], 0, None, 'a FRILLO with the corner empty stays a FRILLO')
    secret(15, 1, care_set('c_adult', good=40, bad=2), 6, 'STARWING', 'an OAKHART with near-perfect care becomes a STARWING')
    secret(4, 1, [p8(YD, 3)], 0, '', 'a FRILLO out on sunny mornings stays (SUNPUFF is the OAKHART secret)')
    secret(18, 2, [p8(md + K_CLEAN, 7)], 7, 'PETALAMB', 'a SMOGWOOL kept clean two days becomes a PETALAMB')
    secret(19, 2, [p8(md + K_FOOD, 7)], 8, 'LILYBILL', 'a GUNKBILL kept fed two days becomes a LILYBILL')
    secret(17, 2, [p8(md + K_PLAY, 7)], 9, 'JAMBLE', 'a THORNHOG kept happy two days becomes a JAMBLE')
    secret(17, 2, [p8(md + K_PLAY, 8)], 0, '', 'a THORNHOG sad yesterday stays')

def t_care():
    """the care record samples every 15 s: needs, yard, plants, play"""
    sav = copy_save('teen')
    sc = ['wait 30', p8(g('dex', 'd_phase'), 1), *[p8(g('deco', 'k_slot') + i, 0xFF) for i in range(6)],
          p8(g('deco', 'd_pad'), 16), p8(g('deco', 'k_slot') + 5, 43),   # (v8.3: the plant's spot is the corner, Deco.pad)
          'press A', 'waitscene 1', 'wait 5', *care_set('c_grow', n=0, good=0, bad=0, garden=0),
          *[x for k in range(4) for x in care_set('c_grow', miss=(k, 0))],
          p8(PET + 4, 2), p32(PET + 0x20, 10), p8(PET + 0x12, 1), p8(PET + 0xC, 0), p32(PET + 0x24, 100),
          p8(PET + 6, 0), p8(PET + 7, 4), p8(PET + 0xB, 1), p8(PET + 0xA, 0),
          'wait 1500', f'peek {cw("c_grow", "w_n"):08x} 22']
    pk = peeks(run(ROM, sc, sav)); d = pk[0][1]
    n, good, bad = struct.unpack('<3H', d[0:6]); miss = struct.unpack('<4H', d[6:14]); yard, garden, play = struct.unpack('<3H', d[14:20])
    check('care: checks every 360 frames on FAST', 3 <= n <= 5, f'n={n}')
    check('care: hungry + messy = not good care', bad == n and good == 0, f'good {good} bad {bad}')
    check('care: food and clean are missing, play and rest are not', miss[0] == n and miss[2] == n and miss[1] == 0 and miss[3] == 0, str(miss))
    check('care: a plant inside and flowers outside earn 2 garden points a check (+4 each time it uses the plant)', garden >= 2 * n and (garden - 2 * n) % 4 == 0, f'garden {garden} n {n}')

    # Phase 7: hunger and happiness at 1 still count as cared for - only 0 (when it calls for them) is missing
    sc = ['wait 30', p8(g('dex', 'd_phase'), 1), 'press A', 'waitscene 1', 'wait 5', *care_set('c_grow', n=0, good=0, bad=0),
          *[x for k in range(4) for x in care_set('c_grow', miss=(k, 0))],
          p8(PET + 4, 2), p32(PET + 0x20, 10), p8(PET + 0x12, 1), p8(PET + 0xC, 0), p32(PET + 0x24, 100), p8(PET + 0xD, 0),
          p8(PET + 6, 1), p8(PET + 7, 1), p8(PET + 0xB, 0), p8(PET + 0xA, 0), *p16(PET + 0x28, 0), *p16(PET + 0x2A, 0), *p16(PET + 0x2C, 0), *p16(PET + 0x2E, 300),
          *[x for _ in range(3) for x in ('wait 500', *p16(PET + 0x28, 0), *p16(PET + 0x2A, 0))],   # (v8.3: hearts drain fast; keep them at 1)
          f'peek {cw("c_grow", "w_n"):08x} 14']
    d = peeks(run(ROM, sc, copy_save('teen')))[0][1]
    n, good, bad = struct.unpack('<3H', d[0:6]); miss = struct.unpack('<4H', d[6:14])
    check('care: hunger 1 and happiness 1 still count as good care (missing only at 0)', n >= 3 and good == n and miss[0] == 0 and miss[1] == 0, f'n {n} good {good} miss {miss}')

def sram_block(sav, at, size):
    return open(sav, 'rb').read()[at:at + size]

def t_save():
    parts = ['wait 30'] + [f'peek {g(p):08x} {n}' for p, n in (('ext', 40), ('life', 20), ('deco', 28), ('album', 76), ('dex', 36))]
    for name in ('teen', 'adult', 'oldchild', 'oldgrown'):
        a, b = copy_save(name), copy_save(name)
        if os.path.exists(BASE):
            va = [x for x in peeks(run(BASE, parts, a))]
        vb = peeks(run(ROM, parts + [f'savesram {TMP}/after.sav'], b))
        if os.path.exists(BASE):
            # (v8.3: the deco part may differ only where pieces moved to their themed spots: the slots, the corner, its checksum)
            def keep(i, b): return b[4:-4] if i == 4 else b[:8] + b[14:19] + b[20:24] if i == 2 else b
            same = all(keep(i, x[1]) == keep(i, y[1]) for i, (x, y) in enumerate(zip(va, vb)))
            check(f'save: v7 save "{name}" converts with nothing lost', same, '')
        d = bytearray(open(f'{TMP}/after.sav', 'rb').read())
        check(f'save: "{name}" has the new save block', save_ver(d) == CUR_VER, '')
        for i in range(0x100, 0x300): d[i] = 0xFF                  # old blocks gone: must load from the new one
        open(f'{TMP}/new_only.sav', 'wb').write(d)
        vc = peeks(run(ROM, parts, f'{TMP}/new_only.sav'))
        check(f'save: "{name}" reloads from the new block alone', vc == vb, '')
    # a phase-1 (version 8) save converts to version 9 with its care counts
    if os.path.exists(BASE8):
        a = copy_save('teen')
        run(BASE8, ['wait 30', p8(g('dex', 'd_phase'), 1), *p16(g('dex', 'd_c_good'), 37), *p16(g('dex', 'd_c_bad'), 5),
                    'press A', 'waitscene 1', 'wait 800', f'savesram {TMP}/v8.sav'], a)
        sc = ['wait 30', f'peek {g("dex", "d_c_good"):08x} 4', f'peek {cw("c_grow", "w_good"):08x} 4', f'savesram {TMP}/v9.sav']
        pk = peeks(run(ROM, sc, f'{TMP}/v8.sav'))
        d9 = open(f'{TMP}/v9.sav', 'rb').read()
        ver = save_ver(d9)
        check('save: a phase-1 save converts to the current version, care counts kept', ver == CUR_VER and pk[0][1] == pk[1][1], f'ver {ver} {pk}')
    # two copies: make a few saves, tear the newest, the previous one must load
    sav = copy_save('teen')
    sc = ['wait 30', 'press A', 'waitscene 1', 'wait 30', p8(PET + 0x12, 1), 'wait 1300',
          f'peek {g("save_seq"):08x} 4', f'savesram {TMP}/two.sav']
    out = run(ROM, sc, sav)
    d = bytearray(open(f'{TMP}/two.sav', 'rb').read())
    okA, okB = d[0x1000:0x1004] == b'CKS8', d[0x2000:0x2004] == b'CKS8'
    check('save: both copies are in use', okA and okB, f'A {okA} B {okB}')
    seqA, seqB = struct.unpack('<I', d[0x1008:0x100C])[0], struct.unpack('<I', d[0x2008:0x200C])[0]
    newest = 0x1000 if seqA > seqB else 0x2000
    d[newest + 20] ^= 0x5A                                           # damage the newest copy
    open(f'{TMP}/torn.sav', 'wb').write(d)
    out = run(ROM, ['wait 30', f'peek {g("save_seq"):08x} 4'], f'{TMP}/torn.sav')
    seq = struct.unpack('<I', peeks(out)[0][1])[0]
    check('save: a damaged newest copy falls back to the previous one', seq == min(seqA, seqB), f'loaded seq {seq}, copies {seqA}/{seqB}')

def t_fast():
    for fast, per in ((0, 60), (1, 24)):   # (v8.3: a tick every second, 2.5x on FAST)
        sav = copy_save('teen')
        sc = BOOT + [p8(PET + 0x12, fast), 'poke32 03000024 0', f'peek {PET + 0x1C:08x} 4', 'wait 1800', f'peek {PET + 0x1C:08x} 4']
        pk = peeks(run(ROM, sc, sav))
        t0, t1 = struct.unpack('<I', pk[0][1])[0], struct.unpack('<I', pk[1][1])[0]
        check(f'fast: FAST {"on" if fast else "off"} steps the clock every {per} frames', t1 - t0 == 1800 // per, f'{t1 - t0} steps in 1800 frames')

def t_fuzz():
    for name, seed in (('hatched', 11), ('teen', 22), ('adult', 33), ('oldgrown', 44)):
        sav = copy_save(name)
        try:
            out = run(ROM, BOOT + [f'fuzz 60000 {seed}', 'scene'], sav, timeout=1200)
            check(f'fuzz: 60000 random frames from "{name}"', 'fuzz done' in out, out[-200:])
        except AssertionError as e:
            check(f'fuzz: 60000 random frames from "{name}"', False, str(e)[:300])

# ---------------------------------------------------------------- v8 phase 5: games, health card, kept shop unlocks
BASE9 = f'{CK}/tests/baseline_v8p4.gba'
DAY = [p32(0x0300004C, 200), p8(0x03000034, 0), p8(0x03000035, 0), 'wait 30']   # mid-morning, awake, lights on
OPEN_PLAY = ['press RIGHT', 'wait 4', 'press RIGHT', 'wait 4', 'press A', 'wait 20']   # the PLAY icon
M_MINI, PH_PICK, PH_PLAY, PH_RESULT = 7, 0, 2, 3
G_PANCAKE, G_HOP, G_GUESS, G_STARS = range(4)
def pk(out): return peeks(out)
def u32(b): return struct.unpack('<I', b[:4])[0]
def start(game): return [p8(PET + 0x13, 1), *OPEN_PLAY, p8(g('pick'), game), 'press A', 'wait 85']
def popcount_keep(b): return bin(u32(b[0:4])).count('1') + (b[4] & 1)

def t_games():
    # the picker: PLAY opens it, DOWN moves, B goes back to the room
    out = run(ROM, BOOT + DAY + OPEN_PLAY + [f'peek {g("mode"):08x} 1', f'peek {g("phase"):08x} 1', f'peek {g("pick"):08x} 1',
              'press DOWN', 'wait 4', f'peek {g("pick"):08x} 1', 'press B', 'wait 10', 'scene', f'peek {g("mode"):08x} 1'], copy_save('adult'))
    v = pk(out)
    check('games: PLAY opens the game picker', v[0][1][0] == M_MINI and v[1][1][0] == PH_PICK, str(v[:2]))
    check('games: DOWN moves the picker (4 games)', v[3][1][0] == (v[2][1][0] + 1) % 4, str(v[2:4]))
    check('games: B leaves the picker for the room', 'scene 1' in out and v[4][1][0] == 0, out[-200:])
    # PANCAKE TOWER: 14 dead-on drops fill the tower; a good round adds hunger and sets the best stack
    sc = BOOT + DAY + start(G_PANCAKE) + [p8(PET + 6, 2), f'peek {g("tune"):08x} 1']
    for i in range(14): sc += [p32(g('px'), 96 << 8), p32(g('lastv'), 0), 'press A', 'wait 40']
    sc += ['wait 70', f'peek {g("phase"):08x} 1', f'peek {g("score"):08x} 4', f'peek {g("pk_w"):08x} 15', f'peek {g("rating"):08x} 1',
           'press A', 'wait 10', f'peek {PET + 6:08x} 1', f'peek {g("keep", "k_best"):08x} 1', 'scene']
    out = run(ROM, sc, copy_save('adult')); v = pk(out)
    check('games: PANCAKE TOWER plays its own tune', v[0][1][0] == 8, str(v[0]))
    check('games: PANCAKE TOWER - 14 dead-on drops make a full tower', v[1][1][0] == PH_RESULT and u32(v[2][1]) == 14, str(v[1:3]))
    check('games: PANCAKE TOWER - a dead-on drop keeps the full width', set(v[3][1]) == {48}, str(v[3]))
    check('games: PANCAKE TOWER - a full tower is PERFECT!, +1 hunger, best stack 14', v[4][1][0] == 3 and v[5][1][0] == 3 and v[6][1][0] == 14, str(v[4:7]))
    # an off-centre drop trims the pancake and the overhang falls
    sc = BOOT + DAY + start(G_PANCAKE) + [p32(g('px'), 106 << 8), p32(g('lastv'), 0), 'press A', 'wait 40', f'peek {g("pk_w"):08x} 2',
                                          f'peek {g("obj"):08x} 40', p32(g('px'), 200 << 8), p32(g('lastv'), 0), 'press A', 'wait 120', f'peek {g("phase"):08x} 1', f'peek {g("score"):08x} 4']
    v = pk(run(ROM, sc, copy_save('adult')))
    check('games: PANCAKE TOWER - an off-centre drop is trimmed', list(v[0][1]) == [48, 38], str(v[0]))
    check('games: PANCAKE TOWER - missing the stack ends the round', v[2][1][0] == PH_RESULT and u32(v[3][1]) == 1, str(v[2:4]))
    # STARGAZER: play the tune back five times; a good round +20 health
    pre = BOOT + DAY + [*p16(PET + 0x16, 50)] + start(G_STARS)
    seq = pk(run(ROM, pre + [f'peek {g("st_seq"):08x} 8', f'peek {g("tune"):08x} 1'], copy_save('adult')))
    keys = ['UP', 'LEFT', 'RIGHT', 'DOWN']; st = list(seq[0][1])
    check('games: STARGAZER plays its own tune', seq[1][1][0] == 14, str(seq[1]))
    sc = list(pre)
    for r in range(5):
        sc.append(f'waitw {g("spawn"):08x} 1')
        sc += [f'press {keys[st[i]]}' for i in range(2 + r)]
    sc += ['wait 80', f'peek {g("phase"):08x} 1', f'peek {g("score"):08x} 4', 'press A', 'wait 10', f'peek {PET + 0x16:08x} 2']
    out = run(ROM, sc, copy_save('adult')); v = pk(out)
    check('games: STARGAZER - five songs played back', 'timeout' not in out and v[0][1][0] == PH_RESULT and u32(v[1][1]) == 5, str(v[:2]))
    check('games: STARGAZER - a good round +20 health', v[2][1] == bytes([70, 0]), str(v[2]))
    sc = list(pre) + [f'waitw {g("spawn"):08x} 1', f'press {keys[(st[0] + 1) % 4]}', 'wait 100', f'peek {g("phase"):08x} 1', f'peek {g("score"):08x} 4']
    v = pk(run(ROM, sc, copy_save('adult')))
    check('games: STARGAZER - a wrong star ends it', v[0][1][0] == PH_RESULT and u32(v[1][1]) == 0, str(v))
    # the other two still run to the end
    # HOP HOP: a round is 12 hurdles and berries, and clearing them all is a perfect score that pays 15 coins
    sc = BOOT + DAY + start(G_HOP) + [f'peek {g("ext", "e_coins"):08x} 2', 'auto 900', f'peek {g("phase"):08x} 1', f'peek {g("score"):08x} 4',
          f'peek {g("rating"):08x} 1', 'press A', 'wait 10', f'peek {g("ext", "e_coins"):08x} 2']
    v = pk(run(ROM, sc, copy_save('adult'))); c0, c1 = struct.unpack('<H', v[0][1])[0], struct.unpack('<H', v[4][1])[0]
    check('games: HOP HOP - jumping every hurdle and catching every berry is a perfect 12 (PERFECT!), +15 coins (no bonus coins since v8.3t)',
          v[1][1][0] == PH_RESULT and u32(v[2][1]) == 12 and v[3][1][0] == 3 and c1 - c0 == 15, f'{v[1:4]} coins {c0}->{c1}')
    # v8.1: doing nothing in HOP HOP scores 0 (a rock that touches it while it flashes is a miss) and pays nothing
    sc = BOOT + DAY + start(G_HOP) + [f'peek {g("ext", "e_coins"):08x} 2', 'wait 900', f'peek {g("score"):08x} 4', f'peek {g("rating"):08x} 1', 'press A', 'wait 10', f'peek {g("ext", "e_coins"):08x} 2']
    v = pk(run(ROM, sc, copy_save('adult'))); c0, c1 = struct.unpack('<H', v[0][1])[0], struct.unpack('<H', v[3][1])[0]
    check('games: HOP HOP - standing still scores 0, BETTER LUCK, no coins', u32(v[1][1]) == 0 and v[2][1][0] == 0 and c1 == c0, f'score {u32(v[1][1])} rating {v[2][1][0]} coins {c0}->{c1}')
    for game, wait, name in ((G_HOP, 900, 'HOP HOP'), (G_GUESS, 1200, 'LEFT OR RIGHT?')):
        v = pk(run(ROM, BOOT + DAY + start(game) + [f'wait {wait}', f'peek {g("phase"):08x} 1'], copy_save('adult')))
        check(f'games: {name} still plays to the end', v[0][1][0] == PH_RESULT, str(v))

def t_fixes():
    # the FEED icon's MEAL/SNACK are drawn with their own colours (palette 40-63) while being eaten
    sc = BOOT + DAY + [p8(PET + 6, 1), 'press A', 'wait 20', 'press A', 'wait 12', 'scene', 'peek 05000050 48']
    out = run(ROM, sc, copy_save('adult')); v = pk(out)
    check('fixes: eating a MEAL loads the detailed food colours', 'scene 3' in out and any(v[0][1]), out[-200:])
    # the music keeps time on slow screens: the menu plays the room tune at the room's pace
    tp = g('tune_pos')
    sc = BOOT + DAY + [p8(PET + 0x13, 1), 'wait 30', f'peek {tp:08x} 2', 'wait 300', f'peek {tp:08x} 2']
    a = pk(run(ROM, sc, copy_save('adult')))
    sc = BOOT + DAY + [p8(PET + 0x13, 1), 'wait 30', f'peek {tp:08x} 2', 'press START', 'wait 300', f'peek {tp:08x} 2']
    b = pk(run(ROM, sc, copy_save('adult')))
    ra = int.from_bytes(a[1][1], 'little') - int.from_bytes(a[0][1], 'little'); rb = int.from_bytes(b[1][1], 'little') - int.from_bytes(b[0][1], 'little')
    check('fixes: the music plays at the same pace in the menu as in the room', ra == rb and ra > 0, f'room {ra} menu {rb}')
    # the status card has no health bar (health is hidden; it still works)
    from PIL import Image
    shot = f'{TMP}/card_nohealth.png'
    run(ROM, BOOT + DAY + [*p16(PET + 0x16, 50)] + ['press RIGHT', 'wait 4'] * 5 + ['press A', 'wait 20', f'shot {shot}'], copy_save('adult'))
    im = Image.open(shot).convert('RGB')
    red = sum(1 for y in range(116, 126) for x in range(81, 145) if (lambda c: c[0] > 240 and c[1] < 100 and c[2] < 100)(im.getpixel((x, y))))
    check('fixes: the status card shows no health bar', red == 0, f'red {red}')
    # naming at hatching: NEW EGG, hatch, type HI, START
    egg = BOOT + ['press START', 'wait 10'] + ['press DOWN', 'wait 3'] * 5 + ['press A', 'wait 5', 'press A', 'waitscene 0', 'wait 30',
          'press DOWN', 'wait 5', 'press A', 'wait 10', 'press A', 'wait 10', 'press LEFT', 'wait 5', 'press A', 'wait 60', p32(PET + 0x20, 0xef), 'waitscene 5', 'wait 5', 'peek 03000381 24', 'press A', 'wait 15']
    sc = egg + [f'peek {g("mode"):08x} 1'] + ['press B', 'wait 3'] * 6 + ['press UP', 'wait 3'] * 3 + ['press A', 'wait 3', 'press RIGHT', 'wait 3', 'press A', 'wait 3',
          'press START', 'wait 5', f'peek {g("mode"):08x} 1', 'scene', f'peek {g("names"):08x} 8', 'peek 03000381 20', f'savesram {TMP}/named.sav']
    out = run(ROM, sc, copy_save('adult')); v = pk(out)
    check('fixes: the hatch box asks for a name, and A opens the naming screen', v and v[0][1][:22] == b"LET'S GIVE IT A NAME!\0" and v[1][1][0] == 11, out[-300:])
    v = v[1:]
    check('fixes: the name typed is kept and greets you in the hatch box', len(v) >= 4 and v[1][1][0] == 0 and v[2][1][:3] == b'HI\0'
          and v[3][1][:17] == b'SAY HELLO TO HI!\0' and 'scene 5' in out, str(v[1:4]))
    v = pk(run(ROM, BOOT + [f'peek {g("names"):08x} 8', 'peek 0203D040 40'], f'{TMP}/named.sav'))
    check('fixes: the name is saved', v[0][1][:3] == b'HI\0', str(v[0]))
    # v8.1: a baby that hasn't been named yet (B on the hatch box) shows no name on its status card
    shot = f'{TMP}/card_noname.png'
    run(ROM, egg[:-2] + ['press B', 'wait 30'] + ['press RIGHT', 'wait 4'] * 5 + ['press A', 'wait 20', f'shot {shot}'], copy_save('adult'))
    im = Image.open(shot).convert('RGB')
    ink = sum(1 for y in range(34, 52) for x in range(14, 110) if sum(im.getpixel((x, y))) < 300)
    check('fixes: no name on the status card before it is named', ink == 0, f'ink {ink}')
    # v8.3: a brand-new game (no save): the egg's name slot is picked after the name tables were made; still no name
    sv = f'{TMP}/fresh.sav'; open(sv, 'wb').write(b'\xff' * 65536)
    shot = f'{TMP}/card_egg.png'
    run(ROM, ['wait 60', 'press A', 'wait 120', p8(PET + 5, 3), 'wait 10'] + ['press RIGHT', 'wait 6'] * 5 + ['press A', 'wait 20', f'shot {shot}'], sv)
    im = Image.open(shot).convert('RGB')
    ink = sum(1 for y in range(34, 52) for x in range(14, 110) if sum(im.getpixel((x, y))) < 300)
    check('fixes: v8.3: a new game\'s egg shows no name on its status card', ink == 0, f'ink {ink}')
    # (v8.3: plain petting no longer raises happiness, so it no longer starts its 90 s rest: see the yard section)
    # v8.3: after a SNACK (or a MEAL) the feed menu opens again on the same choice; B closes it
    sc = BOOT + DAY + [p8(PET + 6, 1), p8(PET + 7, 1), 'press A', 'wait 10', 'press DOWN', 'wait 6', 'press A', 'wait 10', 'waitscene 2', 'wait 10', 'scene', 'peek 030002d4 4', 'press B', 'wait 10', 'scene']
    out = run(ROM, sc, copy_save('adult')); v = [x[1] for x in peeks(out)]
    sc_ = re.findall(r'scene (\d+)', out)
    check('fixes: v8.3: after a snack the feed menu opens again on SNACK, and B closes it', sc_[-2:] == ['2', '1'] and v[0][0] == 1, f'{sc_} {v}')
    # v8.3: awake by day with the lights off it gets glum: an extra happy heart every 2 minutes (with a message)
    res = []
    for lights in (1, 0):
        sc = BOOT + DAY + [p8(PET + 13, lights), p8(PET + 7, 4), *p16(PET + 0x2A, 0), 'wait 7300', f'peek {PET + 7:08x} 1', 'peek 030002a4 30']
        v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]; res.append((v[0][0], bytes(v[1]).split(b'\0')[0]))
    check('fixes: v8.3: lights off by day, it loses an extra happy heart every 2 minutes', res[0][0] == res[1][0] - 1 and b'LIKE THE DARK' in res[0][1], str(res))
    # START right away takes the suggested name; SELECT suggests another
    sc = egg + [f'peek {g("nm"):08x} 8', 'press SELECT', 'wait 3', f'peek {g("nm"):08x} 8', 'press START', 'wait 5', f'peek {g("names"):08x} 8']
    v = pk(run(ROM, sc, copy_save('adult')))
    v = v[1:]; a, b, c = (bytes(x[1]).split(b'\0')[0] for x in v[:3])
    check(f'fixes: the suggested name is taken as is ({c.decode()})', 1 <= len(c) <= 6 and c == b and a != b"", str(v[:3]))

def t_unlocks():
    kp = g('keep')
    peek_keep = [f'peek {kp:08x} 8', f'peek {g("deco", "d_unlock"):08x} 1', f'peek {g("deco", "d_owned"):08x} 4']
    UN = [0, 4, 8, 13, 18, 23, 27]; ADD = [0, 4, 4, 5, 5, 5, 4]
    # a version-9 save: what its shop had opened becomes the kept set
    if os.path.exists(BASE9):
        for name in ('teen', 'adult'):
            a = copy_save(name)
            run(BASE9, BOOT + ['wait 600', f'savesram {TMP}/v9.sav'], a)
            v = pk(run(ROM, ['wait 30'] + peek_keep + [f'savesram {TMP}/v10.sav'], f'{TMP}/v9.sav'))
            n, lv, own = popcount_keep(v[0][1]), v[1][1][0], bin(u32(v[2][1])).count('1')
            d = open(f'{TMP}/v10.sav', 'rb').read()
            ver = save_ver(d)
            check(f'unlocks: a v9 "{name}" save keeps its shop unlocks ({n} at level {lv}) and saves as the current version',
                  v[0][1][6] == 1 and 1 <= lv <= 6 and UN[lv] <= n <= UN[lv] + own and ver == CUR_VER, f'n {n} lv {lv} own {own} ver {ver}')
    # growing opens more, from the 27 not left out this life
    out = run(ROM, BOOT + peek_keep + [p8(g('deco', 'd_unlock'), 2), 'wait 5'] + peek_keep, copy_save('adult'))
    v = pk(out); n0, n1, lv = popcount_keep(v[0][1]), popcount_keep(v[3][1]), v[4][1][0]
    want = min(27, n0 + sum(ADD[3:lv + 1]))
    check(f'unlocks: growing to level {lv} opens {want - n0} more pieces', n1 == want, f'{n0} -> {n1}')
    sc = BOOT + [p8(g('deco', 'd_unlock'), 0)] + ['wait 3', p8(g('deco', 'd_unlock'), 0)] * 4 + ['wait 3'] + peek_keep
    v = pk(run(ROM, sc, copy_save('adult')))
    check('unlocks: a critter opens at most 27 pieces (6 left out each life)', popcount_keep(v[0][1]) == 27, str(v[0]))
    # a new egg after running away starts its own shop (its first level opens 4)
    sc = BOOT + peek_keep + [p8(PET + 4, 8), 'wait 5', p8(PET + 4, 0), 'wait 10'] + peek_keep
    v = pk(run(ROM, sc, copy_save('adult'))); n0, n1 = popcount_keep(v[0][1]), popcount_keep(v[3][1])
    check('unlocks: a new egg starts the shop over', n0 > 4 and n1 == 4 and v[4][1][0] == 1, f'{n0} -> {n1}, level {v[4][1][0]}')
    # so does NEW EGG from the title
    sc = BOOT + ['press START', 'wait 10'] + ['press DOWN', 'wait 3'] * 5 + ['press A', 'wait 5', 'press A', 'waitscene 0', 'wait 30',
          'press DOWN', 'wait 5', 'press A', 'wait 10', 'press A', 'wait 10', 'press LEFT', 'wait 5', 'press A', 'wait 60', 'scene'] + peek_keep
    out = run(ROM, sc, copy_save('adult')); v = pk(out)
    check('unlocks: NEW EGG on the title starts the shop over', v and popcount_keep(v[-3][1]) == 4, out[-300:])

# ---------------------------------------------------------------- v8 phase 6: the journal and the bigger save
def t_journal():
    JR = g('jr'); S = JR + OFF['j_s']
    peek_jr = [f'peek {JR:08x} 16', f'peek {S:08x} 40']
    def jr(v, i=0): return struct.unpack('<4HI', v[i][1][:12]), v[i + 1][1]
    # a version-11 save with two stars in its album: they become the journal's first stars, and it saves as v12 at 0x1000
    if os.path.exists(BASE11):
        al = g('album'); nm = g('names') + 8
        sc = BOOT + [p8(al + 4, 2), p8(al + 8, 3), p8(al + 9, 5), p8(al + 40, 15), p8(al + 41, 10)] + \
             [p8(nm + i, c) for i, c in enumerate(b'KIWI\0\0BEAN\0\0')] + [p8(g('life', 'retired'), 3), 'wait 1000', f'savesram {TMP}/v11.sav']
        run(BASE11, sc, copy_save('adult'))
        d11 = open(f'{TMP}/v11.sav', 'rb').read()
        v = peeks(run(ROM, ['wait 30'] + peek_jr + [f'savesram {TMP}/v12.sav'], f'{TMP}/v11.sav'))
        (n, raised, wonders, games, coins), st = jr(v)
        d12 = open(f'{TMP}/v12.sav', 'rb').read()
        check('journal: a v11 save\'s album becomes the first stars (KIWI the OAKHART, BEAN the ZIPPIT)',
              save_ver(d11) == 0 and n == 2 and raised == 3 and wonders == 3 and st[0:4] == b'KIWI' and st[6] == 15 and st[10:14] == b'BEAN' and st[16] == 10,
              f'n {n} raised {raised} {st[:20]}')
        check('journal: the converted save is the current version at 0x1000/0x2000', save_ver(d12) == CUR_VER, '')
        v2 = peeks(run(ROM, ['wait 30'] + peek_jr, f'{TMP}/v12.sav'))
        check('journal: it reloads from the new save', v2[:2] == v[:2], '')
    # a version-12 save kept the longest life where the wonderful-farewell count now lives: it is counted from the stars
    if os.path.exists(BASE12):
        sc = BOOT + [p8(JR, 3), *p16(JR + 4, 13)] + [p8(S + 10 * i + 7, e << 2) for i, e in enumerate((1, 2, 1))] + ['wait 1000', f'savesram {TMP}/v12b.sav']
        run(BASE12, sc, copy_save('adult'))
        v = peeks(run(ROM, ['wait 30'] + peek_jr + [f'savesram {TMP}/v13.sav'], f'{TMP}/v12b.sav'))
        (n, raised, wonders, *_), st = jr(v)
        check('journal: a v12 save counts its wonderful farewells from its stars (2 of 3)', n == 3 and wonders == 2 and save_ver(open(f'{TMP}/v13.sav', 'rb').read()) == CUR_VER, f'n {n} wonders {wonders}')
    # the farewell: it rises in a pixelated white-out (mosaic + brighten), then the picture zooms out from its new star
    sc = BOOT + care_set('c_adult', n=100, good=100, bad=0) + [p8(g('so_pend'), 1), 'wait 3', f'peek {g("mode"):08x} 1',
          'wait 520', 'peek 0400004c 2', 'peek 04000054 2', 'wait 60', 'peek 04000020 2', 'wait 200', 'peek 04000020 2', 'peek 0400004c 2']
    v = peeks(run(ROM, sc, copy_save('adult'))); w = [struct.unpack('<H', x[1][:2])[0] for x in v[1:]]
    check('journal: the farewell pixelates and whitens as it rises', v[0][1][0] == 3 and w[0] > 0 and w[1] > 0, str(w))
    check('journal: then zooms out from its new star, back to normal', 0x80 <= w[2] < 0x100 and w[3] == 0x100 and w[4] == 0, str([hex(x) for x in w]))
    # Phase 7 jingles: the journal opens with its own chime; the farewell's beam and new star; the runaway walks off
    syms = {l.split()[-1]: int(l.split()[0], 16) for l in subprocess.run(['llvm-objdump', '-t', f'{CK}/ext.elf'], capture_output=True, text=True).stdout.splitlines() if ' SEQ_' in l}
    v = pk(run(ROM, ['wait 30', p8(PET + 0x13, 1), 'wait 40', 'press DOWN', 'wait 4', 'press DOWN', 'wait 4', 'press A', 'wait 2', 'peek 03000288 4'], copy_save('adult')))
    a = pk(run(ROM, BOOT + [p8(PET + 0x13, 1)] + care_set('c_adult', n=100, good=100, bad=0) + [p8(g('so_pend'), 1), 'wait 224', 'peek 0300028c 4', 'wait 340', 'peek 03000288 4'], copy_save('adult')))
    r = pk(run(ROM, BOOT + [p8(PET + 0x13, 1), 'poke8 03000032 1', 'poke8 0300003e 0', 'poke8 0300003f 0', 'poke8 03000064 64', 'poke8 03000065 0', 'waitscene 9', 'wait 310', 'peek 03000288 4'], copy_save('adult')))
    got = [u32(v[0][1]), u32(a[0][1]), u32(a[1][1]), u32(r[0][1])]
    check('journal: the new jingles play (journal, beam, new star, runaway)', got == [syms['SEQ_JOURNAL'], syms['SEQ_BEAM'], syms['SEQ_NEWSTAR'], syms['SEQ_RUNAWAY']], str([hex(x) for x in got]))
    # v8.1: a v13 save (the v8 release) loads with its journal and saves as v14
    if os.path.exists(BASE13):
        sc = BOOT + [p8(JR, 2), *p16(JR + 2, 5), *p16(JR + 4, 2)] + [p8(S + i, c) for i, c in enumerate(b'MOCHI')] + ['wait 1000', f'savesram {TMP}/v13b.sav']
        run(BASE13, sc, copy_save('adult'))
        v = peeks(run(ROM, ['wait 30'] + peek_jr + [f'savesram {TMP}/v14.sav'], f'{TMP}/v13b.sav'))
        (n, raised, wonders, *_), st = jr(v)
        check('journal: a v13 save (the v8 release) keeps its journal and saves as the current version',
              n == 2 and raised == 5 and wonders == 2 and st[:5] == b'MOCHI' and save_ver(open(f'{TMP}/v14.sav', 'rb').read()) == CUR_VER, f'n {n} raised {raised} {st[:6]}')
    # v8.1 B2: a v14 save (B1: wishes, stories, the collection) keeps it all and gains a garden with three seeds
    if os.path.exists(BASE14):
        W = g('more', 'm_wishes'); F0 = g('more', 'm_found0')
        sc = BOOT + [*p16(W, 7), *p16(W + 2, 5), p32(F0, 0x2a), 'wait 1000', f'savesram {TMP}/v14b.sav']
        run(BASE14, sc, copy_save('adult'))
        v = peeks(run(ROM, ['wait 30', f'peek {W:08x} 4', f'peek {F0:08x} 4', f'peek {g("gdn", "g_seeds"):08x} 1', f'savesram {TMP}/v15.sav'], f'{TMP}/v14b.sav'))
        check('journal: a v14 save keeps its wishes, stories and collection, gains a garden, and saves as v15',
              struct.unpack('<2H', v[0][1]) == (7, 5) and struct.unpack('<I', v[1][1])[0] == 0x2a and v[2][1][0] == 3
              and save_ver(open(f'{TMP}/v15.sav', 'rb').read()) == 15, str(v))
    # a farewell writes the critter's story; running away counts it, but gives no star
    sc = BOOT + peek_jr + [p8(g('so_pend'), 1), 'wait 30', f'peek {g("mode"):08x} 1'] + peek_jr
    out = run(ROM, sc, copy_save('adult')); v = peeks(out)
    (n0, r0, w0, *_), _ = jr(v); (n1, r1, w1, *_), st = jr(v, 3)
    k = st[(n1 - 1) * 10:(n1 - 1) * 10 + 10] if n1 else b''
    check('journal: a farewell adds a star with its story', v[2][1][0] == 3 and n1 == n0 + 1 and r1 == r0 + 1 and k and (k[7] >> 2) in (1, 2, 3) and k[8] >= 1 and w1 - w0 == ((k[7] >> 2) == 1),
          f'mode {v[2][1][0]} n {n0}->{n1} raised {r0}->{r1} wonders {w0}->{w1} story {k}')
    sc = BOOT + peek_jr + [p8(PET + 4, 8), 'wait 5'] + peek_jr
    v = peeks(run(ROM, sc, copy_save('adult'))); (n0, r0, *_), _ = jr(v); (n1, r1, *_), _ = jr(v, 2)
    check('journal: a critter that runs away is counted, with no star', n1 == n0 and r1 == r0 + 1, f'n {n0}->{n1} raised {r0}->{r1}')
    # the screen: JOURNAL on the title opens it; L and R turn the tabs; SELECT menu's JOURNAL opens the field guide
    sc = ['wait 30', p8(JR, 3), 'wait 40', 'press DOWN', 'wait 4', 'press DOWN', 'wait 4', 'press A', 'wait 10',
          f'peek {g("mode"):08x} 1', f'peek {g("jtab"):08x} 2', 'press R', 'wait 4', f'peek {g("jtab"):08x} 1', 'press L', 'wait 4', 'press L', 'wait 4',
          f'peek {g("jtab"):08x} 1', 'press B', 'wait 10', f'peek {g("mode"):08x} 1', 'press UP', 'wait 4', 'press UP', 'wait 4', 'press A', 'waitscene 1', 'wait 10',
          'press SELECT', 'wait 10'] + ['press DOWN', 'wait 3'] * 3 + ['press A', 'wait 10', f'peek {g("mode"):08x} 1', f'peek {g("jtab"):08x} 1']   # (v8.1: in SELECT)
    v = peeks(run(ROM, sc, copy_save('adult')))
    m = [x[1][0] for x in v]
    check('journal: JOURNAL on the title opens the night sky on the latest star', m[0] == 10 and m[1] == 0 and v[1][1][1] == 2, str(m))
    check('journal: R and L turn the tabs (sky -> guide, then back past the sky to records)', m[2] == 1 and m[3] == 5, str(m))
    check('journal: B goes back to the title; the SELECT menu\'s JOURNAL opens the field guide', m[4] == 0 and m[5] == 10 and m[6] == 1, str(m))

def t_b1():
    """v8.1 chunk B1: the collection and the wishing stars."""
    F0 = g('more', 'm_found0'); INV = g('ext', 'e_inv'); COINS = g('ext', 'e_coins')
    # buying something opens its collection page for good (and it is still there after a reload)
    sc = BOOT + [*p16(COINS, 900), 'press SELECT', 'wait 6', 'press DOWN', 'wait 4', 'press DOWN', 'wait 4', 'press A', 'wait 20', f'peek {g("stock"):08x} 1', 'press A', 'wait 10',
                 f'peek {F0:08x} 8', f'savesram {TMP}/b1.sav']
    v = peeks(run(ROM, sc, copy_save('adult'))); it = v[0][1][0]; bits = int.from_bytes(v[1][1], 'little')
    check(f'collection: buying item {it} opens its page', (bits >> it) & 1 == 1 and bin(bits).count('1') == 1, hex(bits))
    v = peeks(run(ROM, ['wait 30', f'peek {F0:08x} 8'], f'{TMP}/b1.sav'))
    check('collection: the page stays open after a reload', int.from_bytes(v[0][1], 'little') == bits, '')
    # a save from before the collection counts whatever it already owns
    sc = BOOT + [p8(g('more', 'm_found_ok'), 0), p8(INV + 1, 2), p32(g('deco', 'd_owned'), 1), 'wait 1000', f'savesram {TMP}/b1b.sav']
    run(ROM, sc, copy_save('adult'))
    v = peeks(run(ROM, ['wait 30', f'peek {F0:08x} 9'], f'{TMP}/b1b.sav')); bits = int.from_bytes(v[0][1][:8], 'little')
    check('collection: an older save\'s owned items count as found (FISH, BED)', bits == (1 << 1) | (1 << 14) and v[0][1][8] == 1, hex(bits))
    # a wishing star crosses the night sky; A makes a wish (+5 coins, counted), and does not work the icon bar
    sc = BOOT + ['poke32 0300004c 1f4', 'wait 5', *p16(g('wish_cd'), 20), 'wait 40', f'peek {g("wish_t"):08x} 2', f'peek {COINS:08x} 2',
                 'press A', 'wait 6', f'peek {COINS:08x} 2', f'peek {g("more", "m_wishes"):08x} 2', 'peek 03000020 1', f'peek {g("mode"):08x} 1',
                 'press A', 'wait 6', f'peek {COINS:08x} 2']
    v = peeks(run(ROM, sc, copy_save('adult'))); w = [int.from_bytes(x[1], 'little') for x in v]
    check('wish: a wishing star crosses the window at night', w[0] > 0, str(w))
    check('wish: A makes a wish: +5 coins, counted in the journal', w[2] == w[1] + 5 and w[3] == 1, str(w))
    check('wish: the A is not passed on to the room', w[4] == 1 and w[5] == 0, str(w))
    check('wish: only one wish per star', w[6] == w[2], str(w))
    # v8.1: it says so, and A still works for about 3 s after it has crossed (not after that)
    sc = BOOT + ['poke32 0300004c 1f4', 'wait 5', *p16(g('wish_cd'), 20), 'wait 30', 'peek 030002a4 32', 'wait 210', f'peek {COINS:08x} 2', 'press A', 'wait 6', f'peek {COINS:08x} 2',
                 *p16(g('wish_cd'), 2), 'wait 420', f'peek {COINS:08x} 2', 'press A', 'wait 6', f'peek {COINS:08x} 2']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    msg = bytes(v[0]).split(b'\0')[0].decode('ascii', 'replace')
    check('wish: a shooting star says so (A: MAKE A WISH)', msg == 'A SHOOTING STAR! A: MAKE A WISH', msg)
    c = [struct.unpack('<H', x)[0] for x in v[1:]]
    check('wish: A still makes a wish just after it has crossed', c[1] == c[0] + 5, str(c))
    check('wish: but not long after', c[3] == c[2], str(c))
    sc = BOOT + ['poke32 0300004c 64', 'wait 5', *p16(g('wish_cd'), 20), 'wait 60', f'peek {g("wish_t"):08x} 2']
    v = peeks(run(ROM, sc, copy_save('adult')))
    check('wish: no wishing stars in the daytime', int.from_bytes(v[0][1], 'little') == 0, '')

def t_stories():
    """v8.3v: the bedtime stories are switched off (their code stays for later): no STORY row by day or at night."""
    MODE = g('mode')
    # the menu is OUTSIDE/GARDEN, BAG, SHOP, JOURNAL: up from the top is the JOURNAL, day and night
    for clock, when in (('64', 'by day'), ('1f4', 'at night')):
        v = peeks(run(ROM, BOOT + [f'poke32 0300004c {clock}', 'wait 5', p8(PET + 8, 1), 'press SELECT', 'wait 6', 'press UP', 'wait 4', f'peek {g("cur"):08x} 1', 'press A', 'wait 20', f'peek {MODE:08x} 1'], copy_save('adult')))
        check(f'stories: no STORY in the SELECT menu {when} (up from the top is the JOURNAL)', v[0][1][0] == 3 and v[1][1][0] == 10, str(v))

def t_garden():
    """v8.1 B2: the moonflower garden - plant, water, grow, bloom, fade; visitors; the journal keeps what was found."""
    GD = g('gdn'); MODE = g('mode'); DAY = g('ext', 'e_day'); FOUND = g('gdn', 'g_found'); SEEDS = g('gdn', 'g_seeds')
    night = ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4']
    morning = ['poke32 0300004c 1f4', 'wait 3', 'poke32 0300004c a', 'wait 4']
    garden = ['press SELECT', 'wait 6', 'press A', 'wait 10', 'press A', 'wait 4']   # (v8.1: A to tend)
    leave = ['press B', 'wait 6', 'press B', 'wait 8']
    pk = lambda: [f'peek {GD:08x} 18', f'peek {SEEDS:08x} 1']
    water = ['press A', 'wait 80']   # (v8.2b: A brings the watering can; keys wait till it has gone)
    sc = BOOT + [f'peek {SEEDS:08x} 4'] + night + garden + [f'peek {MODE:08x} 1', 'press A', 'wait 4'] + water + pk() + leave + [f'peek {MODE:08x} 1'] + \
         morning + pk() + morning + pk() + night + garden + water + leave + morning + pk() + \
         night + garden + water + leave + morning + pk() + [f'peek {FOUND:08x} 4'] + \
         night + garden + [f'peek {g("gdn", "g_vis_n"):08x} 4', f'peek {FOUND:08x} 4'] + leave + morning * 4 + pk() + [f'savesram {TMP}/gd.sav']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('garden: an older save starts with three seeds', v[0][0] == 3, str(v[0]))
    check('garden: SELECT > GARDEN at night while it sleeps', v[1][0] == 13, str(v[1]))
    check('garden: A plants a seed, and A again waters it', v[2][0] == 1 and v[2][4] & 1 and v[3][0] == 2, f'{list(v[2][:6])} seeds {v[3][0]}')
    check('garden: B goes back inside', v[4][0] == 0, str(v[4]))
    check('garden: by morning it sprouts', v[5][0] == 2 and not v[5][4] & 1, str(list(v[5][:6])))
    check('garden: a night without water: no growth, and it remembers the missed night', v[7][0] == 2 and v[7][4] & 2, str(list(v[7][:6])))
    check('garden: watered again: a bud, already showing its colour', v[9][0] == 3 and v[9][1] < 5, str(list(v[9][:6])))
    check('garden: and then it blooms, for four nights', v[11][0] == 4 and v[11][3] == 4 and v[11][1] < 9, str(list(v[11][:6])))
    found = int.from_bytes(v[13], 'little')
    check('garden: the bloom is found for the journal', found >> v[11][1] & 1, hex(found))
    vis = v[14]; found2 = int.from_bytes(v[15], 'little')
    check('garden: visitors come to a glowing bloom, and are found', 1 <= vis[0] <= 3 and all(found2 >> (9 + vis[2 + i]) & 1 for i in range(vis[0])), f'{list(vis)} {hex(found2)}')
    check('garden: after four nights the bloom fades and drops a seed', v[16][0] == 0 and v[17][0] == 3, f'{list(v[16][:6])} seeds {v[17][0]}')
    v2 = [x[1] for x in peeks(run(ROM, ['wait 30', f'peek {FOUND:08x} 4', f'peek {SEEDS:08x} 1'], f'{TMP}/gd.sav'))]
    check('garden: kept in the save', int.from_bytes(v2[0], 'little') == found2 and v2[1][0] == 3, str(v2))
    # v8.1: it opens just for looking; A tends (cursor), B stops tending, B again goes in; START: the journal and back
    TEND = g('gpad2'); look = ['press SELECT', 'wait 6', 'press A', 'wait 10']
    sc = BOOT + night + look + [f'peek {TEND:08x} 1', 'press LEFT', 'wait 4', f'peek {g("gcur"):08x} 1', 'press A', 'wait 4', f'peek {TEND:08x} 1', f'peek {GD:08x} 1',
         'press B', 'wait 4', f'peek {TEND:08x} 1', f'peek {MODE:08x} 1', 'press SELECT', 'wait 6', 'press A', 'wait 8', f'peek {MODE:08x} 1', f'peek {g("dex_from"):08x} 1',
         'press B', 'wait 8', f'peek {MODE:08x} 1', 'press B', 'wait 8', f'peek {MODE:08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('garden: opens just for looking (no cursor; left/right do nothing)', v[0] == 0 and v[1] == 0, str(v))
    check('garden: A tends, B stops tending but stays outside', v[2] == 1 and v[3] == 0 and v[4] == 0 and v[5] == 13, str(v))
    check('garden: SELECT > JOURNAL opens the journal, B goes back out to the yard', v[6] == 10 and v[7] == 2 and v[8] == 13, str(v))
    check('garden: B goes inside', v[9] == 0, str(v))
    # daytime: no garden, and no planting
    v = peeks(run(ROM, BOOT + ['poke32 0300004c 64', p8(PET + 12, 0), 'wait 4'] + garden + [f'peek {MODE:08x} 1', f'peek {SEEDS:08x} 1'], copy_save('adult')))
    check('garden: not in the daytime (OUTSIDE takes the critter out instead)', v[0][1][0] != 13 and v[1][1][0] == 3, str(v))
    # a new egg: a fresh garden, but the journal keeps what was found
    sc = BOOT + [p32(FOUND, 0x1234), p8(GD, 4), p8(GD + 1, 1), p8(GD + 3, 2), p8(PET + 4, 8), 'wait 30', p8(PET + 4, 0), 'wait 30',
                 f'peek {GD:08x} 1', f'peek {SEEDS:08x} 1', f'peek {FOUND:08x} 4']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('garden: a new seed brings a fresh garden; the journal keeps its finds', v[0][0] == 0 and v[1][0] == 3 and int.from_bytes(v[2], 'little') & 0x1234 == 0x1234, str(v))
    # the moon: eight phases, and planting on a full moon can grow a moon lily
    out = run(ROM, BOOT + night + garden + ['cyc 120 garden', 'press B', 'wait 8', p8(MODE, 10), p8(g('jtab'), 3), 'wait 10', 'cyc 120 gardentab'], copy_save('adult'))
    for label, limit in (('garden', 200000), ('gardentab', 220000)):
        m = re.search(label + r' avg busy cycles/frame: (\d+).*?max (\d+)', out)
        check(f'budget: {label} screen stays inside a frame', m and int(m.group(2)) < 270000 and int(m.group(1)) < limit, m.group(0) if m else out[-300:])

def t_fly():
    """v8.1: fireflies - rounds from the night yard, twelve kinds to find, the lantern and its morning release;
    also the night's slower clock, the clock in the yard, the rarer shooting stars and the scrolling journal tabs."""
    MODE = g('mode'); SP = g('more', 'm_spare'); FLY = g('fly'); DAY = g('ext', 'e_day')
    night = ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4']
    yard = ['press SELECT', 'wait 6', 'press A', 'wait 20']
    def put(i, x, y, k, t): a = FLY + 12 * i; return [*p16(a, x << 4), *p16(a + 2, y << 4), p8(a + 6, k), p8(a + 7, t), p8(a + 8, 1)]
    jar = lambda: [f'peek {g("jx"):08x} 2']
    # R starts a round; a lit firefly in the jar is caught: a new kind shows its card and is found; a known one just counts
    sc = BOOT + night + yard + ['press R', 'wait 10', f'peek {MODE:08x} 1', f'peek {g("fly_t"):08x} 2'] + put(0, 120, 92, 4, 9) + ['wait 1', 'press A', 'wait 6',
         f'peek {g("fly_state"):08x} 2', f'peek {SP + 6:08x} 2', 'press A', 'wait 6'] + put(1, 120, 92, 4, 9) + ['wait 1', 'press A', 'wait 6',
         f'peek {g("fly_state"):08x} 1', f'peek {g("fly_n"):08x} 1'] + [p8(FLY + 12 * i + 8, 0) for i in range(8)] + [p8(FLY + 12 * i + 9, 250) for i in range(8)] + \
         put(2, 120, 92, 0, 100) + ['wait 1', 'press A', 'wait 6', f'peek {g("fly_n"):08x} 1',
         'wait 1850', f'peek {g("fly_state"):08x} 1', f'peek {SP + 8:08x} 2', 'press B', 'wait 8', f'peek {MODE:08x} 1', f'savesram {TMP}/fly.sav']
    # v8.3: the 8th firefly in a grown-up's lantern in one night is GLIMMER's lantern (More.pad1 bit 2)
    P1 = g('more', 'm_pad1')
    sl = BOOT + night + yard + [p8(g('dex', 'd_phase'), 3), p8(g('ext', 'e_special'), 0), p8(P1, 0), p8(g('fly_n'), 6), 'press R', 'wait 10'] + \
         put(0, 120, 92, 4, 9) + ['wait 1', 'press A', 'wait 6', f'peek {P1:08x} 1', 'press A', 'wait 6'] + put(1, 120, 92, 4, 9) + ['wait 1', 'press A', 'wait 6', f'peek {P1:08x} 1', f'peek {g("fly_n"):08x} 1']
    w = [x[1] for x in peeks(run(ROM, sl, copy_save('adult')))]
    check('fly: v8.3: a grown-up\'s 8th firefly of the night fills its lantern (for GLIMMER)', not w[0][0] & 4 and w[1][0] & 4 and w[2][0] == 8, str(w))
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('fly: R in the night yard starts a 20 s round', v[0][0] == 14 and 1170 <= struct.unpack('<H', v[1])[0] <= 1200, str(v[:2]))
    check('fly: scooping a lit one catches it; a new kind shows its card and goes in the guide', v[2][0] == 1 and v[2][1] == 4 and struct.unpack('<H', v[3])[0] & 16, str(v[2:4]))
    check('fly: a kind already found just goes in the jar', v[4][0] == 0 and v[5][0] == 2, str(v[4:6]))
    check('fly: an unlit one gets away', v[6][0] == 2, str(v[6]))
    check('fly: the round ends after 30 s, and B goes back out to the yard', v[7][0] == 2 and v[9][0] == 13, str(v[7:10]))
    check('fly: the caught total is kept', struct.unpack('<H', v[8])[0] == 2, str(v[8]))
    v = peeks(run(ROM, ['wait 30', f'peek {SP + 6:08x} 2'], f'{TMP}/fly.sav'))
    check('fly: the guide is kept in the save', struct.unpack('<H', v[0][1])[0] & 16, str(v))
    # three rounds a night, then no more (v8.3: the score card's A goes back to the yard; R there starts the next round)
    rnd_ = ['press R', 'wait 6', 'press B', 'wait 6']
    sc = BOOT + night + yard + rnd_ + ['press A', 'wait 6', f'peek {MODE:08x} 1'] + rnd_ + ['press A', 'wait 6'] + rnd_ + \
         [f'peek {g("fly_state"):08x} 1', 'press A', 'wait 6', f'peek {MODE:08x} 1', 'press R', 'wait 6', f'peek {MODE:08x} 1', f'peek {g("gmsg"):08x} 26']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('fly: after a round, back to the yard; three rounds a night, then no more', v[0][0] == 13 and v[1][0] == 2 and v[2][0] == 13 and v[3][0] == 13 and bytes(v[4]).startswith(b'NO MORE FIREFLIES TONIGHT.'), str(v))
    # tonight's kinds: the three common ones, two of the five others; a full moon brings moonglow; no starfly with no blooms
    seen = set(); moon = None; star = 0
    for day in (8, 9, 10, 11, 12, 13):
        v = peeks(run(ROM, BOOT + [p8(DAY, day), f'wait {1 + day * 7}'] + night + yard + ['press R', 'wait 6', f'peek {SP + 14:08x} 2'], copy_save('adult')))
        m = struct.unpack('<H', v[0][1])[0] >> 4; seen.add(m)
        unc = bin(m & 0xF8).count('1')
        if day == 8: moon = m
        star |= m & 0x800
        check(f'fly: night {day}: the three common kinds and two of the others', m & 7 == 7 and unc == 2 and not (m & 0x200 and day != 8), hex(m))
    check('fly: a full moon brings the moonglow, other nights not', moon & 0x100 and all(not (m & 0x100) for m in seen if m != moon), str([hex(m) for m in seen]))
    check('fly: no starfly without two moonflowers in bloom', not star, '')
    check('fly: the uncommon pair changes from night to night', len(seen) >= 3, str([hex(m) for m in seen]))
    # the lantern: in the room all night; in the morning the fireflies fly home and it empties
    v = peeks(run(ROM, BOOT + [f'peek {DAY:08x} 2'], copy_save('adult'))); d = struct.unpack('<H', v[0][1])[0]
    sc = BOOT + night + [p8(g('fly_n'), 6), *p16(g('fly_kinds'), 0x13), *p16(g('fly_night'), d + 1), 'poke32 0300004c 257', 'wait 400', 'peek 030002a4 32', f'peek {g("fly_n"):08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    msg = bytes(v[0]).split(b'\0')[0].decode('ascii', 'replace')
    check('fly: in the morning the lantern\'s fireflies fly home', msg == 'BYE, FIREFLIES! THEY FLEW HOME.' and v[1][0] == 0, f'{msg} {v[1]}')
    # the night clock: two steps in three; the critter's own timer still runs at its usual pace; the clock runs in the yard too
    sc = BOOT + ['poke32 0300004c 1e0', p8(PET + 12, 1), 'wait 2', 'peek 0300004c 4', 'peek 03000044 4', 'wait 2700', 'peek 0300004c 4', 'peek 03000044 4',
                 'poke32 0300004c 64', p8(PET + 12, 0), 'wait 2', 'peek 0300004c 4', 'wait 2700', 'peek 0300004c 4']
    c = [struct.unpack('<I', x[1])[0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('clock: at night it moves two steps in three ticks (a 3 minute night); the critter\'s timer keeps its pace', 29 <= c[2] - c[0] <= 31 and c[3] - c[1] == 45, str(c))
    check('clock: by day it moves a step every tick (8 minutes of daylight)', 44 <= c[5] - c[4] <= 46, str(c))
    sc = BOOT + ['poke32 0300004c 24e', p8(PET + 12, 1), 'wait 4'] + yard + ['peek 0300004c 4', 'wait 1500', 'peek 0300004c 4', f'peek {MODE:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('clock: it runs in the yard at night, and dawn sends you back in', struct.unpack('<I', v[1])[0] < 20 and v[2][0] == 0, str(v))
    # shooting stars: decided once a night, about one in three nights, and just the one
    WCD = g('wish_cd'); n = 0
    for day in range(6):
        v = peeks(run(ROM, BOOT + [p8(DAY, day), 'poke32 0300004c 1e0', p8(PET + 12, 1), 'wait 4', f'peek {WCD:08x} 2'], copy_save('adult')))
        cd = struct.unpack('<H', v[0][1])[0]; n += cd != 0xFFFF
        check(f'wish: night {day}: decided at nightfall (none, or a step in the night)', cd == 0xFFFF or 486 <= cd < 582, hex(cd))
    check('wish: not every night has one', n < 6, str(n))
    sc = BOOT + ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4', *p16(WCD, 20), 'wait 10', f'peek {g("wish_t"):08x} 2', f'peek {WCD:08x} 2']
    v = [struct.unpack('<H', x[1])[0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('wish: after its star, no more that night', v[0] > 0 and v[1] == 0xFFFF, str(v))
    # the journal's tab strip scrolls to show FIREFLIES and RECORDS
    sc = BOOT + ['press SELECT', 'wait 6'] + ['press DOWN', 'wait 3'] * 3 + ['press A', 'wait 10', f'peek {g("jtab"):08x} 1', f'peek {g("jt_first"):08x} 1'] + \
         ['press R', 'wait 4'] * 3 + ['wait 4', f'peek {g("jtab"):08x} 1', f'peek {g("jt_first"):08x} 1', 'press R', 'wait 6', f'peek {g("jtab"):08x} 1', f'peek {g("jt_first"):08x} 1', 'cyc 60 flytab']
    out = run(ROM, sc, copy_save('adult')); v = [x[1][0] for x in peeks(out)]
    check('journal: the tabs scroll to show FIREFLIES and RECORDS', v[0] == 1 and v[1] == 0 and v[2] == 4 and v[3] >= 1 and v[4] == 5 and v[5] >= 2, str(v))
    # the round's screen stays inside a frame
    out = run(ROM, BOOT + night + yard + ['press R', 'wait 30', 'cyc 300 flyround'], copy_save('adult'))
    m = re.search(r'flyround avg busy cycles/frame: (\d+).*?max (\d+)', out)
    check('budget: a firefly round stays inside a frame', m and int(m.group(2)) < 270000, m.group(0) if m else out[-300:])

def t_rebal():
    """v8.1 rebalance: decor no longer raises happiness (the bed's morning still does), and a happy rise no longer
    counts as play by itself: play decor gives 1 play point a use. (Milk and soap: their table entries.)"""
    import re as _re
    src = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'ext.c')).read()
    m = _re.search(r'enum\s*\{\s*(IT_[A-Z]+[^}]*IT_COUNT)\s*\}', src)
    I = {n: i for i, n in enumerate(x.strip().split('=')[0].strip() for x in m.group(1).replace('\n', ' ').split(',') if x.strip())}
    SL = g('deco', 'k_slot')
    day = ['poke32 0300004c 64', p8(PET + 12, 0), p8(PET + 0x12, 1), 'wait 4', 'press RIGHT', 'wait 4', 'press A', 'wait 20']
    def idle(dec):
        sc = BOOT + dec + day + [*care_set('c_adult', n=0, good=0, bad=0, yard=0, garden=0, play=0), p8(PET + 6, 4), p8(PET + 7, 2)]
        for k in range(10): sc += ['wait 1200', p8(PET + 6, 4), p8(PET + 11, 0)]
        sc += [f'peek {cw("c_adult", "w_play"):08x} 2', f'peek {PET + 7:08x} 1']
        v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
        return struct.unpack('<H', v[0])[0], v[1][0]
    play, happy = idle([p8(SL + 0, I['IT_TOYBOX']), p8(SL + 1, I['IT_ARCADE']), p8(SL + 2, I['IT_GUITAR']), p8(SL + 3, I['IT_DISCO'])])
    check('rebalance: decor no longer raises happiness (it can only drop)', happy <= 2, f'happy {happy}')
    check('rebalance: play decor still counts as play, 1 point a use', 4 <= play <= 12, f'play {play}')
    # bed: still happy in the morning
    sc = BOOT + [p8(SL + 0, I['IT_BED'])] + ['poke32 0300004c 257', p8(PET + 12, 1), p8(PET + 7, 1), *p16(PET + 0x2A, 0), 'wait 400', f'peek {PET + 7:08x} 1', 'peek 030002a4 24']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('rebalance: the bed still wakes it up happy +1', v[0][0] >= 2 and b'SLEPT GREAT' in bytes(v[1]), str(v))

def t_health():
    """v8.1: health stays hidden, but a critter that falls below a third of it says so - once, until it is well again."""
    W = g('weak_warned')
    sc = BOOT + ['poke32 0300004c 64', p8(PET + 12, 0), *p16(PET + 0x16, 36), p8(PET + 10, 1), *p16(PET + 0x3C, 40), p8(PET + 6, 4),
                 'wait 1200', 'peek 030002a4 40', f'peek {W:08x} 1', p8(0x030002A4, 0), 'wait 900', 'peek 030002a4 4', f'peek {PET + 0x16:08x} 2']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    msg = bytes(v[0]).split(b'\0')[0].decode('ascii', 'replace')
    check('health: below a third, it says it looks weak', msg.endswith('LOOKS WEAK... IT NEEDS CARE!') and v[1][0] == 1, msg)
    check('health: and only once while it stays low', v[2][0] == 0 and struct.unpack('<H', v[3])[0] < 34, str(v[2:]))
    sc = BOOT + ['poke32 0300004c 64', p8(PET + 12, 0), *p16(PET + 0x16, 80), 'wait 900', f'peek {W:08x} 1']
    v = peeks(run(ROM, sc, copy_save('adult')))
    check('health: a healthy critter gives no warning', v[0][1][0] == 0, str(v))

def t_sprites():
    """v8.2: hardware sprites - the fireflies fly home through the open window as glowing sprites, and the farewell
    star is a sprite that stays sharp through the white-out; both switch themselves off afterwards."""
    SPT = {m.group(1): int(m.group(2)) for m in re.finditer(r'#define SPT_(\w+) (\d+)', open(f'{CK}/sprfx.h').read())}
    DAY = g('ext', 'e_day'); LIVE = g('spr_live')
    night = ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4']
    disp = ['wait 1', 'peek 04000000 2'] * 3
    v = peeks(run(ROM, BOOT + [f'peek {DAY:08x} 2'], copy_save('adult'))); d = struct.unpack('<H', v[0][1])[0]
    morning = [p8(g('fly_n'), 6), *p16(g('fly_kinds'), 0x13), *p16(g('fly_night'), d + 1), 'wait 30', 'poke32 0300004c 257']
    sc = BOOT + night + [p8(g('fly_n'), 6), *p16(g('fly_kinds'), 0x13), *p16(g('fly_night'), d + 1), 'wait 30', f'peek {LIVE:08x} 1', 'peek 07000000 80'] + \
         ['poke32 0300004c 257', f'waitb {g("fly_rel"):08x} 20', f'peek {LIVE:08x} 1'] + \
         [f'waitb {g("fly_rel"):08x} 100', f'peek {LIVE:08x} 1', f'peek {g("spr_n"):08x} 1', f'peek {g("spr_win"):08x} 1', f'peek {g("win"):08x} 4', 'peek 07000000 96', 'peek 04000050 2'] + disp + \
         [x for _ in range(3) for x in ('wait 1', f'peek {g("spr_win"):08x} 1')] + \
         ['cyc 60 release', 'wait 300', f'peek {LIVE:08x} 1', f'peek {g("fly_rel"):08x} 2'] + disp
    out = run(ROM, sc, copy_save('adult')); w = [x[1] for x in peeks(out)]; jar = w[:2]; v = w[2:]
    oam = [struct.unpack('<4H', jar[1][i:i + 8]) for i in range(0, 80, 8)]   # (v8.3: the fireflies inside are sprites too, before it)
    a = next((o for o in oam if o[2] & 0x3FF == SPT['SGLOW']), oam[0])
    check('sprites: at night the jar glows (a soft halo sprite, not specks)', jar[0][0] == 1 and a[2] & 0x3FF == SPT['SGLOW'] and a[0] & 0x0C00 == 0x0400, f'{jar[0]} {a}')
    check('sprites: the glow stays while the jar is still shut in the morning', v[0][0] == 1, str(v[0]))
    oam = struct.unpack('<48H', v[5]); tiles = {oam[i * 4 + 2] & 0x3FF for i in range(v[2][0])}
    check('sprites: the fireflies fly out as glowing sprites (heart and glow)', v[1][0] == 1 and v[2][0] >= 2 and tiles <= {SPT['FGLOW'], SPT['FCORE'], SPT['FCORE2']} and SPT['FGLOW'] in tiles, f'{list(v[1:3])} {tiles}')
    check('sprites: all of them are blended (semi-transparent), the glow is affine', all(oam[i * 4] & 0x0C00 == 0x0400 for i in range(v[2][0])) and any(oam[i * 4] & 0x0100 for i in range(v[2][0])), str(oam[:12]))
    # (the window as the hardware has it: DISPCNT's window 0 bit in the frames sampled, and its corners. The spr_win
    # flag itself drops for a stretch near the end of the flight, so it is not what is checked)
    dcs = [struct.unpack('<H', x)[0] for x in v[7:10]]
    check('sprites: window 0 keeps them inside the open window', any(x & 0x2000 for x in dcs) and 22 < v[4][0] < v[4][1] < 70 and v[4][2] == 45 and v[4][3] == 80, f'{list(v[4])} dispcnt {[hex(x) for x in dcs]}')
    dc = [struct.unpack('<H', x)[0] for x in v[7:10]]
    check('sprites: OBJ, 1D mapping and window 0 are switched on', any(x & 0x3040 == 0x3040 for x in dc) and struct.unpack('<H', v[6])[0] & 0x2400 == 0x2400, str([hex(x) for x in dc]))
    m = re.search(r'release avg busy cycles/frame: (\d+) \(of \d+\) max (\d+)', out); cy, mx = (int(m.group(1)), int(m.group(2))) if m else (-1, -1)
    check(f'sprites: the release never runs over a frame (avg {cy}, busiest {mx} of 280896)', 0 < cy and 0 < mx < 280896, out[-300:])
    dc = [struct.unpack('<H', x)[0] for x in v[15:18]]
    check('sprites: when the window has shut they are gone and OBJ is off again', v[13][0] == 0 and struct.unpack('<H', v[14])[0] == 0 and all(not x & 0x1000 for x in dc), f'{list(v[13:15])} {[hex(x) for x in dc]}')
    # v8.3: fireflies all over the night yard (the garden screen): sprites, none of them during a round
    sc = BOOT + night + ['press SELECT', 'wait 6', 'press A', 'wait 60', f'peek {g("spr_shown"):08x} 1', 'press R', 'wait 30', f'peek {g("mode"):08x} 1', f'peek {g("spr_shown"):08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('sprites: v8.3: fireflies drift all over the night yard (15, as sprites), not during a round', 14 <= v[0] <= 30 and v[1] == 14 and v[2] < 14, str(v))
    # the farewell star
    res = {}
    for kind, care in (('wonder', 100), ('sad', 30)):
        sc = BOOT + care_set('c_adult', n=100, good=care, bad=100 - care) + [p8(g('so_pend'), 1), 'wait 545', f'peek {LIVE:08x} 1', 'peek 07000000 8',
              'wait 105', f'peek {LIVE:08x} 1', 'peek 07000000 16', 'peek 05000200 32', 'wait 200', 'press A', 'wait 10', f'peek {LIVE:08x} 1'] + disp
        res[kind] = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    v = res['wonder']; a = struct.unpack('<4H', v[1]); b = struct.unpack('<8H', v[3])
    check('sprites: through the white-out the star is sharp (its rays drawn solid)', v[0][0] == 1 and a[2] & 0x3FF == SPT['SRAYS'] and a[0] & 0x0C00 == 0, str(a))
    check('sprites: in the night sky the star has turning rays and a halo (double-size affine)', v[2][0] == 1 and b[2] & 0x3FF == SPT['SRAYS'] and b[6] & 0x3FF == SPT['SGLOW'] and b[0] & 0x0300 == 0x0300, str(b))
    lum = lambda x: sum(sum(((c >> s) & 31) for s in (0, 5, 10)) for c in struct.unpack('<16H', x))
    check('sprites: a sad goodbye leaves a fainter star', lum(res['sad'][4]) < lum(v[4]) * 3 // 4, f'{lum(res["sad"][4])} vs {lum(v[4])}')
    dc = [struct.unpack('<H', x)[0] for x in v[6:9]]
    check('sprites: after the farewell the sprites are off', v[5][0] == 0 and all(not x & 0x1000 for x in dc), f'{v[5]} {[hex(x) for x in dc]}')

def t_yard():
    """v8.2b: the yard keeps its own mess; LIGHTS outside; save and quit in the yard comes back to the yard; the
    moonflowers keep their colours on every yard screen; plant, then water with the can; a faint firefly counts;
    eight grown-up days."""
    MORE0 = g('more', 'm_pad0'); MORE1 = g('more', 'm_pad1'); OUT = g('outside'); POOP = PET + 11
    YPN = g('jrec') + 1   # (State.yp_n: the yard's poops as drawn)
    yard = ['press SELECT', 'wait 6', 'press A', 'wait 40']
    soon = lambda: [*p16(PET + 0x2C, 500), *p16(PET + 0x2E, 1), 'wait 100']   # it poops at the next clock step
    # the yard's mess: it poops outside, CLEAN outside cleans only the yard, inside the room's mess is its own
    sc = BOOT + DAY + [f'peek {POOP:08x} 1'] + yard + soon() + soon() + [f'peek {YPN:08x} 1', f'peek {MORE0:08x} 2', f'peek {POOP:08x} 1'] + \
         ['press RIGHT', 'wait 4'] * 4 + ['press A', 'wait 220', f'peek {YPN:08x} 1'] + soon() + \
         ['press SELECT', 'wait 6', 'press A', 'wait 60', f'peek {OUT:08x} 1', f'peek {POOP:08x} 1', f'peek {MORE0:08x} 2']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    room = v[0][0]
    # (since the port: PET[P_POOP] is always the room's mess, More.pad0 always the yard's; nothing is swapped)
    check('yard: the critter poops outside too (the yard\'s own mess)', v[1][0] == 2 and v[2][0] == 2 and v[3][0] == room, str(v[:4]))
    check('yard: CLEAN outside cleans the yard', v[4][0] == 0, str(v[4]))
    check('yard: back inside, the room has its own mess and the yard keeps its', v[5][0] == 0 and v[6][0] == room and v[7][0] == 1, str(v[5:]))
    # LIGHTS outside: a message, and nothing else changes
    sc = BOOT + DAY + yard + ['press RIGHT', 'wait 6', 'press A', 'wait 20', f'peek {OUT:08x} 1', f'peek {PET + 13:08x} 1', 'peek 030002a4 16']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: LIGHTS outside says YOU\'RE OUTSIDE! and changes nothing', v[0][0] == 1 and v[1][0] == 0 and bytes(v[2]).startswith(b"YOU'RE OUTSIDE!"), str(v))
    # save and quit in the yard (with a mess out there), CONTINUE: back in the yard, the mess still there
    sv = copy_save('adult')
    sc = BOOT + DAY + yard + soon() + ['press START', 'wait 8', 'press UP', 'wait 4', 'press A', 'wait 8', 'press A', 'wait 60', f'peek {MORE1:08x} 1']
    run(ROM, sc, sv)
    sc = ['wait 60', 'press A', 'waitscene 1', 'wait 30', f'peek {OUT:08x} 1', f'peek {YPN:08x} 1', f'peek {MORE1:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, sv))]
    check('yard: save and quit in the yard; CONTINUE goes back out there, with its mess', v[0][0] == 1 and v[1][0] == 1 and not v[2][0] & 1, str(v))
    # the flowers' colours on yard screens (palette 40 is the garden's first colour)
    gp0 = int(re.search(r'GP_PAL\[GP_N\]=\{(\d+)', open(f'{CK}/garden.h').read()).group(1))
    sc = BOOT + DAY + yard + ['press SELECT', 'wait 10', 'peek 05000050 2', 'press B', 'wait 10', 'press A', 'wait 12', 'press A', 'wait 40', 'peek 05000050 2', 'scene']
    out = run(ROM, sc, copy_save('adult')); v = [struct.unpack('<H', x[1])[0] for x in peeks(out)]
    check('yard: the moonflowers keep their colours under the SELECT menu and while it eats', v[0] == gp0 and v[1] == gp0, f'{v} want {gp0}')
    # plant, then water: planted dry, A again brings the can
    GD = g('gdn')
    sc = BOOT + ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4', p8(GD + 6, 0), p8(g('gdn', 'g_seeds'), 3), 'press SELECT', 'wait 6', 'press A', 'wait 10', 'press A', 'wait 4',
                 'press RIGHT', 'wait 4', 'press A', 'wait 6', f'peek {GD + 6:08x} 6', 'press A', 'wait 2', f'peek {g("gwater"):08x} 1', f'peek {GD + 6:08x} 6',
                 'press B', 'wait 4', f'peek {g("mode"):08x} 1', 'wait 80', f'peek {g("gwater"):08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: A plants a seed in dry soil', v[0][0] == 1 and not v[0][4] & 1, str(list(v[0])))
    check('yard: A again waters it: the watering can comes, keys wait for it', v[1][0] > 60 and v[2][4] & 1 and v[3][0] == 13 and v[4][0] == 0, str(v[1:]))
    # v8.2c: while the can is out, the cursor can't move to another patch
    sc = BOOT + ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4', p8(GD + 6, 1), p8(GD + 10, 0), 'press SELECT', 'wait 6', 'press A', 'wait 10', 'press A', 'wait 4',
                 'press RIGHT', 'wait 4', 'press A', 'wait 2', 'press RIGHT', 'wait 4', 'press LEFT', 'wait 4', f'peek {g("gcur"):08x} 1', f'peek {g("gwater"):08x} 1',
                 'wait 80', 'press RIGHT', 'wait 4', f'peek {g("gcur"):08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: while the can is out the cursor stays on its patch; after, it moves', v[0] == 1 and v[1] > 0 and v[2] == 2, str(v))
    # a firefly glowing only faintly can be scooped now
    FLY = g('fly'); night = ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4']
    sc = BOOT + night + yard[:3] + ['wait 20', 'press R', 'wait 10'] + [p8(FLY + 12 * i + 8, 0) for i in range(8)] + [p8(FLY + 12 * i + 9, 250) for i in range(8)] + \
         [*p16(FLY, 120 << 4), *p16(FLY + 2, 92 << 4), p8(FLY + 6, 0), p8(FLY + 7, 1), p8(FLY + 8, 1), 'press A', 'wait 4', f'peek {g("fly_n"):08x} 1']
    v = peeks(run(ROM, sc, copy_save('adult')))
    check('yard: a faintly glowing firefly can be scooped', v[-1][1][0] >= 1, str(v[-1]))
    # v8.3: SELECT over the night yard scrolls down to CLEAN UP, which cleans the yard while it sleeps inside
    night = ['poke32 0300004c 1f4', p8(PET + 12, 1), 'wait 4']
    # (v8.3: that menu shows only what works there: GO INSIDE, BAG, SHOP, CLEAN UP while there's a mess, JOURNAL)
    sc = BOOT + night + [p8(MORE0, 3), 'press SELECT', 'wait 6', 'press A', 'wait 30', 'press SELECT', 'wait 8', 'press UP', 'wait 6',
                         f'peek {g("cur"):08x} 1', 'press A', 'wait 10', f'peek {MORE0:08x} 1', f'peek {g("mode"):08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: the night-yard menu has CLEAN UP while the yard has a mess; it cleans the yard', v == [3, 0, 13], str(v))   # (GO INSIDE, BAG, SHOP, CLEAN UP, JOURNAL)
    # ...and with the yard clean, there's no CLEAN UP row: the menu has its five rows and doesn't scroll
    sc = BOOT + night + [p8(MORE0, 0), 'press SELECT', 'wait 6', 'press A', 'wait 30', 'press SELECT', 'wait 8', f'peek {g("cur"):08x} 1', 'press DOWN', 'wait 4', 'press DOWN', 'wait 6',
                         f'peek {g("cur"):08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: with the yard clean, the night-yard menu has no CLEAN UP', v == [3, 1], str(v))   # (GO INSIDE, BAG, SHOP, JOURNAL)
    # v8.3: while it sleeps, the yard's mess can't make it sick (the room's still can)
    sc = BOOT + night + [p8(MORE0, 4), p8(POOP, 0), p8(PET + 10, 0), 'wait 5400', f'peek {PET + 10:08x} 1', f'peek {PET + 0x3A:08x} 2', f'peek {MORE0:08x} 1']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: asleep, the yard\'s mess doesn\'t count toward getting sick', v[0][0] == 0 and v[1] == b'\x00\x00' and v[2][0] == 4, str(v))
    # v8.3: a new seed clears the "tonight" marks (they're day+1 and its days start again at 0), so its first night
    # isn't already used up: before, "NO MORE FIREFLIES TONIGHT." could come before any round
    SP = g('more', 'm_spare')
    sv = copy_save('adult')
    run(ROM, BOOT + [*p16(SP + 12, 1), *p16(SP + 14, 3), *p16(SP + 2, 1), 'wait 4', 'press START', 'wait 8', 'press UP', 'wait 4', 'press A', 'wait 8', 'press A', 'wait 60'], sv)
    sc = ['wait 60', 'press DOWN', 'wait 10', 'press A', 'wait 20', 'press A', 'wait 20', 'press LEFT', 'wait 10', 'press A', 'waitscene 1', 'wait 30', f'peek {SP:08x} 16']
    v = struct.unpack('<8H', peeks(run(ROM, sc, sv))[0][1])
    check('yard: a new seed starts with fresh nights (firefly rounds, wish and story marks cleared)', v[0] == 0 and v[1] == 0 and v[6] == 0 and v[7] == 0, str(v))
    # v8.3: START opens the menu over the night yard too; B goes back to the yard
    M = g('mode')
    sc = BOOT + night + ['press SELECT', 'wait 6', 'press A', 'wait 30', 'press START', 'wait 8', f'peek {M:08x} 1', 'press B', 'wait 8', f'peek {M:08x} 1', 'peek 03000020 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: START opens the menu over the night yard, and B goes back out there', v == [1, 13, 1], str(v))
    # v8.3: A skips the rest of a MEAL (the original finishes it: same weight as waiting), and of food from the bag
    res = []
    for skip in (0, 1):
        sc = BOOT + DAY + ['press A', 'wait 6', 'press A', 'wait 20'] + (['press A', 'wait 6', 'peek 03000020 1'] if skip else []) + ['wait 150', f'peek {PET + 9:08x} 1']
        res.append([x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))])
    INV = g('ext', 'e_inv')
    sc = BOOT + DAY + [p8(INV, 3), 'wait 2', 'press SELECT', 'wait 6', 'press DOWN', 'wait 4', 'press A', 'wait 10', 'press A', 'wait 15', 'press A', 'wait 3', f'peek {M:08x} 1', f'peek {INV:08x} 1']
    b = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: A skips the rest of a meal (it still counts) and of food from the bag', res[1][0] != 3 and res[1][1] == res[0][0] and b == [0, 2], f'{res} {b}')
    # v8.3: the firefly jar's stand comes with the first firefly ever found: in the middle under the window when the
    # left spot is empty, right of the bed (or any left piece) when it isn't. At dawn the jar floats to the window first
    # (the window stays shut until it's there), then back to its stand.
    # (v8.3t: the stand is this critter's: More.pad1 bit 4 is set by its first catch, and a new egg clears it)
    K = g('deco', 'k_slot'); SB = g('st_baked'); P1 = g('more', 'm_pad1')
    res = []
    for found, left in ((0, 0xFF), (1, 0xFF), (1, 14)):
        sc = BOOT + DAY + [p8(K, left), p8(P1, 16 * found), p8(g('fly_n'), 0), p8(g('bg_state'), 0), 'wait 10', f'peek {SB:08x} 1']
        res.append(peeks(run(ROM, sc, copy_save('adult')))[0][1][0])
    check('yard: the jar\'s stand comes with its first firefly: under the window, or right of the bed', res == [0, 1, 2], str(res))
    sc = BOOT + DAY + [p8(K, 0xFF), p8(P1, 16), p8(g('fly_n'), 0), p8(g('bg_state'), 0), 'wait 10', f'peek {SB:08x} 1', f'peek {P1:08x} 1',
                       p8(PET + 4, 8), 'wait 5', p8(PET + 4, 0), 'wait 10', f'peek {P1:08x} 1', f'peek {SB:08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: a new egg has no stand (and no jar) until it catches its own first firefly', v[0] == 1 and v[1] & 16 and not v[2] & 16 and v[3] == 0, str(v))
    sc = BOOT + DAY + [p8(K, 14), p8(P1, 16), p8(g('fly_n'), 5), *p16(g('fly_kinds'), 7), *p16(g('jar_t'), 0), *p16(g('jar_back'), 0), *p16(g('fly_rel'), 1), 'wait 20',
                       f'peek {g("jar_t"):08x} 2', f'peek {g("fly_rel"):08x} 2', 'wait 1400', f'peek {g("fly_rel"):08x} 2', f'peek {g("jar_back"):08x} 2', f'peek {g("fly_n"):08x} 1']
    v = [struct.unpack('<H', x[1])[0] if len(x[1]) == 2 else x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: at dawn the jar floats to the window before it opens, and comes back after', 0 < v[0] < 64 and v[1] == 1 and v[2] == 0 and v[3] == 0 and v[4] == 0, str(v))
    # v8.3: BAG and SHOP open over the night yard too, and go back out there
    sc = BOOT + night + yard[:4] + ['press SELECT', 'wait 8', 'press UP', 'wait 4', 'press UP', 'wait 4', 'press A', 'wait 10', f'peek {M:08x} 1', 'press B', 'wait 10', f'peek {M:08x} 1',
                                    'press SELECT', 'wait 8', 'press UP', 'wait 4', 'press A', 'wait 10', f'peek {M:08x} 1', 'press B', 'wait 10', f'peek {M:08x} 1', 'peek 03000020 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: BAG and SHOP open over the night yard and go back to it', v == [5, 13, 4, 13, 1], str(v))
    # v8.3: the ceiling piece keeps its own colours (244-255) in the room at night; the jar's fireflies are sprites
    hdr = open(f'{CK}/decor.h').read()
    ents = re.findall(r'\{\{([^}]*)\},\{([^}]*)\}\}', re.search(r'DEC_PAL\[DEC_N\]\[2\]\[DEC_BANDN\]=\{(.*?)\};', hdr, re.S).group(1))
    want = [int(x) for x in ents[24 - 14][1].split(',')]   # the lantern (24), its night colours
    # (v8.3w: at clock 530, past the sunset tint, which ends at 520)
    sc = BOOT + ['poke32 0300004c 212', 'wait 4', p8(K + 3, 24), p8(P1, 16), p8(g('fly_n'), 5), *p16(g('fly_kinds'), 7), p8(g('bg_state'), 0), 'wait 30', 'peek 050001e8 24']
    got = list(struct.unpack('<12H', peeks(run(ROM, sc, copy_save('adult')))[0][1]))
    check('yard: the ceiling piece keeps its colours at night (no firefly colours over them)', got == want, f'{got} want {want}')
    # v8.3: themed spots. An old save's pieces move to their own spots (the fishbowl to the new corner; the plant, whose
    # spot is then taken, back into the bag); the toy box goes at the bed's foot, only with the bed
    PAD = g('deco', 'd_pad'); M1 = g('more', 'm_pad1')
    sv = copy_save('adult')
    run(ROM, BOOT + [p8(K + i, 0xFF) for i in range(6)] + [p8(K, 15), p8(K + 1, 16), p8(K + 2, 18), p8(PAD, 0), 'wait 4',
                     'press START', 'wait 8', 'press UP', 'wait 4', 'press A', 'wait 8', 'press A', 'wait 60'], sv)
    v = [x[1] for x in peeks(run(ROM, BOOT + [f'peek {K:08x} 6', f'peek {PAD:08x} 1'], sv))]
    check('yard: an old save\'s pieces move to their themed spots (fishbowl to the corner, the plant back in the bag)', list(v[0][:3]) == [0xFF, 0xFF, 18] and v[1][0] == 15, f'{list(v[0])} {v[1][0]}')
    bag = ['press SELECT', 'wait 6', 'press DOWN', 'wait 4', 'press A', 'wait 10']
    setup = BOOT + DAY + [p8(K + i, 0xFF) for i in range(6)] + [p8(PAD, 0xFF), p8(M1, 0), p32(g('deco', 'd_owned'), 1 | 64), p8(g('ext', 'e_toys'), 0)] + \
            [p8(g('ext', 'e_inv') + i, 0) for i in range(11)] + ['wait 4']
    sc = setup + bag + ['press DOWN', 'wait 4', 'press A', 'wait 10', f'peek {M1:08x} 1'] + bag + ['press A', 'wait 10'] + bag + ['press DOWN', 'wait 4', 'press A', 'wait 10',
         f'peek {M1:08x} 1', f'peek {K:08x} 1'] + bag + ['press A', 'wait 10', f'peek {M1:08x} 1', f'peek {K:08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: the toy box only goes at the bed\'s foot, and goes away with the bed', v[0] & 8 == 0 and v[1] & 8 and v[2] == 14 and v[3] & 8 == 0 and v[4] == 0xFF, str(v))
    # v8.3: bond is gone. Petting only purrs; answering a call gives happy +1 ("IT FEELS LOVED!"), whatever the rest
    nav = ['press DOWN', 'wait 4', 'press RIGHT', 'wait 4', 'press RIGHT', 'wait 4']
    def pet(call):
        sc = BOOT + DAY + [p8(PET + 7, 2), p8(PET + 8, 1), *p16(PET + 0x40, 0), 'wait 2'] + nav + ['wait 90', p8(PET + 14, call), f'peek {PET + 7:08x} 1', 'press A', 'wait 120',
             f'peek {PET + 7:08x} 2', f'peek {PET + 14:08x} 1', 'peek 030002a4 20']
        return [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    a_, b_ = pet(0), pet(1)
    check('yard: petting only purrs; answering a call gives happy +1 (no bond)', a_[1][0] <= a_[0][0] and a_[1][1] == 1 and a_[3].startswith(b'PURR') and
          b_[1][0] == b_[0][0] + 1 and b_[1][1] == 1 and b_[2][0] == 0 and b_[3].startswith(b'IT FEELS LOVED'), f'{[list(x) for x in a_[:2]]} {a_[3][:16]} {[list(x) for x in b_[:3]]} {b_[3][:16]}')
    # the gift: happy +1 and a surprise inside (a seed, coins, a treat or a decor piece)
    INV = g('ext', 'e_inv'); GIFT = 10; SEEDS = g('gdn', 'g_seeds'); CO = g('ext', 'e_coins')
    sc = BOOT + DAY + [p8(INV + i, 0) for i in range(11)] + [p8(INV + GIFT, 1), p8(PET + 7, 1), p8(SEEDS, 0), *p16(CO, 0), 'wait 2'] + \
         ['press SELECT', 'wait 6', 'press DOWN', 'wait 4', 'press A', 'wait 10', f'peek {PET + 7:08x} 1', 'press A', 'wait 4', f'peek {PET + 7:08x} 1', 'peek 030002a4 30',
          f'peek {SEEDS:08x} 1', f'peek {CO:08x} 2', f'peek {INV:08x} 11', f'peek {g("deco", "d_owned"):08x} 4']
    v = [x[1] for x in peeks(run(ROM, sc, copy_save('adult')))]
    got = v[3][0] > 0 or struct.unpack('<H', v[4])[0] > 0 or sum(v[5][:10]) > 0
    check('yard: a gift gives happy +1 and a surprise inside', v[1][0] == min(4, v[0][0] + 1) and b'INSIDE' in v[2] and (got or b' A ' in v[2]), f'{v[0][0]}->{v[1][0]} {v[2].split(bytes(1))[0]}')
    # v8.2c: running away is a six-shot scene: its room at dawn, a look back from the yard, the meadow path, the
    # stream where wild critters take it in, the empty place in the night sky, the end card
    md0 = int(re.search(r'MD_PAL\[MD_N\]=\{(\d+)', open(f'{CK}/meadow.h').read()).group(1))
    sc = BOOT + DAY + [*p16(PET + 0x16, 1), p8(PET + 6, 0), *p16(PET + 0x36, 200), 'waitscene 9']
    for w in (100, 300, 300, 300, 300, 250): sc += [f'wait {w}', f'peek {g("gone_t"):08x} 1', 'peek 05000120 2']
    out = run(ROM, sc, copy_save('adult')); v = peeks(out)
    shots = [v[i][1][0] for i in range(0, 12, 2)]; pal = struct.unpack('<H', v[7][1])[0]
    check('yard: running away plays its six shots in order (room, yard, meadow, stream, sky, end)', shots == [1, 2, 3, 4, 5, 6], str(shots))
    check('yard: the meadow and stream scenes have their own palette', pal == md0, f'{pal} want {md0}')
    # the stream: two Bliplets as 256-colour sprites, in the Bliplet's own colours (the bitmap shows the grown-ups')
    crp = re.search(r'CR_PAL\[CR_FORMS\]\[CR_SLOTS\]=\{\{[^}]*\},\{(\d+)', open(f'{CK}/critters.h').read()).group(1)
    sc = BOOT + DAY + [*p16(PET + 0x16, 1), p8(PET + 6, 0), *p16(PET + 0x36, 200), 'waitscene 9', 'wait 1000', 'peek 07000000 16', 'peek 050002f0 2']
    v = peeks(run(ROM, sc, copy_save('adult'))); o = struct.unpack('<8H', v[0][1]); pc = struct.unpack('<H', v[1][1])[0]
    check('yard: by the stream, two Bliplets (256-colour sprites in their own colours)', o[0] & 0x2000 and o[4] & 0x2000 and pc == int(crp), f'{[hex(x) for x in o]} {pc} want {crp}')
    # v8.2c: the RECORDS tab scrolls
    sc = BOOT + DAY + ['press SELECT', 'wait 6', p8(g('mode'), 10), p8(g('jtab'), 5), 'wait 6'] + ['press DOWN', 'wait 4'] * 4 + [f'peek {g("jrec"):08x} 1', 'press UP', 'wait 4', f'peek {g("jrec"):08x} 1']
    v = [x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))]
    check('yard: RECORDS scrolls down to its last rows and back up', v == [2, 1], str(v))
    # v8.3: weight is kept at least its stage's lightest (baby 5, child 10, teen 20, grown-up 30); the overweight line is unchanged
    W = PET + 9
    v = [x[1][0] for x in peeks(run(ROM, BOOT + DAY + [p8(W, 3), 'wait 3', f'peek {W:08x} 1'], copy_save('adult')))]
    b = [x[1][0] for x in peeks(run(ROM, BOOT + DAY + [p8(W, 2), 'wait 3', f'peek {W:08x} 1'], copy_save('hatched')))]
    check('yard: weight never under its stage\'s lightest (grown-up 30, baby 5)', v == [30] and b == [5], f'{v} {b}')
    # eight grown-up days: it says farewell at noon on its eighth day, not before
    DAYA = g('ext', 'e_day'); AD = g('ext', 'e_adult')
    d = struct.unpack('<H', peeks(run(ROM, BOOT + [f'peek {DAYA:08x} 2'], copy_save('adult')))[0][1])[0]
    res = []
    for back in (7, 8):
        sc = BOOT + ['poke32 0300004c c8', 'wait 2', *p16(AD, (d + 1 - back) & 0xFFFF), 'wait 200', f'peek {g("so_pend"):08x} 1', f'peek {g("mode"):08x} 1']   # (moving the clock back to noon starts a new day)
        res.append([x[1][0] for x in peeks(run(ROM, sc, copy_save('adult')))])
    check('yard: the farewell comes after 8 grown-up days (not 7)', res[0] == [0, 0] and (res[1][0] == 1 or res[1][1] == 3), str(res))

def t_budget():
    out = run(ROM, ['wait 30', 'cyc 120 title', 'press A', 'waitscene 1', 'wait 30', 'cyc 300 room'], copy_save('adult'))
    for label, limit in (('title', 240000), ('room', 200000)):   # (v8.3v: the story screens are switched off)
        m = re.search(rf'{label} avg busy cycles/frame: (\d+)', out)
        v = int(m.group(1)) if m else -1
        check(f'budget: {label} {v} cycles/frame (limit {limit} of 280896)', 0 < v < limit, out[-300:])

TESTS = {'fidelity': t_fidelity, 'growth': t_growth, 'secrets': t_secrets, 'care': t_care, 'save': t_save, 'fast': t_fast,
         'games': t_games, 'fixes': t_fixes, 'unlocks': t_unlocks, 'journal': t_journal, 'b1': t_b1, 'stories': t_stories, 'garden': t_garden, 'health': t_health, 'fly': t_fly, 'sprites': t_sprites, 'yard': t_yard, 'rebal': t_rebal, 'fuzz': t_fuzz, 'budget': t_budget}
if __name__ == '__main__':
    pick = sys.argv[1:] or list(TESTS)
    for t in pick:
        print(f'[{t}]'); TESTS[t]()
    bad = [r for r in results if not r[1]]
    print(f'\n{len(results) - len(bad)}/{len(results)} passed')
    shutil.rmtree(TMP, ignore_errors=True)
    sys.exit(1 if bad else 0)
