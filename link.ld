ENTRY(hook_update)
SECTIONS {
  /* the ported original (orig.c), in the gap after CritterKeep_original.gba: ext.c's layout does not move */
  .port 0x08012000 : { orig.o(.text .text.* .rodata .rodata.*) }
  . = 0x08020000;
  .text : { *(.text.hook_update) *(.text .text.*) }
  .rodata : { *(.rodata .rodata.*) }
  .data : { *(.data .data.*) }
  .bss : { *(.bss .bss.* COMMON) }
  /DISCARD/ : { *(.ARM.exidx*) *(.ARM.attributes) *(.comment) }
}
