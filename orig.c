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

// 0x0800F98C / F9D8 / FA34 / FAA0: the compiler's divide routines (unsigned and signed; the ...mod ones return the
// quotient in r0 and the remainder in r1). Dividing by 0 gives all ones, and the dividend as the remainder.
static u32 udivmod(u32 a, u32 b, u32 *rem){
  if(!b){ *rem=a; return 0xFFFFFFFFu; }
  u32 q=0, r=0;
  for(int i=31;i>=0;i--){ r=(r<<1)|((a>>i)&1); if(r>=b){ r-=b; q|=1u<<i; } }
  *rem=r; return q;
}
u32 o_udiv(u32 a, u32 b){ u32 r; return udivmod(a,b,&r); }
unsigned long long o_udivmod(u32 a, u32 b){ u32 r; u32 q=udivmod(a,b,&r); return ((unsigned long long)r<<32)|q; }
static s32 sdivmod(s32 a, s32 b, s32 *rem){
  if(!b){ *rem=a; return -1; }
  u32 r; u32 q=udivmod(a<0? -(u32)a : (u32)a, b<0? -(u32)b : (u32)b, &r);
  *rem= a<0? -(s32)r : (s32)r;
  return ((a^b)<0)? -(s32)q : (s32)q;
}
s32 o_sdiv(s32 a, s32 b){ s32 r; return sdivmod(a,b,&r); }
unsigned long long o_sdivmod(s32 a, s32 b){ s32 r; s32 q=sdivmod(a,b,&r); return ((unsigned long long)(u32)r<<32)|(u32)q; }

// 0x0800E854: shows a message (up to 38 characters) for 100 frames, with its chime; an awake critter turns to talk
void o_set_message(const char *s){
  volatile char *m=O_MSG_BUF; int n=0;
  while(s[n] && n<38){ m[n]=s[n]; n++; }
  O_MSG_TIMER=100; m[n]=0;
  if(O_PET[0x13]){ O_SND_IDX(0)=0; O_SND_PTR(0)=O_SEQ_MESSAGE; O_SND_CNT(0)=0; }   // (sound on)
  if(O_PET[4] && !O_PET[0x0C]){ O_ANIM_T=0; O_ANIM_KIND=2; O_ANIM_ARG=0; O_SCENE=3; }   // (hatched and awake)
}

// 0x0800EC74: text in the 5x7 font, 6 pixels a character, 9 a line ('\n'); scale 2+ draws each dot as a block.
// Lower case is drawn as upper case; other characters outside ' '..'_' only take up their space.
void o_draw_text(const char *s, int x, int y, u32 c, int scale){
  int x0=x;
  for(;;s++){
    u32 ch=(u8)*s;
    if(ch=='\n'){ y+=9*scale; x=x0; continue; }
    if(!ch) return;
    if(ch-0x61 < 26) ch-=0x20;
    if(!(ch-0x60 > 0xFFFFFFBFu)){ x+=6*scale; continue; }    // (not ' '..'_')
    const u8 *g=O_FONT+7*(ch-32); int ry=y;
    // (v8.3: rewritten for speed, same pixels: two pixels per VRAM write at size 1, and one line per run of lit
    // columns at bigger sizes, where it used to draw every pixel's lines one by one)
    u8 *base=(u8*)O_DRAW_TARGET;
    for(int row=0; row<7; row++, ry+=scale){
      u32 bits=g[row]&0x1F; if(!bits) continue;
      if(scale==1){
        u32 py=(u32)(y+row); if(py>159) continue;
        u8 *rp=base+py*240;
        for(int col=0; col<5; ){
          if(!((bits<<col)&0x10)){ col++; continue; }
          u32 px=(u32)(x+col); if(px>239){ col++; continue; }
          volatile u16 *p=(volatile u16*)(rp+(px&~1u));
          if(px&1){ *p=(u16)((*p&0xFF)|(c<<8)); col++; }
          else if(col<4 && ((bits<<(col+1))&0x10)){ *p=(u16)(c|(c<<8)); col+=2; }
          else { *p=(u16)((*p&~0xFF)|c); col++; }
        }
      } else if(scale>=1){
        for(int col=0; col<5; ){
          if(!((bits<<col)&0x10)){ col++; continue; }
          int e=col; while(e<4 && ((bits<<(e+1))&0x10)) e++;
          int bx=x+col*scale, ex=x+e*scale+scale-1;
          for(int k=0;k<scale;k++) o_hline(bx, ex, (u32)(ry+k), c);
          col=e+1;
        }
      }
    }
    x+=6*scale;
  }
}

