#!/bin/bash
# Builds CritterKeep.gba (the playable ROM) and ./emu (the test emulator), entirely from our sources (port stage 5).
set -e
cd "$(dirname "$0")"
CC="clang --target=armv4t-none-eabi -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -nostdlib -fno-pic -Wno-unused-command-line-argument"
python3 -I gen/orig_data.py --bin                 # the original's data -> orig_data.bin, orig_datainit.bin
$CC -c start.s -o start.o                          # header, startup, interrupt handler, entry stubs, data
$CC -mthumb -Wall -Wno-unused-function -c ext.c -o ext.o
$CC -mthumb -Wall -Wno-unused-function -c orig.c -o orig.o
$CC -mthumb -w -c orig_update.c -o orig_update.o  # lifted (tools/lift.py): exact, generated
$CC -mthumb -w -c orig_render.c -o orig_render.o
$CC -mthumb -w -c orig_main.c -o orig_main.o
ld.lld -T link.ld start.o orig.o orig_update.o orig_main.o orig_render.o ext.o -o ext.elf
python3 -I mkrom.py | tee mkrom.log; if grep -q "writable section" mkrom.log; then echo "BUILD FAILED: no globals allowed"; exit 1; fi
[ -x emu ] && [ emu -nt test_emulator.c ] || cc -O2 -w test_emulator.c -o emu -lm
