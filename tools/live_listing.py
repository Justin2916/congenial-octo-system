#!/usr/bin/env python3
"""Port helper: the reachable code of one original function, with patch.py's jumps applied, branch targets labelled
(L_xxxx), switch tables expanded and literal values shown. For reading while porting.
  python3 tools/live_listing.py 0x080001AC 0x08001F34 [out.txt]"""
import json, os, re, struct, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROM = open(os.path.join(ROOT, 'CritterKeep_original.gba'), 'rb').read()
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
lst = open(os.path.join(ROOT, 'port', 'orig.lst')).read()
J = json.load(open(os.path.join(ROOT, 'port', 'funcs.json')))
funcs = {int(f['start'], 16): f['name'] for f in J['functions']}
tables = {int(k, 16): [int(x, 16) for x in v] for k, v in J['jump_tables'].items()}
PATCH = {0x080002A6: 0x0800096C}          # patch.py: the clock block is skipped
ins = {}
for m in re.finditer(r'^  ([0-9a-f]{8})  ([0-9a-f]+)\s+(\S+)\s*(.*)$', lst, re.M):
    a = int(m.group(1), 16)
    if lo <= a < hi: ins[a] = (len(m.group(2)) // 2, m.group(3), m.group(4))
def bl_target(a):
    hi_, lo_ = struct.unpack_from('<HH', ROM, a - 0x08000000)
    off = ((hi_ & 0x7FF) << 12) | ((lo_ & 0x7FF) << 1)
    if off & 0x400000: off -= 0x800000
    return a + 4 + off
def succ(a):
    if a in PATCH: return [PATCH[a]], 'b'
    sz, m, op = ins[a]; n = a + sz
    t = re.findall(r'#(0x[0-9a-f]+)', op)
    if m == 'bl':
        tg = bl_target(a)
        return ([tg], 'farjump') if tg not in funcs else ([n], 'call')
    if m == 'b': return [int(t[0], 16)], 'b'
    if m.startswith('b') and m not in ('bl', 'bx', 'bic', 'bics') and t: return [int(t[0], 16), n], 'cond'
    if m == 'pop' and 'pc' in op: return [], 'ret'
    if m == 'bx': return [], 'ret'
    if (m in ('mov', 'add')) and op.startswith('pc'):
        cand = [tb for tb in tables if 0 < tb - a <= 12]
        return tables[min(cand)], 'switch'
    return [n], ''
seen = set(); todo = [lo]; targets = set()
while todo:
    a = todo.pop()
    if a in seen or a not in ins: continue
    seen.add(a); s, k = succ(a)
    if k in ('b', 'cond', 'farjump', 'switch'): targets.update(s)
    todo += s
out = []
for a in sorted(seen):
    if a in targets: out.append(f'L_{a & 0xFFFF:04x}:')
    sz, m, op = ins[a]; s, k = succ(a)
    if k == 'call': txt = f'CALL {funcs[bl_target(a)]}'
    elif k == 'farjump': txt = f'b L_{s[0] & 0xFFFF:04x}   (far)'
    elif k == 'switch': txt = f'SWITCH -> ' + ', '.join(f'{i}:L_{x & 0xFFFF:04x}' for i, x in enumerate(s))
    elif a in PATCH: txt = f'b L_{PATCH[a] & 0xFFFF:04x}   (patch.py)'
    elif k in ('b', 'cond'): txt = f'{m} L_{s[0] & 0xFFFF:04x}'
    else: txt = f'{m} {op}'
    out.append(f'  {a & 0xFFFF:04x}  {txt}')
    # the instruction falls through into one not reached the same way? show gaps
    if k in ('b', 'ret', 'switch', 'farjump') or a in PATCH: out.append('')
dst = sys.argv[3] if len(sys.argv) > 3 else None
text = '\n'.join(out) + '\n'
(open(dst, 'w').write(text) if dst else sys.stdout.write(text))
print(f'{len(seen)} live instructions, {sum(ins[a][0] for a in seen)} bytes', file=sys.stderr)
