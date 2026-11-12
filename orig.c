// The original Critter Keep game, ported into our source (port stage 2 onwards; see port/PORT_MAP.md).
// Each function here replaces one in CritterKeep_original.gba: patch.py points the original's own calls at it, and
// tools/lockstep.py checks the game still runs identically. Linked at 0x08012000 (the gap before ext.c), so ext.c's
// layout does not move while we port.
// Like ext.c: no writable globals. The original's state stays at its own addresses (orig.h).
#include "orig.h"

// ------------------------------------------------------------------ helpers (stage 2)

// 0x0800E840: mixes a value into the RNG (never leaves it 0 or 1)
void o_rng_mix(u32 x){ u32 v=O_RNG^x; if(v<2) v=1; O_RNG=v; }

// 0x0800F5F8: the page that is not on screen
u16 *o_back_buffer(void){ return O_SHOWN_PAGE? (u16*)0x0600A000 : (u16*)0x06000000; }

// 0x0800F614: copies a whole 240x160 page with DMA3
void o_dma_copy(void *dst, const void *src){
  REG32(0x040000D4)=(u32)src; REG32(0x040000D8)=(u32)dst; REG32(0x040000DC)=0x84002580u;   // 32-bit, 0x2580 words, start
}

// 0x0800F6C0: writes the save: "CK", then n bytes from offset 4
void o_save_write(const u8 *src, int n){
  volatile u8 *s=(volatile u8*)0x0E000000; s[0]='C'; s[1]='K';
  for(int i=0;i<n;i++) s[4+i]=src[i];
}
// 0x0800F6E0: reads the save if there is one (1), else 0
int o_save_read(u8 *dst, int n){
  volatile u8 *s=(volatile u8*)0x0E000000;
  if(s[0]!='C' || s[1]!='K') return 0;
  for(int i=0;i<n;i++) dst[i]=s[4+i];
  return 1;
}

// 0x0800F2A4: one horizontal span x0..x1 (inclusive) on row y in colour c, clipped to the screen. Mode 4 VRAM is
// written 16 bits at a time, so a lone odd or even end pixel keeps its neighbour.
void o_hline(int x0, int x1, u32 y, u32 c){
  if(y>159) return;
  if(x1>=239) x1=239;
  if(x0<0) x0=0;
  if(x0>x1) return;
  u8 *row=(u8*)O_DRAW_TARGET + y*240;
  // (volatile: VRAM must be written 16 bits at a time. A byte store, which the compiler would otherwise use for
  // these, writes the byte into both pixels of the pair on the GBA.)
  if(x0&1){ volatile u16 *p=(volatile u16*)(row+(x0&~1)); *p=(u16)((*p&0xFF)+(c<<8)); x0++; }
  int end=x1;
  if(!(x1&1) && (u32)x1>=(u32)x0){ volatile u16 *p=(volatile u16*)(row+x1); *p=(u16)((*p&~0xFF)|c); end=x1-1; }
  if(end<x0) return;
  u16 v=(u16)((c<<8)|c); volatile u16 *p=(volatile u16*)(row+x0);
  for(int n=(end-x0+1)>>1; n>0; n--) *p++=v;
}

// 0x0800EFB4: the message box: a rounded frame (colour 10), a lighter inside (15) and a line (38) near the bottom
void o_draw_box(int x, int y, int w, int h){
  o_hline(x+1, x+w-2, y, 10);
  for(int k=1; k<=h-2; k++) o_hline(x, x+w-1, y+k, 10);
  o_hline(x+1, x+w-2, y+h-1, 10);
  o_hline(x+3, x+w-4, y+2, 15);
  for(int k=3; k<=h-4; k++) o_hline(x+2, x+w-3, y+k, 15);
  o_hline(x+3, x+w-4, y+h-3, 15);
  o_hline(x+4, x+w-5, y+h-4, 38);
}
