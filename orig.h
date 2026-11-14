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

#define O_FRAMES       (*(volatile u32*)0x03000018)
#define O_MSG_TIMER    (*(volatile u32*)0x0300001C)
#define O_SCENE        (*(volatile u32*)0x03000020)
#define O_PET          ((volatile u8*)0x03000028)
#define O_SND_PTR(c)   (*(volatile u32*)(0x03000288+4*(c)))
#define O_SND_CNT(c)   (*(volatile u8*)(0x03000290+(c)))
#define O_SND_IDX(c)   (*(volatile u8*)(0x03000292+(c)))
#define O_ANIM_KIND    (*(volatile u32*)0x03000298)
#define O_ANIM_T       (*(volatile u32*)0x0300029C)
#define O_ANIM_ARG     (*(volatile u32*)0x030002A0)
#define O_MSG_BUF      ((volatile char*)0x030002A4)
#define O_CMAP         ((volatile u8*)0x030003B0)      // picture colour map: a pixel's code -> palette index

// the original's data, still read from its ROM until it moves into our source (gen/orig_data.py)
#define O_FONT         ((const u8*)0x0800FC50)   // 5x7 font from ' ' (7 bytes a character, 5 bits a row)
#define O_SEQ_MESSAGE  0x0800FC0Cu              // the message chime (a note sequence)
#define O_SEQ_BERRY    0x0800FC24u              // BERRY CATCH's tune
#define O_STR_BERRY_HINT ((const char*)0x0801132F) // "LEFT/RIGHT: CATCH BERRIES!"
#define O_STR_CRITTER  ((const char*)0x08011122)
#define O_STR_KEEP     ((const char*)0x08010903)
#define O_PAL_BASE     ((const u16*)0x0800FB1C)   // the 40 UI colours
#define O_PAL_PAIRS    ((const u16*)0x0800FB6C)   // colours 17 and 18, a pair per mood
#define O_SPR_HEART       ((const void*)0x0800FFE4)   // picture descriptors {pixels, w, h}
#define O_SPR_HEART_EMPTY ((const void*)0x0801001C)

// the ported functions (orig.c)
void o_rng_mix(u32 x);
u16 *o_back_buffer(void);
void o_dma_copy(void *dst, const void *src);
void o_save_write(const u8 *src, int n);
int  o_save_read(u8 *dst, int n);
void o_hline(int x0, int x1, u32 y, u32 c);
void o_draw_box(int x, int y, int w, int h);
u32 o_udiv(u32 a, u32 b);
unsigned long long o_udivmod(u32 a, u32 b);
s32 o_sdiv(s32 a, s32 b);
unsigned long long o_sdivmod(s32 a, s32 b);
void o_set_message(const char *s);
void o_draw_text(const char *s, int x, int y, u32 c, int scale);
void o_sound_play(int ch, int p, int vol, int env);
void o_draw_sprite(const void *desc, int x, int y, int scale, int flip, int limit);
void o_draw_hearts(int y, int n);
void o_draw_stars(u32 bg);
u16 *o_build_palette(void);
void o_minigame_start(void);
void o_game_init(u32 seed);
void o_prerender(u8 *dst, int night);
#endif
