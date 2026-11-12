#!/usr/bin/env python3
"""Port harness: run two ROMs on the same script and save, frame by frame, and report the first frame where their RAM,
screen or palette differ (and exactly which bytes, by name).

  python3 tools/lockstep.py A.gba B.gba script.txt save.sav [--frames N]

The script is an ordinary ./emu script (wait, press, poke...). Its steps run as usual; after them the harness records
N more frames (default 600; --frames=N). The stack (IWRAM 0x03007000+) is ignored unless --stack with the emulator's 'trail' command. A script line 'TRAIL n' records n frames at that
point instead (several are allowed), so the inputs can sit between recordings.
Both ROMs start from their own copy of the save. Exit status 0 = identical, 1 = they differ.
"""
import os, shutil, struct, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EMU = os.path.join(ROOT, 'emu')
REC = 2 + 128 + 64 + 10 + 2

# names for the original's RAM (see port/PORT_MAP.md); ranges are [start, end)
NAMES = [
    (0x03000000, 0x03000004, 'rng'), (0x03000004, 0x03000010, 'title timers'), (0x03000010, 0x03000012, 'KEYS_PREV'),
    (0x03000018, 0x0300001C, 'FRAMES'), (0x0300001C, 0x03000020, 'MSG_TIMER'), (0x03000020, 0x03000024, 'SCENE'),
    (0x03000024, 0x03000028, 'TICK_CNT'), (0x03000028, 0x03000074, 'PET'), (0x03000074, 0x03000078, 'DRAW_TARGET'),
    (0x03000078, 0x03000280, 'palette work'), (0x03000280, 0x03000288, 'title cursor'), (0x03000288, 0x03000298, 'sound'),
    (0x03000298, 0x030002A4, 'ANIM'), (0x030002A4, 0x030002CC, 'MSG_BUF'), (0x030002CC, 0x030002D8, 'GONE_T/confirm'),
    (0x030002D8, 0x030002DC, 'ICON_CUR'), (0x030002DC, 0x03000381, 'minigame'), (0x03000381, 0x030003AC, 'name text'),
    (0x030003AC, 0x030003B0, 'HELP_PAGE'), (0x030003B0, 0x03000430, 'CMAP'), (0x03000430, 0x0300047C, 'save work'),
    (0x0300047C, 0x03000488, 'vblank/frame'), (0x03007000, 0x03008000, 'stack'),
    (0x02000000, 0x02009600, 'YARD_BG'), (0x02009600, 0x02012C00, 'buffer 2'), (0x02012C00, 0x0201C200, 'buffer 3'),
    (0x0201C200, 0x02025800, 'SCRATCH_BG'), (0x0203D000, 0x0203E000, 'ext tables'), (0x0203E000, 0x02040000, 'ext State'),
]
def name(a):
    for lo, hi, n in NAMES:
        if lo <= a < hi: return f'{n}+{a - lo:#x}'
    return '?'

def run(rom, script, save, tmp, tag):
    sv = os.path.join(tmp, tag + '.sav'); shutil.copy(save, sv)
    tr = os.path.join(tmp, tag + '.trail')
    if os.path.exists(tr): os.remove(tr)
    lines = []
    for l in script:
        if l.startswith('TRAIL '): lines.append(f'trail {l.split()[1]} {tag}.trail')
        else: lines.append(l)
    sc = os.path.join(tmp, tag + '.txt'); open(sc, 'w').write('\n'.join(lines) + '\n')
    subprocess.run([EMU, rom, sc, sv], cwd=tmp, capture_output=True, timeout=1800)
    d = open(tr, 'rb').read() if os.path.exists(tr) else b''
    return [struct.unpack_from(f'<{REC}I', d, i * REC * 4) for i in range(len(d) // (REC * 4))]

def dump_at(rom, script, save, tmp, tag, n_before, k):
    """re-run to the k-th recorded frame and dump RAM and the screen there"""
    sv = os.path.join(tmp, tag + 'd.sav'); shutil.copy(save, sv)
    lines = []; left = k + 1
    for l in script:
        if l.startswith('TRAIL '):
            n = int(l.split()[1]); m = min(n, left); lines.append(f'wait {m}'); left -= m
            if left == 0: break
        else: lines.append(l)
    lines += [f'dump {tag}.ram', f'vdump {tag}.vid']
    sc = os.path.join(tmp, tag + 'd.txt'); open(sc, 'w').write('\n'.join(lines) + '\n')
    subprocess.run([EMU, rom, sc, sv], cwd=tmp, capture_output=True, timeout=1800)
    return open(os.path.join(tmp, tag + '.ram'), 'rb').read(), open(os.path.join(tmp, tag + '.vid'), 'rb').read()

def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    frames = 600; stack = '--stack' in sys.argv
    for a in sys.argv[1:]:
        if a.startswith('--frames='): frames = int(a.split('=')[1])
    if len(args) != 4: sys.exit(__doc__)
    ra, rb, scp, save = [os.path.abspath(x) for x in args]
    script = [l.rstrip('\n') for l in open(scp) if l.strip() and not l.startswith('#')]
    if not any(l.startswith('TRAIL ') for l in script): script.append(f'TRAIL {frames}')
    tmp = tempfile.mkdtemp(prefix='lock')
    A = run(ra, script, save, tmp, 'a'); B = run(rb, script, save, tmp, 'b')
    if not A or len(A) != len(B):
        print(f'recorded {len(A)} and {len(B)} frames: a run stopped early (fault?)'); sys.exit(1)
    keep = lambda r: tuple(v for i, v in enumerate(r) if stack or not (2 + 112 <= i < 2 + 128))   # (IWRAM 0x03007000+ is the stack)
    for k, (x, y) in enumerate(zip(A, B)):
        if keep(x) == keep(y): continue
        print(f'first difference at recorded frame {k} (emulator frame {x[0]}, scene {x[1]} vs {y[1]})')
        blocks = [i for i in range(2, REC) if x[i] != y[i] and (stack or not 114 <= i < 130)]
        what = []
        for i in blocks:
            j = i - 2
            if j < 128: what.append(f'IWRAM {0x03000000 + j * 256:#x}')
            elif j < 192: what.append(f'EWRAM {0x02000000 + (j - 128) * 4096:#x}')
            elif j < 202: what.append(f'screen rows {(j - 192) * 16}-{(j - 192) * 16 + 15}')
            else: what.append('BG palette' if j == 202 else 'OBJ palette')
        print('  differing blocks:', ', '.join(what))
        ma, va = dump_at(ra, script, save, tmp, 'a', 0, k); mb, vb = dump_at(rb, script, save, tmp, 'b', 0, k)
        diffs = []
        for off in range(len(ma)):
            if ma[off] != mb[off] and (stack or not 0x7000 <= off < 0x8000):
                addr = 0x03000000 + off if off < 32768 else 0x02000000 + off - 32768
                diffs.append(addr)
        for addr in diffs[:24]:
            off = addr - 0x03000000 if addr >= 0x03000000 else addr - 0x02000000 + 32768
            print(f'  {addr:#010x} {name(addr):28s} {ma[off]:02x} -> {mb[off]:02x}')
        if len(diffs) > 24: print(f'  ... {len(diffs)} bytes in all')
        px = sum(1 for i in range(38400) if va[i] != vb[i]); print(f'  screen pixels differing: {px}')
        sys.exit(1)
    print(f'identical for {len(A)} frames')

if __name__ == '__main__': main()
