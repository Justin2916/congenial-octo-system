@ Starfall Meadow: the start of the ROM. The cartridge header, our startup code and interrupt handler, the entry
@ stubs ext.c calls (at the addresses the original had them: ext.c's O_* constants) and the original's data, which
@ stays at its own address (the ported code reads it there). Built by build.sh; laid out by link.ld.

  .syntax unified

@ ---------------------------------------------------------------- header (0x08000000)
  .section .header, "ax"
  .arm
  .global _start
_start:
  b     crt0
  .include "header.inc"

@ ---------------------------------------------------------------- startup (0x080000C0)
  .section .crt0, "ax"
  .arm
crt0:
  mov   r0, #0xD2           @ IRQ mode: its stack
  msr   cpsr_c, r0
  ldr   sp, =0x03007FA0
  mov   r0, #0xD3           @ supervisor mode: its stack
  msr   cpsr_c, r0
  ldr   sp, =0x03007FE0
  mov   r0, #0x1F           @ system mode, where the game runs
  msr   cpsr_c, r0
  ldr   sp, =0x03007F00
  ldr   r0, =orig_datainit  @ .data: the 16 bytes of the original's RAM that start set (0x03000000-0x0300000F)
  ldr   r1, =0x03000000
  ldr   r2, =0x03000010
1:cmp   r1, r2
  ldrlt r3, [r0], #4
  strlt r3, [r1], #4
  blt   1b
  ldr   r1, =0x03000010     @ .bss: the rest of the original's RAM starts at 0
  ldr   r2, =0x03000488
  mov   r3, #0
2:cmp   r1, r2
  strlt r3, [r1], #4
  blt   2b
  ldr   r0, =irq            @ the interrupt handler, through the BIOS's vector
  ldr   r1, =0x03007FFC
  str   r0, [r1]
  ldr   r0, =o_main         @ (Thumb; the linker sets the low bit)
  mov   lr, pc
  bx    r0
3:b     3b

@ The interrupt handler: acknowledges what fired (and tells the BIOS, for Halt), and counts vblanks at 0x03000480,
@ which main waits on.
irq:
  mov   r0, #0x04000000
  add   r0, r0, #0x200
  ldr   r1, [r0]            @ IE (low half) and IF (high half)
  and   r1, r1, r1, lsr #16
  add   r2, r0, #2
  strh  r1, [r2]            @ acknowledge
  ldr   r2, =0x03007FF8
  ldrh  r3, [r2]
  orr   r3, r3, r1
  strh  r3, [r2]
  tst   r1, #1
  beq   4f
  ldr   r2, =0x03000480
  ldr   r3, [r2]
  add   r3, r3, #1
  str   r3, [r2]
4:bx    lr
  .ltorg

@ ---------------------------------------------------------------- the entry stubs ext.c calls
@ ext.c's O_* constants are the original's addresses of these functions: each stub jumps to the port.
  .macro STUB name, target
  .section .stub_\name, "ax"
  .thumb
  bx    pc
  nop
  .arm
  ldr   ip, 5f
  bx    ip
5:.word \target
  .endm
  STUB update,   o_game_update      @ 0x080001AC
  STUB render,   o_game_render      @ 0x08001FB4
  STUB text,     o_draw_text        @ 0x0800EC74
  STUB box,      o_draw_box         @ 0x0800EFB4
  STUB sprite,   o_draw_sprite      @ 0x0800F070
  STUB backbuf,  o_back_buffer      @ 0x0800F5F8
  STUB dmacopy,  o_dma_copy         @ 0x0800F614
  STUB sound,    o_sound_play       @ 0x0800F63C
  STUB save,     o_save_write       @ 0x0800F6C0

@ ---------------------------------------------------------------- the original's data (gen/orig_data.py --bin)
  .section .origdata, "a"
  .incbin "orig_data.bin"           @ 0x0800FB1A-0x0801149F: strings, pictures, palettes, the font, tables
  .section .origdatainit, "a"
  .global orig_datainit
orig_datainit:
  .incbin "orig_datainit.bin"       @ 0x080114A0: .data's initial values