// 0x0800F63C: a square-wave note on sound channel 1 (ch 0) or 2: period p (0 = stop), volume and envelope
void o_sound_play(int ch, int p, int vol, int env){
  u32 f=0x8000;
  if(p>=1){ f=o_udiv(1u<<17,(u32)p); if(f>=0x7FF) f=0x7FF; f=0xFFFF8800u-f; }
  if(ch){
    if(p<=0){ REG16(0x04000068)=0; REG16(0x0400006C)=0x8000; }
    else { REG16(0x04000068)=(u16)((vol<<12)|(env<<6)); REG16(0x0400006C)=(u16)f; }
  } else {
    if(p<=0){ REG16(0x04000062)=0; REG16(0x04000064)=0x8000; }
    else { REG16(0x04000060)=8; REG16(0x04000062)=(u16)((vol<<12)|(env<<6)); REG16(0x04000064)=(u16)f; }
  }
}

// 0x0800F070: a picture from a descriptor {pixels, w, h}: one byte a pixel (its low 7 bits index CMAP, 0 = see-through),
// at (x,y), each pixel a scale x scale block, mirrored if flip, only the first `limit` columns.
// A fast path writes whole pixel pairs when the picture is on screen at an even x with an even scale.
void o_draw_sprite(const void *desc, int x, int y, int scale, int flip, int limit){
  const u8 *pix=*(const u8* const*)desc; int w=((const u8*)desc)[4], h=((const u8*)desc)[5];
  if(!(scale&1) && !(x&0x80000001) && y>=0 && scale*w+x<=240 && scale*h+y<=160){
    if(!h) return;
    if(!w || limit<=0) return;               // (every row would be skipped)
    int last= (u32)(limit-1) < (u32)(w-1)? limit-1 : w-1;
    int half=scale>>1;
    u8 *base=(u8*)O_DRAW_TARGET + (x&~1);
    for(int j=0;j<h;j++){
      u8 *rowp=base + (scale*j+y)*240; const u8 *src=pix+w*j;
      for(int i=0;;i++){
        u32 c=O_CMAP[src[i]&0x7F];
        if(c && scale>=1){
          int col= flip? w-1-i : i;
          volatile u16 *p=(volatile u16*)(rowp + col*half*2); u16 v=(u16)((c<<8)+c);
          for(int k=0;k<scale;k++){ for(int m=0;m<half;m++) p[m]=v; p+=120; }
        }
        if(i==last) break;
      }
    }
    return;
  }
  for(int j=0, ry=y; j<h; j++, ry+=scale){
    if(!w) continue;
    const u8 *src=pix+w*j; u32 py=(u32)(y+j);
    for(int i=0;i<w;i++){
      if(i>=limit) continue;
      u32 c=O_CMAP[src[i]&0x7F]; if(!c) continue;
      int col= flip? w-1-i : i;
      if(scale==1){
        u32 px=(u32)(col+x);
        if(py>159 || px>239) continue;
        volatile u16 *p=(volatile u16*)((u8*)O_DRAW_TARGET + py*240 + (px&~1));
        u16 v=*p; *p= (px&1)? (u16)((v&0xFF)+(c<<8)) : (u16)((v&~0xFF)+c);
      } else if(scale>=1){
        int bx=x+col*scale;
        for(int k=0;k<scale;k++) o_hline(bx, bx+scale-1, (u32)(ry+k), c);
      }
    }
  }
}

void hook_sprite(const void *spr,int x,int y,int sc,int flip,int a5);   // (ext.c: the game's picture hook)
// 0x0800F37C: a meter of four hearts on the status card, the first n filled (x 80, 97, 114, 131; double size)
void o_draw_hearts(int y, int n){
  static const u8 cmap[][2]={{0x6F,1},{0x2E,0},{0x65,1},{0x63,0x13},{0x62,0x12},{0x61,0x11},{0x6D,1},{0x45,1},{0x79,0x16},
    {0x67,0x15},{0x77,2},{0x6B,2},{0x73,0x17},{0x72,0x14},{0x71,0x19},{0x70,0x18},{0x7A,0x1A},{0x6E,1}};
  for(int k=0;k<4;k++){
    for(unsigned i=0;i<sizeof cmap/2;i++) O_CMAP[cmap[i][0]]=cmap[i][1];
    O_CMAP[0x78]= n>k? 0x22 : 0x27;                    // the heart's fill
    hook_sprite(n>k? O_SPR_HEART : O_SPR_HEART_EMPTY, 80+17*k, y, 2, 0, 99);   // (through the game's hook, as before)
  }
}

