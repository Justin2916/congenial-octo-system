ENTRY(hook_update)
SECTIONS {
  /* the ported original (orig.c), in the gap after CritterKeep_original.gba: ext.c's layout does not move */
  .port 0x08012000 : { orig.o(.text .text.* .rodata .rodata.*) orig_update.o(.text .text.* .rodata .rodata.*) }
  . = 0x08020000;
  .text : { *(.text.hook_update) *(EXCLUDE_FILE(orig_render.o) .text EXCLUDE_FILE(orig_render.o) .text.*) }
  .rodata : { *(EXCLUDE_FILE(orig_render.o) .rodata EXCLUDE_FILE(orig_render.o) .rodata.*) }
  /* the lifted game_render (64 KB): after ext.c's data, so ext.c does not move */
  .port2 ALIGN(4) : { orig_render.o(.text .text.* .rodata .rodata.*) }
  .data : { *(.data .data.*) }
  .bss : { *(.bss .bss.* COMMON) }
  /DISCARD/ : { *(.ARM.exidx*) *(.ARM.attributes) *(.comment) }
}
