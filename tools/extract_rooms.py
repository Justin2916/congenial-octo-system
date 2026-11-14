#!/usr/bin/env python3
"""Port stage 2: the original's room pictures. Its function 0x0800D690 draws the room (day, or night) into a page and
depends on nothing else, so instead of porting 4.4 KB of drawing code we keep what it draws (except its last 8 pictures, drawn through
the hooked draw_sprite, which are recorded as calls): this runs it once for
each (tools/orig_call.py, needs unicorn) and writes gen/orig_rooms.py, which holds the two pictures run-length coded."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from orig_call import machine, call2
import struct
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import *
SPRITE_CALL = 0x0800E738
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def rle(b):
    o = bytearray(); i = 0
    while i < len(b):
        j = i
        while j < len(b) and b[j] == b[i] and j - i < 255: j += 1
        o += bytes([j - i, b[i]]); i = j
    return bytes(o)
pics = []; cmaps = []
for night in (0, 1):
    runs = []
    for fill in (0x11, 0xEE):      # two different fills of the original's RAM show every byte it writes (even a 0)
        mu = machine(); mu.mem_write(0x03000000, bytes([fill]) * 0x7000)
        mu._calls = []
        def at_sprite(mu, addr, size, ud):   # the 8 pictures at the end go through draw_sprite, which the game hooks:
            sp = mu.reg_read(UC_ARM_REG_SP)   # record each call (and the colour map then) instead of drawing it
            args = [mu.reg_read(r) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)] + list(struct.unpack('<II', mu.mem_read(sp, 8)))
            mu._calls.append((args, bytes(mu.mem_read(0x030003B0, 0x80))))
            mu.reg_write(UC_ARM_REG_PC, (SPRITE_CALL + 4) | 1)
        mu.hook_add(UC_HOOK_CODE, at_sprite, begin=SPRITE_CALL, end=SPRITE_CALL)
        call2(0x0800D690, 0x02000000, night, mu=mu)
        runs.append((fill, bytes(mu.mem_read(0x03000000, 0x7000)), bytes(mu.mem_read(0x02000000, 38400))))
        calls_by_fill = getattr(sys.modules[__name__], '_cbf', {}); calls_by_fill[(night, fill)] = mu._calls; sys.modules[__name__]._cbf = calls_by_fill
    (fa, ra, pa), (fb, rb, pb) = runs
    assert pa == pb
    written = [i for i in range(0x7000) if not (ra[i] == fa and rb[i] == fb)]
    assert all(ra[i] == rb[i] for i in written if not 0x74 <= i < 0x78)
    cm = [(i - 0x3B0, ra[i]) for i in written if 0x3B0 <= i < 0x430]
    assert all(0x74 <= i < 0x78 or 0x3B0 <= i < 0x430 for i in written), [hex(i) for i in written]   # DRAW_TARGET and CMAP only
    pics.append(rle(pa)); cmaps.append(cm)
assert cmaps[0] == cmaps[1]
# the recorded calls: the same by day and night and whatever was in RAM; for each, the colour-map entries the original
# had written by then (relative to its own writes, so entries it never touches are left alone)
cbf = _cbf
assert len(cbf[(0, 0x11)]) == 8
calls = []
for k in range(8):
    (aa, ca), (ab, cb) = cbf[(0, 0x11)][k], cbf[(0, 0xEE)][k]
    assert aa == ab and aa == cbf[(1, 0x11)][k][0]
    cm = [(i, ca[i]) for i in range(0x80) if not (ca[i] == 0x11 and cb[i] == 0xEE)]
    assert all(ca[i] == cb[i] for i, v in cm)
    calls.append((aa, cm))
with open(os.path.join(ROOT, 'gen', 'orig_rooms.py'), 'w') as f:
    f.write('#!/usr/bin/env python3\n"""The original room, by day and by night (240x160, one byte a pixel), as drawn by the original\'s 0x0800D690.\n'
            'Made by tools/extract_rooms.py. Run this to write ../orig_rooms.h (run-length pairs: count, colour)."""\nimport os\n')
    for n, p in zip(('DAY', 'NIGHT'), pics): f.write(f'{n} = bytes.fromhex({p.hex()!r})\n')
    f.write(f'# the colour-map entries it leaves set (CMAP index, value), which later drawing relies on\nCMAP = {cmaps[0]!r}\n')
    f.write('# its last 8 drawings are pictures through draw_sprite, which the game hooks (ext.c): they are made at run time.\n'
            '# Each: (desc, x, y, scale, flip, limit) and the colour-map entries set just before\n')
    f.write(f'SPRITES = {calls!r}\n')
    f.write('''
def unrle(b):
    o = bytearray()
    for i in range(0, len(b), 2): o += bytes([b[i + 1]]) * b[i]
    return bytes(o)
if __name__ == '__main__':
    assert len(unrle(DAY)) == len(unrle(NIGHT)) == 38400
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    with open(os.path.join(root, 'orig_rooms.h'), 'w') as h:
        h.write('// The original room by day and by night: run-length pairs (count, colour). Made by gen/orig_rooms.py.\\n')
        for n, p in (('ROOM_DAY', DAY), ('ROOM_NIGHT', NIGHT)):
            h.write(f'static const unsigned char {n}[{len(p)}] = {{' + ','.join(str(x) for x in p) + '};\\n')
        h.write(f'static const unsigned char ROOM_CMAP[{2 * len(CMAP)}] = {{' + ','.join(f'{i},{v}' for i, v in CMAP) + '};\\n')
        h.write('static const struct { unsigned desc; int x, y, scale, flip, limit; unsigned char n, cm[2*64]; } ROOM_SPRITES[8] = {\\n')
        for (d, x, y, s, fl, lim), cm in SPRITES:
            h.write(f'  {{{d:#x},{x},{y},{s},{fl},{lim},{len(cm)},{{' + ','.join(f'{i},{v}' for i, v in cm) + '}},\\n')
        h.write('};\\n')
    print('orig_rooms.h:', len(DAY) + len(NIGHT), 'bytes')
''')
print('ok', len(pics[0]), len(pics[1]))
