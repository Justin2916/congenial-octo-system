/* Starfall Meadow, built whole from our sources (port stage 5): no original ROM, no patching.
   Fixed addresses: the header and startup at 0x08000000; the entry stubs where ext.c's O_* constants point; the
   original's data where the ported code reads it; the port before ext.c, which starts at 0x08020000 as always. */
ENTRY(_start)
SECTIONS {
  .header       0x08000000 : { start.o(.header) }
  .crt0         0x080000C0 : { start.o(.crt0) }
  .stub_update  0x080001AC : { start.o(.stub_update) }
  .stub_render  0x08001FB4 : { start.o(.stub_render) }
  .stub_text    0x0800EC74 : { start.o(.stub_text) }
  .stub_box     0x0800EFB4 : { start.o(.stub_box) }
  .stub_sprite  0x0800F070 : { start.o(.stub_sprite) }
  .stub_backbuf 0x0800F5F8 : { start.o(.stub_backbuf) }
  .stub_dmacopy 0x0800F614 : { start.o(.stub_dmacopy) }
  .stub_sound   0x0800F63C : { start.o(.stub_sound) }
  .stub_save    0x0800F6C0 : { start.o(.stub_save) }
  .origdata     0x0800FB1A : { start.o(.origdata) }
  .origdatainit 0x080114A0 : { start.o(.origdatainit) }
  /* the original game, ported (orig.c by hand; the lifted update and main) */
  .port 0x08012000 : { orig.o(.text .text.* .rodata .rodata.*) orig_update.o(.text .text.* .rodata .rodata.*)
                       orig_main.o(.text .text.* .rodata .rodata.*) }
  . = 0x08020000;
  .text : { *(.text.hook_update) *(EXCLUDE_FILE(orig_render.o) .text EXCLUDE_FILE(orig_render.o) .text.*) }
  .rodata : { *(EXCLUDE_FILE(orig_render.o) .rodata EXCLUDE_FILE(orig_render.o) .rodata.*) }
  /* the lifted game_render (64 KB) */
  .port2 ALIGN(4) : { orig_render.o(.text .text.* .rodata .rodata.*) }
  .data : { *(.data .data.*) }
  .bss : { *(.bss .bss.* COMMON) }
  /DISCARD/ : { *(.ARM.exidx*) *(.ARM.attributes) *(.comment) }
}
