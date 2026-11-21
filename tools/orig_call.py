#!/usr/bin/env python3
"""Runs one of the original's functions directly (unicorn CPU emulator), for unit-checking a port:
    from orig_call import call; r0 = call(0x0800F98C, a, b)
Memory: the original ROM at 0x08000000, IWRAM/EWRAM/VRAM/SRAM as blank RAM. Returns r0 (and r1 via call2)."""
import os, struct
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB
from unicorn.arm_const import *
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROM = open(os.path.join(ROOT, 'CritterKeep_original.gba'), 'rb').read()
STOP = 0x08011F00   # a 'bx lr' target: we stop when it is reached
def machine():
    mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    for base, size in ((0x08000000, 0x20000), (0x03000000, 0x8000), (0x02000000, 0x40000), (0x06000000, 0x18000),
                       (0x0E000000, 0x10000), (0x04000000, 0x1000), (0x05000000, 0x1000)):
        mu.mem_map(base, size)
    mu.mem_write(0x08000000, ROM)
    return mu
def call2(addr, *args, mu=None, stack=()):
    mu = mu or machine()
    regs = (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)
    for r, v in zip(regs, args[:4]): mu.reg_write(r, v & 0xFFFFFFFF)
    sp = 0x03007F00
    extra = list(args[4:]) + list(stack)
    sp -= 4 * len(extra)
    for i, v in enumerate(extra): mu.mem_write(sp + 4 * i, struct.pack('<I', v & 0xFFFFFFFF))
    mu.reg_write(UC_ARM_REG_SP, sp); mu.reg_write(UC_ARM_REG_LR, STOP | 1)
    mu.emu_start(addr | 1, STOP, count=50_000_000)
    return mu.reg_read(UC_ARM_REG_R0), mu.reg_read(UC_ARM_REG_R1), mu
def call(addr, *args, **kw): return call2(addr, *args, **kw)[0]
