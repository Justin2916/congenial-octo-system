#!/usr/bin/env python3
"""Port stage 1: pull the original game's data out of CritterKeep_original.gba into gen/orig_data.py.

The data sits after the code, from 0x0800FB1C to 0x080114A0, plus 16 bytes of initial RAM values (.data, copied to
0x03000000 by the startup code). This tool reads it once and writes it as readable items: strings (messages and the
pixel-art pictures, which the original stores as text), pointer tables (written with labels, not addresses), and the
remaining binary tables as hex. gen/orig_data.py then rebuilds the exact bytes (and checks that it does).

Run once: python3 tools/extract_orig.py   (it is only needed again if this extraction is improved)
"""
import json, os, re, struct

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROM = open(os.path.join(ROOT, 'CritterKeep_original.gba'), 'rb').read()
BASE = 0x08000000
LO, HI = 0x0800FB1A, 0x080114A0          # (0x0800FB1A is 2 bytes of alignment after the last function)
DATA_LO, DATA_HI = 0x080114A0, 0x080114B0  # .data: initial values for 0x03000000..0x0300000F
CODE_LO = 0x080001AC

def u32(a): return struct.unpack_from('<I', ROM, a - BASE)[0]

# every address the code loads (literal pools) and every pointer stored in the data: those are item starts
lst = open(os.path.join(ROOT, 'port', 'orig.lst')).read()
code_refs = {int(m, 16) for m in re.findall(r'; =(0x[0-9a-f]+)', lst)}
starts = {a for a in code_refs if LO <= a < HI}

def is_ptr(a):
    v = u32(a)
    return a % 4 == 0 and (LO <= v < HI or CODE_LO <= (v & ~1) < LO)

# pointer words inside data become labels too (iterate: a pointer table found by a pointer)
ptr_words = set()
changed = True
while changed:
    changed = False
    for s in sorted(starts):
        a = s
        # a pointer table: aligned words that all point into the ROM, as long as they keep doing so
        while a < HI and is_ptr(a) and (a == s or a not in starts):
            if a not in ptr_words: ptr_words.add(a); changed = True
            v = u32(a)
            if LO <= v < HI and v not in starts: starts.add(v); changed = True
            a += 4
starts.add(LO)

def label(a):
    return f'D_{a:08X}'

def printable(b): return all(32 <= c < 127 for c in b)

items = []      # (addr, kind, value)
ss = sorted(starts) + [HI]
for k in range(len(ss) - 1):
    a, end = ss[k], ss[k + 1]
    while a < end:
        if a in ptr_words:
            v = u32(a)
            tgt = label(v) if LO <= v < HI else f'0x{v:08X}'      # into code: kept as an address (a function or a case)
            items.append((a, 'ptr', tgt)); a += 4; continue
        # a nul-terminated string (at least 2 printable characters)
        z = ROM.find(b'\0', a - BASE, end - BASE)
        if z != -1 and z - (a - BASE) >= 1 and printable(ROM[a - BASE:z]) and (z - (a - BASE) >= 2 or ROM[a - BASE] in b'!>Z?.'):
            items.append((a, 'str', ROM[a - BASE:z].decode())); a = z + BASE + 1; continue
        # otherwise raw bytes, up to the next pointer word or string start
        b = a + 1
        while b < end and b not in ptr_words:
            z2 = ROM.find(b'\0', b - BASE, end - BASE)
            if b % 1 == 0 and z2 - (b - BASE) >= 4 and printable(ROM[b - BASE:z2]) and ROM[b - BASE - 1] == 0: break
            b += 1
        items.append((a, 'bytes', ROM[a - BASE:b - BASE].hex())); a = b

# a readable name for each labelled item (a word from its first string, if it is one)
def nice(a, kind, val):
    if kind == 'str':
        w = re.sub(r'[^A-Z0-9]+', '_', val.upper()).strip('_')[:24]
        if re.fullmatch(r'[.a-zA-Z]+', val[:10]) and len(val) >= 25: return f'ART_{a & 0xFFFFF:05X}'   # pixel art
        return 'S_' + (w or f'{a:05X}')
    return f'T_{a & 0xFFFFF:05X}'
names = {}
used = set()
for a, kind, val in items:
    if a in starts:
        n = nice(a, kind, val)
        while n in used: n += '_'
        used.add(n); names[a] = n

with open(os.path.join(ROOT, 'gen', 'orig_data.py'), 'w') as f:
    f.write('''#!/usr/bin/env python3
"""The original game's data (CritterKeep_original.gba, 0x0800FB1A-0x080114A0, and its .data), as readable items.
Made by tools/extract_orig.py; edit freely once the port no longer needs it byte-identical.

Each item: (address in the original, label or None, kind, value)
  kind 'str'   - a nul-terminated string. Messages, and the pixel-art pictures (one character per pixel, rows run on)
  kind 'ptr'   - a 4-byte pointer: to another item's label, or to code (an address in the original's code)
  kind 'bytes' - anything else, as hex (palettes, the font, coordinate and timing tables...)
Who uses each labelled item is in port/PORT_MAP.md.

python3 gen/orig_data.py         rebuilds the bytes and checks them against the original ROM (when it is present)
"""
import os, struct

ITEMS = [
''')
    for a, kind, val in items:
        lab = names.get(a)
        if kind == 'ptr' and val.startswith('D_'): val = names[int(val[2:], 16)]
        f.write(f'    (0x{a:08X}, {lab!r:28s}, {kind!r:7s}, {val!r}),\n')
    f.write(f''']

# the initial values of the original's RAM at 0x03000000 (copied there at startup)
DATA_INIT = bytes.fromhex({ROM[DATA_LO - BASE:DATA_HI - BASE].hex()!r})
LO, HI = 0x{LO:08X}, 0x{HI:08X}

def build():
    """the data region's bytes, laid out exactly as in the original"""
    lab = {{n: a for a, n, k, v in ITEMS if n}}
    out = bytearray()
    for a, n, k, v in ITEMS:
        assert LO + len(out) == a, hex(a)
        if k == 'str': out += v.encode() + b'\\0'
        elif k == 'ptr': out += struct.pack('<I', lab[v] if v in lab else int(v, 16))
        else: out += bytes.fromhex(v)
    return bytes(out)

if __name__ == '__main__':
    b = build()
    rom = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'CritterKeep_original.gba')
    if os.path.exists(rom):
        r = open(rom, 'rb').read()
        assert b == r[LO - 0x08000000:HI - 0x08000000], 'the data does not match the original'
        assert DATA_INIT == r[HI - 0x08000000:HI - 0x08000000 + 16]
        print(f'ok: {{len(b)}} bytes, {{len(ITEMS)}} items, identical to the original')
    else:
        print(f'{{len(b)}} bytes, {{len(ITEMS)}} items')
''')
print(len(items), 'items,', len(names), 'labels,', sum(1 for i in items if i[1] == 'str'), 'strings,',
      sum(1 for i in items if i[1] == 'ptr'), 'pointers')
