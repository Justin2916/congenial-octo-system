import os, struct, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
orig = bytearray(open(os.path.join(HERE,'CritterKeep_original.gba'),'rb').read())
ext = open(os.path.join(HERE,'ext.bin'),'rb').read()
syms = {}
for line in subprocess.run(['llvm-nm',os.path.join(HERE,'ext.elf')],capture_output=True,text=True).stdout.splitlines():
    p=line.split()
    if len(p)==3: syms[p[2]]=int(p[0],16)
# make sure there are no writable sections
hdr = subprocess.run(['llvm-objdump','-h',os.path.join(HERE,'ext.elf')],capture_output=True,text=True).stdout
for l in hdr.splitlines():
    p=l.split()
    if len(p)>=3 and p[1] in ('.data','.bss') and int(p[2],16)!=0: sys.exit('writable section present: '+l)

BASE=0x08020000
rom = orig + b'\xff'*(BASE-0x08000000-len(orig))
rom += ext

def bl_decode(addr):
    o=addr-0x08000000
    hi,lo=struct.unpack('<HH',rom[o:o+4])
    assert hi>>11==0x1E and lo>>11==0x1F, hex(addr)
    off=((hi&0x7FF)<<12)|((lo&0x7FF)<<1)
    if off&0x400000: off-=0x800000
    return addr+4+off
def bl_patch(addr,target,expect):
    assert bl_decode(addr)==expect, (hex(addr),hex(bl_decode(addr)))
    off=target-(addr+4); assert -0x400000<=off<0x400000
    hi=0xF000|((off>>12)&0x7FF); lo=0xF800|((off>>1)&0x7FF)
    o=addr-0x08000000; rom[o:o+4]=struct.pack('<HH',hi,lo)
    assert bl_decode(addr)==target

for a in (0x0800f7dc,0x0800f7e6,0x0800f7f0,0x0800f7fa,0x0800f804,0x0800f80a,0x0800f810):
    bl_patch(a, syms['hook_update'], 0x080001ac)
for a in (0x0800f752,0x0800f814):
    bl_patch(a, syms['hook_render'], 0x08001fb4)

bl_patch(0x08007012, syms['hook_room_critter'], 0x0800f070)
# every other original sprite draw goes through the critter-aware wrapper (critters get the new art, the rest pass through)
for a in (0x8002bb4,0x8002bca,0x8002e54,0x80037c2,0x8005442,0x8005508,0x80087ac,0x800973e,0x8009bf4,0x8009fce,0x800a064,0x800a0f8,0x800cc6c,0x800e738,0x800f452,0x800f4d6,0x800f55a,0x800f5e0):
    bl_patch(a, syms['hook_sprite'], 0x0800f070)
bl_patch(0x080047ae, syms['hook_bg_copy'], 0x0800f614)
# critter sprite table and name tables -> RAM copies maintained by the extension (for secret forms)
RAM_SPR=0x0203D000; RAM_NAMES=0x0203D040
def lit_patch(addr,new,expect):
    o=addr-0x08000000
    assert struct.unpack('<I',rom[o:o+4])[0]==expect, hex(addr)
    rom[o:o+4]=struct.pack('<I',new)
for a in (0x08002d5c,0x080031c4,0x08007118): lit_patch(a, RAM_SPR, 0x0800feb8)
for a in (0x08001bac,0x08003b90,0x080074e0): lit_patch(a, RAM_NAMES, 0x0800fe2c)
for a in (0x08001bc8,0x08007a00): lit_patch(a, RAM_NAMES+0x28, 0x0800fe54)
def h_patch(addr,new,expect):
    o=addr-0x08000000
    assert struct.unpack('<H',rom[o:o+2])[0]==expect, hex(addr)
    rom[o:o+2]=struct.pack('<H',new)
# v8: the extension runs the game clock (sim_tick in ext.c). The original's clock block is skipped entirely:
# its first instruction becomes "b 0x0800096c" (straight to the scene dispatch, r1 still holds SCENE there).
off=(0x0800096c-(0x080002a6+4))//2
h_patch(0x080002a6, 0xE000|(off&0x7FF), 0x4a32)


# v8 phase 5 fixes: the FEED icon's eating art (MEAL 8x8 rice ball, SNACK 8x8 cupcake) -> a blank picture; ext.c
# feed_draw() draws detailed versions in their place. (Their menu icons, which use other literals, are kept.)
lit_patch(0x08009ae0, syms['blank_food_art'], 0x08010832)
lit_patch(0x080074c4, syms['blank_food_art'], 0x080112b6)
lit_patch(0x080088c0, syms['age_year'], 0x080107bf)    # v8.1: " DAY" -> " YEAR"
lit_patch(0x0800842c, syms['age_years'], 0x08010e4f)   # v8.1: " DAYS" -> " YEARS"

# header title (v8 final: the game is now STARFALL MEADOW; the game code CKPE stays, so saves still match)
rom[0xA0:0xAC]=b'STARFALL\0\0\0\0'
# header complement
chk=0
for i in range(0xA0,0xBD): chk=(chk+rom[i])&0xFF
rom[0xBD]=(-(chk+0x19))&0xFF
# pad to a multiple of 4 KB
while len(rom)%0x1000: rom.append(0xFF)
open(os.path.join(HERE,'CritterKeep.gba'),'wb').write(rom)
print('ok', hex(len(rom)), 'hook_update', hex(syms['hook_update']), 'hook_render', hex(syms['hook_render']))
