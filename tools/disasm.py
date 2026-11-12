#!/usr/bin/env python3
"""Port stage 1: a recursive disassembler for CritterKeep_original.gba.

Walks the original's Thumb code from main (and the ARM startup), following branches, BL calls and jump tables, and
marks literal pools as data. Writes:
  port/orig.lst     - the listing (functions in address order, labels, literal values)
  port/funcs.json   - every function: start, end, size, callers, callees, literal words it loads, RAM it touches
Usage: python3 tools/disasm.py   (needs capstone: pip install capstone)
"""
import json, os, struct, sys
import capstone
from capstone import arm as A

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROM = open(os.path.join(ROOT, 'CritterKeep_original.gba'), 'rb').read()
BASE = 0x08000000
END = BASE + len(ROM)
OUT = os.path.join(ROOT, 'port')

md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB); md.detail = True
mda = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM); mda.detail = True

def u32(a): return struct.unpack_from('<I', ROM, a - BASE)[0]
def u16(a): return struct.unpack_from('<H', ROM, a - BASE)[0]
def in_rom(a): return BASE <= a < END

# known entry points: main (from crt0's literal), the IRQ handler (ARM), and what ext.c calls
ENTRIES = {0x0800F714: 'main', 0x080001AC: 'game_update', 0x08001FB4: 'game_render', 0x0800F5F8: 'back_buffer',
           0x0800F614: 'dma_copy', 0x0800F63C: 'sound_play', 0x0800F6C0: 'save_write', 0x0800EFB4: 'draw_box',
           0x0800EC74: 'draw_text', 0x0800F070: 'draw_sprite'}

def insn_at(a):
    """one Thumb instruction at a (BL is 4 bytes)"""
    code = ROM[a - BASE:a - BASE + 4]
    for i in md.disasm(code, a, 1): return i
    return None

funcs = {}           # start -> dict
owner = {}           # insn address -> function start
insns = {}           # address -> capstone insn
data = {}            # literal address -> (size, value)
tables = {}          # jump table address -> list of targets
notes = []           # unresolved things

def walk(start):
    if start in funcs: return
    f = funcs[start] = {'start': start, 'blocks': set(), 'calls': set(), 'lits': set(), 'ram': set(), 'io': set(),
                        'indirect': [], 'end': start}
    todo = [start]; seen = set()
    while todo:
        a = todo.pop()
        while True:
            if a in seen or not in_rom(a): break
            seen.add(a)
            i = insn_at(a)
            if i is None: notes.append(f'bad insn at {a:#x} in {start:#x}'); break
            insns[a] = i; owner.setdefault(a, start)
            nxt = a + i.size
            m = i.mnemonic
            ops = i.operands
            # literal loads: ldr rX, [pc, #imm]
            if m == 'ldr' and len(ops) == 2 and ops[1].type == A.ARM_OP_MEM and ops[1].mem.base == A.ARM_REG_PC:
                la = ((a + 4) & ~3) + ops[1].mem.disp
                v = u32(la); data[la] = (4, v); f['lits'].add(la)
                if 0x02000000 <= v < 0x04000000: f['ram'].add(v)
                elif 0x04000000 <= v < 0x04000400: f['io'].add(v)
            if m == 'bl':
                t = ops[0].imm
                ti = insn_at(t)
                if ti is not None and ti.mnemonic == 'push' or t in ENTRIES or is_leaf(t):
                    f['calls'].add(t); walk(t); a = nxt; continue
                f.setdefault('farjumps', set()).add(t); todo.append(t); break   # clang's long branch inside a function
            if m == 'b':
                todo.append(ops[0].imm); break
            if m.startswith('b') and m not in ('bl', 'blx', 'bx', 'bic', 'bics') and len(ops) == 1 and ops[0].type == A.ARM_OP_IMM:
                todo.append(ops[0].imm); a = nxt; continue          # conditional branch
            if m == 'pop' and any(o.reg == A.ARM_REG_PC for o in ops): break
            if m == 'bx':
                if ops[0].reg == A.ARM_REG_LR: break
                p = insns.get(a - 2)
                if p is not None and p.mnemonic == 'pop' and any(o.reg == ops[0].reg for o in p.operands): break   # pop {rX}; bx rX = return
                t = jump_table(a)
                if t: todo.extend(t); break
                f['indirect'].append(a); break
            if m == 'mov' and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_PC:
                t = jump_table(a)
                if t: todo.extend(t); break
                f['indirect'].append(a); break
            if m == 'add' and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_PC:
                t = jump_table(a)
                if t: todo.extend(t); break
                f['indirect'].append(a); break
            if m == 'svc': pass
            a = nxt
    f['blocks'] = seen
    f['end'] = max(x + insns[x].size for x in seen)

