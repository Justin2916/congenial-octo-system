#!/usr/bin/env python3
"""Port helper: lifts one of the original's Thumb functions to C, instruction by instruction (exact, not pretty).

  python3 tools/lift.py NAME START END OUT.c [--arity N]

Registers become local variables (r0-r12, sp, lr) and the flags N Z C V are computed exactly. Branches become gotos,
the compiler's switch tables become C switches, memory accesses keep their exact sizes (volatile). The stack frame is
real memory on our own stack. Calls go to the same place they go in the *patched* game (CritterKeep.gba): ported
functions and ext.c's hooks by symbol name, anything else by its address in the original. Literal-pool values are
read from the patched ROM too (patch.py redirects some), and values that are addresses in our code become symbols.
patch.py's jump inside game_update (0x080002A6 -> 0x0800096C) is applied.

Only reachable code is emitted. The result is a starting point: a scene at a time it is then rewritten by hand as
readable C, checked each time with tools/port_check.py.
"""
import json, os, re, struct, subprocess, sys
import capstone
from capstone import arm as A

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ORIG = open(os.path.join(ROOT, 'CritterKeep_original.gba'), 'rb').read()
PATCHED = open(os.path.join(ROOT, 'CritterKeep.gba'), 'rb').read()
J = json.load(open(os.path.join(ROOT, 'port', 'funcs.json')))
FUNCS = {int(f['start'], 16) for f in J['functions']}
TABLES = {int(k, 16): [int(x, 16) for x in v] for k, v in J['jump_tables'].items()}
PATCH_B = {0x080002A6: 0x0800096C}
SYMS = {}
for line in subprocess.run(['llvm-nm', os.path.join(ROOT, 'ext.elf')], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 3 and p[1] in 'TtRrDd': SYMS.setdefault(int(p[0], 16), p[2])

md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB); md.detail = True
def u32(rom, a): return struct.unpack_from('<I', rom, a - 0x08000000)[0]
def insn(a):
    for i in md.disasm(ORIG[a - 0x08000000:a - 0x08000000 + 4], a, 1): return i
def bl_target(rom, a):
    hi, lo = struct.unpack_from('<HH', rom, a - 0x08000000)
    off = ((hi & 0x7FF) << 12) | ((lo & 0x7FF) << 1)
    if off & 0x400000: off -= 0x800000
    return a + 4 + off

RN = {A.ARM_REG_R0 + i: f'r{i}' for i in range(13)}
RN.update({A.ARM_REG_SP: 'sp', A.ARM_REG_LR: 'lr', A.ARM_REG_PC: 'PC'})
COND = {A.ARM_CC_EQ: 'Z', A.ARM_CC_NE: '!Z', A.ARM_CC_HS: 'C', A.ARM_CC_LO: '!C', A.ARM_CC_MI: 'N', A.ARM_CC_PL: '!N',
        A.ARM_CC_VS: 'V', A.ARM_CC_VC: '!V', A.ARM_CC_HI: '(C&&!Z)', A.ARM_CC_LS: '(!C||Z)', A.ARM_CC_GE: '(N==V)',
        A.ARM_CC_LT: '(N!=V)', A.ARM_CC_GT: '(!Z&&N==V)', A.ARM_CC_LE: '(Z||N!=V)'}

LIT_EXTERNS = set()
def const_expr(v):
    """a literal: a symbol when it is an address in our code (orig.c / ext.c), else the number"""
    if v in SYMS and 0x08012000 <= v < 0x0A000000: LIT_EXTERNS.add(SYMS[v]); return f'(u32)&{SYMS[v]}'
    if (v - 1) in SYMS and 0x08012000 <= v - 1 < 0x0A000000: LIT_EXTERNS.add(SYMS[v - 1]); return f'((u32)&{SYMS[v - 1]}|1)'
    return f'0x{v:08X}u'

