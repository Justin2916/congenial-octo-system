#!/usr/bin/env python3
"""Makes CritterKeep.gba from ext.elf (port stage 5: the whole game is ours, so this replaces patch.py).
Checks there are no writable globals (the game runs from ROM), sets the header's title and check byte, pads to 4 KB."""
import os, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
elf = os.path.join(HERE, 'ext.elf')
hdr = subprocess.run(['llvm-objdump', '-h', elf], capture_output=True, text=True).stdout
for l in hdr.splitlines():
    p = l.split()
    if len(p) >= 3 and p[1] in ('.data', '.bss') and int(p[2], 16) != 0: sys.exit('writable section present: ' + l)
subprocess.run(['llvm-objcopy', '-O', 'binary', '--gap-fill', '0xff', elf, os.path.join(HERE, 'ext.bin')], check=True)
rom = bytearray(open(os.path.join(HERE, 'ext.bin'), 'rb').read())
rom[0xA0:0xAC] = b'STARFALL\0\0\0\0'                       # the title (the game code CKPE stays, so saves still match)
chk = 0
for i in range(0xA0, 0xBD): chk = (chk + rom[i]) & 0xFF
rom[0xBD] = (-(chk + 0x19)) & 0xFF                          # the header's complement check
while len(rom) % 0x1000: rom.append(0xFF)
open(os.path.join(HERE, 'CritterKeep.gba'), 'wb').write(rom)
print('ok', hex(len(rom)))