def is_leaf(t):
    """a small function with no frame: reaches 'bx lr' without a push/pop or a branch away"""
    a = t
    for _ in range(48):
        i = insn_at(a)
        if i is None or i.mnemonic in ('push', 'pop', 'b', 'bl'): return False
        if i.mnemonic == 'bx': return i.operands[0].reg == A.ARM_REG_LR
        a += i.size
    return False

def jump_table(a):
    """the compiler's three switch forms (the bound is the nearest earlier 'cmp rI, #n'):
      words:  lsls rX, rI, #2 / adr rT, #k / ldr rX, [rT, rX] (/ ...) / mov pc, rX      - absolute targets after the code
      bytes:  add rX, pc / ldrb rX, [rX, #4] / lsls rX, rX, #1 / add pc, rX            - target = here+4 + 2*entry
      halves: lsls rX, rI, #1 / add rX, pc / ldrh rX, [rX, #4] / lsls rX, #1 / add pc, rX"""
    prev = []; b = a
    for _ in range(40):
        b -= 2
        if b not in insns: b -= 2
        if b not in insns: break
        prev.append(insns[b])
    n = None
    for i in prev:
        if i.mnemonic == 'cmp' and len(i.operands) == 2 and i.operands[1].type == A.ARM_OP_IMM: n = i.operands[1].imm + 1; break
    me = insns[a]; out = None
    if me.mnemonic == 'add':            # bytes / halves
        k = next((i for i in prev[:4] if i.mnemonic in ('ldrb', 'ldrh')), None)
        ap = next((i for i in prev[:5] if i.mnemonic == 'add' and len(i.operands) == 2 and i.operands[1].type == A.ARM_OP_REG and i.operands[1].reg == A.ARM_REG_PC), None)
        ad = next((i for i in prev[:5] if i.mnemonic == 'adr'), None)
        if k is None or n is None or (ap is None and ad is None): return None
        if ap is not None: tb = ap.address + 4 + k.operands[1].mem.disp
        else: tb = ((ad.address + 4) & ~3) + ad.operands[1].imm      # adr rT / ldrh rX, [rT, rX]
        base = a + 4
        if k.mnemonic == 'ldrb':
            es = [ROM[tb - BASE + j] for j in range(n)]; sz = 1
        else:
            es = [u16(tb + 2 * j) for j in range(n)]; sz = 2
        out = [base + 2 * e for e in es]
        for j in range(n): data[tb + sz * j] = (sz, es[j])
        if sz == 1 and n % 2: data[tb + n] = (1, ROM[tb + n - BASE])   # (alignment pad)
    else:                               # words, via adr
        ad = next((i for i in prev[:4] if i.mnemonic == 'adr'), None)
        if ad is None: return None
        tb = ((ad.address + 4) & ~3) + ad.operands[1].imm
        if n is None:                   # no bound: read while they look like code in this area
            n = 0
            while n < 256 and (u32(tb + 4 * n) & 1) and abs((u32(tb + 4 * n) & ~1) - a) < 0x4000: n += 1
        es = [u32(tb + 4 * j) for j in range(n)]
        if not all(e & 1 and in_rom(e & ~1) for e in es): return None
        out = [e & ~1 for e in es]
        for j in range(n): data[tb + 4 * j] = (4, es[j])
    tables[tb] = out
    return out

