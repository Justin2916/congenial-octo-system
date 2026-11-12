#!/bin/bash
set -e
cd "$(dirname "$0")"
clang --target=armv4t-none-eabi -mthumb -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -nostdlib -fno-pic -Wall -Wno-unused-function -c ext.c -o ext.o
clang --target=armv4t-none-eabi -mthumb -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -nostdlib -fno-pic -Wall -Wno-unused-function -c orig.c -o orig.o
clang --target=armv4t-none-eabi -mthumb -mcpu=arm7tdmi -O2 -ffreestanding -fno-builtin -nostdlib -fno-pic -Wall -Wno-unused-function -Wno-unused-label -Wno-unused-but-set-variable -c orig_update.c -o orig_update.o
ld.lld -T link.ld orig.o orig_update.o ext.o -o ext.elf
llvm-objcopy -O binary ext.elf ext.bin
python3 -I patch.py | tee patch.log; if grep -q "writable section" patch.log; then echo "BUILD FAILED: no globals allowed"; exit 1; fi
[ -x emu ] && [ emu -nt test_emulator.c ] || cc -O2 -w test_emulator.c -o emu -lm