// 0x0800EDB8: 40 twinkling stars over the sky (only on pixels still the sky's colour bg). Their places come from a
// golden-ratio hash; each twinkles on its own 8-step cycle (FRAMES/16), a cross on step 3.
static u32 px_get(u8 *row, int x){ u16 v=*(volatile u16*)(row+(x&~1)); return (x&1)? v>>8 : v&0xFF; }
static void px_set28(u8 *row, int x){   // colour 28, written the original's way (16 bits)
  volatile u16 *p=(volatile u16*)(row+(x&~1)); u16 v=*p;
  *p= (x&1)? (u16)((v&0xFF)|0x1C00) : (u16)((v|0x1C)&~0xE3);
}
void o_draw_stars(u32 bg){
  u8 *dt=(u8*)O_DRAW_TARGET; u32 t=O_FRAMES>>4, h=0;
  for(int i=0;i<40;i++, h+=0x9E3779B1u){
    int y=(int)((150*(h>>16))>>16);
    u32 hx=(h>>8)&0xFFFF, xr=hx*240; int x=(int)(xr>>16);
    u8 *row=dt+240*y; u32 phase=(t+i)&7;
    if(px_get(row,x)==bg){
      u32 c= phase? 0x1C : 0x0C;
      volatile u16 *p=(volatile u16*)(row+(x&~1)); u16 v=*p;
      *p= (x&1)? (u16)((v&0xFF)+(c<<8)) : (u16)((v&~0xFF)+c);
    }
    if(phase!=3) continue;
    if((u32)(x-1)<=0xEF && px_get(row,x-1)==bg) px_set28(row,x-1);
    if(hx<=0xFEEE && px_get(row,x+1)==bg) px_set28(row,x+1);
    if((u32)(y-1)<=0x9F && px_get(row-240,x)==bg) px_set28(row-240,x);
    if(px_get(row+240,x)==bg) px_set28(row+240,x);
  }
}

// 0x0800CC94: this frame's 40 UI colours (main copies them to the palette). Colours 17 and 18 come from a pair chosen by
// 0x0300007C. With the lights off (flag 0x03000078 = 1, an egg or older that is not gone) every colour but 9-16, 36,
// 38 and 39 is dimmed to 7/32; otherwise at night (from step 480) some take a blue night tint.
u16 *o_build_palette(void){
  u8 f=*(volatile u8*)0x03000078;
  int dim= f==1 && O_PET[0x0D] && O_PET[4]!=8;
  int night= f & ((*(volatile u32*)0x0300004C>>5) > 14);
  const u16 *pair=O_PAL_PAIRS + 2*(*(volatile u32*)0x0300007C);
  volatile u16 *out=(volatile u16*)0x03000080;
  // the night tint's colours (3-35): 1 = tinted
  static const u8 TINT[33]={1,1,1,1,0,1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,0,0,1,1,1};
  for(int i=0;i<40;i++){
    u32 c= i==17? pair[0] : i==18? pair[1] : O_PAL_BASE[i];
    int keep= (i>=9 && i<=16) || i==36 || i==38 || i==39;
    if(!keep){
      if(dim){
        u32 r=c&0x1F, g=(c>>5)&0x1F, b=(c>>10)&0x1F;
        c=((7*g)&0xE0) + ((7*r)>>5) + ((0xE0*b)&0x1C00);
      } else if(night && i>=3 && i<=35 && TINT[i-3]){
        u32 r=c&0x1F, g=(c>>5)&0x1F, b=(c>>10)&0x1F;
        c=((18*g)&~0x1Eu) + ((18*r)>>5) + ((0x2C0*b)&0x7C00) + 0x1000;
      }
    }
    out[i]=(u16)c;
  }
  return (u16*)0x03000080;
}

// 0x0800E8F0: starts a round of BERRY CATCH (scene 8): no berries yet, the catcher in the middle, 1200 frames on the
// clock, its tune; the room (day or night picture) copied to the scratch page with bands of colour 9 at the top and
// bottom, and the hint. (The original draws the hint with its own copy of draw_text; this is the same.)
void o_minigame_start(void){
  for(int k=0;k<8;k++) *(volatile u8*)(0x030002DC+9+12*k)=0;     // each berry's 'live' flag
  REG32(0x0300033C)=120; REG32(0x03000340)=0; REG32(0x03000348)=30; REG32(0x0300034C)=0;
  REG32(0x030002CC)=0; REG32(0x03000350)=0; REG32(0x03000354)=0; O_SCENE=8; REG32(0x03000344)=1200;
  if(O_PET[0x13]){ O_SND_IDX(1)=0; O_SND_PTR(1)=O_SEQ_BERRY; O_SND_CNT(1)=0; }
  O_DRAW_TARGET=(u16*)0x0201C200;
  o_dma_copy((void*)O_DRAW_TARGET, (*(volatile u32*)0x0300004C>>5)>=15? (void*)0x02009600 : (void*)0x02000000);
  volatile u32 *p=(volatile u32*)O_DRAW_TARGET;
  for(int i=0;i<26*60;i++){ p[i]=0x09090909u; p[134*60+i]=0x09090909u; }
  o_draw_text(O_STR_BERRY_HINT, 43, 143, 12, 1);
}