class Lifter:
    def __init__(self, start, end):
        self.start, self.end = start, end
        self.seen = {}; self.targets = set(); self.externs = set()
    def succ(self, a, i):
        if a in PATCH_B: return [PATCH_B[a]], 'b'
        m = i.mnemonic; n = a + i.size; ops = i.operands
        if m == 'bl':
            t = bl_target(ORIG, a)
            return ([t], 'far') if (t not in FUNCS and self.start <= t < self.end) else ([n], 'call')
        if m == 'b':
            if i.cc in (A.ARM_CC_AL, A.ARM_CC_INVALID): return [ops[0].imm], 'b'
            return [ops[0].imm, n], 'cond'
        if m.startswith('b') and len(m) == 3 and m[1:] in ('eq', 'ne', 'hs', 'lo', 'mi', 'pl', 'vs', 'vc', 'hi', 'ls', 'ge', 'lt', 'gt', 'le', 'cs', 'cc'):
            return [ops[0].imm, n], 'cond'
        if m == 'pop' and any(o.reg == A.ARM_REG_PC for o in ops): return [], 'ret'
        if m == 'bx': return [], 'ret'
        if m in ('mov', 'add') and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_PC:
            cand = [tb for tb in TABLES if 0 < tb - a <= 12]
            return TABLES[min(cand)], 'switch'
        return [n], ''
    def walk(self):
        todo = [self.start]
        while todo:
            a = todo.pop()
            if a in self.seen or not (self.start <= a < self.end): continue
            i = insn(a); self.seen[a] = i
            s, k = self.succ(a, i)
            if k in ('b', 'cond', 'far', 'switch'): self.targets.update(s)
            todo += s

    # ---- one instruction
    def mem(self, op, a):
        m = op.mem
        if m.base == A.ARM_REG_PC: return None
        base = RN[m.base]
        if m.index: return f'({base}+{RN[m.index]})'
        return f'({base}+{m.disp})' if m.disp else base
    def setnz(self, v): return f'N=({v})>>31; Z=({v})==0;'
    def emit(self, a, i):
        m = i.mnemonic; ops = i.operands; n = a + i.size
        R = lambda k: RN[ops[k].reg]
        def opnd(k):
            o = ops[k]
            if o.type == A.ARM_OP_IMM: return f'{o.imm & 0xFFFFFFFF}u'
            r = RN[o.reg]
            return '(u32)(PCV)' if r == 'PC' else r
        out = []
        if a in PATCH_B: return [f'goto L_{PATCH_B[a]:08x};   /* patch.py */']
        if m in ('ldr', 'ldrb', 'ldrh', 'ldrsb', 'ldrsh') and ops[1].type == A.ARM_OP_MEM and ops[1].mem.base == A.ARM_REG_PC:
            la = ((a + 4) & ~3) + ops[1].mem.disp
            return [f'{R(0)}={const_expr(u32(PATCHED, la))};']
        if m in ('ldr', 'ldrb', 'ldrh', 'ldrsb', 'ldrsh', 'str', 'strb', 'strh'):
            ad = self.mem(ops[1], a)
            ty = {'ldr': 'u32', 'str': 'u32', 'ldrb': 'u8', 'strb': 'u8', 'ldrh': 'u16', 'strh': 'u16', 'ldrsb': 's8', 'ldrsh': 's16'}[m]
            if m.startswith('ldr'): return [f'{R(0)}=(u32)(s32)*(volatile {ty}*)({ad});' if ty[0] == 's' else f'{R(0)}=*(volatile {ty}*)({ad});']
            return [f'*(volatile {ty}*)({ad})=({ty}){R(0)};']
        if m == 'adr':
            return [f'{R(0)}=0x{((a + 4) & ~3) + ops[1].imm:08X}u;']
        if m in ('push', 'pop', 'ldm', 'stm', 'ldmia', 'stmia'):
            if m == 'push':
                regs = [RN[o.reg] for o in ops]
                out.append(f'sp-={4 * len(regs)};')
                for k, r in enumerate(regs): out.append(f'*(volatile u32*)(sp+{4 * k})={r};')
                return out
            if m == 'pop':
                regs = [RN[o.reg] for o in ops]
                for k, r in enumerate(regs):
                    if r == 'PC': out.append('sp+=4; return;' if False else f'/* pop pc */')
                    else: out.append(f'{r}=*(volatile u32*)(sp+{4 * k});')
                out.append(f'sp+={4 * len(regs)};')
                if 'PC' in regs: out.append('return;')
                return out
            base = R(0); regs = [RN[o.reg] for o in ops[1:]]
            if m.startswith('ldm'):
                for k, r in enumerate(regs): out.append(f'{r}=*(volatile u32*)({base}+{4 * k});')
                if base not in regs: out.append(f'{base}+={4 * len(regs)};')
            else:
                for k, r in enumerate(regs): out.append(f'*(volatile u32*)({base}+{4 * k})={r};')
                out.append(f'{base}+={4 * len(regs)};')
            return out
        if m == 'bl':
            t = bl_target(PATCHED, a)       # where the patched game sends it
            if t in self.seen or (self.start <= bl_target(ORIG, a) < self.end and bl_target(ORIG, a) not in FUNCS):
                return [f'goto L_{bl_target(ORIG, a):08x};   /* long branch */']
            if t in SYMS:
                f = SYMS[t]; self.externs.add(f)
                fn = f'((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&{f})'
            else:
                fn = f'((unsigned long long(*)(u32,u32,u32,u32,u32,u32))0x{t | 1:08X}u)'
            return [f'{{ unsigned long long rv={fn}(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }}   /* {f if t in SYMS else hex(t)} */']
        if m == 'b' or (m.startswith('b') and len(m) == 3 and m not in ('bic',)):
            tgt = ops[0].imm
            if i.cc in (A.ARM_CC_AL, A.ARM_CC_INVALID): return [f'goto L_{tgt:08x};']
            return [f'if({COND[i.cc]}) goto L_{tgt:08x};']
        if m == 'bx':
            return ['return;']
        if m in ('mov', 'add') and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_PC:
            ts = TABLES[min(tb for tb in TABLES if 0 < tb - a <= 12)]
            src = R(1) if m == 'mov' else f'({a + 4}u+{R(1)})'
            if m == 'mov': src = f'({R(1)}&~1u)'
            out.append(f'switch({src}){{')
            for t in sorted(set(ts)): out.append(f'  case 0x{t:08X}u: goto L_{t:08x};')
            out.append('  default: return; }   // (never: the original has no other case)')
            return out
        # ---- data processing
        PCV = f'0x{a + 4:08X}u'
        def o(k):
            s = opnd(k); return s.replace('(u32)(PCV)', PCV)
        if m in ('mov', 'movs'):
            if len(ops) == 2:
                s = [f'{R(0)}={o(1)};']
                if m == 'movs': s.append(self.setnz(R(0)))
                return s
        if m in ('mvn', 'mvns'): return [f'{R(0)}=~{o(1)};', self.setnz(R(0))]
        if m in ('add', 'adds', 'sub', 'subs', 'adc', 'adcs', 'sbc', 'sbcs', 'rsb', 'rsbs', 'neg', 'negs', 'cmp', 'cmn'):
            setf = m.endswith('s') or m in ('cmp', 'cmn')
            if m in ('cmp', 'cmn'): d, x, y = None, o(0), o(1)
            elif m in ('neg', 'negs'): d, x, y = R(0), '0u', o(1)
            elif m in ('rsb', 'rsbs'): d, x, y = R(0), o(2), o(1)       # rsbs rd, rn, #0 -> 0 - rn
            elif len(ops) == 2: d, x, y = R(0), o(0), o(1)
            else: d, x, y = R(0), o(1), o(2)
            if m in ('rsb', 'rsbs'): x, y = '0u', o(1)
            base = m.rstrip('s') if m not in ('cmp', 'cmn') else m
            if base in ('neg', 'rsb'): base = 'sub'
            if base == 'cmp': base = 'sub'
            if base == 'cmn': base = 'add'
            cin = {'add': '0', 'sub': '1', 'adc': 'C', 'sbc': 'C'}[base]
            yy = f'(~(u32)({y}))' if base in ('sub', 'sbc') else f'(u32)({y})'
            res = f'(u32)({x})+{yy}+(u32){cin}'
            s = [f'{{ u32 X_=(u32)({x}), Y_={yy}; unsigned long long W_=(unsigned long long)X_+Y_+(u32){cin}; u32 R_=(u32)W_;']
            if setf: s.append(' N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31;')
            if d: s.append(f' {d}=R_;')
            s.append(' }')
            return [''.join(s)]
        if m in ('and', 'ands', 'orr', 'orrs', 'eor', 'eors', 'bic', 'bics', 'tst', 'mul', 'muls'):
            if m == 'tst': return [f'{{ u32 R_={o(0)}&{o(1)}; N=R_>>31; Z=R_==0; }}']
            d = R(0); x, y = (o(0), o(1)) if len(ops) == 2 else (o(1), o(2))
            op = {'and': '&', 'orr': '|', 'eor': '^', 'bic': '&~', 'mul': '*'}[m.rstrip('s') if m != 'muls' else 'mul']
            return [f'{d}=(u32)({x}{op}({y}));', self.setnz(d)]
        if m in ('lsl', 'lsls', 'lsr', 'lsrs', 'asr', 'asrs', 'ror', 'rors'):
            d = R(0)
            if len(ops) == 3: x, y = o(1), o(2)
            else: x, y = o(0), o(1)
            kind = m[:3]
            imm = ops[-1].type == A.ARM_OP_IMM
            # exact ARM semantics, including shifts by register of 0 and of 32 or more
            s = f'{{ u32 X_=(u32)({x}), S_=(u32)({y})&0xFF; u32 R_;'
            if kind == 'lsl':
                if imm: s += ' if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;}'
                else: s += ' if(S_==0){R_=X_;} else if(S_<32){C=(X_>>(32-S_))&1; R_=X_<<S_;} else if(S_==32){C=X_&1; R_=0;} else {C=0; R_=0;}'
            elif kind == 'lsr':
                if imm: s += ' if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;}'
                else: s += ' if(S_==0){R_=X_;} else if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else if(S_==32){C=X_>>31; R_=0;} else {C=0; R_=0;}'
            elif kind == 'asr':
                if imm: s += ' if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=(u32)((s32)X_>>S_);} else {C=X_>>31; R_=(u32)((s32)X_>>31);}'
                else: s += ' if(S_==0){R_=X_;} else if(S_<32){C=(X_>>(S_-1))&1; R_=(u32)((s32)X_>>S_);} else {C=X_>>31; R_=(u32)((s32)X_>>31);}'
            else:
                s += ' if(S_==0){R_=X_;} else { S_&=31; if(S_==0){C=X_>>31; R_=X_;} else {R_=(X_>>S_)|(X_<<(32-S_)); C=R_>>31;} }'
            s += f' {d}=R_; N=R_>>31; Z=R_==0; }}'
            return [s]
        if m == 'nop' or (m == 'mov' and len(ops) == 2 and ops[0].reg == ops[1].reg): return ['/* nop */']
        raise SystemExit(f'lift: no rule for {a:#x} {m} {i.op_str}')

    def c(self, name, arity):
        self.walk()
        order = sorted(self.seen)
        # an instruction that falls through to code that is not emitted right after it gets an explicit goto
        gaps = {}
        for k, a in enumerate(order):
            s, kind = self.succ(a, self.seen[a])
            nxt = a + self.seen[a].size
            if kind in ('', 'call', 'cond') and a not in PATCH_B and nxt in self.seen and (k + 1 == len(order) or order[k + 1] != nxt):
                gaps[a] = nxt; self.targets.add(nxt)
        body = []
        for a in order:
            i = self.seen[a]
            lab = f'L_{a:08x}: ;' if (a in self.targets or a == self.start) else ''
            code = ' '.join(self.emit(a, i))
            body.append(f'{lab:14s} {code:100s} /* {a & 0xFFFFF:05x} {i.mnemonic} {i.op_str} */')
            if a in gaps: body.append(f'               goto L_{gaps[a]:08x};')
        out = [f'// Lifted from the original 0x{self.start:08X}-0x{self.end:08X} by tools/lift.py: exact, not yet readable.',
               'typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;',
               'typedef unsigned int u32; typedef int s32;']
        for f in sorted(self.externs | LIT_EXTERNS): out.append(f'extern char {f}[];')
        args = ', '.join(f'u32 a{k}' for k in range(arity)) or 'void'
        out.append(f'void {name}({args}){{')
        out.append('  u32 r0=0,r1=0,r2=0,r3=0,r4=0,r5=0,r6=0,r7=0,r8=0,r9=0,r10=0,r11=0,r12=0,lr=0; u32 N=0,Z=0,C=0,V=0;')
        out.append('  u32 frame_[160]; u32 sp=(u32)&frame_[160];')
        for k in range(arity): out.append(f'  r{k}=a{k};')
        out.append('  (void)r8;(void)r9;(void)r10;(void)r11;(void)r12;(void)lr;(void)V;(void)N;(void)Z;(void)C;')
        return '\n'.join(out + body + ['}']) + '\n'

if __name__ == '__main__':
    name, start, end, out = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16), sys.argv[4]
    arity = int(sys.argv[sys.argv.index('--arity') + 1]) if '--arity' in sys.argv else 1
    L = Lifter(start, end)
    open(out, 'w').write(L.c(name, arity))
    print(f'{name}: {len(L.seen)} instructions lifted -> {out}')
