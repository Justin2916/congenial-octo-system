import os, re, struct, subprocess, sys
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
PORT=0x08012000   # orig.c (the ported original) sits in the gap before ext.c; ext.bin starts there
assert len(orig) <= PORT-0x08000000
rom = orig + b'\xff'*(PORT-0x08000000-len(orig))
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

# --- the port (orig.c): every call the original's own code makes to a ported function goes to ours instead.
# Call sites come from the listing (port/orig.lst, made by tools/disasm.py).
CALLS={}
for m in re.finditer(r'^  ([0-9a-f]{8})  [0-9a-f]{8}  bl (\w+)', open(os.path.join(HERE,'port','orig.lst')).read(), re.M):
    CALLS.setdefault(m.group(2),[]).append(int(m.group(1),16))
def redirect(orig_addr, name, sym):
    n=0
    for a in CALLS.get(name,[]):
        if bl_decode(a)==orig_addr: bl_patch(a, syms[sym], orig_addr); n+=1
    return n
PORTED=[(0x0800e840,'sub_0800e840','o_rng_mix'), (0x0800f5f8,'back_buffer','o_back_buffer'), (0x0800f614,'dma_copy','o_dma_copy'),
        (0x0800f6c0,'save_write','o_save_write'), (0x0800f6e0,'sub_0800f6e0','o_save_read'), (0x0800f2a4,'sub_0800f2a4','o_hline'),
        (0x0800efb4,'draw_box','o_draw_box'),
        (0x0800f98c,'sub_0800f98c','o_udiv'), (0x0800f9d8,'sub_0800f9d8','o_udivmod'), (0x0800fa34,'sub_0800fa34','o_sdiv'),
        (0x0800faa0,'sub_0800faa0','o_sdivmod'), (0x0800e854,'sub_0800e854','o_set_message'), (0x0800ec74,'draw_text','o_draw_text'),
        (0x0800f63c,'sound_play','o_sound_play'),
        (0x0800f070,'draw_sprite','o_draw_sprite'), (0x0800f37c,'sub_0800f37c','o_draw_hearts'),
        (0x0800edb8,'sub_0800edb8','o_draw_stars'), (0x0800cc94,'sub_0800cc94','o_build_palette'),
        (0x0800e8f0,'sub_0800e8f0','o_minigame_start'), (0x0800ce5c,'sub_0800ce5c','o_game_init'),
        (0x0800d690,'sub_0800d690','o_prerender'),
        (0x080001ac,'game_update','o_game_update')]

for a in (0x0800f7dc,0x0800f7e6,0x0800f7f0,0x0800f7fa,0x0800f804,0x0800f80a,0x0800f810):
    bl_patch(a, syms['hook_update'], 0x080001ac)
for a in (0x0800f752,0x0800f814):
    bl_patch(a, syms['hook_render'], 0x08001fb4)

bl_patch(0x08007012, syms['hook_room_critter'], 0x0800f070)
# every other original sprite draw goes through the critter-aware wrapper (critters get the new art, the rest pass through)
for a in (0x8002bb4,0x8002bca,0x8002e54,0x80037c2,0x8005442,0x8005508,0x80087ac,0x800973e,0x8009bf4,0x8009fce,0x800a064,0x800a0f8,0x800cc6c,0x800e738,0x800f452,0x800f4d6,0x800f55a,0x800f5e0):
    bl_patch(a, syms['hook_sprite'], 0x0800f070)
bl_patch(0x080047ae, syms['hook_bg_copy'], 0x0800f614)
ported_calls=sum(redirect(*p) for p in PORTED)   # (after the hooks above, which keep their own call sites)
# ...and the original function's own entry jumps to ours, so every other caller (ext.c's O_* calls, the hooks) gets the
# port too: bx pc / nop (to ARM) ; ldr ip, [pc] ; bx ip ; .word ours|1 (16 bytes, 18 when the entry is not 4-aligned)
SIZES={0x0800e840:16, 0x0800f5f8:18, 0x0800f614:12, 0x0800f6c0:32, 0x0800f6e0:50, 0x0800f2a4:204, 0x0800efb4:188,
       0x0800f98c:76, 0x0800f9d8:92, 0x0800fa34:108, 0x0800faa0:122, 0x0800e854:110, 0x0800ec74:314, 0x0800f63c:120,
       0x0800f070:552, 0x0800f37c:624, 0x0800edb8:486, 0x0800cc94:426, 0x0800e8f0:798, 0x0800ce5c:2086, 0x0800d690:4432, 0x080001ac:7560}
def tramp(addr, target):
    o=addr-0x08000000; code=b''
    if addr%4: code+=struct.pack('<H',0x46C0)
    code+=struct.pack('<HH',0x4778,0x46C0)+struct.pack('<III',0xE59FC000,0xE12FFF1C,target|1)
    if len(code)>SIZES[addr]: return 0
    rom[o:o+len(code)]=code; return 1
trampolines=sum(tramp(a, syms[s]) for a,n,s in PORTED)
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
print('ok', hex(len(rom)), 'ported calls', ported_calls, 'entries', trampolines, 'hook_update', hex(syms['hook_update']), 'hook_render', hex(syms['hook_render']))