// 0x0800CE5C: startup. Sound on; the RNG seeded; the save loaded if it is valid (magic CKP1, a x31 checksum from
// 0x5EED in its last word, a stage of 8 or less), else a fresh start; the day and night rooms drawn (0x0800D690); the
// title page: colour 9, a hill (colour 21) and CRITTER / KEEP in big letters with shadows. Then the title scene.
void o_prerender(u8 *dst, int night);
void o_game_init(u32 seed){
  O_PET[0x13]=1; O_RNG=seed|1;
  u8 *buf=(u8*)0x03000430; int ok=0;
  if(o_save_read(buf,0x4C) && *(u32*)buf==0x31504B43u){
    u32 h=0x5EED; for(int i=0;i<0x48;i++) h=h*31+buf[i];
    ok= h==*(u32*)(buf+0x48) && buf[4]<=8;
  }
  if(ok){ for(int i=0;i<0x4C;i++) O_PET[i]=buf[i]; *(volatile u8*)0x03000280=1; }
  else { *(volatile u8*)0x03000280=0; *(volatile u16*)(O_PET+0x12)=0x100; }       // (not FAST, sound on)
  REG32(0x03000284)=0;
  o_prerender((u8*)0x02000000,0); o_prerender((u8*)0x02009600,1);
  O_DRAW_TARGET=(u16*)0x02012C00;
  { volatile u32 *p=(volatile u32*)0x02012C00; for(int i=0;i<9600;i++) p[i]=0x09090909u; }
  for(int y=-50;y<=50;y++){                       // the hill: half an ellipse 150 wide each way, 50 high, at (120,176)
    u32 a=0x57E4u*(u32)(y*y), b=375000; int w=150;
    while(w>0 && b*(u32)w+a>=0x35A4E91u){ b-=2500; w--; }
    o_hline(120-w, 120+w, (u32)(y+176), 21);
  }
  o_draw_text(O_STR_CRITTER, 62, 17, 10, 3); o_draw_text(O_STR_CRITTER, 59, 14, 13, 3);
  o_draw_text(O_STR_KEEP, 89, 43, 10, 3);    o_draw_text(O_STR_KEEP, 86, 40, 2, 3);
  O_SCENE=0;
}

// 0x0800D690: the room, by day (night=0) or night, drawn into a page. It depends on nothing else, so it is kept as the
// two pictures it draws (gen/orig_rooms.py -> orig_rooms.h), plus the colour-map entries it leaves set. Its last 8
// drawings are pictures through draw_sprite, which the game hooks (ext.c's hook_sprite), so those are made here.
#include "orig_rooms.h"
void o_prerender(u8 *dst, int night){
  O_DRAW_TARGET=(u16*)dst;
  const u8 *r= night? ROOM_NIGHT : ROOM_DAY; unsigned n= night? sizeof ROOM_NIGHT : sizeof ROOM_DAY;
  volatile u8 *d=dst;
  for(unsigned i=0;i<n;i+=2){ u8 c=r[i+1]; for(int k=r[i];k>0;k--) *d++=c; }
  for(int k=0;k<8;k++){            // its last 8 pictures, through draw_sprite as the game hooks it (ext.c)
    for(int i=0;i<ROOM_SPRITES[k].n;i++) O_CMAP[ROOM_SPRITES[k].cm[2*i]]=ROOM_SPRITES[k].cm[2*i+1];
    hook_sprite((const void*)ROOM_SPRITES[k].desc, ROOM_SPRITES[k].x, ROOM_SPRITES[k].y, ROOM_SPRITES[k].scale,
                ROOM_SPRITES[k].flip, ROOM_SPRITES[k].limit);
  }
  for(unsigned i=0;i<sizeof ROOM_CMAP;i+=2) O_CMAP[ROOM_CMAP[i]]=ROOM_CMAP[i+1];
}