def main():
    for e in sorted(ENTRIES): walk(e)
    # function pointers sitting in literal pools / tables that point at code (Thumb bit set) are more entries
    for rnd in range(4):
        cand = set()
        for la, (sz, v) in list(data.items()):
            if v & 1 and BASE + 0x1AC <= (v & ~1) < 0x080114A0 and (v & ~1) not in funcs and (v & ~1) not in insns:
                i = insn_at(v & ~1)
                if i and i.mnemonic == 'push': cand.add(v & ~1)
        if not cand: break
        for c in sorted(cand): walk(c)
    os.makedirs(OUT, exist_ok=True)
    callers = {s: set() for s in funcs}
    for s, f in funcs.items():
        for c in f['calls']: callers.setdefault(c, set()).add(s)
    # coverage: what part of the ROM is code, literal data, or not yet explained
    code = set()
    for a, i in insns.items(): code.update(range(a, a + i.size))
    lit = set()
    for la, (sz, v) in data.items(): lit.update(range(la, la + sz))
    out = []
    for s in sorted(funcs):
        f = funcs[s]
        out.append({'start': f'{s:#010x}', 'end': f'{f["end"]:#010x}', 'size': f['end'] - s,
                    'name': ENTRIES.get(s, f'sub_{s:08x}'),
                    'calls': sorted(f'{c:#010x}' for c in f['calls']), 'callers': sorted(f'{c:#010x}' for c in callers.get(s, ())),
                    'ram': sorted(f'{r:#010x}' for r in f['ram']), 'io': sorted(f'{r:#010x}' for r in f['io']),
                    'indirect': [f'{x:#010x}' for x in f['indirect']]})
    json.dump({'functions': out, 'jump_tables': {f'{k:#010x}': [f'{t:#010x}' for t in v] for k, v in tables.items()},
               'notes': notes}, open(os.path.join(OUT, 'funcs.json'), 'w'), indent=1)
    # listing
    with open(os.path.join(OUT, 'orig.lst'), 'w') as L:
        labels = {s: ENTRIES.get(s, f'sub_{s:08x}') for s in funcs}
        for t in tables.values():
            for x in t: labels.setdefault(x, f'case_{x:08x}')
        a = BASE + 0x1AC
        while a < 0x080114A0:
            if a in labels: L.write(f'\n{labels[a]}:\n')
            if a in insns:
                i = insns[a]; txt = f'{i.mnemonic} {i.op_str}'
                if i.mnemonic == 'bl': txt = f'bl {labels.get(i.operands[0].imm, hex(i.operands[0].imm))}'
                elif i.mnemonic == 'ldr' and i.operands[1].type == A.ARM_OP_MEM and i.operands[1].mem.base == A.ARM_REG_PC:
                    txt += f'   ; ={u32(((a + 4) & ~3) + i.operands[1].mem.disp):#x}'
                L.write(f'  {a:08x}  {i.bytes.hex():8}  {txt}\n'); a += i.size
            elif a in data:
                sz, v = data[a]; L.write(f'  {a:08x}  .word {v:#010x}\n'); a += sz
            else:
                # a run of unexplained bytes (art, text, tables...)
                b = a
                while b < 0x080114A0 and b not in insns and b not in data and b not in labels: b += 1
                L.write(f'  {a:08x}  .bytes {b - a} ({ROM[a - BASE:min(b, a + 16) - BASE].hex()}...)\n'); a = b
    unk = (0x080114A0 - BASE - 0x1AC) - len(code) - len(lit)
    print(f'{len(funcs)} functions, {len(insns)} instructions ({len(code)} bytes of code), {len(lit)} bytes of literals, '
          f'{len(tables)} jump tables, {unk} bytes not yet explained (data); {len(notes)} notes')
    ind = [(s, x) for s, f in funcs.items() for x in f['indirect']]
    if ind: print('unresolved indirect jumps:', [(f'{s:#x}', f'{x:#x}') for s, x in ind])

if __name__ == '__main__': main()
