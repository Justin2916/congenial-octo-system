// The original game's state and hardware, for orig.c (see port/PORT_MAP.md for the full RAM map).
#ifndef ORIG_H
#define ORIG_H
typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;

#define REG16(a) (*(volatile u16*)(a))
#define REG32(a) (*(volatile u32*)(a))

// the original's RAM (IWRAM 0x03000000): it keeps these exact addresses until the whole game is ported
#define O_RNG          (*(volatile u32*)0x03000000)
#define O_DRAW_TARGET  (*(u16* volatile*)0x03000074)
#define O_SHOWN_PAGE   (*(volatile u32*)0x0300047C)

// the ported functions (orig.c)
void o_rng_mix(u32 x);
u16 *o_back_buffer(void);
void o_dma_copy(void *dst, const void *src);
void o_save_write(const u8 *src, int n);
int  o_save_read(u8 *dst, int n);
void o_hline(int x0, int x1, u32 y, u32 c);
void o_draw_box(int x, int y, int w, int h);
#endif
