// Starfall Meadow (was Critter Keep) — extension module.
// Linked at 0x08020000 and hooked into the original game's main loop and room renderer.
// The original game logic is left intact. This module adds:
//  - pause menu with SAVE AND QUIT
//  - SELECT menu: Outside / Shop / Bag
//  - going outside (the room scene runs normally, the background becomes the yard)
//  - living sky (yard + room window) that follows the in-game clock
//  - daily shop (foods, care items, toys, decor), bag, home decor the critter uses
//  - 5 secret forms unlocked through care, critter cries
//  - minigame engine (Berry Catch 10s, Hop Hop, Left or Right?) with B to quit

typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;

// ------------------------------------------------------------------ original game interface
#define O_UPDATE   ((void(*)(u32))0x080001ADu)
#define O_RENDER   ((void(*)(void))0x08001FB5u)
#define O_BACKBUF  ((u16*(*)(void))0x0800F5F9u)
#define O_DMACOPY  ((void(*)(void*,const void*))0x0800F615u)
#define O_SOUND    ((void(*)(int,int,int,int))0x0800F63Du)
#define O_SAVE     ((void(*)(const volatile void*,int))0x0800F6C1u)
#define O_BOX      ((void(*)(int,int,int,int))0x0800EFB5u)
#define O_TEXT     ((void(*)(const char*,int,int,int,int))0x0800EC75u)
#define O_SPRITE   ((void(*)(const void*,int,int,int,int,int))0x0800F071u)

#define KEYS_PREV   (*(volatile u16*)0x03000010)
#define FRAMES      (*(volatile u32*)0x03000018)
#define TITLE_CONFIRM (*(volatile u8*)0x030002D0)   // 1 while "START OVER WITH A NEW EGG?" is up
#define MSG_TIMER   (*(volatile u32*)0x0300001C)
#define SCENE       (*(volatile u32*)0x03000020)
#define PET         ((volatile u8*)0x03000028)
#define PET_CLOCK   (*(volatile u32*)0x0300004C)
#define DRAW_TARGET (*(u16* volatile*)0x03000074)
#define TITLE_CUR   (*(volatile u32*)0x03000284)
#define ICON_CUR    (*(volatile u32*)0x030002D8)   // the icon bar's cursor (0 FEED, 1 LIGHTS, ...)
#define MSG_BUF     ((volatile char*)0x030002A4)
#define HELP_PAGE   (*(volatile u32*)0x030003AC)
#define CMAP        ((volatile u8*)0x030003B0)
#define SND_PTR(c)  (*(volatile u32*)(0x03000288+4*(c)))
#define SND_CNT(c)  (*(volatile u8*)(0x03000290+(c)))
#define SND_IDX(c)  (*(volatile u8*)(0x03000292+(c)))
#define PAL         ((volatile u16*)0x05000000)

#define P_STAGE 4
#define P_NAME 5
#define P_HUNGER 6
#define P_HAPPY 7
#define P_BOND 8
#define P_WEIGHT 9
#define P_SICK 10
#define P_POOP 11
#define P_ASLEEP 12
#define P_LIGHTS 13
#define P_CALL 14
#define P_FAST 0x12
#define P_SOUNDON 0x13
#define P_BEST 0x14

#define ROM_SPR_TABLE  ((const void* const*)0x0800FEB8u)
#define ROM_NAME_TABLE ((const char* const*)0x0800FE2Cu)   // 10 names followed by 8 form names
#define RAM_SPR   ((const void**)0x0203D000u)               // patched into the original's sprite lookups
#define RAM_NAMES ((const char**)0x0203D040u)               // patched into the original's name lookups
#define CRITTER_SPR(st) (RAM_SPR[(st)&7])
#define NAME_STR(i)     (ROM_NAME_TABLE[(i)])
#define SPR_ROCK  ((const void*)0x0800FF8Cu)
#define SPR_BERRY ((const void*)0x0800FF94u)
#define SPR_STAR  ((const void*)0x0800FF18u)

#define BG_ROOM_DAY   ((u8*)0x02000000u)
#define BG_ROOM_NIGHT ((u8*)0x02009600u)
#define YARD_BG       ((u8*)0x02026000u)   // composed background: yard, or room + static decor
#define SCRATCH_BG    ((u8*)0x0202F800u)

// original sound sequences
#define SEQ_HAPPY   ((const void*)0x0800FB90u)
#define SEQ_FANFARE ((const void*)0x0800FBA0u)
#define SEQ_EVOLVE  ((const void*)0x0800FBC0u)
#define SEQ_DENY    ((const void*)0x0800FC0Cu)
#define SEQ_BLIP    ((const void*)0x0800FC1Cu)
#define SEQ_CONFIRM ((const void*)0x0800FC24u)

#define K_A 1
#define K_B 2
#define K_SELECT 4
#define K_START 8
#define K_RIGHT 16
#define K_LEFT 32
#define K_UP 64
#define K_DOWN 128
#define K_R 256
#define K_L 512

// colours (original palette)
#define C_OUTLINE 1
#define C_WHITE 2
#define C_WALL 3
#define C_WALL2 4
#define C_WOOD 5
#define C_WOOD_DK 6
#define C_SKY 7
#define C_OFFWHITE 8
#define C_BAR 9
#define C_BORDER 10
#define C_LTEXT 11
#define C_HINT 12
#define C_ORANGE 13
#define C_RED 14
#define C_CREAM 15
#define C_DTEXT 16
#define C_PALEYEL 18
#define C_PINK 19
#define C_RED2 20
#define C_GREEN 21
#define C_GOLD 22
#define C_TEAL 23
#define C_BROWN2 24
#define C_TAN 25
#define C_GRAY 26
#define C_LTYEL 27
#define C_PALEYEL2 28
#define C_ROSE 29
#define C_BLUE 31
#define C_PALEBLUE 32
#define C_MINT 33
#define C_WOOD3 35
#define C_SLATE 36
#define C_ROYAL 37
#define C_KHAKI 38
#define C_LILAC 39
// extra colours (>= 64 are never touched by the original)
#define C_DKGREEN 64
#define C_DEEPGREEN 65
#define C_NGRASS 66
#define C_NGRASS2 67
#define C_LTGREEN 68
#define C_NWOOD 69
#define C_NWALL 70
#define C_TERRA 71
#define C_GLASS 72
// animated sky
#define C_SKY0 80
#define C_SKY1 81
#define C_SKY2 82
#define C_SKY3 83
#define C_STAR 84
#define C_CLOUD 85
#define C_CLOUD2 86
#define C_SUN 87
#define C_SUN2 88
#define C_MOON 89
// wall picture: 24 day colours + 24 night colours
#define C_PIC 96
#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))

// ------------------------------------------------------------------ runtime support
void *memset(void *d, int c, unsigned n){ u8 *p=d; while(n--) *p++=(u8)c; return d; }
void *memcpy(void *d, const void *s, unsigned n){ u8 *p=d; const u8 *q=s; while(n--) *p++=*q++; return d; }
void __aeabi_memset(void *d, unsigned n, int c){ memset(d,c,n); }
void __aeabi_memset4(void *d, unsigned n, int c){ memset(d,c,n); }
void __aeabi_memclr(void *d, unsigned n){ memset(d,0,n); }
void __aeabi_memclr4(void *d, unsigned n){ memset(d,0,n); }
void __aeabi_memcpy(void *d, const void *s, unsigned n){ memcpy(d,s,n); }
void __aeabi_memcpy4(void *d, const void *s, unsigned n){ memcpy(d,s,n); }
// v8.3: the divisor is shifted up to the dividend and back, a step per bit of the quotient (so x%5 for a small x is a few
// steps; it was over 30, enough that the night room's firefly jar pushed it past a frame on real hardware)
static u32 udivmod(u32 n, u32 d, u32 *rem){
  u32 q=0; int s=0;
  if(n>=d){
    while(!(d&0x80000000u) && (d<<1)<=n){ d<<=1; s++; }
    for(;;){ if(n>=d){ n-=d; q|=1u<<s; } if(!s) break; d>>=1; s--; }
  }
  if(rem) *rem=n; return q;
}
u32 __aeabi_uidiv(u32 n, u32 d){ return d? udivmod(n,d,0) : 0; }
s32 __aeabi_idiv(s32 n, s32 d){ if(!d) return 0; int neg=(n<0)^(d<0); u32 q=udivmod(n<0?-n:n, d<0?-d:d, 0); return neg?-(s32)q:(s32)q; }
unsigned long long __aeabi_uidivmod(u32 n, u32 d){ u32 r=0, q= d? udivmod(n,d,&r):0; return ((unsigned long long)r<<32)|q; }
long long __aeabi_idivmod(s32 n, s32 d){ u32 r=0; if(!d) return 0; u32 q=udivmod(n<0?-n:n, d<0?-d:d, &r); s32 qq=((n<0)^(d<0))?-(s32)q:(s32)q; s32 rr= n<0? -(s32)r : (s32)r; return ((long long)(u32)rr<<32)|(u32)qq; }

// ------------------------------------------------------------------ items
enum { IT_APPLE, IT_FISH, IT_CAKE, IT_CANDY, IT_SALAD, IT_PIZZA, IT_MILK, IT_COOKIE,
       IT_VITAMIN, IT_SOAP, IT_GIFT,
       IT_BALL, IT_KITE, IT_BUBBLES,
       IT_BED, IT_FISHBOWL, IT_PLANT, IT_RADIO, IT_CLOCK, IT_BOOKS,
       IT_TOYBOX, IT_TV, IT_PORTRAIT, IT_MOBILE, IT_LANTERN, IT_SWING, IT_SANDBOX, IT_BIRDBATH, IT_GARDEN,
       IT_FRISBEE, IT_SPRINKLER, IT_PINWHEEL,
       IT_BEANBAG, IT_PIANO, IT_ARCADE, IT_FIREPLACE, IT_MIRROR, IT_GUITAR, IT_DISCO, IT_PLANE,
       IT_TRAMPOLINE, IT_SLIDE, IT_POND, IT_FLOWERS, IT_COUNT };
#define N_FOODS 8
#define N_CONSUMABLE 11
#define IS_TOY(i) (((i)>=IT_BALL && (i)<=IT_BUBBLES) || ((i)>=IT_FRISBEE && (i)<=IT_PINWHEEL))
#define IS_DECOR(i) (((i)>=IT_BED && (i)<=IT_GARDEN) || (i)>=IT_BEANBAG)
#define TOYBIT(i) ((i)<=IT_BUBBLES? (i)-IT_BALL : (i)-IT_FRISBEE+3)
#define DECOBIT(i) ((i)-IT_BED)
// decor spots: 0 floor left, 1 floor right, 2 wall, 3 ceiling, 4 yard by the tree, 5 yard by the house
#define N_SLOTS 6
static const u8 SLOT_OF[30]={0,0,1,1,2,2, 0,1,2,3,3, 4,4,5,5, 0xFF,0xFF,0xFF, 0,0,1,1,2,2,3,3,4,4,5,5};
#define DECOR_SLOT(i) (SLOT_OF[(i)-IT_BED])
#define DECOR_SLOT_OK(i) (IS_DECOR(i)? DECOR_SLOT(i) : 0)
#define YARD_SLOT(s) ((s)>=4)

typedef struct { const char *name; const char *desc; u16 price; s8 hunger, happy, weight, bond; const char *art; } Item;

static const char ART_APPLE[] =
  "....oo...."  "...ogo...."  ".oooooooo."  "orrrrrrwwo"  "orrrrrrrwo"
  "orrrrrrrro"  "orrrrrrrro"  ".orrrrrro."  "..orrrro.."  "...oooo...";
static const char ART_FISH[] =
  ".........."  "....ooo..."  "o..ouuuo.."  "oooUuuuuo."  "ouUuuuuwuo"
  "ouUuuuuuuo"  "oooUuuuuo."  "o..ouuuo.."  "....ooo..."  "..........";
static const char ART_CAKE[] =
  "....rr...."  "...rRro..."  "..occcco.."  ".oppppppo."  "occcccccco"
  "oppppppppo"  "occcccccco"  "onnnnnnnno"  ".oooooooo."  "..........";
static const char ART_CANDY[] =
  "..oooo...."  ".opppwo..."  "oppwpppo.."  "opwppwpo.."  "opppwppo.."
  ".oppppo..."  "..oooo...."  "...on....."  "...on....."  "...oo.....";
static const char ART_SALAD[] =
  ".........."  "...g.G...."  "..gGggrg.."  ".gGgrggGg."  "oooooooooo"
  "oUUUUUUUUo"  ".oUUUUUUo."  "..oUUUUo.."  "...oooo..."  "..........";
static const char ART_PIZZA[] =
  "oooooooooo"  "onnnnnnnno"  ".oyryyyyo."  ".oyyyryyo."  "..oyryyo.."
  "..oyyyyo.."  "...oyro..."  "...oyyo..."  "....oo...."  "..........";
static const char ART_MILK[] =
  "...oooo..."  "..owwwwo.."  ".owwwwwwo."  "oooooooooo"  "owwwwwwwwo"
  "owuuuuuwwo"  "owuwwwuwwo"  "owuuuuuwwo"  "owwwwwwwwo"  "oooooooooo";
static const char ART_COOKIE[] =
  "...oooo..."  ".oonnnnoo."  ".onBnnnno."  "onnnnnBnno"  "onnBnnnnno"
  "onnnnnnBno"  ".onnnBnno."  ".oonnnnoo."  "...oooo..."  "..........";
static const char ART_VITAMIN[] =
  ".........."  ".....oooo."  "....orrrwo"  "...orrrrro"  "..owwrrro."
  ".owwwwoo.."  ".owwwwwo.."  "..owwwo..."  "...ooo...."  "..........";
static const char ART_SOAP[] =
  ".......o.."  "......olo."  "...o...o.."  "..olo....."  "...o......"
  ".oooooooo."  "ommmmmmwmo"  "ommmmmmmmo"  "ommmmmmmmo"  ".oooooooo.";
static const char ART_GIFT[] =
  "..o....o.."  ".oro..oro."  "..orooro.."  "oooooooooo"  "oPPPrrPPPo"
  "oooooooooo"  ".oPPrrPPo."  ".oPPrrPPo."  ".oPPrrPPo."  ".oooooooo.";
static const char ART_BALL[] =
  "...oooo..."  "..orrwwo.."  ".orrrwwuo."  "orrrwwuuuo"  "oyyywwuuuo"
  "oyyywwuuuo"  "oyyyywwuuo"  ".oyyywwuo."  "..oyywwo.."  "...oooo...";
static const char ART_KITE[] =
  "....oo...."  "...oPPo..."  "..oPPtto.."  ".oPPPttto."  "oPPPPtttto"
  ".otttPPPo."  "..otttPo.."  "...otPo..."  "....oo...."  ".....o.o..";
static const char ART_BUBBLES[] =
  ".....oooo."  "....os..wo"  "....o....o"  "....os..so"  ".....oooo."
  "....on...."  "...on....."  "..on......"  ".on......."  ".o........";
static const char ART_BED[] =
  ".........."  ".........."  ".........."  "..oooooooo"  ".owwoPPPPo"
  "owwwoPPPPo"  "oooooooooo"  "onnnnnnnno"  "oBBBBBBBBo"  ".o......o.";
static const char ART_FISHBOWL[] =
  "..oooooo.."  ".o......o."  "o.ssssss.o"  "ossYssssso"  "osYYYssswo"
  "ossYssssso"  "osssssssso"  ".osssssso."  "..oooooo.."  ".oBBBBBBo.";
static const char ART_PLANT[] =
  "...p..p..."  "..pYp.g.p."  "...p.gg.pY"  ".g..gg..p."  ".gg.g.gg.."
  "..ggggg..."  ".oooooooo."  "..oTTTTo.."  "..oTTTTo.."  "...oooo...";
static const char ART_RADIO[] =
  "........o."  ".......o.."  "......o..."  ".oooooooo."  "oRRRRRRRRo"
  "okkkRyyyRo"  "okokRyyyRo"  "okkkRRRRRo"  "oRRRRRRRRo"  ".oo....oo.";
static const char ART_CLOCK[] =
  "...oooo..."  ".oowwwwoo."  ".owwwowwo."  "owwwwowwwo"  "owwwwowwwo"
  "owwwwooowo"  "owwwwwwwwo"  ".owwwwwwo."  ".oowwwwoo."  "...oooo...";
static const char ART_BOOKS[] =
  ".........."  ".o.o..o..."  "oRoUoyoGo."  "oRoUoyoGoo"  "oRoUoyoGoP"
  "oRoUoyoGoP"  "oRoUoyoGoP"  "oRoUoyoGoP"  "oooooooooo"  "BBBBBBBBBB";

static const char ART_TOYBOX[] =
  "..o....oo."  ".oUo..oYYo"  ".oUo..oYYo"  "oooooooooo"  "oyyyyyyyyo"
  "oRRRRRRRRo"  "oRRRyyRRRo"  "oRRRRRRRRo"  "oRRRRRRRRo"  "oooooooooo";
static const char ART_TV[] =
  "...o..o..."  "....oo...."  "oooooooooo"  "okkkkkkkko"  "oksssssUko"
  "oksssUssko"  "okssssssko"  "okkkkkkkko"  "oooooooooo"  ".oo....oo.";
static const char ART_PORTRAIT[] =
  "oooooooooo"  "oBBBBBBBBo"  "oBccccccBo"  "oBcyyyycBo"  "oBcoyyocBo"
  "oBcyrrycBo"  "oBccccccBo"  "oBBBBBBBBo"  "oooooooooo"  "..........";
static const char ART_MOBILE[] =
  "....o....."  "..oooooo.."  "..o..o..o."  "..o..o..o."  ".yyy.o.yyy"
  "..y.yyy.y."  ".....y...."  ".........."  ".........."  "..........";
static const char ART_LANTERN[] =
  "....o....."  "...ooo...."  "..oRRRo..."  ".oRYRYRo.."  ".oRYRYRo.."
  ".oRYRYRo.."  "..oRRRo..."  "...ooo...."  "....y....."  "....y.....";
static const char ART_SWING[] =
  "oooooooooo"  ".o......o."  ".o......o."  ".o......o."  ".o......o."
  ".o......o."  ".o......o."  "nnnnnnnnnn"  "nBBBBBBBBn"  "..........";
static const char ART_SANDBOX[] =
  ".........."  "....r....."  "....oo...."  "...oyyo..."  "..oyyyyo.."
  ".ByyyyyyB."  "BBBBBBBBBB"  "BnnnnnnnnB"  "BBBBBBBBBB"  "..........";
static const char ART_BIRDBATH[] =
  "......oo.."  ".....oBBY."  "......oo.."  "oooooooooo"  "osssssssso"
  ".oooooooo."  "....kk...."  "....kk...."  "...kkkk..."  "..kkkkkk..";
static const char ART_GARDEN[] =
  "..g...g..."  ".grg.gYg.."  "..g...g.g."  ".ggg.ggggg"  "BBBBBBBBBB"
  "BBBBBBBBBB"  "w.w.w.w.w."  "wwwwwwwwww"  "w.w.w.w.w."  "..........";

static const char ART_FRISBEE[] =
  ".........."  ".........."  "...oooo..."  ".ooRRRRoo."  "oRRrrrrRRo"
  "oRrrwwrrRo"  ".ooRRRRoo."  "...oooo..."  ".........."  "..........";
static const char ART_SPRINKLER[] =
  "s...ss...s"  ".s.s..s.s."  "..s....s.."  "....oo...."  "...okko..."
  "....kk...."  "..oooooo.."  ".oggggggo."  ".oGGGGGGo."  "..oooooo..";
static const char ART_PINWHEEL[] =
  ".rr......."  ".rrr...yy."  "..rrr.yyy."  "...rwyyy.."  "..sswpp..."
  ".sss.ppp.."  ".ss...ppp."  ".....n.pp."  ".....n...."  ".....n....";
static const char ART_BEANBAG[] =
  ".........."  ".........."  "...oooo..."  ".oouwuuoo."  "ouwuUUuuuo"
  "ouuUUUUuuo"  "ouuuuuuuuo"  "ouuuuuuuuo"  ".oooooooo."  "..........";
static const char ART_PIANO[] =
  ".........."  "oooooooooo"  "oRRRRRRRRo"  "oRccccccRo"  "oRRRRRRRRo"
  "oooooooooo"  "owowwowowo"  "owwwwwwwwo"  "oRRRRRRRRo"  "oo......oo";
static const char ART_ARCADE[] =
  "..oooooo.."  "..oyyyyo.."  "..oooooo.."  "..ommmmo.."  "..omwmmo.."
  "..oooooo.."  ".oUrUyUUo."  "..oUUUUo.."  "..oUkkUo.."  "..oooooo..";
static const char ART_FIREPLACE[] =
  "BBBBBBBBBB"  "oooooooooo"  "oTTTTTTTTo"  "oToooooTTo"  "oTooyooToo"
  "oToyYyoTTo"  "oToYyYyoTo"  "oToBBBBoTo"  "oTTTTTTTTo"  "oooooooooo";
static const char ART_MIRROR[] =
  "...oooo..."  "..oyyyyo.."  ".oyLLLLyo."  ".oyLwLLyo."  ".oyLLwLyo."
  ".oyLLLLyo."  ".oyLLLLyo."  "..oyyyyo.."  "...oooo..."  "..........";
static const char ART_GUITAR[] =
  "........oo"  ".......ooo"  "......ob.."  ".....ob..."  "..oooob..."
  ".oYYYYo..."  "oYYooYYo.."  "oYYooYYo.."  ".oYBBYo..."  "..oooo....";
static const char ART_DISCO[] =
  "....o....."  "....o....."  "..oooooo.."  ".okwkwkwo."  ".owkwkwko."
  ".okwkwkwo."  ".owkwkwko."  "..oooooo.."  "w........p"  "..s....y..";
static const char ART_PLANE[] =
  ".....o...."  ".....o...."  "o....o...."  "Ro...o...."  "RRooooooo."
  "oRRRRRwwRo"  ".oRRRRRRo."  "...oRRo..."  "....oo...."  "..........";
static const char ART_TRAMPOLINE[] =
  ".........."  ".........."  "..oooooo.."  ".oUUUUUUo."  "oUooooooUo"
  ".oUUUUUUo."  "..oooooo.."  "..k....k.."  ".k......k."  "..........";
static const char ART_SLIDE[] =
  "oo........"  "oRo......."  "oRyo......"  "o.yyo....."  "oo.yyo...."
  "o...yyo..."  "oo...yyo.."  "o.....yyo."  "oo.....yyy"  "o......ooo";
static const char ART_POND[] =
  ".........."  "....g....."  "...gg..g.."  ".kkkkkkkk."  "kssusssusk"
  "kusssussuk"  "kuggusssuk"  ".kuuuuuuk."  "..kkkkkk.."  "..........";
static const char ART_FLOWERS[] =
  "..p...y..."  ".pyp.yry.."  "..p...y.l."  "..g.l.g.lw"  "..g.g.g.g."
  ".gggggggg."  "BBBBBBBBBB"  "BnBnBnBnBn"  "BBBBBBBBBB"  "..........";

static const Item ITEMS[IT_COUNT] = {
  {"APPLE",    "HUNGER +1",             8, 1,0,0,0, ART_APPLE},
  {"FISH",     "HUNGER +2",            14, 2,0,0,0, ART_FISH},
  {"CAKE",     "HAPPY +2  WEIGHT +2",  18, 0,2,2,0, ART_CAKE},
  {"CANDY",    "HAPPY +1  WEIGHT +1",   6, 0,1,1,0, ART_CANDY},
  {"SALAD",    "HUNGER +1 WEIGHT -2",  10, 1,0,-2,0,ART_SALAD},
  {"PIZZA",    "HUNGER +3 WEIGHT +2",  20, 3,0,2,0, ART_PIZZA},
  {"MILK",     "HUNGER +1",            10, 1,0,0,0, ART_MILK},
  {"COOKIE",   "HAPPY +1",              6, 0,1,0,0, ART_COOKIE},
  {"VITAMIN",  "CURES + NO SICKNESS TODAY",40, 0,0,0,0, ART_VITAMIN},
  {"SOAP",     "BUBBLE BATH: ALL CLEAN",15, 0,0,0,0, ART_SOAP},
  {"GIFT",     "HAPPY +1  BOND +1",    45, 0,1,0,1, ART_GIFT},
  {"BALL",     "YARD TOY: KICK IT AROUND",60, 0,0,0,0, ART_BALL},
  {"KITE",     "YARD TOY: MORE FRESH AIR",90, 0,0,0,0, ART_KITE},
  {"BUBBLES",  "YARD TOY: BUBBLES TO CHASE",45,0,0,0,0, ART_BUBBLES},
  {"BED",      "DECOR: NAPS + COMFY NIGHTS",120,0,0,0,0, ART_BED},
  {"FISHBOWL", "DECOR: A FISH TO WATCH",150,0,0,0,0, ART_FISHBOWL},
  {"PLANT",    "DECOR: GROWS EVERY DAY",  70,0,0,0,0, ART_PLANT},
  {"RADIO",    "DECOR: MUSIC TO DANCE TO",110,0,0,0,0, ART_RADIO},
  {"CLOCK",    "DECOR: SHOWS THE TIME",   80,0,0,0,0, ART_CLOCK},
  {"BOOKSHELF","DECOR: READING = BOND",  130,0,0,0,0, ART_BOOKS},
  {"TOYBOX",   "DECOR: TOYS TO PLAY WITH", 90,0,0,0,0, ART_TOYBOX},
  {"TV",       "DECOR: CARTOONS TO WATCH",160,0,0,0,0, ART_TV},
  {"PORTRAIT", "DECOR: ITS PICTURE! BOND",100,0,0,0,0, ART_PORTRAIT},
  {"MOBILE",   "CEILING: SPINNING STARS",  80,0,0,0,0, ART_MOBILE},
  {"LANTERN",  "CEILING: A COZY GLOW",     70,0,0,0,0, ART_LANTERN},
  {"SWING",    "YARD: A SWING ON THE TREE",140,0,0,0,0, ART_SWING},
  {"SANDBOX",  "YARD: BUILD SANDCASTLES", 100,0,0,0,0, ART_SANDBOX},
  {"BIRDBATH", "YARD: BIRDS COME TO VISIT",120,0,0,0,0, ART_BIRDBATH},
  {"GARDEN",   "YARD: GROWS FREE APPLES", 150,0,0,0,0, ART_GARDEN},
  {"FRISBEE",  "YARD TOY: CATCH = DOUBLE PLAY",75,0,0,0,0, ART_FRISBEE},
  {"SPRINKLER","YARD TOY: SPLASH AND PLAY",110,0,0,0,0, ART_SPRINKLER},
  {"PINWHEEL", "YARD TOY: MORE YARD COINS", 65,0,0,0,0, ART_PINWHEEL},
  {"BEANBAG",  "DECOR: A SQUISHY SEAT",     85,0,0,0,0, ART_BEANBAG},
  {"PIANO",    "DECOR: PLAYING = BOND",    170,0,0,0,0, ART_PIANO},
  {"ARCADE",   "DECOR: RETRO VIDEO GAMES", 180,0,0,0,0, ART_ARCADE},
  {"FIREPLACE","DECOR: A WARM, COZY FIRE", 160,0,0,0,0, ART_FIREPLACE},
  {"MIRROR",   "DECOR: MAKE FUNNY FACES",   90,0,0,0,0, ART_MIRROR},
  {"GUITAR",   "DECOR: STRUM A LITTLE SONG",120,0,0,0,0, ART_GUITAR},
  {"DISCO BALL","CEILING: DANCE PARTY!",   140,0,0,0,0, ART_DISCO},
  {"PLANE",    "CEILING: A LITTLE PLANE",   95,0,0,0,0, ART_PLANE},
  {"TRAMPOLINE","YARD: BOUNCE BOUNCE!",    130,0,0,0,0, ART_TRAMPOLINE},
  {"SLIDE",    "YARD: ZOOM DOWN THE SLIDE",160,0,0,0,0, ART_SLIDE},
  {"POND",     "YARD: CATCH A FREE FISH",  170,0,0,0,0, ART_POND},
  {"FLOWERBED","YARD: BUTTERFLIES VISIT",   90,0,0,0,0, ART_FLOWERS},
};

// v8: every item belongs to one kind of care; using it gives that kind a small boost (shown in the shop and bag)
enum { K_OUT=4, K_DECO=5, N_KINDS };
static const char *const KIND_NAME[N_KINDS]={"FOOD","PLAY","CLEAN","REST","OUTSIDE","DECOR"};
#define KF 0
#define KP 1
#define KC 2
#define KR 3
#define KO 4
#define KD 5   // v8.1: just to look at (no care boost)
static const u8 ITEM_KIND[IT_COUNT]={
  KF,KF,KF,KF,KF,KF,KF,KF, KR,KC,KP,           // apple fish cake candy salad pizza milk cookie, vitamin soap gift
  KP,KO,KP,                                     // ball kite bubbles
  KR,KR,KO,KP,KD,KR,                            // bed fishbowl plant radio clock books
  KP,KP,KD,KD,KR,KP,KP,KC,KO,                   // toybox tv portrait mobile lantern swing sandbox birdbath garden
  KP,KC,KO,                                     // frisbee sprinkler pinwheel
  KR,KP,KP,KR,KC,KP,KP,KD,                      // beanbag piano arcade fireplace mirror guitar disco plane
  KP,KP,KF,KO };                                // trampoline slide pond flowerbed
// v8.1: the journal's COLLECTION - every item, in the order the shop's kinds come in. A page opens for good the
// first time one is bought (More.found0/1, one bit per item).
#define JT_ITEMS 2
#define JT_GARDEN 3
#define JT_FLY 4      // v8.1: the firefly field guide
#define JT_RECORDS 5  // (v8.2c: it scrolls)
#define REC_ROWS 12
#define REC_SHOW 10
#define N_JTAB 6
static const u8 COL_ORDER[IT_COUNT]={ IT_APPLE,IT_FISH,IT_CAKE,IT_CANDY,IT_SALAD,IT_PIZZA,IT_MILK,IT_COOKIE, IT_VITAMIN,IT_SOAP,IT_GIFT,
  IT_BALL,IT_KITE,IT_BUBBLES,IT_FRISBEE,IT_SPRINKLER,IT_PINWHEEL,
  IT_BED,IT_FISHBOWL,IT_TOYBOX,IT_BEANBAG,IT_PIANO, IT_PLANT,IT_RADIO,IT_TV,IT_ARCADE,IT_FIREPLACE, IT_CLOCK,IT_BOOKS,IT_PORTRAIT,IT_MIRROR,IT_GUITAR,
  IT_MOBILE,IT_LANTERN,IT_DISCO,IT_PLANE,
  IT_SWING,IT_SANDBOX,IT_TRAMPOLINE,IT_SLIDE, IT_BIRDBATH,IT_GARDEN,IT_POND,IT_FLOWERS };
static const char *const COL_TEXT[IT_COUNT]={
  "A CRISP SNACK.|HUNGER +1.|A GARDEN IN THE YARD|GROWS ONE FREE A DAY.",
  "A HEARTY MEAL.|HUNGER +2.|A POND IN THE YARD|CATCHES ONE A DAY.",
  "A BIG SWEET TREAT.|HAPPY +2, WEIGHT +2.|TOO MANY AND IT GETS|CHUBBY!",
  "A LITTLE SWEET.|HAPPY +1, WEIGHT +1.",
  "A LIGHT, LEAFY MEAL.|HUNGER +1,|WEIGHT -2.|GOOD FOR A CHUBBY|CRITTER.",
  "A BIG CHEESY MEAL.|HUNGER +3,|WEIGHT +2.|FILLS IT RIGHT UP.",
  "A WARM GLASS.|HUNGER +1.",
  "A CRUNCHY TREAT.|HAPPY +1.|NO WEIGHT GAINED.",
  "CURES SICKNESS, THEN|KEEPS IT FROM GETTING|SICK FOR THE REST OF|THE DAY.",
  "A BUBBLE BATH.|CLEANS UP ANY MESS.",
  "A WRAPPED SURPRISE.|HAPPY +1, BOND +1.",
  "TAKE IT TO THE YARD.|IT KICKS THE BALL|AROUND. EVERY KICK|COUNTS AS PLAY.",
  "TAKE IT TO THE YARD.|FRESH AIR MAKES IT|HAPPY MORE OFTEN|WHILE IT FLIES.",
  "TAKE IT TO THE YARD.|BUBBLES DRIFT UP TO|CHASE. EVERY POP|COUNTS AS PLAY.",
  "A COMFY BED.|BOUNCING HEALS IT A|LITTLE. IT WAKES UP|HAPPY +1 EVERY DAY.",
  "A FISH TO WATCH.|WATCHING IT HEALS|IT A LITTLE.",
  "A LEAFY POT PLANT.|IT GROWS EACH DAY.|SNIFFING IT COUNTS|AS TIME WITH PLANTS.",
  "MUSIC TO DANCE TO.|DANCING COUNTS AS|PLAY.",
  "SHOWS THE TIME OF|DAY. IT'S JUST TO|LOOK AT.",
  "READING TOGETHER:|BOND +1, HEALS IT A|LITTLE. IT READS|LESS OFTEN THAN IT|PLAYS.",
  "A BOX OF TOYS.|PLAYING WITH THEM|COUNTS AS PLAY.",
  "CARTOONS TO WATCH.|COUNTS AS PLAY, ONLY|WHILE THE LIGHTS|ARE ON.",
  "ITS OWN PICTURE!|ADMIRING IT:|BOND +1.",
  "STARS THAT SPIN.|JUST LOVELY TO|WATCH.",
  "A COZY GLOW.|IT HEALS IT A|LITTLE.",
  "A SWING ON THE TREE.|SWINGING COUNTS AS|PLAY.",
  "A PILE OF SAND.|BUILDING CASTLES|COUNTS AS PLAY.",
  "BIRDS COME TO VISIT.|WHILE IT WATCHES, A|MESS TAKES LONGER|TO MAKE IT SICK.",
  "GROWS ONE FREE APPLE|A DAY (WHILE THE BAG|HAS ROOM). COUNTS AS|TIME WITH PLANTS.",
  "TAKE IT TO THE YARD.|IT GLIDES ACROSS. A|CATCH COUNTS AS|DOUBLE PLAY.",
  "TAKE IT TO THE YARD.|RUNNING THROUGH THE|SPRAY COUNTS AS PLAY|AND KEEPS IT CLEAN.",
  "TAKE IT TO THE YARD.|COINS TURN UP IN THE|GRASS MORE OFTEN, UP|TO 4 A DAY.",
  "A SQUISHY SEAT.|FLOPPING ON IT HEALS|IT A LITTLE.",
  "PLAYING TOGETHER:|BOND +1, COUNTS AS|PLAY.",
  "RETRO VIDEO GAMES.|COUNTS AS PLAY, ONLY|WHILE THE LIGHTS|ARE ON.",
  "A WARM FIRE.|IT HEALS IT A|LITTLE.",
  "FUNNY FACES! WHILE|IT PLAYS, A MESS|TAKES LONGER TO|MAKE IT SICK.",
  "A LITTLE SONG.|STRUMMING COUNTS AS|PLAY.",
  "A DANCE PARTY!|DANCING COUNTS AS|PLAY.",
  "A PLANE ON A STRING.|JUST FUN TO WATCH|IT FLY ROUND.",
  "BOUNCE BOUNCE!|BOUNCING COUNTS AS|PLAY.",
  "ZOOM DOWN!|SLIDING COUNTS AS|PLAY.",
  "CATCHES ONE FREE|FISH A DAY (WHILE|THE BAG HAS ROOM),|AND HUNGER +1.",
  "BUTTERFLIES VISIT.|CHASING THEM COUNTS|AS TIME WITH PLANTS." };
static const char ART_COIN[] = "..ooo.." ".oyyyo." "oyywyyo" "oyywyyo" "oyywyyo" ".oyyyo." "..ooo..";
static const char ART_HEART[] = ".rr.rr." "rrwrrrr" "rrrrrrr" ".rrrrr." "..rrr.." "...r...";
static const char ART_SPARK[] = "..w.." "....." "w.y.w" "....." "..w..";
static const char ART_NOTE[]  = "..oo." "..ooo" "..o.." "..o.." "ooo.." "ooo..";

// ------------------------------------------------------------------ secret forms
typedef struct { const char *data; u8 w, h, pad[2]; } Spr;
static const char SP_SUNPUFF[] =
  "................" ".......DD......." "..D....DD....D.." "...D.oooooo.D..." "....oAAAAAAo...."
  "...oAAAAAAAAo..." "DDoAAeAAAAeAAoDD" "DDoAAEAAAAEAAoDD" "..oAUAAAAAAUAo.." "..oAAAAAmmAAAo.."
  "..oABBBBBBBBAo.." "...oBBBBBBBBo..." "..D.oBBBBBBo.D.." ".D...oooooo...D." "................" "................";
static const char SP_CHONKLE[] =
  "................" "................" "..oo........oo.." ".oHFo......oFHo." ".oFFoooooooFFFo."
  "oFFFFFFFFFFFFFFo" "oFFFeFFFFFFeFFFo" "oFFFEFFFFFFEFFFo" "oFUFFFFmmFFFFUFo" "oFFFGGGGGGGGFFFo"
  "oFFGGGGGGGGGGFFo" "oFFGGGGGGGGGGFFo" "oFFGGGGGGGGGGFFo" ".oFFGGGGGGGGFFo." "..oooooooooooo.." "................";
static const char SP_ZIPPIT[] =
  "................" "..o.........o..." ".oIo.......oIo.." ".oIIoooooooIIo.." ".oIIIIIIIIIIIo.."
  "oIIeIIIIIIIeIIo." "oIIEIIIIIIIEIIoK" "oIUIIIIIIIIIUIoK" "oIIIIIImmIIIIIKK" ".oIJJJJJJJJJIoK."
  ".oIJJJJJJJJJIoKK" ".oIIJJJJJJJIIo.K" "..oIIIIIIIIIo..." "..oIIoooooIIo..." "..ooo.....ooo..." "................";
static const char SP_GLIMMER[] =
  ".......N........" "......NNN......." ".......N........" ".....oooooo....." "....oLLLLLLo...."
  "...oLLLLLLLLo..." "..oLLeLLLLeLLo.." "..oLLELLLLELLo.." "N.oLULLLLLLULo.N" "..oLLLLmmLLLLo.."
  "..oLMMMMMMMMLo.." "..oLMMMMMMMMLo.." "...oLMMMMMMLo..." "..N.oLLLLLLo.N.." "....oLooooLo...." "....ooo..ooo....";
static const char SP_NESTLE[] =
  "................" "................" "......oooo......" ".....oPPPPo....." "....oPPPPPPo...."
  "...oPPePPePPo..." "...oPPEPPEPPo..." "..oPUPPddPPUPo.." "..oPPPPmmPPPPo.." "..oPPPPPPPPPPo.."
  ".oRoPPPPPPPPoRo." "oRQRoooooooRQRoo" "oQRQRQRQRQRQRQRo" ".oQRQRQRQRQRQRo." "..oooooooooooo.." "................";
static const Spr SPECIAL_SPR[5] = { {SP_SUNPUFF,16,16,{0,0}}, {SP_CHONKLE,16,16,{0,0}}, {SP_ZIPPIT,16,16,{0,0}}, {SP_GLIMMER,16,16,{0,0}}, {SP_NESTLE,16,16,{0,0}} };
static void sprite(const void *spr,int x,int y,int sc,int flip,int a5);
#include "critters.h"
// v7 growth tree: egg -> BLIPLET -> SPROUTLE -> a teen picked by care -> a grown-up -> maybe a secret form.
// Forms 0..12 keep their old numbers (old albums), the new ones are 13..20. MOSSMOO, DRIZZLE and CHONKLE stay
// in the ROM for old albums / later use, but nothing grows into them any more.
#define F_FLUFFBUD 3
#define F_FRILLO   4
#define F_STARWING 5
#define F_BLOOMLE  13
#define F_THORNLE  14
#define F_OAKHART  15
#define F_DUNEROSE 16
#define F_THORNHOG 17
#define F_SMOGWOOL 18
#define F_GUNKBILL 19
#define F_PETALAMB 20
#define F_LILYBILL 21
#define F_JAMBLE   22
#define N_FORMS    23
static const Spr NEW_SPR[10] = { {SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},
  {SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}},{SP_SUNPUFF,16,16,{0,0}} };   // identities for the critter renderer
static const u8 SPECIAL_FORM[10] = { 0, 8, 9, 10, 11, 12, F_STARWING, F_PETALAMB, F_LILYBILL, F_JAMBLE };   // ext.special -> form
enum { ST_EGG, ST_BABY, ST_CHILD, ST_TEEN, ST_ADULT, ST_SECRET };
static const char *const STAGE_NAME[6] = { "EGG","BABY","CHILD","TEEN","GROWN-UP","SECRET" };
static const char *const SIDE_NAME[3] = { "", " - GOOD CARE", " - LESS CARE" };
// v8: everything about a form lives in this one table (name, side, stage, cry, bestiary text)
typedef struct { const char *name; u8 side, stage, shape, pad; u16 cry; const char *lore, *hint; } Form;
#define HINT_GOOD "A BLOOMLE CAN GROW|INTO THIS."
#define HINT_LESS "A THORNLE CAN GROW|INTO THIS."
static const Form FORMS[N_FORMS] = {
 /* 0*/ {"EGG",0,ST_EGG,0,0,0,
        "A STAR-SEED THAT FELL|WITH A SHOOTING STAR.|GIVE IT A LITTLE TIME|AND IT WILL HATCH.", "FALLS WITH A SHOOTING|STAR ON CLEAR NIGHTS."},
 /* 1*/ {"BLIPLET",0,ST_BABY,0,0,880,
        "A GUMDROP CHICK THAT|PEEPS AT ANYTHING|SHINY. ITS SPROUT|WILTS WHEN IT FEELS|LONELY.", "HATCHES FROM A|STAR-SEED."},
 /* 2*/ {"SPROUTLE",0,ST_CHILD,1,0,740,
        "IT SUNBATHES TO GROW|ITS LEAVES. HOW IT IS|CARED FOR NOW DECIDES|WHAT IT BECOMES.", "WHAT A BLIPLET GROWS|INTO."},
 /* 3*/ {"FLUFFBUD",1,ST_ADULT,0,0,620,
        "A BUNNY AS SOFT AS A|DANDELION CLOCK. IT|NEVER SAYS NO TO A|GAME, AND SHEDS SEEDS|WHEN IT LAUGHS.", HINT_GOOD"|PLAY LOTS OF GAMES."},
 /* 4*/ {"FRILLO",1,ST_ADULT,2,0,330,
        "ITS GILLS ARE LITTLE|FERN FRONDS THAT PERK|UP WHEN IT IS HAPPY.|IT LIKES A BIT OF|EVERYTHING.", HINT_GOOD"|CARE FOR IT EVENLY."},
 /* 5*/ {"STARWING",1,ST_SECRET,1,0,560,
        "SAID TO HAVE FALLEN|FROM A WISHING STAR.|IT ONLY STAYS WITH|SOMEONE WHO NEVER|LETS IT DOWN.", "A WELL-CARED-FOR|GROWN-UP WHOSE CARE|STAYS NEAR PERFECT."},
 /* 6*/ {"MOSSMOO",2,ST_ADULT,2,0,440,
        "A SLOW, GENTLE GRAZER.|THE TOADSTOOL ON ITS|BACK GROWS A NEW SPOT|FOR EVERY NAP.", ""},
 /* 7*/ {"DRIZZLE",2,ST_ADULT,2,0,300,
        "A SMALL RAIN CLOUD|THAT WATERS ITSELF.", ""},
 /* 8*/ {"SUNPUFF",1,ST_SECRET,0,0,700,
        "AN OAKHART THAT LIVED|IN THE SUN. ITS PETALS|TURN TO FOLLOW THE SUN|ALL DAY LONG.", "AN OAKHART THAT|GREETS THE MORNING|SUN ON 3 DAYS."},
 /* 9*/ {"CHONKLE",2,ST_SECRET,2,0,260,
        "IT STUFFS ITS CHEEKS|WITH SEEDS FOR LATER,|THEN FORGETS WHERE IT|PUT LATER.", ""},
 /*10*/ {"ZIPPIT",1,ST_SECRET,1,0,820,
        "FAST AS A COMET, IT|LEAVES A TRAIL OF|DANDELION FLUFF. IT|NEVER ADMITS IT LOST.", "A FLUFFBUD THAT|WISHES ON 2|SHOOTING STARS."},
 /*11*/ {"GLIMMER",1,ST_SECRET,0,0,990,
        "ITS CACTUS FLOWERS|OPEN AT NIGHT AND GLOW|LIKE LANTERNS. FIRE-|FLIES FOLLOW IT HOME.", "A DUNEROSE WITH 8|FIREFLIES IN ITS|LANTERN IN ONE NIGHT."},
 /*12*/ {"NESTLE",1,ST_SECRET,1,0,520,
        "IT WOVE ITS FRONDS|INTO A NEST AND GUARDS|THE EGG INSIDE. IT|LOVES A COZY HOME.", "A FRILLO IN A ROOM|WITH ALL 4 DECOR|SPOTS FILLED."},
 /*13*/ {"BLOOMLE",1,ST_TEEN,0,0,680,
        "ITS BUD HAS OPENED|INTO A FLOWER. IT|STANDS TALL, WAVES AT|EVERYONE, AND HUMS.", "A SPROUTLE THAT WAS|CARED FOR WELL."},
 /*14*/ {"THORNLE",2,ST_TEEN,2,0,560,
        "A LITTLE PRICKLY ON|THE OUTSIDE. GIVE IT|TIME - EVEN BRAMBLES|GROW BERRIES.", "A SPROUTLE THAT HAD|A HARDER TIME."},
 /*15*/ {"OAKHART",1,ST_ADULT,2,0,360,
        "THE OLD KEEPER OF THE|WOODS. BIRDS NEST IN|ITS ANTLERS, AND ITS|MOSS KEEPS IT WARM IN|WINTER.", HINT_GOOD"|SPEND LOTS OF TIME|OUTSIDE."},
 /*16*/ {"DUNEROSE",1,ST_ADULT,1,0,640,
        "WHEN IT BLOOMS, THE|WHOLE ROOM SMELLS|SWEET. IT LOVES TO BE|AROUND GROWING THINGS.", HINT_GOOD"|GROW UP AROUND|PLANTS."},
 /*17*/ {"THORNHOG",2,ST_ADULT,2,0,420,
        "IT CURLS UP IN ITS|BRAMBLE COAT WHEN IT|FEELS DOWN. THE|BERRIES ON ITS BACK|ARE SWEET.", HINT_LESS"|IT WAS SAD A LOT."},
 /*18*/ {"SMOGWOOL",2,ST_ADULT,2,0,300,
        "ITS WOOL SOAKED UP|SMOKE AND GRIME. IT|SIGHS LITTLE PUFFS OF|SMOG, BUT A GOOD WASH|MIGHT CHANGE THAT...", HINT_LESS"|IT LIVED IN A MESS."},
 /*19*/ {"GUNKBILL",2,ST_ADULT,2,0,380,
        "IT PADDLES IN AN OILY|POND AND LEAVES SLICK|FOOTPRINTS. IT STILL|DREAMS OF CLEAN WATER.", HINT_LESS"|IT WENT HUNGRY."},
 /*20*/ {"PETALAMB",2,ST_SECRET,0,0,760,
        "ONCE A SMOGWOOL. WITH|ITS WOOL WASHED CLEAN,|IT BURST INTO BLOOM.|PETALS FOLLOW IT.", "A SMOGWOOL KEPT|CLEAN FOR TWO DAYS."},
 /*21*/ {"LILYBILL",2,ST_SECRET,1,0,700,
        "ONCE A GUNKBILL.|WELL FED, IT SWAM|ITS POND CLEAN. NOW|LILIES BLOOM WHERE|IT PADDLES.", "A GUNKBILL KEPT|WELL FED FOR TWO|DAYS."},
 /*22*/ {"JAMBLE",2,ST_SECRET,0,0,620,
        "ONCE A THORNHOG.|KEPT HAPPY, ITS|THORNS WENT SOFT AND|ITS BERRIES RIPENED.", "A THORNHOG KEPT|HAPPY FOR TWO DAYS."},
};
// v8.1: before a secret form is found, the field guide shows only a riddle (the answers are in the manual)
static const char *const RIDDLE[N_FORMS] = {
  [F_STARWING]="NOT ONE NEED MISSED,|NOT ONE CALL|UNANSWERED.",
  [8]="WHO RISES WITH THE|SUN, MORNING AFTER|MORNING?",
  [10]="WHAT CHASES COMETS|FOR WISHES OF ITS|OWN?",
  [11]="WHOSE LANTERN IS FULL|OF LITTLE LIGHTS?",
  [12]="WHAT WEAVES ITS FERN|FRONDS WHEN EVERY|CORNER IS COZY?",
  [F_PETALAMB]="WHAT BLOOMS WHEN|SMOKY WOOL IS WASHED,|DAY AFTER DAY?",
  [F_LILYBILL]="NOT ONE HUNGRY DAY|FOR THE OILY|PADDLER.",
  [F_JAMBLE]="WHAT RIPENS ON A|BRAMBLE COAT THAT|SMILES FOR DAYS?",
};
#define DEX_N 20
static const u8 DEX_ORDER[DEX_N] = { 0,1,2, F_BLOOMLE,F_THORNLE, F_OAKHART,F_DUNEROSE,F_FLUFFBUD,F_FRILLO, F_THORNHOG,F_SMOGWOOL,F_GUNKBILL,
                                     F_STARWING,8,10,11,12, F_PETALAMB,F_LILYBILL,F_JAMBLE };
enum { SF_NONE, SF_SUNPUFF, SF_CHONKLE, SF_ZIPPIT, SF_GLIMMER, SF_NESTLE, SF_STARWING, SF_PETALAMB, SF_LILYBILL, SF_JAMBLE, SF_COUNT };
static void set_special_cmap(void){
  CMAP['A']=C_GOLD;  CMAP['B']=C_PALEYEL; CMAP['D']=C_ORANGE;               // sunpuff
  CMAP['F']=C_TAN;   CMAP['G']=C_CREAM;   CMAP['H']=C_PINK;                 // chonkle
  CMAP['I']=C_BLUE;  CMAP['J']=C_PALEBLUE; CMAP['K']=C_GOLD;               // zippit
  CMAP['L']=C_LILAC; CMAP['M']=C_PALEYEL2; CMAP['N']=C_LTYEL;              // glimmer
  CMAP['P']=C_MINT;  CMAP['Q']=C_WOOD3; CMAP['R']=C_TAN; CMAP['d']=C_ORANGE; // nestle
  CMAP['U']=C_PINK;                                                         // cheeks
}

// ------------------------------------------------------------------ state
#define G_MAGIC 0xC0FFEE43u
enum { M_NONE, M_MENU, M_EXTRAS, M_SENDOFF, M_SHOP, M_BAG, M_EAT, M_MINI, M_ALBUM, M_SURE, M_DEX, M_NAME, M_STORY, M_GARDEN, M_FLY };

typedef struct {
  u32 magic; u16 coins; u16 day;
  u8 inv[N_CONSUMABLE];
  u8 toys, decor; u8 slot[3]; u8 active_toy;
  u8 yard_coins, last_game, special, prev_stage, stars, plant_day, seen_tip;
  u16 adult_day, yard_mins;
  u32 sum;
} Ext;

typedef struct { s32 x, y, vx, vy; u8 type, active, flip, pad; } Obj;
// decor data (separate SRAM block; replaces the 3-spot fields in Ext)
typedef struct { u32 magic; u32 owned; u8 slot[N_SLOTS]; u8 plant_day, garden_day, pond_day, unlock_lvl; u8 vit_day, pad; u32 seed; u32 sum; } Deco;  // vit_day: (day+1) a VITAMIN protects
// life-cycle data (separate SRAM block so older saves stay valid)
typedef struct { u32 magic; u16 hatch_day; u16 care_good, care_bad; u8 retired, music_off, warned, pad; u32 sum; } Life;
// star album: the critters that earned a star since the stars last reset (SRAM 0x0E000200)
#define ALBUM_N 32
typedef struct { u32 magic; u8 n, pad[3]; u8 name[ALBUM_N], form[ALBUM_N]; u32 sum; } Album;
// bestiary + family line (SRAM 0x0E000280): which family this egg is, this critter's gender, and every form
// (and gender) ever raised. Older saves get it filled in from what they already show.
typedef struct { u32 magic; u8 gender, phase, side, adult; u8 teen_day, mess_day, games, yard;
                 u16 c_good, c_bad, c_sad, c_mess; u32 seen, boy, girl; u32 sum; } Dex;
enum { PH_OLD, PH_GROW, PH_TEEN, PH_ADULT };
// v8: one care record. Every care check (every 15 s) looks at five kinds of care: FOOD, PLAY, CLEAN and REST (each can
// be missing), and OUTSIDE (time in the yard, plants inside, flowers or a garden outside). It keeps two windows: while
// growing up (picks the teen and the grown-up) and as a grown-up (picks its secret form and its farewell).
enum { K_FOOD, K_PLAY, K_CLEAN, K_REST, N_NEEDS };
typedef struct { u16 n, good, bad; u16 miss[N_NEEDS]; u16 yard, garden, play, pad; } CareWin;   // counts of care checks
typedef struct { CareWin grow, adult; u8 run[N_NEEDS]; u8 miss_day[N_NEEDS]; } Care;
// (Care.run: checks in a row missing; miss_day: the last day it went missing, grown-ups)
// v8 phase 5: this critter's shop - the pieces unlocked so far (one bit per SHOP_POOL entry; a new egg starts over),
// plus this life's best PANCAKE TOWER
typedef struct { u32 un0; u8 un1, best, ok, pad; } Keep;
// v8 phase 5 fixes: the name the player gave this critter at hatching, and the names of the critters in the star album
// (6 letters, not 0-terminated when full; an empty one falls back to the original's 10 preset names)
#define NAME_LEN 6
typedef struct { char cur[8]; char alb[ALBUM_N][NAME_LEN]; } Names;
// v8 phase 6: the journal - a story for every critter that said farewell (a star each), and the records
#define STORY_N 40
typedef struct { char name[NAME_LEN]; u8 form, info, days, loved; } Story;   // info: gender (bit 0; bit 1 = not known), ending << 2
typedef struct { u16 n, raised, wonders, games; u32 coins; u8 loved[4]; Story s[STORY_N]; } Journal;
// v8.1 (save version 14): what was missing in the last day or so (the status card's !), the journal's COLLECTION
// (an item's page opens the first time it is bought, for good), and room for the night's extras
enum { R_HUNGRY, R_UNHAPPY, R_MESSY, R_SICK, R_LIGHTS, N_RMISS };
typedef struct { u32 found0, found1; u8 found_ok, rday, pad0, pad1; u16 rn[2]; u16 rmiss[2][N_RMISS]; u16 wishes, stories, spare[8]; } More;
// v8.1 B2: the moonflower garden (save part 10, v15). Three patches in the yard, tended at night while the critter sleeps.
enum { GK_SILVER, GK_ROSE, GK_GOLD, GK_SKY, GK_MINT, GK_STAR, GK_MOON, GK_TWIN, GK_AURORA, GK_N };
typedef struct { u8 stage, kind, col2, left, flags, pday; } Patch;   // stage 0 empty, 1 seed, 2 sprout, 3 bud, 4 bloom;
  // flags: 1 watered tonight, 2 missed a night; col2: a twinglow's two colours (low/high nibble); pday: the night it was planted
typedef struct { s16 x,y; s8 vx,vy; u8 k,t,live,wait,turn,pad; } Fly;   // v8.1: one firefly (x,y in 1/16 px)
typedef struct { Patch p[3]; u8 seeds, vis_day, vis_n, news; u8 vis[3], ok; u32 found; u16 blooms, visits; u16 spare[6]; } Garden;
  // found: bits 0-8 the flowers (GK_*), 9-18 the visitors - kept forever, like the journal

#define SPR_MAX 40   /* v8.3: 15 yard fireflies take 30 */
#define SPR_AFF 12
#define SPR_PALS 12
typedef struct {
  u32 magic;
  u16 prev, suppress;
  u8 mode, cur, confirm, from_help;
  u32 rng;
  u32 last_clock;
  Ext ext;
  u32 t, sky_t;
  // outside
  u8 outside, yard_night, poop_hold, coin_on;
  u16 fresh, coin_timer, coin_life, bubble_timer, plays, yard_frames;
  s32 coinx;
  s32 bx, by, bvx, bvy;
  Obj bub[4];
  // shop/bag
  u8 stock[5]; u8 bag_list[IT_COUNT]; u8 bag_n, bag_top, eat_item;
  // minigame
  u8 game, phase, rating, stun;
  s32 score, timer, px, pvy, py, spawn, round, guess, look, reveal, lastv, lastx;
  Obj obj[10];
  char sbuf[48];
  char sbuf2[48];
  s32 room_cx, room_cy;
  // decor + cries
  u16 decor_cd[N_SLOTS]; u8 fx_slot, fx_t, last_happy, last_call, last_asleep, check_special;
  u16 cry_delay, idle_cry, cry_cool;
  u16 cry_buf[2*14];
  s16 sk[6];
  u8 bg_state, pstage, shop_news, pad9;
  Life life;
  Deco deco;
  u8 tune, tune_vol, sendoff, sendoff_pending;
  u16 tune_pos, tune_timer, tune_gap, care_timer;
  u32 sendoff_t;
  u8 so_special, so_lost, so_bonus, sure_cur, help_page, drum_pos;
  u16 drum_timer;
  Album album;
  s8 cpose; u8 hp_prev; u16 happy_t;
  u8 bgx_yard, tbg, walk_t, stretch_t;
  Dex dex; u8 mess_run, tdex, dex_cur, dex_from;
  u8 was_asleep, facing, cpal_n, still;
  u16 cpal[40];
  s16 egg_rest; u8 egg_t, egg_ph;
  s32 prev_cx, prev_px;
  u32 sim_r, save_seq;
  Care care;
  u8 pick, pk_eat, pk_land, pk_pad;   // v8 phase 5: game picker cursor; PANCAKE TOWER stack
  s16 pk_x[16]; u8 pk_w[16];
  u8 st_seq[8];                       // STARGAZER's tune
  Keep keep;
  u16 mus_last, mus_acc;              // music clock (timer 3), so busy screens don't slow the music down
  Names names;
  u8 want_name, nm_x, nm_y, nm_pad; char nm[8];   // naming at hatch: cursor, the name being typed
  u16 dma_v, dma_pad;                 // dma_fill's source value
  // v8 phase 6: hardware effects - a fade (BLDY, 8.8 fixed), mosaic and a little screen bump; what fx_watch saw last frame
  u16 fx_lvl, fx_step; u8 fx_kind, mos_t, mos_len, mos_max, shake_t, fx_seen;
  u8 fx_scene, fx_out, fx_mode, fx_light, fx_stage, fx_pad;
  Journal jr; u8 jtab, jstar, jcare, jitem;   // v8 phase 6: the journal and its screen
  u8 mos_fix, zoom_t, gone_form, gone_bg; u16 gone_t, gone_pad;
  More more;   // v8.1
  Garden gdn;  // v8.1 B2
  u16 wish_t, wish_cd; u8 wish_x, wish_y, wish_got, wish_pad;   // v8.1: the wishing star crossing the night sky
  u8 st_cur, st_page, st_pad[2];                                // v8.1: bedtime stories (st_page 0xFF: picking one)
  u8 gcur, gwater, gdetail, gpad; u16 gt, gmsg_t; char gmsg[34];  // v8.1 B2: the night garden (gdetail: the journal page open)
  u16 gui_key, jg_key; u8 gmsg_n, g_anim, weak_warned, gpad2;
  u8 ex_yard, jt_first, v81pad[2];
  Fly fly[8]; u16 fly_t, fly_kinds, fly_night, fly_rel, fly_key;   // v8.1: a firefly round; tonight's lantern (fly_night: day+1)
  u8 fly_n, fly_rn, fly_state, fly_card, fly_cool, fly_pop, fly_popk, jx, jy, jfly, fpad[2];   // v8.1: the SELECT menu opened over the night yard; the first journal tab shown   // (weak_warned: v8.1 low-health warning given)                // (what was last drawn into SCRATCH_BG: words are slow to draw)   // farewell: held mosaic, slow zoom out; the runaway scene
  // v8.2: hardware sprites. The render fills this shadow; the next update (in vblank) copies it to OAM/palette
  u16 so[SPR_MAX][3]; s16 sa[SPR_AFF][4]; u16 sp[SPR_PALS][16];
  u8 spr_n, spr_naff, spr_npal, spr_ready, spr_live, spr_shown, spr_win, spr_pad; u8 win[4];   // win: x0,x1,y0,y1
  u8 jrec, yp_n, spr_bl, spr_bl_ok;   // (spr_bl: Bliplet sprites wanted this frame; spr_bl_ok: their tiles are in OBJ VRAM)   // v8.2c: the RECORDS tab's first row shown; the yard's poops to draw
  u8 fx_hold, fx_hpad[3];
  u16 dark_t, dark_pad;
  u16 yf_key; u8 yf_nk, yf_pad; u16 yf_pal[5][2][7];   // v8.3: the yard fireflies' palettes, for tonight's kinds (yf_key)               // v8.3: ticks awake by day with the lights off (an extra happy heart lost every DARK_TICKS)             // v8.3: frames a fade stays at its start (the runaway's cuts: until both pages hold the new shot)
} State;

#define G (*(State*)0x0203E000u)
// v8.2b: the yard keeps its own mess. More.pad1 bit 0: it was saved and quit from the yard.
// (Since the port: PET[P_POOP] is always the room's mess and More.pad0 always the yard's. The original's own code
// reads and clears the mess where the critter is through ext_mess_here(), and draws the room's through
// ext_mess_drawn(), so nothing is swapped any more. More.pad1 bit 1, the old swap flag, is unused.)
volatile u8 *ext_mess_here(void){ return G.outside? (volatile u8*)&G.more.pad0 : PET+P_POOP; }
u32 ext_mess_drawn(void){ return G.outside? 0 : PET[P_POOP]; }   // the room's poops (outside, yard_poops draws the yard's)
#define MESS_HERE (*ext_mess_here())
static void fly_update(u16 pr,u16 keys); static void fly_screen_draw(void); static void fly_start(void); static int fly_rounds_left(void);
static void lantern_draw(void); static void win_open_pre(void); static void win_open_post(void); static void fly_dance(void); static void fly_morning(void); static void fly_palette(void); static void jr_fly_keys(u16 pr);
#define HELP_PAGES 5

// ------------------------------------------------------------------ small utils
static u32 rnd32(void){ u32 x=G.rng; x^=x<<13; x^=x>>17; x^=x<<5; G.rng=x; return x; }
static int rnd(int n){ return (int)(((rnd32()>>16) * (u32)n) >> 16); }
static int clampi(int v,int lo,int hi){ return v<lo?lo:(v>hi?hi:v); }
static int slen(const char *s){ int n=0; while(s[n]) n++; return n; }
static char *scpy(char *d, const char *s){ while(*s) *d++=*s++; *d=0; return d; }
static char *sint(char *d, int v){ char t[12]; int n=0; if(v<0){ *d++='-'; v=-v; } do { int q=0; while(v>=10){ v-=10; q++; } t[n++]=(char)('0'+v); v=q; } while(v); while(n) *d++=t[--n]; *d=0; return d; }
static int fast_mode(void){ return PET[P_FAST]!=0; }
static int fastdiv(int n){ return fast_mode()? (n*2)/5 : n; }   // v8: FAST runs everything 2.5 times as fast
static int is_night(void){ return (PET_CLOCK>>5) > 14; }
static const char *pet_name(void){ return G.names.cur[0]? G.names.cur : NAME_STR(PET[P_NAME]); }

// ------------------------------------------------------------------ drawing (mode 4, 16-bit writes only)
static void pset(int x,int y,int c){
  if((unsigned)x>=240u||(unsigned)y>=160u) return;
  volatile u16 *p=DRAW_TARGET+((y*240+x)>>1);
  if(x&1) *p=(u16)((*p&0x00FF)|(c<<8)); else *p=(u16)((*p&0xFF00)|c);
}
static int pget(int x,int y){ volatile u16 *p=DRAW_TARGET+((y*240+x)>>1); return (x&1)? (*p>>8) : (*p&0xFF); }
static void fill(int x,int y,int w,int h,int c){
  if(x<0){ w+=x; x=0; } if(y<0){ h+=y; y=0; } if(x+w>240) w=240-x; if(y+h>160) h=160-y; if(w<=0||h<=0) return;
  u16 cc=(u16)(c|(c<<8));
  for(int yy=y; yy<y+h; yy++){
    int x0=x, x1=x+w-1;
    if(x0&1){ pset(x0,yy,c); x0++; }
    if(!(x1&1)){ pset(x1,yy,c); x1--; }
    volatile u16 *p=DRAW_TARGET+((yy*240+x0)>>1);
    for(int xx=x0; xx<x1; xx+=2) *p++=cc;
  }
}
static void disc(int cx,int cy,int r,int c){ for(int dy=-r; dy<=r; dy++){ int w=0; while((w+1)*(w+1)+dy*dy<=r*r+r) w++; fill(cx-w,cy+dy,2*w+1,1,c); } }
static void ring(int cx,int cy,int r,int c){ for(int dy=-r; dy<=r; dy++) for(int dx=-r; dx<=r; dx++){ int d=dx*dx+dy*dy; if(d<=r*r+r && d>=(r-1)*(r-1)+(r-1)) pset(cx+dx,cy+dy,c); } }
static void text(const char *s,int x,int y,int c){ O_TEXT(s,x,y,c,1); }
static void textc(const char *s,int cx,int y,int c){ text(s, cx-(slen(s)*6-1)/2, y, c); }
static void texts(const char *s,int x,int y,int c){ O_TEXT(s,x+1,y+1,C_BORDER,1); O_TEXT(s,x,y,c,1); }
static void textbig(const char *s,int cx,int y,int c,int sc){ int w=slen(s)*6*sc; O_TEXT(s,cx-w/2+sc,y+sc,C_BORDER,sc); O_TEXT(s,cx-w/2,y,c,sc); }
// v8.1: the minigames' big words, with a dark outline all round so they read on a busy room or yard
static void textbig_o(const char *s,int cx,int y,int c,int sc){ int w=slen(s)*6*sc, x=cx-w/2;
  O_TEXT(s,x-1,y-1,C_OUTLINE,sc); O_TEXT(s,x+sc,y+sc,C_OUTLINE,sc); O_TEXT(s,x,y,c,sc); }   // (two dark copies, up-left and down-right: big words are slow to draw)

static u8 cmap_item(char ch){
  switch(ch){
    case 'o': return C_OUTLINE; case 'r': return C_RED; case 'R': return C_RED2; case 'g': return C_GREEN; case 'G': return C_DKGREEN;
    case 'y': return C_GOLD; case 'Y': return C_ORANGE; case 'w': return C_WHITE; case 'u': return C_BLUE; case 'U': return C_ROYAL;
    case 'p': return C_PINK; case 'P': return C_ROSE; case 'c': return C_CREAM; case 'n': return C_TAN; case 'B': return C_WOOD_DK;
    case 'm': return C_MINT; case 'l': return C_LILAC; case 't': return C_TEAL; case 's': return C_SKY; case 'k': return C_GRAY;
    case 'T': return C_TERRA; case 'L': return C_GLASS; case 'b': return C_BROWN2; case 'S': return C_SLATE;
  }
  return 0;
}
static void art(const char *a,int w,int h,int x,int y,int sc,int cols){
  if(cols>w) cols=w;
  for(int r=0;r<h;r++){ const char *row=a+r*w; int yy=y+r*sc;
    for(int q=0;q<cols;q++){ u8 c=cmap_item(row[q]); if(!c) continue;
      if(sc==1) pset(x+q,yy,c);
      else { int xx=x+q*sc;
        if(sc==2 && !(xx&1) && (unsigned)xx<239u && (unsigned)yy<159u){ u16 cc=(u16)(c|(c<<8)); volatile u16 *p=DRAW_TARGET+((yy*240+xx)>>1); p[0]=cc; p[120]=cc; }
        else for(int k=0;k<sc;k++){ for(int j=0;j<sc;j++) pset(xx+j,yy+k,c); } } } }
}
static void item_art(int it,int x,int y,int sc){ art(ITEMS[it].art,10,10,x,y,sc,10); }
static void coin_icon(int x,int y){ art(ART_COIN,7,7,x,y,1,7); }

static void label(const char *s){
  int w=slen(s)*6+7, x=120-w/2, y=33;
  fill(x+1,y,w-2,12,C_BORDER); fill(x,y+1,w,10,C_BORDER);
  textc(s,120,y+3,C_LTEXT);
}
static void set_msg(const char *s){ int i=0; while(s[i] && i<37){ MSG_BUF[i]=s[i]; i++; } MSG_BUF[i]=0; MSG_TIMER=110; }
static void set_msg_name(const char *suffix){ char b[48]; char *p=scpy(b,pet_name()); scpy(p,suffix); set_msg(b); }

// ------------------------------------------------------------------ sound
static void sfx(int ch,const void *seq){ if(!PET[P_SOUNDON]) return; SND_PTR(ch)=(u32)seq; SND_CNT(ch)=0; SND_IDX(ch)=0; }
static void sound_tick(void){
  for(int ch=0; ch<2; ch++){
    u32 p=SND_PTR(ch); if(!p) continue;
    if(SND_CNT(ch)){ SND_CNT(ch)--; continue; }
    const u8 *e=(const u8*)p + SND_IDX(ch)*4; u8 dur=e[2];
    if(!dur){ O_SOUND(ch,0,0,0); SND_PTR(ch)=0; continue; }
    u16 f=(u16)(e[0]|(e[1]<<8));
    if(f<2) O_SOUND(ch,0,0,0); else O_SOUND(ch,f,0xB,2);
    SND_IDX(ch)++; SND_CNT(ch)=(u8)(dur-1);
  }
}
static void silence(void){ SND_PTR(0)=0; SND_PTR(1)=0; O_SOUND(0,0,0,0); O_SOUND(1,0,0,0); }
typedef struct { u16 f; u8 d, pad; } Note;
static const Note SEQ_COIN[]  = {{1568,4,0},{2093,9,0},{0,0,0}};
static const Note SEQ_CATCH[] = {{1047,3,0},{1319,4,0},{0,0,0}};
static const Note SEQ_STAR[]  = {{1319,3,0},{1568,3,0},{2093,6,0},{0,0,0}};
static const Note SEQ_BONK[]  = {{196,5,0},{147,8,0},{0,0,0}};
static const Note SEQ_JUMP[]  = {{523,3,0},{784,4,0},{0,0,0}};
static const Note SEQ_DOOR[]  = {{392,5,0},{523,5,0},{659,8,0},{0,0,0}};
static const Note SEQ_TICK[]  = {{880,3,0},{0,0,0}};
static const Note SEQ_GO[]    = {{1047,10,0},{0,0,0}};
static const Note SEQ_KICK[]  = {{330,2,0},{440,3,0},{0,0,0}};
static const Note SEQ_POP[]   = {{1760,2,0},{2349,2,0},{0,0,0}};
// Phase 7 jingles: opening the journal, the farewell's beam and new star, the runaway
static const Note SEQ_JOURNAL[] = {{784,4,0},{988,4,0},{1175,4,0},{1568,10,0},{0,0,0}};
static const Note SEQ_BEAM[]    = {{523,4,0},{659,4,0},{784,4,0},{1047,4,0},{1319,12,0},{0,0,0}};
static const Note SEQ_NEWSTAR[] = {{1319,4,0},{1760,4,0},{2093,4,0},{2637,6,0},{2093,4,0},{2637,16,0},{0,0,0}};
static const Note SEQ_FAINTSTAR[]={{880,8,0},{784,8,0},{659,20,0},{0,0,0}};
static const Note SEQ_SHOOT[]   = {{2637,2,0},{2349,2,0},{2093,2,0},{1760,3,0},{0,0,0}};   // v8.1: a wishing star
static const Note SEQ_WISH[]    = {{1047,3,0},{1568,3,0},{2093,3,0},{2637,4,0},{3136,10,0},{0,0,0}};
static const Note SEQ_RUNAWAY[] = {{523,10,0},{440,10,0},{392,10,0},{330,24,0},{0,0,0}};
#include "tunes.h"
#include "picture.h"
#include "bgfx.h"
#include "decor.h"
#include "icons.h"
#include "foodart.h"
#include "baricons.h"
#include "minis.h"
#include "title.h"
#include "sprfx.h"
#include "poop.h"

// ---- background music on the wave channel (the original only uses the two square channels)
#define REG16(a) (*(volatile u16*)(a))
#define REG32(a) (*(volatile u32*)(a))
static void music_mute(void){ REG16(0x04000072)=0; }
static void drums_mute(void){ REG16(0x04000078)=0; REG16(0x0400007C)=0x8000; }
// soft percussion on the noise channel: envelope (vol, step) and noise settings per sound
static void drum_hit(int c){
  static const u16 ENV[6]={0, 0x6100, 0x4200, 0x3100, 0x2200, 0x2100};   // kick, brush, hat, shaker, cricket
  static const u16 NZ[6] ={0, 0x0056, 0x0021, 0x0001, 0x0011, 0x0008};
  u16 e=ENV[c];
  if(G.tune_vol==3){ int v=(e>>12)>>1; if(v<1) v=1; e=(u16)((e&0x0FFF)|(v<<12)); }   // quieter at night
  REG16(0x04000080)=(u16)(REG16(0x04000080)|0x8800);
  REG16(0x04000078)=e; REG16(0x0400007C)=(u16)(0x8000|NZ[c]);
}
static void music_setup(void){
  static const u8 WAVE[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};
  REG16(0x04000070)=0x40;                         // play bank 1 so writes land in bank 0
  for(int i=0;i<8;i++) REG16(0x04000090+2*i)=(u16)(WAVE[2*i]|(WAVE[2*i+1]<<8));
  REG16(0x04000070)=0x80;                         // play bank 0
  REG16(0x04000080)=(u16)(REG16(0x04000080)|0x4400); // mix channel 3 left + right
}
static void music_play(int tune,int vol){
  if(G.tune==tune && G.tune_vol==vol) return;
  if(G.tune!=tune){ G.tune_pos=0; G.tune_timer=0; G.tune_gap=0; G.drum_pos=0; G.drum_timer=0; drums_mute(); music_setup(); }
  G.tune=(u8)tune; G.tune_vol=(u8)vol;
}
static void music_stop(void){ if(G.tune!=0xFF){ G.tune=0xFF; music_mute(); drums_mute(); } }
static void music_tick(void){
  if(G.tune==0xFF) return;
  { const u8 *d=DRUMS[G.tune];
    if(d){ if(!G.drum_timer){ u8 c=d[2+G.drum_pos]; if(c) drum_hit(c); if(++G.drum_pos>=d[1]) G.drum_pos=0; G.drum_timer=d[0]; } G.drum_timer--; } }
  if(G.tune_timer){ G.tune_timer--; if(G.tune_timer==G.tune_gap && G.tune_gap) music_mute(); return; }
  const u8 *t=TUNES[G.tune];
  if(t[G.tune_pos]==255) G.tune_pos=0;
  u8 n=t[G.tune_pos], len=t[G.tune_pos+1]; G.tune_pos+=2;
  if(n && NOTE_RATE[n]){ REG16(0x04000072)=(u16)(G.tune_vol==2? 0x4000 : G.tune_vol==3? 0x6000 : 0x2000); REG16(0x04000074)=(u16)(NOTE_RATE[n]|0x8000); }
  else music_mute();
  G.tune_timer=(u16)(len-1); G.tune_gap = len>4? 2 : 0;
}

// ---- critter cries: one shape per family, pitched per form (like early handheld monster games)
// pitch table: form index 0..7 = original stages, 8..12 = secret forms
// multipliers in 1/16 steps, 2 frames per step
static const u8 CRY_SHAPES[3][10] = {
  {16,20,24,26,24,21,18,15,13, 0},   // chirp: up then down
  {16,22,16,22,17,23,15,13, 0, 0},   // warble
  {12,14,13,16,19,22,22,18,14,11},   // squawk: low rise
};
static int form_index(void){ u8 st=PET[P_STAGE]; if(st>7) return 0;
  if(st>=3){
    if(G.ext.special) return SPECIAL_FORM[G.ext.special<SF_COUNT? G.ext.special : 0];
    if(G.dex.phase==PH_TEEN) return G.dex.side==2? F_THORNLE : F_BLOOMLE;
    if(G.dex.phase==PH_ADULT && G.dex.adult) return G.dex.adult;
  }
  return st; }
static const char *form_name(int f){ return FORMS[f<N_FORMS? f : 0].name; }
static const void *form_spr(int f){ if(f>=13) return &NEW_SPR[f-13]; if(f>=8) return &SPECIAL_SPR[f-8]; return ROM_SPR_TABLE[f&7]; }
// which side a Sproutle is heading for: 7 in 10 good care checks (or none taken yet) = the good side
static int care_side(void){ u32 g=G.care.grow.good, b=G.care.grow.bad; return (g+b==0 || g*10>=(g+b)*7)? 1 : 2; }
static void cry(void){
  int f=form_index(); if(!f || !PET[P_SOUNDON]) return;
  const u8 *sh=CRY_SHAPES[FORMS[f].shape]; int n=0;
  for(int i=0;i<10 && sh[i];i++){ u16 hz=(u16)(((u32)FORMS[f].cry*sh[i]*(G.dex.gender? 17u : 15u))>>8); G.cry_buf[n*2]=hz; G.cry_buf[n*2+1]=2; n++; }
  G.cry_buf[n*2]=0; G.cry_buf[n*2+1]=0;
  sfx(1,G.cry_buf); G.cry_cool=90;
}

// ------------------------------------------------------------------ extended save (SRAM 0x0E000100)
#define EXT_MAGIC 0x32584B43u /* "CKX2" */
#define EXT1_MAGIC 0x31584B43u /* "CKX1" (first update) */
// every save block starts with a magic word and ends with a checksum of the bytes in between
static u32 blk_sum(const void *b,unsigned n){ const u8 *p=(const u8*)b; u32 h=0x5EED; for(unsigned i=0;i<n-4;i++) h=h*31+p[i]; return h; }
static void blk_save(void *b,unsigned n,u32 magic,u32 addr){
  *(u32*)b=magic; *(u32*)((u8*)b+n-4)=blk_sum(b,n);
  const u8 *p=(const u8*)b; volatile u8 *s=(volatile u8*)addr; for(unsigned i=0;i<n;i++) s[i]=p[i]; }
static int blk_load(void *b,unsigned n,u32 magic,u32 addr){
  u8 *p=(u8*)b; volatile u8 *s=(volatile u8*)addr; for(unsigned i=0;i<n;i++) p[i]=s[i];
  return *(u32*)b==magic && *(u32*)(p+n-4)==blk_sum(b,n); }
#define SRAM_EXT   0x0E000100u
#define SRAM_LIFE  0x0E000180u
#define SRAM_DECO  0x0E0001C0u
#define SRAM_ALBUM 0x0E000200u
#define SRAM_DEX   0x0E000280u
#define DEX_MAGIC  0x32584544u
static void save_all(void);
// v8.1: the collection's bits (kept forever, across every critter)
static int found_has(int it){ return it<32? (int)((G.more.found0>>it)&1u) : (int)((G.more.found1>>(it-32))&1u); }
static void found_set(int it){ if(found_has(it)) return; if(it<32) G.more.found0|=1u<<it; else G.more.found1|=1u<<(it-32); }
static int found_count(void){ int n=0; for(int i=0;i<IT_COUNT;i++) n+=found_has(i); return n; }
static int owned(int it);
static void found_init(void){   // a save from before the collection: whatever is owned right now counts as found
  for(int i=0;i<IT_COUNT;i++) if(i<N_CONSUMABLE? G.ext.inv[i]!=0 : owned(i)) found_set(i);
  G.more.found_ok=1; save_all(); }
static void ext_save(void){ save_all(); }
static void ext_load(void){
  if(blk_load(&G.ext,sizeof(Ext),EXT_MAGIC,SRAM_EXT)) return;
  volatile u8 *s=(volatile u8*)SRAM_EXT;
  // try the first update's layout: magic, coins, day, inv[14], toys, yard_coins, last_game, pad, (pad x2), sum @28
  u8 o[32]; for(int i=0;i<32;i++) o[i]=s[i];
  u32 m1=o[0]|(o[1]<<8)|(o[2]<<16)|((u32)o[3]<<24), s1=o[28]|(o[29]<<8)|(o[30]<<16)|((u32)o[31]<<24), h=0x5EED;
  for(int i=0;i<28;i++) h=h*31+o[i];
  memset(&G.ext,0,sizeof(Ext));
  for(int i=0;i<3;i++) G.ext.slot[i]=0xFF;
  G.ext.active_toy=0xFF; G.ext.last_game=0xFF; G.ext.prev_stage=PET[P_STAGE]; G.ext.plant_day=0;
  if(m1==EXT1_MAGIC && s1==h){
    int c=(o[4]|(o[5]<<8))*3; G.ext.coins=(u16)(c>9999?9999:c);       // prices went up: carry over buying power
    G.ext.day=(u16)(o[6]|(o[7]<<8));
    for(int i=0;i<N_CONSUMABLE;i++) G.ext.inv[i]=o[8+i];
    G.ext.toys=(u8)(o[22]&7); G.ext.yard_coins=o[23];
    for(int i=0;i<3;i++) if(G.ext.toys&(1<<i)){ G.ext.active_toy=(u8)(IT_BALL+i); break; }
  } else G.ext.coins=20;
  G.ext.adult_day=G.ext.day;
  ext_save();
}
#define ALBUM_MAGIC 0x31414B43u /* "CKA1" */
static void dex_save(void){ save_all(); }
static void album_save(void){ save_all(); }
static void album_load(void){
  if(blk_load(&G.album,sizeof(Album),ALBUM_MAGIC,SRAM_ALBUM) && G.album.n<=ALBUM_N) return;
  memset(&G.album,0,sizeof(Album)); album_save();
}
static void album_add(int name,int form){
  if(G.album.n>=ALBUM_N){ for(int i=1;i<ALBUM_N;i++){ G.album.name[i-1]=G.album.name[i]; G.album.form[i-1]=G.album.form[i];
      for(int k=0;k<NAME_LEN;k++) G.names.alb[i-1][k]=G.names.alb[i][k]; } G.album.n=ALBUM_N-1; }
  G.album.name[G.album.n]=(u8)name; G.album.form[G.album.n]=(u8)form;
  for(int k=0;k<NAME_LEN;k++) G.names.alb[G.album.n][k]=G.names.cur[k];
  G.album.n++; album_save();
}
#define LIFE_MAGIC 0x314C4B43u /* "CKL1" */
#define LIFESPAN 8            /* in-game days as a grown-up before the send-off (it leaves at noon on day 8; v8.2b: was 12) */
#define LIFESPAN_NOON 180     /* clock step for 12:00 */
static void life_save(void){ save_all(); }
static void dex_load(void){
  if(blk_load(&G.dex,sizeof(Dex),DEX_MAGIC,SRAM_DEX) && G.dex.phase<=PH_ADULT) return;
  memset(&G.dex,0,sizeof(Dex)); G.dex.gender=(u8)rnd(2);        // an older save: what it has already shown
  { u8 st=PET[P_STAGE]; G.dex.phase = (*(volatile u32*)PET==0x31504B43u && st>=3 && st<=7)? PH_OLD : PH_GROW; }  // grown-ups keep the old rules
  G.dex.seen=1u;
  if(*(volatile u32*)PET==0x31504B43u){ u8 st=PET[P_STAGE]; if(st>=1 && st<=7) G.dex.seen|=2u; if(st>=2 && st<=7) G.dex.seen|=4u; }
  for(int i=0;i<G.album.n && i<ALBUM_N;i++) if(G.album.form[i]<N_FORMS) G.dex.seen|=1u<<G.album.form[i];
  if(*(volatile u32*)PET==0x31504B43u && PET[P_STAGE]<=7){ int f=form_index(); G.dex.seen|=1u<<f; if(f) G.dex.boy|=G.dex.gender? 0 : 1u<<f, G.dex.girl|=G.dex.gender? 1u<<f : 0; }
  dex_save();
}
static void life_load(void){
  if(blk_load(&G.life,sizeof(Life),LIFE_MAGIC,SRAM_LIFE)) return;
  memset(&G.life,0,sizeof(Life)); G.life.hatch_day=G.ext.adult_day; life_save();
}
#define DECO_MAGIC 0x32444B43u /* "CKD2" */
#define DECO1_MAGIC 0x31444B43u /* "CKD1" (v4 layout: magic, owned, slot[6], plant_day, garden_day, sum) */
static void deco_save(void){ save_all(); }
static void deco_load(void){
  if(blk_load(&G.deco,sizeof(Deco),DECO_MAGIC,SRAM_DECO)) return;
  volatile u8 *s=(volatile u8*)SRAM_DECO;
  u8 o[20]; for(int i=0;i<20;i++) o[i]=s[i];
  u32 m1=o[0]|(o[1]<<8)|(o[2]<<16)|((u32)o[3]<<24), s1=o[16]|(o[17]<<8)|(o[18]<<16)|((u32)o[19]<<24), h=0x5EED;
  for(int i=0;i<16;i++) h=h*31+o[i];
  memset(&G.deco,0,sizeof(Deco));
  if(m1==DECO1_MAGIC && s1==h){
    G.deco.owned=o[4]|(o[5]<<8)|(o[6]<<16)|((u32)o[7]<<24);
    for(int i=0;i<N_SLOTS;i++) G.deco.slot[i]=o[8+i];
    G.deco.plant_day=o[14]; G.deco.garden_day=o[15];
  } else {
    G.deco.owned=G.ext.decor; for(int i=0;i<N_SLOTS;i++) G.deco.slot[i]= i<3? G.ext.slot[i] : 0xFF;
    G.deco.plant_day=G.ext.plant_day; G.deco.garden_day=0xFF;
  }
  G.deco.pond_day=0xFF; G.deco.unlock_lvl=0xFF;     // 0xFF: set from the critter's current age without a message
  G.deco.seed=0;                                    // picked on the first live frame
  deco_save();
}
static void pet_save(void){ u32 h=0x5EED; for(int i=0;i<0x48;i++) h=h*31+PET[i]; *(volatile u32*)(PET+0x48)=h; O_SAVE(PET,0x4C); ext_save(); }
static void add_coins(int n){ if(n>0) G.jr.coins+=(u32)n; int c=G.ext.coins+n; if(c>9999) c=9999; G.ext.coins=(u16)c; ext_save(); }

// ------------------------------------------------------------------ v8 save: one versioned block, kept in two copies
// Everything the extension keeps (Ext, Life, Deco, Album, Dex) is written as one block: a header (magic, version,
// size, sequence number), the parts, and a checksum over all of it. Saves alternate between two copies, so a save cut
// short by power loss still leaves the previous one. A v7 save (five separate blocks) is read once and converted.
#define SRAM_SAVE_A  0x0E000400u
#define SRAM_SAVE_B  0x0E000600u
#define SAVE_MAGIC   0x38534B43u /* "CKS8" */
#define SRAM_SAVE2_A 0x0E001000u   /* v12: the journal no longer fits in 512 bytes, so the save moved here */
#define SRAM_SAVE2_B 0x0E002000u
#define SAVE_VERSION 15
#define SAVE_PARTS   11
#define SAVE_HDR     16
static u8 *save_part(int i,u32 *n){
  switch(i){ case 0: *n=sizeof(Ext);   return (u8*)&G.ext;
             case 1: *n=sizeof(Life);  return (u8*)&G.life;
             case 2: *n=sizeof(Deco);  return (u8*)&G.deco;
             case 3: *n=sizeof(Album); return (u8*)&G.album;
             case 4: *n=sizeof(Dex);   return (u8*)&G.dex;
             case 5: *n=sizeof(Care);  return (u8*)&G.care;
             case 6: *n=sizeof(Keep);  return (u8*)&G.keep;
             case 7: *n=sizeof(Names); return (u8*)&G.names;
             case 8: *n=sizeof(Journal); return (u8*)&G.jr;
             case 9: *n=sizeof(More); return (u8*)&G.more;
             default: *n=sizeof(Garden); return (u8*)&G.gdn; }
}
static u32 save_size_v(int ver){ u32 t=SAVE_HDR+4, n; int parts= ver>=15? 11 : ver>=14? 10 : ver>=12? 9 : ver>=11? 8 : ver>=10? 7 : ver>=9? 6 : 5; for(int i=0;i<parts;i++){ save_part(i,&n); t+=n; } return t; }
static u32 save_size(void){ return save_size_v(SAVE_VERSION); }
static void save_all(void){
  u32 seq=G.save_seq+1, size=save_size(), h=0x5EED, o=0, n;
  volatile u8 *d=(volatile u8*)((seq&1)? SRAM_SAVE2_B : SRAM_SAVE2_A);
  u8 hdr[SAVE_HDR]={0};
  for(int i=0;i<4;i++){ hdr[i]=(u8)(SAVE_MAGIC>>(8*i)); hdr[8+i]=(u8)(seq>>(8*i)); }
  hdr[4]=SAVE_VERSION; hdr[5]=0; hdr[6]=(u8)size; hdr[7]=(u8)(size>>8);
  for(int i=0;i<SAVE_HDR;i++){ d[o++]=hdr[i]; h=h*31+hdr[i]; }
  for(int k=0;k<SAVE_PARTS;k++){ const u8 *p=save_part(k,&n); for(u32 i=0;i<n;i++){ d[o++]=p[i]; h=h*31+p[i]; } }
  for(int i=0;i<4;i++) d[o++]=(u8)(h>>(8*i));
  G.save_seq=seq;
}
static u32 rd32s(volatile u8 *s){ return s[0]|(s[1]<<8)|(s[2]<<16)|((u32)s[3]<<24); }
static int save_valid(volatile u8 *s,u32 *seq){   // returns the block's version (8, 9 or 10), 0 if it is not a good save
  int ver=s[4]|(s[5]<<8); if(ver<8 || ver>15) return 0;
  u32 size=save_size_v(ver);
  if(rd32s(s)!=SAVE_MAGIC || (u32)(s[6]|(s[7]<<8))!=size) return 0;
  u32 h=0x5EED; for(u32 i=0;i<size-4;i++) h=h*31+s[i];
  if(rd32s(s+size-4)!=h) return 0;
  *seq=rd32s(s+8); return ver;
}
static int load_pair(u32 A,u32 B){
  u32 sa=0, sb=0; int va=save_valid((volatile u8*)A,&sa), vb=save_valid((volatile u8*)B,&sb);
  if(!va && !vb) return 0;
  int useb = vb && (!va || (s32)(sb-sa)>0);
  volatile u8 *s=(volatile u8*)(useb? B : A); u32 o=SAVE_HDR, n;
  int ver= useb? vb : va, parts= ver>=15? 11 : ver>=14? 10 : ver>=12? 9 : ver>=11? 8 : ver>=10? 7 : ver>=9? 6 : 5;
  for(int k=0;k<parts;k++){ u8 *p=save_part(k,&n); for(u32 i=0;i<n;i++) p[i]=s[o++]; }
  G.save_seq= useb? sb : sa;
  return ver;
}
static int load_all(void){   // the v12 copies first; an older save is still read from where it was (0x400/0x600)
  int v=load_pair(SRAM_SAVE2_A,SRAM_SAVE2_B); return v? v : load_pair(SRAM_SAVE_A,SRAM_SAVE_B); }

// ------------------------------------------------------------------ tables for forms
static void refresh_tables(void){
  for(int i=0;i<8;i++) RAM_SPR[i]=ROM_SPR_TABLE[i];
  for(int i=0;i<10;i++) RAM_NAMES[i]=ROM_NAME_TABLE[i];
  if(G.names.cur[0] && PET[P_NAME]<10) RAM_NAMES[PET[P_NAME]]=G.names.cur;   // the name the player gave it
  else if((PET[P_STAGE]==0 || G.want_name) && PET[P_NAME]<10) RAM_NAMES[PET[P_NAME]]="";   // v8.1: blank until it is named
  for(int i=0;i<8;i++) RAM_NAMES[10+i]=FORMS[i].name;
  u8 st=PET[P_STAGE];
  // v7: the original's five grown-up slots all show whatever this critter is (teen, grown-up, secret), and while it is
  // a Sproutle they already show the teen it is heading for, so the original's "grew up" message names it right
  int f=-1;
  if(st>=3 && st<=7 && !G.ext.special && G.dex.phase==PH_TEEN && G.dex.adult){   // growing up right now
    for(int i=3;i<8;i++){ RAM_SPR[i]=form_spr(G.dex.adult); RAM_NAMES[10+i]=form_name(G.dex.adult); }
    RAM_SPR[st]=form_spr(form_index()); RAM_NAMES[10+st]=form_name(form_index()); return; }
  if(st>=3 && st<=7 && (G.ext.special || G.dex.phase>=PH_TEEN)) f=form_index();
  else if(st==2 && G.dex.phase==PH_GROW) f= care_side()==2? F_THORNLE : F_BLOOMLE;
  if(f>=0){ for(int i=3;i<8;i++){ RAM_SPR[i]=form_spr(f); RAM_NAMES[10+i]=form_name(f); } return; }
  if(G.ext.special && st>=3 && st<=7){ RAM_SPR[st]=form_spr(form_index()); RAM_NAMES[10+st]=form_name(form_index()); }
}

// ------------------------------------------------------------------ sky
// time of day keyframes over the 600-step clock: dawn, day, day, dusk, night, night, (dawn)
typedef struct { u16 t; u16 c[8]; } SkyKey;   // sky0..3, cloud, cloud2, star, unused
static const SkyKey SKY_KEYS[7] = {
  {  0, {RGB(12,13,24),RGB(20,16,24),RGB(28,18,18),RGB(31,22,13),RGB(31,24,22),RGB(24,17,20),RGB(18,16,24),0}},
  { 40, {RGB(13,21,31),RGB(16,24,31),RGB(19,27,31),RGB(24,30,31),RGB(31,31,31),RGB(23,26,30),RGB(16,24,31),0}},
  {400, {RGB(13,21,31),RGB(16,24,31),RGB(19,27,31),RGB(24,30,31),RGB(31,31,31),RGB(23,26,30),RGB(16,24,31),0}},
  {455, {RGB(10,9,22), RGB(20,12,22),RGB(29,15,14),RGB(31,21,9), RGB(31,20,17),RGB(22,12,17),RGB(14,11,24),0}},
  {490, {RGB(2,3,10),  RGB(3,4,13),  RGB(4,6,16),  RGB(6,8,19),  RGB(8,9,16),  RGB(5,6,12),  RGB(31,31,26),0}},
  {585, {RGB(2,3,10),  RGB(3,4,13),  RGB(4,6,16),  RGB(6,8,19),  RGB(8,9,16),  RGB(5,6,12),  RGB(31,31,26),0}},
  {600, {RGB(12,13,24),RGB(20,16,24),RGB(28,18,18),RGB(31,22,13),RGB(31,24,22),RGB(24,17,20),RGB(18,16,24),0}},
};
static u16 lerp_col(u16 a,u16 b,int f){ // f 0..256
  int r=(a&31)+((((b&31)-(a&31))*f)>>8), g=((a>>5)&31)+(((((b>>5)&31)-((a>>5)&31))*f)>>8), bl=((a>>10)&31)+(((((b>>10)&31)-((a>>10)&31))*f)>>8);
  return (u16)RGB(r,g,bl);
}
static void sky_palette(void){
  u32 t=PET_CLOCK; if(t>599) t=599;
  int k=0; while(k<5 && t>=SKY_KEYS[k+1].t) k++;
  int span=SKY_KEYS[k+1].t-SKY_KEYS[k].t, f= span? (int)(((t-SKY_KEYS[k].t)<<8)/(u32)span) : 0;
  for(int i=0;i<7;i++) PAL[C_SKY0+i]=lerp_col(SKY_KEYS[k].c[i],SKY_KEYS[k+1].c[i],f);
  PAL[C_SUN]=RGB(31,27,8); PAL[C_SUN2]=RGB(31,20,6); PAL[C_MOON]=RGB(30,29,22);
  if(t<30){ PAL[C_SUN]=RGB(31,20,8); PAL[C_SUN2]=RGB(30,13,6); }
  if(t>440 && t<480){ PAL[C_SUN]=RGB(31,18,8); PAL[C_SUN2]=RGB(29,11,6); }
}
// ---- v7 phase 3: pre-drawn decor art (gen/decor.py), one 12-colour palette band per decor slot
static int decor_entry(int it){
  if(it==IT_PLANT){ int g=(u8)((u8)G.ext.day - G.deco.plant_day); return g>=2? 31 : g==1? 30 : 2; }
  return it-IT_BED;
}
// v8.2b: the pinwheel, the sprinkler and the resting frisbee stand here, in the free strip between the first two
// moonflower patches (x 80 and 132), clear of every decor piece (by the tree: up to x 88; by the house: from x 168)
#define TOY_X 108
static int toy_of(int it){ return it==IT_BALL? TOY_BALL : it==IT_KITE? TOY_KITE : it==IT_FRISBEE? TOY_FRISBEE : it==IT_SPRINKLER? TOY_SPRINKLER : it==IT_PINWHEEL? TOY_PINWHEEL : -1; }
static void decor_palettes(int night){
  int yard=G.bgx_yard, s0= yard? 4 : 0, s1= yard? 6 : 4;
  for(int s=s0;s<s1;s++){ int it=G.deco.slot[s]; if(it==0xFF) continue;
    const u16 *p=DEC_PAL[decor_entry(it)][night]; volatile u16 *d=PAL+DEC_BAND0+DEC_BANDN*(s-s0);
    for(int i=0;i<DEC_BANDN;i++) d[i]=p[i]; }
  if(yard && G.ext.active_toy!=0xFF){ int t=toy_of(G.ext.active_toy);
    if(t>=0){ const u16 *p=TOY_PAL[t][night]; volatile u16 *d=PAL+DEC_BAND0+2*DEC_BANDN; for(int i=0;i<DEC_BANDN;i++) d[i]=p[i]; } }
}
static void blit_box(int x0,int y0,int w,int h,const u8 *pix,const u8 *rows){
  for(int r=0;r<h;r++, pix+=w){
    int a=rows[2*r], z=rows[2*r+1], y=y0+r; if(a==255 || (unsigned)y>=160u) continue;
    for(int x=a;x<=z;x++){ int c=pix[x]; if(c && (unsigned)(x0+x)<240u) pset(x0+x, y, c); }
  }
}
static void icon_draw(int it,int cx,int cy){ const Icon *I=&ICONS[it]; blit_box(cx-I->w/2, cy-I->h/2, I->w, I->h, ICON_PIX+I->pix, ICON_ROWS+2*I->row); }
// phase 4: code art (minis.h) with a small colour table, two pixels at a time
static void blit_codes(const u8 *g,int w,int h,int x,int y,const u8 *col,int flip){
  for(int r=0;r<h;r++, g+=w){
    int yy=y+r; if((unsigned)yy>=160u) continue;
    volatile u16 *row=DRAW_TARGET+yy*120;
    int X0= x<0? 0 : x, X1= x+w-1>239? 239 : x+w-1;
    for(int X=X0&~1; X<=X1; X+=2){
      int v0=0, v1=0;
      if(X>=X0){ int i=X-x; v0=col[g[flip? w-1-i : i]]; }
      if(X+1<=X1){ int i=X+1-x; v1=col[g[flip? w-1-i : i]]; }
      if(!v0 && !v1) continue;
      volatile u16 *p=row+(X>>1);
      if(v0&&v1) *p=(u16)(v0|(v1<<8)); else if(v0) *p=(u16)((*p&0xFF00)|v0); else *p=(u16)((*p&0x00FF)|(v1<<8));
    }
  }
}
static const u8 COL_STAR[8]={0,C_GOLD,C_LTYEL,C_ORANGE,C_GREEN,C_OUTLINE,C_WHITE,C_ORANGE};
static const u8 COL_BERRY[8]={0,C_RED,C_PINK,C_RED2,C_GREEN,C_OUTLINE,C_WHITE,C_RED2};
static const u8 COL_ROCK[8]={0,C_GRAY,C_LTEXT,C_HINT,C_GREEN,C_OUTLINE,C_WHITE,C_SLATE};
static const u8 COL_ARROW[8]={0,C_WHITE,C_WHITE,C_LTEXT,C_GREEN,C_OUTLINE,C_WHITE,18};
static const u8 COL_ARROWR[8]={0,C_RED,C_PINK,C_RED2,C_GREEN,C_OUTLINE,C_WHITE,C_RED2};
static const u8 COL_DIM[8]={0,C_GRAY,C_LTEXT,C_HINT,C_GREEN,C_OUTLINE,C_LTEXT,C_HINT};
static void mini_art(int a,int x,int y,const u8 *col,int flip){ const MiniArt *m=&MINI_ART[a]; blit_codes(MINI_PIX+m->off,m->w,m->h,x,y,col,flip); }
static void decor_art(int it){
  int e=decor_entry(it); const u8 *b=DEC_BOX[e];
  blit_box(b[0],b[1],b[2],b[3],DEC_PIX+DEC_PIXOFF[e],DEC_ROWS+2*DEC_ROWOFF[e]);
}
static void toy_draw(int t,int k,int ax,int ay){
  const ToyFrame *f=&TOY_FR[TOY_FIRST[t]+k];
  blit_box(ax+f->dx, ay+f->dy, f->w, f->h, DEC_PIX+f->pix, DEC_ROWS+2*f->row);
}
static int title_showing(void){ return SCENE==0 && (G.mode==M_NONE || G.mode==M_SURE); }
static int so_unified(void); static void sky_key(int k);
static int ra_shot(void); static void runaway_palette(int s);
static void farewell_palette(void){   // the farewell's yard and the runaway's rainy night, set before anything is drawn
  int run= G.mode==M_NONE && SCENE==9, rise= G.mode==M_SENDOFF && so_unified() && G.sendoff_t<560;
  if(run){ int s=ra_shot(); if(s>=0){ runaway_palette(s); return; } }   // v8.2c: the new runaway's own colours
  if(!run && !rise) return;
  int night= run || G.sendoff==2;   /* E_SAD */
  { const u16 *q= night? BGX_YARD_NIGHTPAL : BGX_YARD_DAYPAL; for(int i=0;i<BGX_N;i++) PAL[BGX_FIRST+i]=q[i]; }
  if(night){ PAL[C_SKY0]=RGB(3,4,7); PAL[C_SKY1]=RGB(4,5,9); PAL[C_SKY2]=RGB(5,6,10); PAL[C_SKY3]=RGB(6,7,11); PAL[C_CLOUD]=RGB(8,9,12); PAL[C_CLOUD2]=RGB(6,7,10); }
  else sky_key(3);
}
static void story_palette(void);
static void garden_palette(void);
static int garden_palette_on(void);
static void set_extra_palette(void){
  PAL[C_DKGREEN]=RGB(6,17,6); PAL[C_DEEPGREEN]=RGB(4,13,8); PAL[C_NGRASS]=RGB(4,10,10); PAL[C_NGRASS2]=RGB(2,7,8);
  PAL[C_LTGREEN]=RGB(15,26,10); PAL[C_NWOOD]=RGB(10,7,6); PAL[C_NWALL]=RGB(12,11,17); PAL[C_TERRA]=RGB(25,11,6); PAL[C_GLASS]=RGB(22,29,31);
  if(!G.outside){ const u16 *pp = PAL[C_WALL]!=RGB(30,27,22)? PIC_PAL_NIGHT : PIC_PAL_DAY; for(int i=0;i<24;i++) PAL[C_PIC+i]=pp[i]; }   // (outside: the food's)   // 120-143 now belong to the critter
  { int night = PAL[C_WALL]!=RGB(30,27,22);
    const u16 *q = G.bgx_yard? (night? BGX_YARD_NIGHTPAL : BGX_YARD_DAYPAL) : (night? BGX_ROOM_NIGHTPAL : BGX_ROOM_DAYPAL);
    if(G.mode==M_SHOP || (G.mode==M_DEX && G.jtab==JT_ITEMS)){ for(int i=0;i<ICON_N;i++) PAL[ICON_FIRST+i]=ICON_PAL[i]; }   // the shop shows no room: its icons borrow the range
    else if(G.mode==M_ALBUM || (G.mode==M_DEX && G.jtab==0)){ for(int i=0;i<ALB_N;i++) PAL[ALB_FIRST+i]=ALB_PAL[i]; }   // the album shows many critters on one shared palette
    else if(title_showing() || (G.mode==M_SENDOFF && G.tbg) || ra_shot()==4){   // painted title / star world: their own colours, twinkling stars
      for(int i=0;i<TBG_TWINKLE;i++) PAL[TBG_FIRST+i]=TBG_PAL[i];
      for(int k=0;k<4;k++) PAL[TBG_FIRST+TBG_TWINKLE+k]=TBG_TW[(k+(G.sky_t>>4))&3]; }
    else { for(int i=0;i<BGX_N;i++) PAL[BGX_FIRST+i]=q[i]; decor_palettes(night); if(!garden_palette_on()) fly_palette(); } }
  sky_palette();
  farewell_palette();
  if(G.mode==M_STORY) story_palette();
  else if(garden_palette_on()) garden_palette();
}
static int sky_band(int sy){ return sy<44? C_SKY0 : sy<58? C_SKY1 : sy<72? C_SKY2 : C_SKY3; }
// clip rectangle + offset from sky coordinates to screen
#define sk_x0 G.sk[0]
#define sk_y0 G.sk[1]
#define sk_x1 G.sk[2]
#define sk_y1 G.sk[3]
#define sk_dx G.sk[4]
#define sk_dy G.sk[5]
static void sky_pset(int sx,int sy,int c){
  int x=sx+sk_dx, y=sy+sk_dy;
  if(x<sk_x0||x>sk_x1||y<sk_y0||y>sk_y1) return;
  volatile u16 *p=DRAW_TARGET+((y*240+x)>>1); u16 v=*p; int old=(x&1)? (v>>8) : (v&0xFF);
  if(old<C_SKY0 || old>C_STAR) return;
  if(x&1) *p=(u16)((v&0x00FF)|(c<<8)); else *p=(u16)((v&0xFF00)|c);
}
// paint a horizontal run (sky coords) only over sky pixels
static void sky_span(int sy,int sx0,int sx1,int c){
  int y=sy+sk_dy; if(y<sk_y0||y>sk_y1) return;
  int x0=sx0+sk_dx, x1=sx1+sk_dx; if(x0<sk_x0) x0=sk_x0; if(x1>sk_x1) x1=sk_x1;
  if(x0>x1) return;
  volatile u16 *p=DRAW_TARGET+y*120+(x0>>1), *e=DRAW_TARGET+y*120+(x1>>1);
  u32 lo_ok = !(x0&1), hi_last = (x1&1);
  for(; p<=e; p++){
    u16 v=*p; u32 lo=v&0xFF, hi=v>>8; u32 nv=v;
    if(lo_ok && lo-C_SKY0<=(u32)(C_STAR-C_SKY0)) nv=(nv&0xFF00)|(u32)c;
    if((p!=e || hi_last) && hi-C_SKY0<=(u32)(C_STAR-C_SKY0)) nv=(nv&0x00FF)|((u32)c<<8);
    if(nv!=v) *p=(u16)nv;
    lo_ok=1;
  }
}
static int sky_visible(int sx0,int sy0,int sx1,int sy1){ return sx1+sk_dx>=sk_x0 && sx0+sk_dx<=sk_x1 && sy1+sk_dy>=sk_y0 && sy0+sk_dy<=sk_y1; }
static const u8 HW9[19]={3,5,6,7,8,8,9,9,9,9,9,9,9,8,8,7,6,5,3};
static const u8 HW7[15]={2,4,5,6,6,7,7,7,7,7,6,6,5,4,2};
static const s8 CLOUD_ROWS[16][3]={{-10,6,10},{-9,4,12},{-8,3,13},{-7,2,14},{-6,2,14},{-5,-2,19},{-4,-3,20},{-3,-4,21},{-2,-5,22},{-1,-5,22},{0,-5,22},{1,-5,22},{2,-5,22},{3,-4,21},{4,-3,20},{5,-2,19}};
static const u8 STARS[][2]={{12,34},{40,48},{70,34},{96,58},{130,40},{150,62},{176,32},{206,52},{58,66},{116,70},{188,70},{20,60},{84,44},{64,32},{100,36},{226,38},{160,46},{34,40},{140,56},{76,52}};
static void draw_cloud(int x,int y){
  if(!sky_visible(x-5,y-10,x+22,y+5)) return;
  for(int i=0;i<16;i++){ int dy=CLOUD_ROWS[i][0]; sky_span(y+dy,x+CLOUD_ROWS[i][1],x+CLOUD_ROWS[i][2], dy>=3? C_CLOUD2 : C_CLOUD); }
}
static void draw_sun(int x,int y){
  if(!sky_visible(x-9,y-9,x+9,y+9)) return;
  for(int dy=-9;dy<=9;dy++){ int w=HW9[dy+9]; if(dy>=-7 && dy<=7){ int w2=HW7[dy+7]; sky_span(y+dy,x-w,x-w2-1,C_SUN2); sky_span(y+dy,x-w2,x+w2,C_SUN); sky_span(y+dy,x+w2+1,x+w,C_SUN2); } else sky_span(y+dy,x-w,x+w,C_SUN2); }
}
// v8: every critter that rose to the sky is a star you can see from the window at night: gold for good care, amber
// for less care, moon-white for secret forms (the album's critters; the newest one twinkles), then plain stars for older ones
static const u8 CRIT_STAR[32]={78,67,71,56,101,63,103,37,88,42,77,42,96,57,63,37,73,35,71,64,89,65,65,45,63,59,94,34,96,45,104,57};
static void crit_stars(void){   // v8 phase 6: from the journal - every critter that rose (a sad goodbye is a faint dot)
  int n=G.jr.n, first= n>16? n-16 : 0, k=0;
  for(int i=first;i<n && k<16;i++,k++){
    if(i==n-1 && ((G.sky_t>>3)&7)==0) continue;                                // the newest twinkles
    const Story *st=&G.jr.s[i]; int f=st->form, c= f<N_FORMS? (FORMS[f].stage==ST_SECRET? C_MOON : FORMS[f].side==2? C_SUN2 : C_SUN) : C_SUN;   // sky colours stay bright at night
    int x=CRIT_STAR[k*2], y=CRIT_STAR[k*2+1];
    if(((st->info>>2)&3)==2){ sky_pset(x,y,C_STAR); continue; }
    sky_pset(x,y,C_STAR); sky_pset(x-1,y,c); sky_pset(x+1,y,c); sky_pset(x,y-1,c); sky_pset(x,y+1,c);
  }
}
// v8.1: a wishing star now and then on a clear night, sliding down across the window (and the yard's sky).
// Press A while it is crossing to make a wish: a few coins, counted in the journal.
#define WISH_LEN 96
#define WISH_COINS 5
#define WISH_WAIT 180   // v8.1: frames after it has crossed that A still makes a wish (about 3 s)
static int wish_sky(void){ u32 t=PET_CLOCK; return t>=480 && t<594; }
static void wish_draw(void){
  int t=G.wish_t; if(!t || t>WISH_LEN) return;
  int hx=G.wish_x-t/2, hy=G.wish_y+t/3;
  for(int j=20;j>=1;j--){ int x=hx+j, y=hy-(j*2+1)/3, c= j<5? C_STAR : j<12? C_MOON : C_SUN; sky_pset(x,y,c); if(j<10) sky_pset(x,y+1,j<4? C_MOON : C_SUN); }
  sky_pset(hx,hy,C_STAR); sky_pset(hx-1,hy,C_STAR); sky_pset(hx,hy+1,C_STAR); sky_pset(hx,hy-1,C_MOON); sky_pset(hx+1,hy,C_MOON);
  if((t>>2)&1){ sky_pset(hx-2,hy,C_MOON); sky_pset(hx,hy+2,C_MOON); sky_pset(hx,hy-2,C_MOON); }   // it sparkles
  if(G.wish_got && (t&4)){ sky_pset(hx-3,hy-3,C_STAR); sky_pset(hx+3,hy-4,C_STAR); sky_pset(hx-2,hy+4,C_STAR); }
}
static void live_update(u32 k);
static void garden_msg(const char *m);
static int wish_open(void){ return G.wish_t && !G.wish_got && G.wish_t<=WISH_LEN+WISH_WAIT; }   // A would make a wish now
static void wish_tick(void){
  if(G.wish_t){ if(++G.wish_t>WISH_LEN+WISH_WAIT+2){ G.wish_t=0; if(G.mode==M_GARDEN) G.gui_key=0; } return; }
  // v8.1: about one night in three has a shooting star, just the one. wish_cd: 0 = tonight not decided yet,
  // 0xFFFF = none (or no more) tonight, else the clock step it falls at
  if(!wish_sky()){ G.wish_cd=0; return; }
  if(!G.wish_cd) G.wish_cd= rnd(3)? 0xFFFF : (u16)(486+rnd(96));
  if(SCENE!=1 || (G.mode!=M_NONE && G.mode!=M_GARDEN)) return;   // (it waits while a firefly round is on)
  if(G.wish_cd==0xFFFF || PET_CLOCK<G.wish_cd) return;
  G.wish_cd=0xFFFF; G.wish_t=1; G.wish_got=0; G.wish_x=(u8)(100+rnd(16)); G.wish_y=(u8)(20+rnd(9));
  sfx(1,SEQ_SHOOT);
  if(G.mode==M_GARDEN){ G.gmsg_t=0; G.gui_key=0; }   // the yard says so in its bottom bar, keeping the sky clear
  else { set_msg("A SHOOTING STAR! A: MAKE A WISH"); MSG_TIMER=WISH_LEN+WISH_WAIT; }
}
static void secret_wish(void); static void sun_morning(void); static void secret_lantern(void); static void secret_counts_reset(void);
static int wish_try(u16 pr){   // A while it crosses (or just after): a wish
  if(SCENE!=1 || !G.wish_t || G.wish_got || G.wish_t>WISH_LEN+WISH_WAIT || !(pr&K_A)) return 0;
  G.wish_got=1; secret_wish(); if(G.more.wishes<65535) G.more.wishes++; G.more.spare[0]=(u16)(G.ext.day+1);   // (spare[0]: the night of the last wish, for the moonflowers)
  add_coins(WISH_COINS); sfx(0,SEQ_WISH);
  if(G.mode==M_GARDEN) garden_msg("YOU MADE A WISH! +5 COINS"); else set_msg("YOU MADE A WISH! +5 COINS");
  return 1;
}
// window: x 22..69, y 40..79 shows sky x 60..107, y 30..69.   yard: whole sky area.
static void draw_sky(int window){
  if(window){ sk_x0=22; sk_x1=69; sk_y0=40; sk_y1=79; sk_dx=-38; sk_dy=10; }
  else { sk_x0=0; sk_x1=239; sk_y0=28; sk_y1=95; sk_dx=0; sk_dy=0; }
  u32 t=PET_CLOCK; if(t>599) t=599;
  // stars
  if(t>=470 || t<20){ for(unsigned i=0;i<sizeof STARS/2;i++){ if(((G.sky_t>>5)+i*7)%11==0) continue; sky_pset(STARS[i][0],STARS[i][1],C_STAR); }
    crit_stars(); }
  wish_draw();
  if(window) fly_dance();
  // sun or moon on an arc
  if(t<482){ int u=(int)t*512/480-256; int x=-12+(int)t*264/480, y=34+((58*u*u)>>16);
    draw_sun(x,y); }
  if(t>=478){ int tt=(int)t-478; int u=tt*512/122-256; int x=-12+tt*264/122, y=36+((52*u*u)>>16);
    // v8.1: the moon's phase changes over eight nights (0 full, 4 new); the shadowed part is left to the sky
    static const s8 OFF[8]={99,11,8,4,0,-4,-8,-11};
    int ph=G.ext.day&7, off=OFF[ph];
    if(sky_visible(x-7,y-7,x+7,y+7)) for(int dy=-7;dy<=7;dy++){ int w=HW7[dy+7]; for(int dx=-w;dx<=w;dx++){
      int ex=dx-off; if(ph && (ph==4 || ex*ex+dy*dy<=46)) continue; sky_pset(x+dx,y+dy,C_MOON); } } }
  // drifting clouds
  static const s16 CX[4]={10,95,170,250}, CY[4]={42,56,36,50}, CS[4]={22,15,28,19};
  for(int i=0;i<4;i++){ int x=(int)(((u32)CX[i]*256 + G.sky_t*CS[i])>>8); x%=320; draw_cloud(x-40,CY[i]); }
}
// replace the static window picture in both room backgrounds with animatable sky bands
static void apply_window_panes(void){
  u16 *save=DRAW_TARGET;
  for(int b=0;b<2;b++){
    DRAW_TARGET=(u16*)(b? BG_ROOM_NIGHT : BG_ROOM_DAY);
    for(int y=40;y<=79;y++){ if(y==59||y==60) continue; int c=sky_band(y-10); fill(22,y,23,1,c); fill(47,y,23,1,c); }
  }
  DRAW_TARGET=save;
}
static void check_window_panes(void){
  u16 *save=DRAW_TARGET; DRAW_TARGET=(u16*)BG_ROOM_DAY; int v=pget(30,50); DRAW_TARGET=save;
  if(v<C_SKY0 || v>C_STAR){ apply_window_panes(); G.bg_state=0; }
}

// ------------------------------------------------------------------ init
static int has_item(int it,int s0,int s1);
static void care_from_v8(void){   // the old records (v7/v8) become the new one
  memset(&G.care,0,sizeof(Care));
  CareWin *g=&G.care.grow, *a=&G.care.adult;
  g->good=G.dex.c_good; g->bad=G.dex.c_bad; g->n=(u16)(G.dex.c_good+G.dex.c_bad);
  g->miss[K_PLAY]=G.dex.c_sad; g->miss[K_CLEAN]=G.dex.c_mess;
  g->yard=(u16)(G.dex.yard*4); g->play=(u16)(G.dex.games*4);
  if(has_item(IT_PLANT,0,4) && (has_item(IT_FLOWERS,4,6) || has_item(IT_GARDEN,4,6))) g->garden=120;
  a->good=G.life.care_good; a->bad=G.life.care_bad; a->n=(u16)(G.life.care_good+G.life.care_bad);
  for(int k=0;k<N_NEEDS;k++) G.care.miss_day[k]=(u8)G.ext.day;
  G.care.miss_day[K_CLEAN]=G.dex.mess_day;
}

// ================================================================== HARDWARE EFFECTS (v8 phase 6)
// The GBA does these itself, with no drawing: BLDCNT/BLDY brighten or darken the whole picture, MOSAIC pixelates BG2,
// and BG2's reference point nudges the picture for a bump. The original never touches these registers.
#define REG_BG2CNT (*(volatile u16*)0x0400000C)
#define REG_BG2X   (*(volatile u32*)0x04000028)
#define REG_BG2Y   (*(volatile u32*)0x0400002C)
#define REG_MOSAIC (*(volatile u16*)0x0400004C)
#define REG_BLDCNT (*(volatile u16*)0x04000050)
#define REG_BLDY   (*(volatile u16*)0x04000054)
enum { FX_WHITE=2, FX_BLACK=3 };
static void fx_fade(int kind,int from,int frames){   // starts at 'from' (0-16) and eases back to the normal picture
  if(G.fx_lvl && (G.fx_lvl>>8)>from && G.fx_kind==kind) return;
  G.fx_kind=(u8)kind; G.fx_lvl=(u16)(from<<8); G.fx_step=(u16)((from<<8)/(frames>0? frames : 1)); if(!G.fx_step) G.fx_step=1; }
static void fx_mosaic(int len,int max){ G.mos_len=(u8)len; G.mos_t=(u8)len; G.mos_max=(u8)max; }
static void fx_shake(int t){ G.shake_t=(u8)t; }
static void fx_tick(void){
  int lvl=G.fx_lvl>>8;
  REG_BLDCNT = lvl? (u16)(0x0034|(G.fx_kind<<6)) : 0; REG_BLDY=(u16)lvl;     // BG2 and the backdrop
  if(G.fx_hold) G.fx_hold--;
  else if(G.fx_lvl) G.fx_lvl = G.fx_lvl>G.fx_step? G.fx_lvl-G.fx_step : 0;
  if(G.mos_fix){ int m=G.mos_fix>15? 15 : G.mos_fix; REG_BG2CNT|=0x40; REG_MOSAIC=(u16)(m|(m<<4)); }
  else if(G.mos_t){ int t=G.mos_t--, h=G.mos_len/2; if(!h) h=1; int m= t>h? (G.mos_len-t)*G.mos_max/h : t*G.mos_max/h; if(m>15) m=15;
    REG_BG2CNT|=0x40; REG_MOSAIC=(u16)(m|(m<<4)); }
  else { REG_BG2CNT&=(u16)~0x40; REG_MOSAIC=0; }
  if(G.zoom_t){   // the farewell's slow zoom out from its new star (120,58): BG2 scaled by the hardware
    int k=G.zoom_t--, pa=0x100-(k*k*0x60)/(120*120);
    REG16(0x04000020)=(u16)pa; REG16(0x04000026)=(u16)pa; REG_BG2X=(u32)(120*256-120*pa); REG_BG2Y=(u32)(58*256-58*pa); return; }
  REG16(0x04000020)=0x100; REG16(0x04000026)=0x100;
  if(G.shake_t){ int t=G.shake_t--, a= t>8? 3 : t>4? 2 : 1, dx=(t&2)? a : -a, dy=(t&1)? 1 : 0; REG_BG2X=(u32)(dx<<8); REG_BG2Y=(u32)(dy<<8); }
  else { REG_BG2X=0; REG_BG2Y=0; }
}
// watches for the moments that get an effect: the title, scene changes, lights, hatching and evolving
static void fx_watch(void){
  u8 sc=(u8)SCENE, out=G.outside, mode=G.mode, light=PET[P_LIGHTS], st=PET[P_STAGE];
  if(!G.fx_seen){ G.fx_seen=1; G.fx_scene=sc; G.fx_out=out; G.fx_mode=mode; G.fx_light=light; G.fx_stage=st; if(sc==0) fx_fade(FX_BLACK,16,40); fx_tick(); return; }
  if(sc!=G.fx_scene){
    if(sc==0) fx_fade(FX_BLACK,16,36);                          // back to the title
    else if(G.fx_scene==0) fx_fade(FX_BLACK,16,24);             // into the game
  }
  if(out!=G.fx_out) fx_fade(FX_BLACK,12,18);                    // room and yard
  if(mode!=G.fx_mode && (mode==M_MINI || G.fx_mode==M_MINI || mode==M_ALBUM || mode==M_DEX || G.fx_mode==M_ALBUM || G.fx_mode==M_DEX || mode==M_STORY || G.fx_mode==M_STORY))
    fx_fade(FX_BLACK,10,14);
  if(light!=G.fx_light && sc!=0 && st>=1 && st<=7){            // the lights ease off and on instead of snapping
    if(light) fx_fade(FX_WHITE,7,24); else fx_fade(FX_BLACK,9,24); }
  if(st!=G.fx_stage && G.fx_stage<=7){
    if(G.fx_stage==0 && st==1){ fx_fade(FX_WHITE,12,30); fx_mosaic(28,6); fx_shake(14); }   // hatched
    else if(st>=2 && st<=7 && st>G.fx_stage && G.fx_stage>=1){ fx_fade(FX_WHITE,16,45); fx_mosaic(34,8); }   // evolved
  }
  G.fx_scene=sc; G.fx_out=out; G.fx_mode=mode; G.fx_light=light; G.fx_stage=st;
  fx_tick();
}

// ================================================================== HARDWARE SPRITES (v8.2)
// Small glowing effects drawn by the GBA itself over the bitmap: the fireflies flying home and the farewell star.
// The render fills a shadow (spr_*); the next update copies it to OAM, the OBJ palette and the window registers.
// That update runs just after the original's vblank flip, which also rewrites DISPCNT every frame: we set the
// OBJ (and window) bits again each time, so the sprites switch themselves off as soon as nothing asks for them.
// Every sprite is semi-transparent and blended additively (EVA=EVB=16): it only ever adds light, and a sprite
// fades by darkening its own 16-colour palette bank. OBJs are never a brightness target, so the farewell's
// white-out leaves them sharp.
#define REG_DISPCNT (*(volatile u16*)0x04000000)
#define OAM16   ((volatile u16*)0x07000000)
#define OBJ_PAL ((volatile u16*)0x05000200)
#define OBJ_VRAM ((volatile u32*)0x06014000)
// sin in Q8 over 64 steps (a full turn)
static const s16 SIN64[64]={0,25,50,74,98,121,142,162,181,198,213,226,237,245,251,255,256,255,251,245,237,226,213,198,181,162,142,121,98,74,50,25,
  0,-25,-50,-74,-98,-121,-142,-162,-181,-198,-213,-226,-237,-245,-251,-255,-256,-255,-251,-245,-237,-226,-213,-198,-181,-162,-142,-121,-98,-74,-50,-25};
static void bl_tiles_load(void);
static void spr_sync(void){
  if(G.spr_ready){
    G.spr_ready=0;
    if(G.spr_n && !G.spr_live){   // switching on: fresh tiles, and every OAM entry hidden first
      for(int i=0;i<SPR_TILE_WORDS;i++) OBJ_VRAM[i]=SPR_TILES[i];
      for(int i=0;i<128;i++) OAM16[i*4]=0x0200;
      G.spr_shown=0; }
    for(int i=0;i<G.spr_n;i++){ OAM16[i*4]=G.so[i][0]; OAM16[i*4+1]=G.so[i][1]; OAM16[i*4+2]=G.so[i][2]; }
    for(int i=G.spr_n;i<G.spr_shown;i++) OAM16[i*4]=0x0200;
    for(int j=0;j<G.spr_naff;j++) for(int k=0;k<4;k++) OAM16[(j*4+k)*4+3]=(u16)G.sa[j][k];
    for(int b=0;b<G.spr_npal;b++) for(int c=0;c<16;c++) OBJ_PAL[b*16+c]=G.sp[b][c];
    if(G.spr_bl){ if(!G.spr_bl_ok){ bl_tiles_load(); G.spr_bl_ok=1; }
      for(int i=0;i<CR_SLOTS;i++) OBJ_PAL[CR_SLOT_IDX[i]]=CR_PAL[1][i]; }   // the Bliplet's own colours, in the OBJ palette's critter slots
    else G.spr_bl_ok=0;
    if(G.spr_win){ REG16(0x04000040)=(u16)((G.win[0]<<8)|G.win[1]); REG16(0x04000044)=(u16)((G.win[2]<<8)|G.win[3]);
      if(G.spr_win==2){ REG16(0x04000048)=0x0024; REG16(0x0400004A)=0x0034; }   // a menu's box: no OBJ inside it
      else { REG16(0x04000048)=0x0034;    // inside window 0 (the open window): BG2, OBJ, blending
      REG16(0x0400004A)=0x0024; } }  // everywhere else: BG2 and blending, no OBJ
    G.spr_shown=G.spr_n; G.spr_live=G.spr_n? 1 : 0;
  }
  // OBJ, 1D tile mapping and window 0: ours to set and clear (main keeps them when it flips the page)
  REG_DISPCNT=(u16)((REG_DISPCNT&~0x3040) | (G.spr_live? (0x1040 | (G.spr_win? 0x2000 : 0)) : 0));
  if(G.spr_live){
    REG_BLDCNT|=0x2400;                                       // BG2 and the backdrop sit under the blended OBJs
    REG16(0x04000052)=0x1010;                                 // EVA=16, EVB=16: add the light
  }
}
static void spr_begin(void){ G.spr_n=0; G.spr_naff=0; G.spr_npal=0; G.spr_win=0; G.spr_bl=0; G.spr_ready=1; }
// v8.2c: Bliplets as 256-colour sprites (their art straight from critters.h, 32 px, a few poses), so they can stand
// beside a critter of another form: the bitmap has one set of critter colours, the OBJ palette has its own.
#define BL_UNIT 560                    // first tile (32-byte units) of the Bliplet poses in OBJ VRAM; 32 units a pose
static const u8 BL_POSES[6]={POSE_IDLE,POSE_BLINK,POSE_HAPPY,POSE_WALK1,POSE_WALK2,POSE_LOVE};
static void bl_tiles_load(void){
  for(int j=0;j<6;j++){
    const u8 *src=CR_PIX+CR_PIXOFF[1][BL_POSES[j]][1], *rows=CR_ROWS+2*CR_ROWOFF[1][BL_POSES[j]][1];
    volatile u32 *dst=OBJ_VRAM+(BL_UNIT-512+j*32)*8;
    for(int ty=0;ty<4;ty++) for(int tx=0;tx<4;tx++) for(int r=0;r<8;r++){   // 8bpp, 1D: 16 tiles of 8x8, row by row
      int y=ty*8+r, a=rows[2*y], b=rows[2*y+1]; u32 w0=0, w1=0;
      for(int k=0;k<8;k++){ int x=tx*8+k; u32 v= (a!=255 && x>=a && x<=b)? src[y*32+x] : 0; if(k<4) w0|=v<<(8*k); else w1|=v<<(8*(k-4)); }
      dst[(ty*4+tx)*16+r*2]=w0; dst[(ty*4+tx)*16+r*2+1]=w1; } }
}
static void spr_bliplet(int cx,int feet,int pose_i,int flip){   // pose_i: index into BL_POSES; feet: the bottom row
  if(G.spr_n>=SPR_MAX) return; G.spr_bl=1;
  int x=cx-16, y=feet-31;
  u16 *o=G.so[G.spr_n++]; o[0]=(u16)((y&255)|0x2000); o[1]=(u16)((x&511)|0x8000|(flip? 0x1000 : 0)); o[2]=(u16)(BL_UNIT+pose_i*32);
}
// a palette bank: 16 colours from 0 (transparent)
static int spr_pal(void){ if(G.spr_npal>=SPR_PALS) return -1; int b=G.spr_npal++; for(int c=0;c<16;c++) G.sp[b][c]=0; return b; }
static u16 rgb_scale(u16 c,int k){   // k: 0..256
  int r=(c&31)*k>>8, g=((c>>5)&31)*k>>8, b=((c>>10)&31)*k>>8; return (u16)RGB(r>31?31:r,g>31?31:g,b>31?31:b); }
static u16 rgb_mix(u16 a,u16 b,int k){   // a..b, k: 0..256
  int r=(a&31)+(((b&31)-(a&31))*k>>8), g=((a>>5)&31)+((((b>>5)&31)-((a>>5)&31))*k>>8), bl=((a>>10)&31)+((((b>>10)&31)-((a>>10)&31))*k>>8);
  return (u16)RGB(r,g,bl); }
// an affine matrix: scale (Q8, 256 = full size) and turn (0..63)
static int spr_aff(int scale,int turn){
  if(G.spr_naff>=SPR_AFF || scale<8) return -1;
  int j=G.spr_naff++, inv=(256*256)/scale, c=SIN64[(turn+16)&63], s=SIN64[turn&63];
  G.sa[j][0]=(s16)(c*inv>>8); G.sa[j][1]=(s16)(-s*inv>>8); G.sa[j][2]=(s16)(s*inv>>8); G.sa[j][3]=(s16)(c*inv>>8);
  return j; }
// one sprite: size 0/1/2 (8/16/32 square), centred on (cx,cy); aff<0: plain; dbl: affine double-size box
static void spr_add(int cx,int cy,int size,int tile,int bank,int aff,int dbl,int opaque){
  if(G.spr_n>=SPR_MAX || bank<0) return;
  int half=(8<<size)>>1; if(aff>=0 && dbl) half*=2;
  int x=cx-half, y=cy-half; if(x<=-2*half || x>=240 || y<=-2*half || y>=160) return;
  u16 a0=(u16)((y&255) | (opaque? 0 : 0x0400)), a1=(u16)((x&511) | (size<<14));
  if(aff>=0){ a0|=(u16)(dbl? 0x0300 : 0x0100); a1|=(u16)(aff<<9); }
  u16 *o=G.so[G.spr_n++]; o[0]=a0; o[1]=a1; o[2]=(u16)(tile | (bank<<12));
}


static void keep_from_v9(void);
static void journal_from_album(void);
static void garden_reset(void){ memset(G.gdn.p,0,sizeof(G.gdn.p)); G.gdn.seeds=3; G.gdn.vis_day=0xFF; G.gdn.vis_n=0; G.gdn.news=0; G.gdn.ok=1; }
static void init_once(void){
  if(G.magic==G_MAGIC) return;
  memset(&G,0,sizeof(State));
  G.magic=G_MAGIC;
  G.rng = (*(volatile u32*)0x03000000) ^ 0x9E3779B9u ^ FRAMES; if(!G.rng) G.rng=1;
  { int ver=load_all();
    if(!ver){ ext_load(); life_load(); deco_load(); album_load(); dex_load(); }   // a v7 save: convert once
    if(ver<9) care_from_v8();
    if(ver<10) keep_from_v9();
    if(ver<12) journal_from_album();
    if(ver==12){ G.jr.wonders=0; for(int i=0;i<G.jr.n;i++) if(((G.jr.s[i].info>>2)&3)==1) G.jr.wonders++; }   // v12 kept the longest life here
    if(ver<15){ garden_reset(); save_all(); } }   // (v14: More starts empty; v15: a garden with three seeds)
  if(!G.more.found_ok) found_init();
  G.last_clock = PET_CLOCK; G.tune=0xFF;
  G.last_happy=PET[P_HAPPY]; G.last_call=PET[P_CALL]; G.last_asleep=PET[P_ASLEEP];
  G.idle_cry=(u16)(3600+rnd(3600));
  refresh_tables();
}

static void to_room(u32 keys);
static void dex_mark(int f,int gender){
  u32 b=1u<<f; int ch=0;
  if(!(G.dex.seen&b)){ G.dex.seen|=b; ch=1; }
  if(f){ u32 *g= gender? &G.dex.girl : &G.dex.boy; if(!(*g&b)){ *g|=b; ch=1; } }
  if(ch) dex_save();
}
static void dex_tick(void){   // whatever is being raised right now counts as met
  if(*(volatile u32*)PET!=0x31504B43u || PET[P_STAGE]>7) return;
  dex_mark(form_index(),G.dex.gender);
}
static void to_room(u32 keys){ SCENE=1; G.mode=M_NONE; G.suppress=(u16)keys; }

// ================================================================== OUTSIDE
static void prerender_yard(int night){ O_DMACOPY(YARD_BG, night? BGX_YARD_NIGHT : BGX_YARD_DAY); G.bgx_yard=1; }   // see gen/bgfx.py
static void toys_reset(int bx,int bubble_wait){ G.bx=bx<<8; G.by=0; G.bvx=0; G.bvy=0; G.bubble_timer=(u16)bubble_wait; for(int i=0;i<4;i++){ G.bub[i].active=0; G.bub[i].type=0; } }
static void go_outside(void){
  G.outside=1; G.poop_hold=MESS_HERE; G.fresh=0; G.coin_on=0; G.coin_timer=(u16)(600+rnd(900)); G.plays=0;
  toys_reset(170,90);
  G.yard_night=(u8)is_night(); G.bg_state=0;
  sfx(0,SEQ_DOOR); G.cry_delay=40;
  if(!(G.ext.seen_tip&1)){ G.ext.seen_tip|=1; ext_save(); set_msg("SELECT: GO BACK INSIDE"); }
  else set_msg_name(" IS OUTSIDE!");
}
static void go_inside(const char *suffix){ G.outside=0; G.bg_state=0; sfx(0,SEQ_DOOR); if(suffix) set_msg_name(suffix); }
static int toy_on(int it){ return G.ext.active_toy==it && (G.ext.toys&(1<<TOYBIT(it))); }
static void care_play(int pts);
static void outside_play(int weight){
  PET[P_CALL]=0;
  G.plays=(u16)(G.plays+weight); care_play(weight);
  if(G.plays>=4){ G.plays=(u16)(G.plays-4); if(PET[P_HAPPY]<4){ PET[P_HAPPY]++; set_msg_name(" HAD A BLAST!"); } }
}
// called after every live update of the original while outside
static void outside_tick(void){
  u8 st=PET[P_STAGE];
  if(SCENE==0 || st<2 || st>7){ G.outside=0; return; }
  if(SCENE<1 || SCENE>5) return;
  if(PET[P_ASLEEP]){ go_inside(" GOT SLEEPY..."); return; }
  if(PET[P_SICK]){ go_inside(" FEELS SICK..."); return; }
  if(PET[P_LIGHTS]){ go_inside(" WENT BACK INSIDE."); return; }
  G.yard_night=(u8)is_night();
  sun_morning();   // (v8.3: SUNPUFF)
  // time outside and fresh air
  if(++G.yard_frames>=3600){ G.yard_frames=0; if(G.ext.yard_mins<60000) G.ext.yard_mins++; ext_save(); }   // (old grown-ups' SUNPUFF)
  int interval = fastdiv(2700); if(toy_on(IT_KITE)) interval = fastdiv(1800);
  if(++G.fresh>=interval){ G.fresh=0; if(PET[P_HAPPY]<4){ PET[P_HAPPY]++; set_msg_name(" LOVES THE FRESH AIR!"); } }
  // coins in the grass, picked up when the critter walks over them
  int pw=toy_on(IT_PINWHEEL);
  if(!G.coin_on && G.ext.yard_coins<(pw?4:3) && G.coin_timer && --G.coin_timer==0){ G.coin_on=1; G.coin_life=1500; G.coinx=24+rnd(150); }
  if(G.coin_on){
    int cx=G.room_cx;
    if(G.room_cy && G.coinx+6>cx+10 && G.coinx<cx+54){ G.coin_on=0; G.ext.yard_coins++; add_coins(5); sfx(0,SEQ_COIN); set_msg("FOUND 5 COINS!"); G.coin_timer=(u16)(pw? 500+rnd(600) : 1200+rnd(1200)); }
    else if(--G.coin_life==0){ G.coin_on=0; G.coin_timer=(u16)(pw? 500+rnd(600) : 1200+rnd(1200)); }
  }
  // ball: gets kicked when the critter bumps into it
  if(toy_on(IT_BALL)){
    int bx=G.bx>>8, cx=G.room_cx;
    if(G.room_cy && G.by==0 && G.bvx>-64 && G.bvx<64 && bx+10>cx+8 && bx<cx+56){
      int dir = (bx+5 > cx+32)? 1 : -1; G.bvx=dir*(400+rnd(300)); G.bvy=-(400+rnd(200)); sfx(0,SEQ_KICK); outside_play(1);
    }
    G.bx+=G.bvx; G.by+=G.bvy;
    if(G.by<0 || G.bvy){ G.bvy+=40; if(G.by>=0){ G.by=0; G.bvy = (G.bvy>200)? -(G.bvy/3) : 0; } }
    if(G.by==0){ G.bvx -= G.bvx/24; if(G.bvx>-8 && G.bvx<8) G.bvx=0; }
    if(G.bx<(4<<8)){ G.bx=4<<8; G.bvx=-G.bvx; } if(G.bx>(198<<8)){ G.bx=198<<8; G.bvx=-G.bvx; }
  }
  // bubbles
  if(toy_on(IT_BUBBLES)){
    if(G.bubble_timer && --G.bubble_timer==0){
      for(int i=0;i<4;i++) if(!G.bub[i].active){ Obj *b=&G.bub[i]; b->active=1; b->x=(G.room_cx+20+rnd(28))<<8; b->y=(G.room_cy+10)<<8; b->vy=-(40+rnd(40)); b->vx=rnd(2)?32:-32; b->type=(u8)(3+rnd(3)); break; }
      G.bubble_timer=(u16)(80+rnd(120));
    }
    for(int i=0;i<4;i++){ Obj *b=&G.bub[i]; if(!b->active) continue;
      b->y+=b->vy; b->x+=b->vx; if(((G.t+i*7)&31)==0) b->vx=-b->vx;
      if(b->y < (34<<8)){ b->active=0; sfx(0,SEQ_POP); outside_play(1); } }
  }
  // frisbee: glides across the yard, dipping lowest in the middle; a catch counts as two plays
  if(toy_on(IT_FRISBEE)){
    if(G.bub[0].type) G.bub[0].type--;                       // catch sparkle timer
    if(!G.bvy){
      if(G.bubble_timer && --G.bubble_timer==0){ int dir=rnd(2)? 1 : -1; G.bvx=dir*(260+rnd(120)); G.bx=(dir>0? -10 : 210)*256; G.bvy=1; }
      else if(!G.bubble_timer) G.bubble_timer=60;
    } else {
      G.bx+=G.bvx; int x=G.bx>>8, d= G.bvx>0? x+10 : 210-x;
      if(d<0) d=0; if(d>220) d=220;
      G.by=(46 + (d*(220-d))/320)<<8;
      int y=G.by>>8, cx=G.room_cx, cy=G.room_cy;
      if(cy && x+10>cx+12 && x<cx+52 && y+4>=cy){
        G.bvy=0; G.bubble_timer=(u16)(200+rnd(180)); G.bub[0].type=40; G.bub[0].x=x; G.bub[0].y=y;
        sfx(0,SEQ_KICK); outside_play(2); if(!MSG_TIMER && !rnd(3)) set_msg_name(" CAUGHT THE FRISBEE!");
      } else if(x<-12 || x>212){ G.bvy=0; G.bubble_timer=(u16)(60+rnd(90)); }
    }
  }
  // sprinkler: a sweeping arc of water; running through the spray counts as play
  if(toy_on(IT_SPRINKLER)){
    int p=(int)(G.sky_t>>3)&31, L=(p<16? p : 31-p)*7-52, land=TOY_X+L, cx=G.room_cx;
    if(G.room_cy && land+10>cx+8 && land-10<cx+56){ if(++G.bubble_timer>=100){ G.bubble_timer=0; sfx(0,SEQ_POP); outside_play(1); *(volatile u16*)(PET+0x3A)=0; } }
  }
}
// v8.2c: the yard's poops stand on free ground, clear of the house, every decor piece, toy and moonflower (mapped
// from every combination). Three depths, smaller further back; the first poop takes the front spot, the last the back.
// (x, bottom y) of each spot, in the order they fill; drawn back to front.
static const u8 YP_X[4]={122,86,116,88}, YP_Y[4]={131,120,116,104}, YP_SZ[4]={0,1,1,2};   // size 0 front, 1 middle, 2 back
static void yard_poops(void){
  int n= G.mode==M_GARDEN || G.mode==M_FLY? G.more.pad0 : G.yp_n; if(n>4) n=4; if(!n) return;
  static const u8 ORDER[4]={3,2,1,0};   // back (y 104), middle right (116), middle left (120), front (131)
  for(int o=0;o<4;o++){ int i=ORDER[o]; if(i>=n) continue;
    int s=YP_SZ[i], w= s==0? POOP_FRONT_W : s==1? POOP_MID_W : POOP_BACK_W, h= s==0? POOP_FRONT_H : s==1? POOP_MID_H : POOP_BACK_H;
    int x=YP_X[i], y=YP_Y[i]-h+1;
    for(int r=0;r<h;r++) for(int cc=0;cc<w;cc++){ int v= s==0? POOP_FRONT[r][cc] : s==1? POOP_MID[r][cc] : POOP_BACK[r][cc]; if(v) pset(x+cc,y+r,v); } }
}
static void garden_draw(int glow);
static void draw_outside_extras(void){
  int cxp=G.room_cx, cyp=G.room_cy? G.room_cy : 62;
  if(toy_on(IT_KITE)){
    int kx=cxp+80+((G.sky_t>>3)&15), ky=34+(((G.sky_t>>4)&7)); if(((G.sky_t>>3)&16)) kx=cxp+80+15-((G.sky_t>>3)&15);
    if(kx>196) kx=196;
    s32 x0=(kx+10)<<8, y0=(ky+20)<<8, dx=((cxp+32-kx-10)<<8)>>4, dy=((cyp+16-ky-20)<<8)>>4;
    for(int i=1;i<16;i++){ x0+=dx; y0+=dy; if(i&1) pset(x0>>8,y0>>8,G.yard_night?C_GRAY:C_OFFWHITE); }
    toy_draw(TOY_KITE,(int)(G.sky_t>>4)&1,kx,ky);
  }
  if(G.coin_on){ int bob=((G.sky_t>>3)&1); art(ART_COIN,7,7,G.coinx,120+bob,1,7); if((G.sky_t>>3)&2) art(ART_SPARK,5,5,G.coinx+5,114,1,5); }
  if(toy_on(IT_BALL)){ fill((G.bx>>8)+1,129,8,2,G.yard_night?C_NGRASS2:C_DKGREEN); toy_draw(TOY_BALL,(int)(((u32)(G.bx>>8)>>2)%3u),G.bx>>8,119+(G.by>>8)); }
  if(toy_on(IT_BUBBLES)) for(int i=0;i<4;i++){ Obj *b=&G.bub[i]; if(!b->active) continue; int x=b->x>>8, y=b->y>>8; ring(x,y,b->type,C_WHITE); pset(x-1,y-1,C_WHITE); }
  if(toy_on(IT_FRISBEE)){
    if(G.bvy){ int x=G.bx>>8, y=G.by>>8;
      fill(x+1,129,8,1,G.yard_night?C_NGRASS2:C_DKGREEN);
      toy_draw(TOY_FRISBEE,0,x,y); }
    else toy_draw(TOY_FRISBEE,1,TOY_X-1,123);
    if(G.bub[0].type) art(ART_SPARK,5,5,G.bub[0].x+3,G.bub[0].y-6-((40-G.bub[0].type)>>2),1,5);
  }
  if(toy_on(IT_SPRINKLER)){
    static const u8 AY[9]={0,13,22,28,30,28,22,13,0};
    int p=(int)(G.sky_t>>3)&31, L=(p<16? p : 31-p)*7-52, ph=(int)(G.sky_t>>1)&1;
    for(int i=1;i<=8;i++){ if(((i+ph)&1)) continue; int x=TOY_X+((L*i)>>3), y=120-AY[i]; fill(x,y,2,2,C_SKY); pset(x,y,C_WHITE); }
    for(int i=1;i<=8;i++){ if(!((i+ph)&1)) continue; int x=TOY_X-((L*i)>>4), y=120-(AY[i]>>1); pset(x,y,C_PALEBLUE); }
    toy_draw(TOY_SPRINKLER,0,TOY_X-4,120);
  }
  if(toy_on(IT_PINWHEEL)){
    fill(TOY_X,99,1,30,C_WOOD_DK);
    int f=(int)(G.sky_t>>2)&1, cx=TOY_X, cy=98;
    toy_draw(TOY_PINWHEEL,f,cx,cy);
  }
}

// ================================================================== DECOR (inside)
static int decor_in(int slot){ return G.deco.slot[slot]!=0xFF; }
static void note_icon(int x,int y){ art(ART_NOTE,5,6,x,y,1,5); }
static void draw_clock_hands(int cx,int cy){
  // map the 600-step day to hours: 0..480 = 6:00..22:00, 480..600 = 22:00..6:00
  u32 t=PET_CLOCK; if(t>599) t=599;
  int mins = t<480? 360 + (int)t*2 : 1320 + ((int)t-480)*4;   // minutes since midnight (approx)
  if(mins>=1440) mins-=1440;
  static const s8 CS[12][2]={{0,-8},{4,-7},{7,-4},{8,0},{7,4},{4,7},{0,8},{-4,7},{-7,4},{-8,0},{-7,-4},{-4,-7}};
  int h=(mins/60)%12, m=(mins%60)/5;
  for(int k=1;k<=6;k++) pset(cx+((CS[m][0]*k)>>3), cy+((CS[m][1]*k)>>3), C_OUTLINE);
  for(int k=1;k<=4;k++) pset(cx+((CS[h][0]*k)>>3), cy+((CS[h][1]*k)>>3), C_RED);
}
static void ellipse(int cx,int cy,int rx,int ry,int c){
  int a=rx*rx, b=ry*ry, ab=a*b;
  for(int dy=-ry; dy<=ry; dy++){ int w=0; int t=dy*dy*a; while((w+1)*(w+1)*b + t <= ab + a) w++; fill(cx-w,cy+dy,2*w+1,1,c); }
}
static void rrect(int x,int y,int w,int h,int c){ fill(x+2,y,w-4,1,c); fill(x+1,y+1,w-2,1,c); fill(x,y+2,w,h-4,c); fill(x+1,y+h-2,w-2,1,c); fill(x+2,y+h-1,w-4,1,c); }
static void draw_decor(int anim){
  int s0 = G.outside? 4 : 0, s1 = G.outside? 6 : 4;
  for(int s=s0;s<s1;s++){
    int it=G.deco.slot[s]; if(it==0xFF) continue;
    if(!anim){   // the still part of every piece is pre-drawn art; the switch below only animates
      decor_art(it);
      if(it==IT_PORTRAIT){ G.cpose=POSE_IDLE+1; sprite(CRITTER_SPR(PET[P_STAGE]),134,42,1,0,99); G.cpose=0; G.pstage=PET[P_STAGE]; }
      continue;
    }
    switch(it){
      case IT_BED:
        if(anim) break;
        fill(6,114,40,4,C_WOOD_DK); fill(4,104,44,10,C_OUTLINE); fill(5,105,42,8,C_TAN); fill(6,100,40,6,C_OUTLINE); fill(7,101,38,5,C_ROSE);
        fill(8,98,12,7,C_OUTLINE); fill(9,99,10,5,C_WHITE); fill(6,118,3,6,C_WOOD_DK); fill(43,118,3,6,C_WOOD_DK);
        break;
      case IT_FISHBOWL: {
        if(!anim){
          fill(10,116,30,10,C_OUTLINE); fill(11,117,28,8,C_WOOD3);
          disc(25,102,13,C_OUTLINE); disc(25,102,12,C_GLASS); fill(12,88,27,4,C_WALL); fill(16,91,19,1,C_OUTLINE);
          for(int y=95;y<114;y++) for(int x=13;x<38;x++){ int dx=x-25, dy=y-102; if(dx*dx+dy*dy<=140) pset(x,y,C_SKY); }
          break; }
        u32 q=G.sky_t>>2; int ph=(int)(q%28u); int fl=ph>=14; int fx= fl? 30-(ph-14) : 17+ph;
        int fy=103+(((G.sky_t>>4)&3)==1);
        fill(fx,fy,5,3,C_ORANGE); pset(fl?fx+5:fx-1,fy,C_ORANGE); pset(fl?fx+5:fx-1,fy+2,C_ORANGE); pset(fl?fx:fx+4,fy,C_OUTLINE);
        if(((G.sky_t>>4)&7)==0) pset(fx+2,fy-4,C_WHITE);
        break; }
      case IT_PLANT: {
        if(anim) break;
        int g=(u8)((u8)G.ext.day - G.deco.plant_day); if(g>2) g=2;
        fill(206,110,22,16,C_OUTLINE); fill(207,111,20,14,C_TERRA); fill(205,108,24,4,C_OUTLINE); fill(206,109,22,2,C_TERRA);
        int top=g==0? 100 : g==1? 88 : 76;
        fill(216,top,2,108-top,C_DKGREEN);
        for(int y=top+4;y<106;y+=6){ fill(209,y,7,3,C_GREEN); fill(218,y+3,7,3,C_GREEN); }
        if(g==2){ disc(216,top,4,C_PINK); disc(216,top,1,C_GOLD); disc(208,top+10,3,C_PINK); disc(225,top+14,3,C_PINK); }
        else if(g==1) disc(216,top,2,C_GREEN);
        break; }
      case IT_RADIO:
        if(!anim){
          fill(200,104,34,20,C_OUTLINE); fill(201,105,32,18,C_RED2); fill(204,108,12,12,C_GRAY);
          for(int y=109;y<120;y+=3) fill(205,y,10,1,C_OUTLINE);
          fill(220,108,10,5,C_PALEYEL); disc(225,117,2,C_GOLD);
          for(int k=0;k<10;k++) pset(228+k/3,103-k,C_OUTLINE); fill(203,124,3,3,C_OUTLINE); fill(228,124,3,3,C_OUTLINE);
          break; }
        pset(222+((G.sky_t>>5)&3),110,C_OUTLINE);
        if(!PET[P_ASLEEP] && !PET[P_LIGHTS]){ int k=(G.sky_t>>2)&31; note_icon(206+(k>>2),96-k); if(k>12) note_icon(222-((k-12)>>2),96-(k-12)); }
        break;
      case IT_CLOCK:
        if(!anim){ disc(146,48,11,C_OUTLINE); disc(146,48,10,C_WOOD3); disc(146,48,8,C_CREAM);
          pset(146,41,C_OUTLINE); pset(153,48,C_OUTLINE); pset(146,55,C_OUTLINE); pset(139,48,C_OUTLINE); break; }
        draw_clock_hands(146,48); pset(146,48,C_OUTLINE);
        break;
      case IT_BOOKS: {
        if(anim) break;
        fill(126,58,44,3,C_WOOD3); fill(126,61,44,1,C_OUTLINE); fill(128,62,3,4,C_WOOD_DK); fill(165,62,3,4,C_WOOD_DK);
        static const u8 bc[7]={C_RED2,C_BLUE,C_GOLD,C_GREEN,C_ROSE,C_TEAL,C_LILAC}; static const u8 bh[7]={14,12,15,11,13,14,10};
        int x=130; for(int i=0;i<7;i++){ fill(x,58-bh[i],5,bh[i],C_OUTLINE); fill(x+1,59-bh[i],3,bh[i]-1,bc[i]); pset(x+2,61-bh[i]+3,C_CREAM); x+=5; }
        fill(x+2,50,1,8,C_OUTLINE); fill(x+3,48,4,10,C_TEAL);
        break; }
      case IT_TOYBOX:
        if(anim) break;
        disc(16,101,5,C_OUTLINE); disc(16,101,4,C_BLUE); pset(15,99,C_WHITE);
        disc(34,99,4,C_OUTLINE); disc(34,99,3,C_TAN); disc(30,96,2,C_OUTLINE); disc(38,96,2,C_OUTLINE); pset(30,96,C_TAN); pset(38,96,C_TAN); pset(33,99,C_OUTLINE); pset(35,99,C_OUTLINE);
        fill(6,105,40,22,C_OUTLINE); fill(7,106,38,20,C_RED2); fill(5,103,42,5,C_OUTLINE); fill(6,104,40,3,C_GOLD);
        fill(22,112,8,6,C_GOLD); fill(23,113,6,4,C_ORANGE); fill(10,110,4,12,C_RED); fill(38,110,4,12,C_RED);
        break;
      case IT_TV:
        if(!anim){
          for(int k=0;k<8;k++){ pset(206+k,94-k,C_OUTLINE); pset(226-k,94-k,C_OUTLINE); }
          fill(197,94,38,28,C_OUTLINE); fill(198,95,36,26,C_SLATE); fill(200,97,26,19,C_OUTLINE);
          disc(230,101,2,C_GOLD); disc(230,108,2,C_GRAY); fill(203,122,6,4,C_WOOD_DK); fill(223,122,6,4,C_WOOD_DK);
          break; }
        if(!PET[P_ASLEEP] && !PET[P_LIGHTS]){
          static const u8 tvc[4]={C_SKY,C_PINK,C_MINT,C_PALEYEL};
          int ph=(int)(G.sky_t>>5)&3; fill(201,98,24,17,tvc[ph]);
          int bx=201+(int)((G.sky_t>>1)%20u); fill(bx,108,4,4,C_ORANGE); fill(201,113,24,2,C_GREEN);
          pset(203+((G.sky_t>>3)&15),101,C_WHITE);
        } else fill(201,98,24,17,C_OUTLINE);
        break;
      case IT_PORTRAIT:
        if(anim) break;
        fill(127,36,30,28,C_OUTLINE); fill(128,37,28,26,C_WOOD3); fill(130,39,24,22,C_OUTLINE); fill(131,40,22,20,C_CREAM);
        G.cpose=POSE_IDLE+1; sprite(CRITTER_SPR(PET[P_STAGE]),134,42,1,0,99); G.cpose=0; G.pstage=PET[P_STAGE];
        break;
      case IT_MOBILE: {
        if(!anim){ fill(101,28,1,6,C_OUTLINE); fill(88,34,28,2,C_WOOD_DK); break; }
        static const u8 MX[3]={89,101,113}, ML[3]={10,16,8};
        static const s8 MS[8]={0,1,2,1,0,-1,-2,-1};
        for(int i=0;i<3;i++){ int sw=MS[((G.sky_t>>4)+i*3)&7];
          int L=ML[i], acc=0, x=MX[i], ad= sw<0? -sw : sw, st= sw<0? -1 : 1;
          for(int k=0;k<L;k++){ pset(x,36+k,C_OUTLINE); acc+=ad; if(acc>=L){ acc-=L; x+=st; } }
          int sx=MX[i]+sw, sy=36+L+2; fill(sx-2,sy,5,1,C_GOLD); fill(sx,sy-2,1,5,C_GOLD); fill(sx-1,sy-1,3,3,C_GOLD); }
        break; }
      case IT_LANTERN:
        if(!anim){ fill(101,28,1,8,C_OUTLINE); fill(97,35,9,2,C_OUTLINE); disc(101,46,9,C_OUTLINE); disc(101,46,8,C_RED2);
          for(int y=39;y<54;y+=4) fill(94,y,15,1,C_ORANGE); fill(97,55,9,2,C_OUTLINE); fill(101,57,1,6,C_GOLD); break; }
        if(((G.sky_t>>3)&7)!=0) disc(101,46,3,C_GOLD); else disc(101,46,2,C_PALEYEL);
        break;
      case IT_SWING: {
        if(!anim){ fill(30,60,34,3,C_WOOD_DK); fill(30,59,34,1,C_OUTLINE); break; }
        static const s8 SW[16]={0,1,2,3,4,3,2,1,0,-1,-2,-3,-4,-3,-2,-1};
        int d=SW[(G.sky_t>>2)&15];
        { int acc=0, off=0, ad= d<0? -d : d, st= d<0? -1 : 1;
          for(int k=0;k<38;k++){ pset(36+off,63+k,C_WOOD); pset(56+off,63+k,C_WOOD); acc+=ad; if(acc>=38){ acc-=38; off+=st; } } }
        fill(32+d,101,30,3,C_OUTLINE); fill(33+d,101,28,2,C_WOOD3);
        break; }
      case IT_SANDBOX:
        if(anim) break;
        fill(20,114,48,14,C_OUTLINE); fill(22,116,44,10,C_KHAKI); fill(20,113,48,2,C_WOOD); fill(20,126,48,2,C_WOOD);
        fill(36,108,12,8,C_TAN); fill(34,105,4,11,C_TAN); fill(46,105,4,11,C_TAN); pset(34,104,C_TAN); pset(37,104,C_TAN); pset(46,104,C_TAN); pset(49,104,C_TAN);
        fill(41,111,3,5,C_WOOD_DK); fill(42,98,1,8,C_OUTLINE); fill(43,98,4,3,C_RED);
        disc(58,120,2,C_RED2); fill(56,117,1,3,C_OUTLINE);
        break;
      case IT_BIRDBATH: {
        if(!anim){ fill(184,104,7,22,C_OUTLINE); fill(185,104,5,22,C_GRAY); fill(179,124,17,4,C_OUTLINE); fill(180,124,15,3,C_GRAY);
          fill(171,96,33,9,C_OUTLINE); fill(172,97,31,7,C_GRAY); fill(174,97,27,3,C_SKY); break; }
        int ph=(int)(G.sky_t>>6)&7; if(ph==7) break;     // sometimes the bird flies off
        int bx = 174 + ((ph*5)%22), hop = ((G.sky_t>>3)&3)==0;
        fill(bx,91-hop,5,4,C_OUTLINE); fill(bx+1,92-hop,3,2,C_BROWN2); pset(bx+5,92-hop,C_ORANGE); pset(bx+3,92-hop,C_OUTLINE);
        break; }
      case IT_GARDEN: {
        if(anim) break;
        fill(166,114,44,12,C_WOOD_DK); for(int x=168;x<208;x+=6) fill(x,116,3,1,C_BROWN2);
        for(int i=0;i<5;i++){ int x=170+i*8; fill(x+1,106,1,9,C_DKGREEN); fill(x-1,108,5,2,C_GREEN); fill(x,104,3,2,C_GREEN); disc(x+1,111,2,(i&1)? C_RED : C_GREEN); }
        for(int x=166;x<210;x+=4){ fill(x,121,2,7,C_WHITE); } fill(166,123,44,1,C_WHITE);
        break; }
      // ---- v5 decor
      case IT_BEANBAG:
        if(anim) break;
        fill(8,125,36,3,C_WOOD_DK);
        rrect(6,103,40,23,C_OUTLINE); rrect(7,104,38,21,C_BLUE);
        rrect(14,107,24,6,C_ROYAL); fill(10,106,4,2,C_PALEBLUE); fill(9,109,2,3,C_PALEBLUE);
        fill(22,120,1,4,C_ROYAL); fill(30,118,1,5,C_ROYAL);
        break;
      case IT_PIANO:
        if(anim) break;
        fill(5,91,42,3,C_OUTLINE); fill(6,92,40,1,C_RED);
        fill(6,94,40,30,C_OUTLINE); fill(7,95,38,28,C_RED2);
        fill(15,97,22,8,C_OUTLINE); fill(16,98,20,6,C_CREAM); fill(18,100,16,1,C_GRAY); fill(18,102,10,1,C_GRAY);
        fill(6,107,40,8,C_OUTLINE); fill(7,108,38,6,C_WHITE);
        for(int x=9;x<45;x+=4) fill(x,112,1,2,C_GRAY);
        for(int x=10;x<44;x+=4) if(((x-10)>>2)%7!=2 && ((x-10)>>2)%7!=6) fill(x,108,2,4,C_OUTLINE);
        fill(9,118,34,1,C_RED); fill(8,123,3,4,C_WOOD_DK); fill(41,123,3,4,C_WOOD_DK);
        break;
      case IT_ARCADE:
        if(!anim){
          fill(206,74,26,53,C_OUTLINE); fill(207,75,24,51,C_ROYAL); fill(207,75,2,51,C_BLUE); fill(229,75,2,51,C_BLUE);
          fill(209,76,20,6,C_GOLD); fill(211,78,16,2,C_RED);
          fill(208,84,22,18,C_OUTLINE);
          fill(204,103,30,7,C_OUTLINE); fill(205,104,28,5,C_SLATE);
          fill(212,100,1,5,C_OUTLINE); disc(212,100,1,C_RED); fill(219,105,2,2,C_RED); fill(223,105,2,2,C_GOLD); fill(227,105,2,2,C_MINT);
          fill(214,113,10,9,C_OUTLINE); fill(215,114,8,7,C_SLATE); fill(218,116,2,3,C_GOLD);
          break; }
        if(!PET[P_ASLEEP] && !PET[P_LIGHTS]){
          fill(209,85,20,16,C_ROYAL);
          u32 t=G.sky_t; int bx=(int)(t%36u); if(bx>=18) bx=35-bx; int by=(int)((t*2/3)%26u); if(by>=13) by=25-by;
          fill(209+bx,86+by,2,2,C_WHITE);
          int px=bx-2; if(px<0) px=0; if(px>14) px=14; fill(209+px,99,6,1,C_MINT);
          for(int i=0;i<5;i++) fill(211+i*4,87,3,2,(i&1)?C_PINK:C_GOLD);
        } else fill(209,85,20,16,C_OUTLINE);
        break;
      case IT_FIREPLACE:
        if(!anim){
          fill(196,86,40,41,C_OUTLINE); fill(197,87,38,39,C_TERRA);
          for(int y=90,r=0;y<126;y+=4,r++){ fill(197,y,38,1,C_TAN); for(int x=199+(r&1)*4;x<235;x+=8) fill(x,y+1,1,3,C_TAN); }
          fill(193,83,46,4,C_OUTLINE); fill(194,84,44,2,C_WOOD3);
          fill(204,99,24,27,C_OUTLINE); fill(206,97,20,2,C_OUTLINE);
          fill(207,121,18,3,C_WOOD_DK); fill(209,119,14,2,C_BROWN2);
          break; }
        { static const s8 FLK[8]={0,2,3,1,4,2,0,3};
          static const u8 FW[12]={0,1,1,2,2,2,3,3,3,3,3,2};   // half-width by depth from the tip: a rounded teardrop
          for(int i=0;i<3;i++){ int fx=211+i*6, h=8+FLK[((G.sky_t>>2)+i*3)&7], sway=((G.sky_t>>3)+i)&1;
            static const u16 RC[13]={0,0,0,0,0,0,0,469,402,352,313,282,256};   // 11*256/(h-1): no divide per row
            for(int r=0;r<h;r++){ int k=(r*RC[h])>>8, w=FW[k>11?11:k], y=119-h+r, cx=fx+((r<h/3)? sway : 0);
              fill(cx-w,y,2*w+1,1,C_RED);
              if(w>=2 && r>1) fill(cx-w+1,y,2*w-1,1,C_ORANGE);
              if(w>=3 && r>h/2) fill(cx-1,y,3,1,C_GOLD);
              if(r>h-3 && w>=2) pset(cx,y,C_PALEYEL); } } }
        break;
      case IT_MIRROR:
        if(!anim){
          fill(146,29,1,3,C_OUTLINE);
          ellipse(146,47,12,16,C_OUTLINE); ellipse(146,47,11,15,C_GOLD); ellipse(146,47,9,13,C_OUTLINE); ellipse(146,47,8,12,C_GLASS);
          for(int k=0;k<6;k++){ pset(140+k,44-k,C_WHITE); pset(143+k,51-k,C_WHITE); pset(144+k,51-k,C_WHITE); }
          break; }
        if(((G.sky_t>>4)&7)==0) art(ART_SPARK,5,5,148,37,1,5);
        break;
      case IT_GUITAR:
        if(anim) break;
        fill(144,29,9,1,C_OUTLINE); fill(146,30,5,6,C_OUTLINE); fill(147,31,3,4,C_WOOD_DK);
        pset(145,31,C_GOLD); pset(151,31,C_GOLD); pset(145,34,C_GOLD); pset(151,34,C_GOLD);
        fill(147,36,3,13,C_OUTLINE); fill(148,36,1,13,C_WOOD3);
        for(int y=38;y<48;y+=3) pset(148,y,C_GOLD);
        disc(148,51,5,C_OUTLINE); disc(148,59,7,C_OUTLINE); fill(145,54,7,3,C_OUTLINE);
        disc(148,51,4,C_RED2); disc(148,59,6,C_RED2); fill(146,54,5,3,C_RED2);
        disc(148,55,2,C_OUTLINE); fill(145,60,7,2,C_OUTLINE); fill(146,51,1,3,C_WHITE); fill(143,58,1,3,C_RED);
        pset(147,57,C_PALEYEL); pset(149,57,C_PALEYEL); pset(147,59,C_PALEYEL); pset(149,59,C_PALEYEL);
        break;
      case IT_DISCO: {
        if(!anim){ fill(101,28,1,6,C_OUTLINE); disc(101,42,8,C_OUTLINE); disc(101,42,7,C_GRAY);
          for(int y=36;y<49;y+=3) fill(95,y,13,1,C_SLATE); for(int x=96;x<107;x+=3) fill(x,35,1,14,C_SLATE); break; }
        int ph=(int)(G.sky_t>>3)&3;
        static const s8 FX6[6][2]={{-4,-4},{2,-5},{4,0},{-1,3},{-5,1},{3,4}};
        for(int i=0;i<6;i++) if(((i+ph)&3)==0) pset(101+FX6[i][0],42+FX6[i][1],C_WHITE);
        if(!PET[P_LIGHTS]){
          static const u8 DC[4]={C_PINK,C_SKY,C_GOLD,C_MINT};
          for(int i=0;i<6;i++){ int x=(int)((i*41 + (G.sky_t>>1))%240u), y=34+((i*13)%44);
            int v=pget(x,y); if((BGX_WALLISH[v>>5]>>(v&31))&1){ fill(x,y,2,2,DC[(i+ph)&3]); } }
        }
        break; }
      case IT_PLANE: {
        if(!anim){ fill(101,28,1,4,C_OUTLINE); break; }
        static const s8 PX[16]={0,4,8,11,12,11,8,4,0,-4,-8,-11,-12,-11,-8,-4};
        int ph=(int)(G.sky_t>>3)&15, px=101+PX[ph], dir= (ph<4||ph>=12)? 1 : -1, py=48+((ph&4)? 1 : 0);
        { int acc=0, x=101, ad= px>101? px-101 : 101-px, st= px>101? 1 : -1, L=py-2-32;
          for(int k=0;k<L;k++){ pset(x,32+k,C_OUTLINE); acc+=ad; while(acc>=L && L){ acc-=L; x+=st; } } }
        fill(px-7,py,14,4,C_OUTLINE); fill(px-6,py+1,12,2,C_RED2); fill(px-2,py+1,4,1,C_WHITE);
        if(dir>0){ fill(px-7,py-3,3,3,C_OUTLINE); fill(px-6,py-2,1,2,C_RED2); fill(px+7,py-1+((G.sky_t>>1)&1)*4,1,2,C_GRAY); pset(px+4,py+1,C_SKY); }
        else { fill(px+4,py-3,3,3,C_OUTLINE); fill(px+5,py-2,1,2,C_RED2); fill(px-8,py-1+((G.sky_t>>1)&1)*4,1,2,C_GRAY); pset(px-5,py+1,C_SKY); }
        fill(px-3,py+4,6,1,C_OUTLINE);
        break; }
      case IT_TRAMPOLINE:
        if(anim) break;
        fill(21,114,3,14,C_OUTLINE); fill(43,115,3,13,C_OUTLINE); fill(65,114,3,14,C_OUTLINE);
        fill(22,115,1,13,C_GRAY); fill(44,116,1,12,C_GRAY); fill(66,115,1,13,C_GRAY);
        ellipse(44,108,27,6,C_OUTLINE); ellipse(44,108,26,5,C_BLUE); ellipse(44,108,22,3,C_OUTLINE); ellipse(44,108,21,2,C_SLATE);
        fill(17,109,55,6,C_OUTLINE); fill(18,110,53,4,C_BLUE); fill(18,110,53,1,C_PALEBLUE);
        for(int x=21;x<70;x+=5) fill(x,112,2,1,C_ROYAL);
        break;
      case IT_SLIDE:
        if(anim) break;
        fill(22,84,2,44,C_SLATE); fill(31,84,2,44,C_SLATE);
        for(int y=90;y<126;y+=6) fill(24,y,7,1,C_SLATE);
        fill(20,81,18,4,C_OUTLINE); fill(21,82,16,2,C_RED2);
        for(int k=0;k<38;k++){ pset(36+k,82+k,C_OUTLINE); fill(36+k,83+k,1,4,C_GOLD); pset(36+k,87+k,C_OUTLINE); }
        fill(74,120,10,4,C_GOLD); fill(74,119,10,1,C_OUTLINE); fill(74,124,10,1,C_OUTLINE);
        fill(56,106,2,22,C_SLATE);
        break;
      case IT_POND: {
        if(!anim){
          ellipse(188,120,23,8,C_GRAY); ellipse(188,120,21,6,C_BLUE); ellipse(186,119,15,3,C_SKY);
          for(int x=168;x<210;x+=7) fill(x,(x&4)? 112 : 127,4,2,C_GRAY);
          fill(207,104,1,13,C_DKGREEN); fill(209,107,1,10,C_DKGREEN); fill(206,102,2,4,C_BROWN2); fill(209,105,2,4,C_BROWN2);
          disc(176,121,3,C_GREEN); fill(176,119,2,2,C_BLUE); pset(196,118,C_WHITE); pset(197,118,C_WHITE);
          break; }
        int ph=(int)(G.sky_t>>5)&7, fx= ph<5? 174 : 192, fy= ph<5? 117 : 121, hop=((G.sky_t>>2)&15)==0;
        if(ph==4 || ph==7) { fx=183; fy=112; }
        fill(fx,fy-hop,6,3,C_OUTLINE); fill(fx+1,fy-hop,4,2,C_GREEN); pset(fx+1,fy-1-hop,C_OUTLINE); pset(fx+4,fy-1-hop,C_OUTLINE);
        if(((G.sky_t>>4)&7)==3) ring(192,121,3,C_PALEBLUE);
        break; }
      case IT_FLOWERS: {
        if(!anim){
          fill(166,119,44,9,C_OUTLINE); fill(167,120,42,7,C_WOOD_DK); for(int x=169;x<208;x+=5) pset(x,122,C_BROWN2);
          static const u8 FC[7]={C_PINK,C_GOLD,C_RED,C_LILAC,C_WHITE,C_PINK,C_ORANGE};
          for(int i=0;i<7;i++){ int x=170+i*6, top=106+(i&1)*4;
            fill(x,top,1,119-top,C_DKGREEN); fill(x+1,top+5,2,1,C_GREEN); fill(x-2,top+7,2,1,C_GREEN);
            disc(x,top,2,FC[i]); pset(x,top,C_GOLD); }
          break; }
        static const s8 SY[16]={0,1,2,3,3,2,1,0,-1,-2,-3,-3,-2,-1,0,0};
        for(int i=0;i<2;i++){
          u32 t=G.sky_t+i*90; int bx=(int)((t>>1)%60u); if(bx>=30) bx=59-bx; bx+=168+i*10;
          int by=96+i*6+SY[(t>>2)&15], open=((t>>2)&1);
          int c= i? C_GOLD : C_LILAC;
          if(open){ fill(bx-3,by,3,3,c); fill(bx+1,by,3,3,c); } else { fill(bx-1,by-1,1,3,c); fill(bx+1,by-1,1,3,c); }
          pset(bx,by,C_OUTLINE); pset(bx,by+1,C_OUTLINE);
        }
        break; }
    }
  }
  if(anim && G.fx_t){
    static const u8 FXX[N_SLOTS]={20,212,142,101,44,186}, FXY[N_SLOTS]={92,90,66,62,90,88};
    int k=90-G.fx_t, s=G.fx_slot, x=FXX[s], y=FXY[s];
    if(YARD_SLOT(s) != G.outside) return;
    int it=G.deco.slot[s];
    if(it==IT_RADIO || it==IT_PIANO || it==IT_GUITAR) { note_icon(x-6+(k>>3),y-k/3); note_icon(x+8-(k>>3),y-8-k/3); }
    else if(it==IT_BED || it==IT_BOOKS || it==IT_PORTRAIT || it==IT_SWING || it==IT_BEANBAG || it==IT_FIREPLACE || it==IT_TRAMPOLINE) art(ART_HEART,7,6,x,y-k/3,1,7);
    else if(it==IT_FISHBOWL){ ring(x+4,y-k/3,2,C_WHITE); ring(x-4,y-6-k/4,1,C_WHITE); }
    else art(ART_SPARK,5,5,x,y-k/3,1,5);
  }
}
static const char *DECOR_MSG[30]={" BOUNCES ON THE BED!"," WATCHES THE FISH."," SNIFFS THE FLOWERS."," DANCES TO THE MUSIC!",0," READS A BOOK.",
  " PLAYS WITH ITS TOYS!"," WATCHES CARTOONS!"," ADMIRES ITS PORTRAIT."," WATCHES THE MOBILE SPIN."," FEELS ALL COZY.",
  " SWINGS ON THE SWING!"," BUILDS A SANDCASTLE!"," WATCHES THE BIRDS."," PICKED AN APPLE!", 0,0,0,
  " FLOPS ON THE BEANBAG!"," PLAYS THE PIANO!"," PLAYS ARCADE GAMES!"," WARMS UP BY THE FIRE."," MAKES FACES IN THE MIRROR!",
  " STRUMS THE GUITAR!"," DANCES UNDER THE DISCO BALL!"," WATCHES THE PLANE FLY."," BOUNCES ON THE TRAMPOLINE!",
  " ZOOMS DOWN THE SLIDE!"," CAUGHT A FISH!"," CHASES BUTTERFLIES!"};
static void decor_boost(int it){   // v8: what using a piece does for its kind of care
  switch(ITEM_KIND[it]){
    case KP: care_play(1); break;                                                            // play
    case KR: { u16 h=*(volatile u16*)(PET+0x16); h=(u16)(h+10>100? 100 : h+10); *(volatile u16*)(PET+0x16)=h; break; }   // rest: health
    case KC: *(volatile u16*)(PET+0x3A)=0; break;                                           // clean: a messy room takes longer to make it sick
    case KF: if(PET[P_HUNGER]<4) PET[P_HUNGER]++; break;                                     // food
    case KO: { u8 st=PET[P_STAGE]; if(st>=1 && st<=7){ CareWin *w=(G.dex.phase==PH_GROW || G.dex.phase==PH_TEEN)? &G.care.grow : &G.care.adult;
               u32 t=w->garden+4u; w->garden=(u16)(t>60000? 60000 : t); } break; }   // outside: garden points
  }
}
static void decor_tick(void){
  if(G.fx_t) G.fx_t--;
  for(int s=0;s<N_SLOTS;s++) if(G.decor_cd[s]) G.decor_cd[s]--;
  if(SCENE!=1 || PET[P_ASLEEP] || PET[P_STAGE]==0 || PET[P_STAGE]==8 || MSG_TIMER || !G.room_cy) return;
  int cx=G.room_cx;
  int s0 = G.outside? 4 : 0, s1 = G.outside? 6 : 4;
  for(int s=s0;s<s1;s++){
    int it=G.deco.slot[s]; if(it==0xFF || it==IT_CLOCK || G.decor_cd[s]) continue;
    if((it==IT_TV || it==IT_ARCADE) && PET[P_LIGHTS]) continue;
    int near = (s==0||s==4)? cx<=34 : (s==1||s==5)? cx>=96 : s==2? (cx>=56 && cx<=100) : (cx>=24 && cx<=76);
    if(!near || rnd(200)) continue;
    G.fx_slot=(u8)s; G.fx_t=90; G.decor_cd[s]=(u16)fastdiv(9000);
    if(it==IT_BOOKS || it==IT_PORTRAIT || it==IT_PIANO){ if(PET[P_BOND]<4) PET[P_BOND]++; G.decor_cd[s]=(u16)fastdiv(20000); }
    else if(it==IT_GARDEN){
      if(G.deco.garden_day==(u8)G.ext.day) continue;           // one apple a day
      G.deco.garden_day=(u8)G.ext.day; deco_save();
      if(G.ext.inv[IT_APPLE]<9){ G.ext.inv[IT_APPLE]++; ext_save(); }
    }
    else if(it==IT_POND){
      if(G.deco.pond_day==(u8)G.ext.day) continue;             // one fish a day
      G.deco.pond_day=(u8)G.ext.day; deco_save();
      if(G.ext.inv[IT_FISH]<9){ G.ext.inv[IT_FISH]++; ext_save(); }
    }
    // v8.1 rebalance: using decor no longer raises happiness (each piece still has its message, its animation and
    // its kind's boost below); the bed's happy morning stays
    decor_boost(it);
    PET[P_CALL]=0;
    set_msg_name(DECOR_MSG[it-IT_BED]); cry();
    return;
  }
}

// ================================================================== SECRET FORMS
// a teen grows up after two days. Good side: OAKHART (lots of time outside), DUNEROSE (plants around it),
// FLUFFBUD (lots of games), else FRILLO. Less-care side: THORNHOG (sad a lot), SMOGWOOL (lived in a mess), else GUNKBILL.
static int has_item(int it,int s0,int s1){ for(int s=s0;s<s1;s++) if(G.deco.slot[s]==it) return 1; return 0; }
// what a teen grows into: good care - the kind of care it got most, measured against a target for each
// (OUTSIDE: about 7 minutes in the yard; PLANTS: 10 minutes with a plant inside and flowers or a garden outside;
// PLAY: about 3 minigames or lots of play - v8.3: a third less, as growing up takes a third less time).
// None reached: FRILLO, the all-rounder. Less care - the need it missed most.
#define T_YARD   27
#define T_GARDEN 80
#define T_PLAY   11
static void care_lean(const CareWin *w,u32 *yard,u32 *garden,u32 *play){
  *yard=(u32)w->yard*1000u/T_YARD; *garden=(u32)w->garden*1000u/T_GARDEN; *play=(u32)w->play*1000u/T_PLAY; }
static int pick_grown_up(void){
  const CareWin *w=&G.care.grow;
  if(G.dex.side==1){
    u32 y,g,p; care_lean(w,&y,&g,&p);
    u32 best=y; int f=F_OAKHART;
    if(g>best){ best=g; f=F_DUNEROSE; }
    if(p>best){ best=p; f=F_FLUFFBUD; }
    return best>=1000? f : F_FRILLO;
  }
  u32 n= w->n? w->n : 1;
  u32 food=(u32)w->miss[K_FOOD]*10000u/n/25, mess=(u32)w->miss[K_CLEAN]*10000u/n/25, sad=(u32)w->miss[K_PLAY]*10000u/n/30;
  u32 best=food; int f=F_GUNKBILL;
  if(mess && mess>=best){ best=mess; f=F_SMOGWOOL; }
  if(sad && sad>=best){ f=F_THORNHOG; }
  return f;
}
static void evolve_to(int target);
// v8: called from the game clock (room, awake). Picks the grown-up and plays the growing-up scene; the
// bookkeeping happens in track_stage once the scene has changed its stage.
static void teen_grow_up(void){
  if(G.dex.phase!=PH_TEEN || G.dex.adult || PET[P_STAGE]<3 || PET[P_STAGE]>7) return;
  if((u8)((u8)G.ext.day-G.dex.teen_day)<2) return;
  G.dex.adult=(u8)pick_grown_up(); dex_save();
  refresh_tables();                               // the teen keeps its own slot, the grown-up fills the rest
  evolve_to(PET[P_STAGE]==5? 6 : 5);
}
static void grown_up_done(void){   // the growing-up scene has finished
  G.dex.phase=PH_ADULT; G.dex.mess_day=(u8)G.ext.day;
  G.ext.adult_day=G.ext.day; G.ext.stars=0; G.ext.yard_mins=0;            // its grown-up days start now
  secret_counts_reset();
  memset(&G.care.adult,0,sizeof(CareWin)); for(int k=0;k<N_NEEDS;k++){ G.care.run[k]=0; G.care.miss_day[k]=(u8)G.ext.day; }
  G.life.warned=0; save_all();
}
// v8.3: what the good-side secrets count, as a grown-up (Dex.yard and Dex.games, old v7 fields only read when a
// pre-v9 save converts; Life.pad; More.pad1 bit 2): sunny mornings in the yard, wishes, a lantern of 8 fireflies
static void secret_counts_reset(void){ G.dex.yard=0; G.dex.games=0; G.life.pad=0; G.more.pad1&=(u8)~4; }
static int grown_now(void){ u8 st=PET[P_STAGE]; return st>=3 && st<=7 && G.dex.phase==PH_ADULT && !G.ext.special; }
static void sun_morning(void){   // in the yard early in the morning (the first ~2 minutes of daylight), once a day
  if(!grown_now() || !G.outside || PET_CLOCK>=150 || G.life.pad==(u8)(G.ext.day+1)) return;
  if(MSG_TIMER) return;   // (wait for the message box, so it is always seen)
  G.life.pad=(u8)(G.ext.day+1); if(G.dex.yard<255) G.dex.yard++; save_all(); G.check_special=1;
  set_msg_name(" BASKS IN THE MORNING SUN!");
}
static void secret_wish(void){ if(grown_now()){ if(G.dex.games<255) G.dex.games++; G.check_special=1; } }
static void secret_lantern(void){ if(grown_now() && G.fly_n>=8 && !(G.more.pad1&4)){ G.more.pad1|=4; G.check_special=1; } }
static void check_special(void){
  u8 st=PET[P_STAGE];
  if(G.ext.special || st<3 || st>7 || G.dex.phase==PH_GROW || G.dex.phase==PH_TEEN) return;
  if((u16)(G.ext.day - G.ext.adult_day) < 3) return;   // (v8.3: not before its 4th grown-up day; was its 3rd)
  int f=0;
  if(G.dex.phase==PH_OLD){        // a grown-up from before v7 keeps the old rules
    if(PET[P_WEIGHT]>=45) f=SF_CHONKLE;
    else if(PET[P_BOND]>=4 && PET[P_HAPPY]>=4 && PET[P_HUNGER]>=3) f=SF_GLIMMER;
    else if(G.ext.stars>=6) f=SF_ZIPPIT;
    else if(G.ext.yard_mins>=15) f=SF_SUNPUFF;
    else if(decor_in(0) && decor_in(1) && decor_in(2) && decor_in(3)) f=SF_NESTLE;
  } else if(G.dex.side==1){       // v8: each good-side grown-up has its own secret, keeping up what it is good at
    const CareWin *w=&G.care.adult; u32 g=w->good, b=w->bad; int a=G.dex.adult;
    if(a==F_OAKHART && G.dex.yard>=3) f=SF_SUNPUFF;                                    // v8.3: greets the morning sun on 3 days
    else if(a==F_DUNEROSE && (G.more.pad1&4)) f=SF_GLIMMER;                            // v8.3: 8 fireflies in its lantern in one night
    else if(a==F_FLUFFBUD && G.dex.games>=2) f=SF_ZIPPIT;                             // v8.3: wished on 2 shooting stars
    else if(a==F_FRILLO && decor_in(0) && decor_in(1) && decor_in(2) && decor_in(3)) f=SF_NESTLE;   // a full room
    else if(g+b>=20 && g*10>=(g+b)*9) f=SF_STARWING;                                  // any of them: near-perfect care
  } else {                        // less-care grown-ups turn around when what they missed stays fixed for two days
    u8 d=(u8)G.ext.day; int a=G.dex.adult;
    if(a==F_SMOGWOOL && (u8)(d-G.care.miss_day[K_CLEAN])>=2) f=SF_PETALAMB;
    else if(a==F_GUNKBILL && (u8)(d-G.care.miss_day[K_FOOD])>=2) f=SF_LILYBILL;
    else if(a==F_THORNHOG && (u8)(d-G.care.miss_day[K_PLAY])>=2) f=SF_JAMBLE;
  }
  if(!f) return;
  G.ext.special=(u8)f; ext_save(); refresh_tables();
  char b[48]; char *p=scpy(b,pet_name()); p=scpy(p," BECAME A "); p=scpy(p,form_name(SPECIAL_FORM[f])); scpy(p,"!"); set_msg(b);
  sfx(0,SEQ_EVOLVE); G.cry_delay=110;
}
static void new_unlock_seed(void);
static void life_reset(int keep_stars,int bonus);
static void journal_end(int ending);
static void track_stage(void){
  u8 st=PET[P_STAGE];
  if(st==G.ext.prev_stage) return;
  u8 prev=G.ext.prev_stage; G.ext.prev_stage=st;
  if(st==8 && prev>=1 && prev<=7) journal_end(0);   // it ran out of health and ran away: counted, but no star
  if(st==0 && prev==8){ life_reset(0,0); G.ext.prev_stage=0; }
  if(st==0){ G.ext.special=0; G.ext.stars=0; G.ext.yard_mins=0; new_unlock_seed();
    G.more.pad0=0; G.more.pad1&=(u8)~3;   // v8.2b: a new egg, a clean yard
    G.dex.gender=(u8)rnd(2); G.dex.phase=PH_GROW; G.dex.side=0; G.dex.adult=0; G.dex.games=0; G.dex.yard=0;   // a new egg: boy or girl,
    G.dex.c_good=G.dex.c_bad=G.dex.c_sad=G.dex.c_mess=0; memset(&G.care,0,sizeof(Care));
    G.more.rn[0]=G.more.rn[1]=0; memset(G.more.rmiss,0,sizeof(G.more.rmiss)); G.more.spare[2]=0; memset(G.names.cur,0,8); garden_reset(); dex_save(); }        // and a fresh care record
  if(st>=3 && st<=7 && prev>=3 && prev<=7 && G.dex.phase==PH_TEEN && G.dex.adult) grown_up_done();
  if(st>=3 && st<=7 && prev==2 && G.dex.phase==PH_GROW){   // the original grew it up: it becomes a teen first
    G.dex.phase=PH_TEEN; G.dex.side=(u8)care_side(); G.dex.teen_day=(u8)G.ext.day; dex_save(); }
  if(st==1 && prev==0){ G.want_name=1; G.life.hatch_day=G.ext.day; memset(&G.care.adult,0,sizeof(CareWin)); G.life.warned=0; life_save(); }
  if(st>=3 && st<=7 && (prev<3 || prev==8)){ G.ext.adult_day=G.ext.day; G.ext.stars=0; G.ext.yard_mins=0; G.ext.special=0; secret_counts_reset();
    memset(&G.care.adult,0,sizeof(CareWin)); G.life.warned=0; life_save(); }
  if(st>=1 && st<=7) G.cry_delay=120;
  ext_save(); refresh_tables();
}
static void garden_morning(void);
static void track_day(void){
  u32 c=PET_CLOCK;
  if(c < G.last_clock && G.last_clock - c > 60){ G.ext.day++; G.ext.yard_coins=0; garden_morning(); ext_save(); G.check_special=1; G.bg_state=0; }
  G.last_clock=c;
}
static void check_life(void);
static void care_sample(void);
static void care_play(int pts);
static void shop_level_watch(void);
// things to notice after every live tick of the original
static void life_watch(void){
  if(SCENE==0 || *(volatile u32*)PET!=0x31504B43u) return;   // title screen / no critter loaded
  if(PET[P_SICK] && G.deco.vit_day==(u8)(G.ext.day+1)) PET[P_SICK]=0;      // a VITAMIN today keeps sickness away
  track_day(); track_stage();
  // v8.3: an unnamed egg or baby shows no name. A brand-new game picks the egg's name slot after the tables were made
  // (egg to egg is no stage change), so refresh them whenever that slot isn't the blank one.
  if((PET[P_STAGE]==0 || G.want_name) && !G.names.cur[0] && PET[P_NAME]<10 && RAM_NAMES[PET[P_NAME]][0]) refresh_tables();
  if(G.cry_cool) G.cry_cool--;
  u8 h=PET[P_HAPPY], call=PET[P_CALL], sl=PET[P_ASLEEP];
  // (v8.1: a happy rise no longer counts as play by itself: toys, minigames and play decor count their own)
  if(h>G.last_happy && !sl && !G.cry_cool) cry();
  if(call && !G.last_call && !sl) G.cry_delay=20;
  if(G.last_asleep && !sl && G.deco.slot[0]==IT_BED){ if(PET[P_HAPPY]<4) PET[P_HAPPY]++; set_msg_name(" SLEPT GREAT!"); G.cry_delay=30; }
  if(G.last_asleep && !sl) fly_morning();   // v8.1: last night's fireflies fly home
  if(G.last_asleep && !sl && G.more.spare[2]){ G.more.spare[2]=0; if(PET[P_BOND]<4) PET[P_BOND]++; save_all();   // v8.1: last night's story
    set_msg_name(" DREAMT OF ITS STORY!"); G.cry_delay=30; }
  G.last_happy=PET[P_HAPPY]; G.last_call=call; G.last_asleep=sl;
  if(G.cry_delay && --G.cry_delay==0 && !PET[P_ASLEEP]) cry();
  if(SCENE==1 && !sl && PET[P_STAGE]>=1 && PET[P_STAGE]<=7 && --G.idle_cry==0){ G.idle_cry=(u16)(3600+rnd(5400)); if(!MSG_TIMER) cry(); }
  if(G.check_special && SCENE==1 && !sl){ G.check_special=0; check_special(); check_life(); }
  else if(SCENE==1 && !sl && (FRAMES&63)==0 && !G.sendoff_pending) check_life();
  care_sample();
  shop_level_watch();
  if(G.outside && G.mode!=M_GARDEN && G.mode!=M_FLY && !G.ex_yard) outside_tick();   // (the night yard is its own thing)
  decor_tick();
}
// ================================================================== GAME CLOCK (v8)
// The original's once-a-tick life simulation, now run here (the original's own copy is patched out, see patch.py).
// It is a faithful port: same fields in the original's critter record, same timers and odds, same random numbers,
// so the original's screens and actions keep working. What grows into what is decided here too.
#define P16(o) (*(volatile u16*)(PET+(o)))
#define P32(o) (*(volatile u32*)(PET+(o)))
#define PET_MAGIC   0x31504B43u
#define TICK_CNT    (*(volatile u32*)0x03000024)
#define SAVE_CNT    (*(volatile u32*)0x03000294)
#define ORIG_RNG    (*(volatile u32*)0x03000000)
#define ANIM_T      (*(volatile u32*)0x0300029C)
#define ANIM_KIND   (*(volatile u32*)0x03000298)
#define ANIM_ARG    (*(volatile u32*)0x030002A0)
#define GONE_T      (*(volatile u32*)0x030002CC)
#define SEQ_ALERT   ((const void*)0x0800FBE0u)
#define SEQ_GONE    ((const void*)0x0800FBF8u)
#define P_MISTAKES  0x0F
#define P_MISTAKES2 0x10
#define P_MEDS      0x11
// record layout (u16 unless noted): 0x16 health, 0x18 days, 0x1C ticks (u32), 0x20 ticks in this stage (u32),
// 0x24 clock (u32), 0x28 hunger timer, 0x2A happy timer, 0x2C/0x2E poop timer/interval, 0x30/0x32 call timer/interval,
// 0x34 call open, 0x36 starving, 0x38 unhappy, 0x3A messy, 0x3C sick, 0x3E lights left on, 0x40 cooldown,
// 0x42 meds, 0x44 health timer, 0x46 fuss timer
#define DARK_TICKS       120                /* v8.3: lights off by day, awake: an extra happy heart every 2 minutes */
#define TICK_FRAMES      60                 /* v8.3: one tick every 60 frames (1 s); a day is 600 clock steps: 8 min of day, 3 of night */
#define TICK_FRAMES_FAST 24                 /* FAST mode: 2.5 times as fast */

static const u8 HUNGER_RATE[4]={30,45,60,72}, HAPPY_RATE[4]={36,51,66,78};   // (v8.3: ticks per heart; a grown-up loses one every ~1:12 / 1:18)   // baby, child, teen, grown-up
static u32 orng(void){ u32 x=G.sim_r; x^=x<<13; x^=x>>17; x^=x<<5; G.sim_r=x; return x; }
static u32 xs_back(u32 y){   // the step before y (xorshift is reversible)
  u32 x=y; for(int i=0;i<7;i++) x=y^(x<<5); y=x;
  x=y; for(int i=0;i<2;i++) x=y^(x>>17); y=x;
  x=y; for(int i=0;i<3;i++) x=y^(x<<13); return x;
}
static void mistake(void){ if(PET[P_MISTAKES]<250) PET[P_MISTAKES]++; if(PET[P_MISTAKES2]<250) PET[P_MISTAKES2]++; }
static void sim_msg(const char *s){ int i=0; while(s[i] && i<37){ MSG_BUF[i]=s[i]; i++; } MSG_BUF[i]=0; MSG_TIMER=101; }
static void sim_save(void){   // the critter record only, like the original (the extension's own data saves when it changes)
  P32(0)=PET_MAGIC; u32 h=0x5EED; for(int i=0;i<0x48;i++) h=h*31+PET[i]; P32(0x48)=h; O_SAVE(PET,0x4C); }
static void evolve_to(int target){ ANIM_T=0; ANIM_KIND=7; ANIM_ARG=(u32)target; SCENE=3; sfx(1,SEQ_EVOLVE); }
static int rate_index(u8 st){
  if(st<=2) return st? st-1 : 0;
  if(G.dex.phase==PH_OLD) return st<=4? 2 : 3;
  return (G.dex.phase==PH_TEEN && !G.ext.special)? 2 : 3;
}
static void sim_grow(u8 st){
  u32 t=P32(0x20);
  if(st==1){ if(t>239) evolve_to(2); }                       // baby -> child after 240 steps
  else if(st==2){ if(t>599) evolve_to(3); }                  // child -> teen after a day (its kind was picked by care)
  else if(st>=3 && st<=7){
    if(G.dex.phase==PH_TEEN) teen_grow_up();                 // teen -> grown-up after two days
    else if(G.dex.phase==PH_OLD && (st==3 || st==4) && (t>>2)>=225){   // pre-v7 grown-ups keep the old rules
      if(st==3) evolve_to(PET[P_BOND]<=2? 6 : PET[P_MISTAKES]<2? 5 : 6);
      else evolve_to(PET[P_MISTAKES]<2? 6 : 7);
    }
  }
}
static void sim_tick(void){
  u8 st=PET[P_STAGE];
  if(st==8) return;
  P32(0x1C)++; P32(0x20)++;
  u32 sv=SAVE_CNT; SAVE_CNT=sv+1;
  // v8.1: at night the clock moves on only two steps in three, so the night lasts 4.5 min (not 3); everything the
  // critter does still runs at its usual pace, since that counts these steps (P32 0x1C), not the clock
  // v8.3: daylight moves 8 steps every 7 ticks (7 min), the night a step every 2 ticks (4 min); the critter's own
  // counters still count every tick, and every care timer below counts 1.5 times the ticks it did, so care keeps its pace
  // v8.3 (Nov 12): daylight a step every tick (8 min), the night two steps in three ticks (3 min): still an 11-minute day
  { u32 c=P32(0x24); if(c<480) c++; else if(P32(0x1C)%3) c++; P32(0x24)= c>=600? 0 : c; }
  if(sv>=14){ SAVE_CNT=0; sim_save(); }
  if(st==0){                                                  // the egg hatches after 20 steps, in the room
    if(P32(0x20)<20 || SCENE!=1) return;
    ANIM_T=0; ANIM_KIND=6; ANIM_ARG=1; SCENE=3; sfx(1,SEQ_FANFARE); return;
  }
  int ri=rate_index(st), pbase= st==1? 120 : 165;
  int day=P32(0x24)<480, sleeping, awake;
  u8 asleep=PET[P_ASLEEP];
  if(!asleep && !day){                                        // bedtime
    P16(0x3E)=0; PET[P_ASLEEP]=1; if(PET[P_CALL]) PET[P_CALL]=0;
    sim_msg("GETTING SLEEPY... LIGHTS OFF?"); sfx(0,SEQ_ALERT); P16(0x46)=0;
    asleep=1;
  } else if(asleep && day){                                   // morning
    P16(0x0C)=0; if(P16(0x18)<999) P16(0x18)++;
    sim_msg("GOOD MORNING!"); sfx(1,SEQ_HAPPY);
    asleep=0;
  }
  if(asleep){
    awake=0; sleeping=1;
    if(!PET[P_LIGHTS] && P16(0x3E)<1000){ u16 v=++P16(0x3E); if(v==68){ mistake(); if(PET[P_HAPPY]) PET[P_HAPPY]--; } }
  } else {
    awake=1; sleeping=0;
    // v8.3: awake by day with the lights off, it gets glum: an extra happy heart every DARK_TICKS (2 minutes)
    if(day && PET[P_LIGHTS] && !G.outside && st>=1 && st<=7){
      if(++G.dark_t>=DARK_TICKS){ G.dark_t=0; if(PET[P_HAPPY]){ PET[P_HAPPY]--; { char b[40]; char *q=scpy(b,pet_name()); scpy(q," DOESN'T LIKE THE DARK..."); sim_msg(b); } } } }
    else G.dark_t=0;
    { u16 v=++P16(0x28); if(v>=HUNGER_RATE[ri]){ P16(0x28)=0; if(PET[P_HUNGER] && --PET[P_HUNGER]==0){ sfx(0,SEQ_ALERT); P16(0x46)=0; } } }
    { u16 v=++P16(0x2A); if(v>=HAPPY_RATE[ri]){ P16(0x2A)=0; if(PET[P_HAPPY] && --PET[P_HAPPY]==0){ sfx(0,SEQ_ALERT); P16(0x46)=0; } } }
    { u16 v=++P16(0x2C); if(v>=P16(0x2E)){ P16(0x2C)=0; u32 r=orng(); P16(0x2E)=(u16)((((r>>16)*105u)>>16)+pbase); if(MESS_HERE<=3) MESS_HERE++; } }
    if(PET[P_CALL]){ u16 v=++P16(0x34); if(v>=60){ PET[P_CALL]=0; mistake(); } }
    else { u16 v=++P16(0x30);
      if(v>=P16(0x32)){ P16(0x30)=0; P16(0x34)=0; PET[P_CALL]=1; u32 r=orng(); P16(0x32)=(u16)((((r>>16)*225u)>>16)+225); sfx(0,SEQ_ALERT); P16(0x46)=0; } }
  }
  u8 hunger=PET[P_HUNGER]; u16 starve=0;
  if(hunger) P16(0x36)=0;
  else { starve=P16(0x36); if(starve<1000){ starve=++P16(0x36); if(starve==90) mistake(); } }
  u8 happy=PET[P_HAPPY];
  if(happy) P16(0x38)=0; else if(P16(0x38)<1000){ if(++P16(0x38)==90) mistake(); }
  int mess=PET[P_POOP]+G.more.pad0;   // (v8.2b: the room's and the yard's mess both count)
  if(mess<2) P16(0x3A)=0;
  else if(++P16(0x3A)>=120){                                   // a messy room can make it sick
    P16(0x3A)=0;
    if(!(orng()&0x80000000u) && !PET[P_SICK]){ P16(0x3C)=0; PET[P_SICK]=(u8)((orng()>>31)+1); sfx(0,SEQ_ALERT); P16(0x46)=0; }
  }
  int sick=0;
  if(PET[P_SICK]){ sick=1; if(P16(0x3C)<1000){ if(++P16(0x3C)==180) mistake(); } }
  if(PET[P_MEDS]){ if(++P16(0x42)>=135){ P16(0x42)=0; PET[P_MEDS]--; } }
  if(P16(0x40)) P16(0x40)--;   // PET's cooldown: the original's game_update sets 90 (one-second ticks)
  u16 ht=++P16(0x44);
  int sv_starve= starve>45, dmg= (sick && P16(0x3C)>45)? (sv_starve? 2 : 1) : (sv_starve? 1 : 0), healed=0;
  if(dmg){ dmg*=2; if(ht>=9){ P16(0x44)=0; u16 h=P16(0x16); P16(0x16)=(u16)(h>dmg? h-dmg : 0); } }   // (v8.3: 2x the loss every 9 ticks = the old pace)
  else if(ht>=6){ P16(0x44)=0; if(P16(0x16)<=99) P16(0x16)++; healed=1; }
  // v8.1: health stays hidden, but once it falls below a third the critter says so (once, until it is well again)
  if(dmg && P16(0x16)<34 && P16(0x16) && !G.weak_warned){ G.weak_warned=1; set_msg_name(" LOOKS WEAK... IT NEEDS CARE!"); sfx(0,SEQ_ALERT); }
  else if(P16(0x16)>=60) G.weak_warned=0;
  if(!healed && P16(0x16)==0){                                // it ran out of health
    G.gone_form=(u8)form_index(); SCENE=9; PET[P_STAGE]=8; GONE_T=0; O_SOUND(0,0,0,0); sfx(1,SEQ_GONE); sim_save(); return;
  }
  if(!sleeping){                                              // awake: it fusses while it needs something
    int need= !hunger || !happy || MESS_HERE>2 || sick;   // (the mess where it is)
    if((need || PET[P_CALL]) && ++P16(0x46)>=45){ sfx(0,SEQ_ALERT); P16(0x46)=0; }
    if(SCENE==1) sim_grow(st);                                // and grows up only while awake in its room
  }
  (void)awake;
}
// once a frame, before the original's update (scenes 1-5, like the original's own clock)
static void sim_frame(void){
  u32 sc=SCENE; if(sc<1 || sc>5) return;
  u32 n=TICK_CNT+1; TICK_CNT=n;
  if((int)n < (fast_mode()? TICK_FRAMES_FAST : TICK_FRAMES)) return;
  TICK_CNT=0;
  G.sim_r=ORIG_RNG; orng();                 // the original steps its random numbers at the start of every frame:
  sim_tick();                             // take that step now, use the numbers in order, then wind back one step
  ORIG_RNG=xs_back(G.sim_r);                // so the original's own step lands exactly where the clock left off
}
static void live_update(u32 k){
  u32 sc=SCENE, ak=ANIM_KIND;
  sim_frame(); O_UPDATE(k); life_watch();
  // v8.3: after a MEAL or a SNACK the feed menu opens again, on the same choice (B closes it, as always)
  if(sc==3 && ak<=1 && SCENE==1 && G.mode==M_NONE && PET[P_STAGE]>=1 && PET[P_STAGE]<=7 && !PET[P_ASLEEP]){
    SCENE=2; *(volatile u32*)0x030002D4=ak; }
}

// ================================================================== LIFE CYCLE
static int grown_days(void){ return (u16)(G.ext.day - G.ext.adult_day); }
static const u8 MIN_W[8]={0,5,10,18,18,28,28,28};    // the original's lightest weight per stage
static int overfed(void){ u8 st=PET[P_STAGE]; return st<=7 && PET[P_WEIGHT]>=MIN_W[st]*2+24; }   // (v8.3: the line stays on the original's table)
// v8.3: each stage's lightest weight, in pounds: baby 5, child 10, teen 20, grown-up and secret forms 30. The weight
// is kept at least this (growing up, salad, Hop Hop, old saves); the overweight line above is unchanged.
static int min_weight(void){ u8 st=PET[P_STAGE]; return st<=1? 5 : st==2? 10 : G.dex.phase==PH_TEEN? 20 : 30; }
static void weight_floor(void){ u8 st=PET[P_STAGE]; if(st>=1 && st<=7){ int lo=min_weight(); if(PET[P_WEIGHT]<lo) PET[P_WEIGHT]=(u8)lo; } }
static CareWin *care_win(void){ return (G.dex.phase==PH_GROW || G.dex.phase==PH_TEEN)? &G.care.grow : &G.care.adult; }
static void cw_add(u16 *v,int k){ u32 t=*v+(u32)k; *v=(u16)(t>60000? 60000 : t); }
static void care_play(int pts){ u8 st=PET[P_STAGE]; if(st>=1 && st<=7){ cw_add(&care_win()->play,pts); } }
static int garden_points(void){ return has_item(IT_PLANT,0,4) + (has_item(IT_FLOWERS,4,6) || has_item(IT_GARDEN,4,6)); }
static void care_sample(void){
  u8 st=PET[P_STAGE]; if(st<1 || st>7 || SCENE<1 || SCENE>5) return;
  if(++G.care_timer < fastdiv(900)) return;
  G.care_timer=0;
  int ofed=overfed(), night_light=PET[P_ASLEEP] && !PET[P_LIGHTS], miss[N_NEEDS];
  miss[K_FOOD]  = PET[P_HUNGER]==0 || ofed;
  miss[K_PLAY]  = PET[P_HAPPY]==0;
  miss[K_CLEAN] = MESS_HERE!=0;
  miss[K_REST]  = PET[P_SICK]!=0 || night_light;
  int good = PET[P_HUNGER]>=1 && PET[P_HAPPY]>=1 && !PET[P_SICK] && !MESS_HERE && !night_light && !ofed;   // all-round good care
  CareWin *w=care_win();
  cw_add(&w->n,1); cw_add(good? &w->good : &w->bad,1);
  for(int k=0;k<N_NEEDS;k++) if(miss[k]) cw_add(&w->miss[k],1);
  { More *m=&G.more;   // v8.1: the last day or so, for the status card's !
    if(m->rday!=(u8)G.ext.day){ m->rday=(u8)G.ext.day; m->rn[1]=m->rn[0]; m->rn[0]=0; for(int k=0;k<N_RMISS;k++){ m->rmiss[1][k]=m->rmiss[0][k]; m->rmiss[0][k]=0; } }
    u8 r[N_RMISS]={(u8)(PET[P_HUNGER]==0 || ofed),(u8)(PET[P_HAPPY]==0),(u8)(MESS_HERE!=0),(u8)(PET[P_SICK]!=0),(u8)night_light};
    if(m->rn[0]<60000) m->rn[0]++;
    for(int k=0;k<N_RMISS;k++) if(r[k] && m->rmiss[0][k]<60000) m->rmiss[0][k]++; }
  if(G.outside) cw_add(&w->yard,1);
  cw_add(&w->garden,garden_points());
  if(G.dex.phase==PH_ADULT)            // a need left missing for about a minute counts as missed today
    for(int k=0;k<N_NEEDS;k++){ if(!miss[k]) G.care.run[k]=0; else if(++G.care.run[k]>=4){ G.care.run[k]=0; G.care.miss_day[k]=(u8)G.ext.day; } }
  save_all(); if(G.dex.phase==PH_GROW) refresh_tables();
}
static int care_pct(void){ u32 t=G.care.adult.good+G.care.adult.bad; return t? (int)((G.care.adult.good*100u)/t) : 50; }
static void check_life(void){
  u8 st=PET[P_STAGE]; if(st<3 || st>7) return;
  int d=grown_days();
  if(d>LIFESPAN || (d==LIFESPAN && PET_CLOCK>=LIFESPAN_NOON && PET_CLOCK<600)) G.sendoff_pending=1;
  else if(d>=LIFESPAN-1 && !G.life.warned){ G.life.warned=1; life_save(); set_msg_name(" GAZES UP AT THE STARS..."); }
}
static void new_egg(void){
  u8 snd=PET[P_SOUNDON], fast=PET[P_FAST];
  for(int i=0;i<0x4C;i++) PET[i]=0;
  *(volatile u16*)(PET+0x32)=300; *(volatile u16*)(PET+0x2E)=180; *(volatile u16*)(PET+0x16)=100; *(volatile u16*)(PET+6)=0x0202;
  PET[P_SOUNDON]=snd; PET[P_FAST]=fast;
  *(volatile u32*)0x03000004=120; *(volatile u32*)0x03000008=120; *(volatile u32*)0x0300000C=60; *(volatile u8*)0x030002D0=0;
  *(volatile u32*)PET=0x31504B43u;
  PET[P_NAME]=(u8)rnd(10);
  memset(&G.care,0,sizeof(Care)); G.life.warned=0; G.life.hatch_day=G.ext.day; life_save();
  pet_save();
  SCENE=1; MSG_TIMER=0; G.outside=0; G.bg_state=0; G.sendoff_pending=0;
  set_msg("A STAR-SEED FELL IN THE MEADOW!"); sfx(1,SEQ_HAPPY);
}
// A new beginning: everything starts fresh. Only a WONDERFUL ending keeps (and adds to) the stars;
// the ending's bonus coins stack on top of the usual 20.
static void life_reset(int keep_stars, int bonus){
  u8 mo=G.life.music_off, tips=G.ext.seen_tip, ret=G.life.retired;
  memset(&G.ext,0,sizeof(Ext));
  for(int i=0;i<3;i++) G.ext.slot[i]=0xFF;
  G.ext.active_toy=0xFF; G.ext.last_game=0xFF; G.ext.prev_stage=PET[P_STAGE]; G.ext.coins=(u16)(20+bonus); G.ext.seen_tip=tips;
  ext_save();
  memset(&G.deco,0,sizeof(Deco));
  for(int i=0;i<N_SLOTS;i++) G.deco.slot[i]=0xFF;
  G.deco.plant_day=G.deco.garden_day=G.deco.pond_day=0xFF;
  new_unlock_seed();
  memset(&G.life,0,sizeof(Life)); G.life.music_off=mo; G.life.retired= keep_stars? ret : 0; memset(&G.care,0,sizeof(Care)); life_save();
  if(!keep_stars){ memset(&G.album,0,sizeof(Album)); album_save(); }
  G.outside=0; G.bg_state=0; G.sendoff_pending=0; G.shop_news=0; G.check_special=0;
  for(int i=0;i<N_SLOTS;i++) G.decor_cd[i]=0;
  G.last_clock=PET_CLOCK;
}
// NEW EGG from the title screen: a clean slate, stars and album included
static void full_restart(void){ life_reset(0,0); }

static const s8 SINE16[16]={0,3,6,7,8,7,6,3,0,-3,-6,-7,-8,-7,-6,-3};
// endings: 1 WONDERFUL (care 80%+), 3 GOOD (60-79%), 2 SAD (under 60%)
#define E_WONDER 1
#define E_SAD 2
#define E_GOOD 3
// ---- the journal's records: every critter that says farewell becomes a star with its story; one that runs away
// (its health ran out) is counted but leaves no star
static int care_most(const CareWin *w){   // what the status card shows as the care it got most: 0 a bit of all
  u32 y,g,p; care_lean(w,&y,&g,&p); int m=0; u32 best=250;
  if(y>=best){ best=y; m=1; } if(g>best){ best=g; m=2; } if(p>best){ best=p; m=3; } return m; }
static void story_name(Story *s,const char *nm){ for(int k=0;k<NAME_LEN;k++) s->name[k]=0; for(int k=0;k<NAME_LEN && nm[k];k++) s->name[k]=nm[k]; }
static int life_days(void){ int d=P16(0x18); return d<1? 1 : d>255? 255 : d; }   // v8.1: its age in years, as the status card shows it
static Story *story_new(void){
  Journal *j=&G.jr;
  if(j->n>=STORY_N){ for(int i=1;i<STORY_N;i++) memcpy(&j->s[i-1],&j->s[i],sizeof(Story)); j->n=STORY_N-1; }
  return &j->s[j->n++];
}
static void journal_end(int ending){
  Journal *j=&G.jr; int d=life_days();
  if(j->raised<9999) j->raised++;
  if(ending==E_WONDER && j->wonders<9999) j->wonders++;
  if(ending){
    Story *s=story_new(); story_name(s, G.names.cur[0]? G.names.cur : NAME_STR(PET[P_NAME]));
    s->form=(u8)form_index(); s->info=(u8)((G.dex.gender&1)|(ending<<2)); s->days=(u8)d;
    s->loved=(u8)care_most(G.care.adult.n? &G.care.adult : &G.care.grow); if(j->loved[s->loved]<255) j->loved[s->loved]++;
  }
  save_all();
}
static void journal_from_album(void){   // an older save: its star album becomes the first stars
  Journal *j=&G.jr; memset(j,0,sizeof(Journal));
  int n=G.album.n<ALBUM_N? G.album.n : ALBUM_N;
  for(int e=0;e<n;e++){ Story *s=story_new(); char nb[NAME_LEN+1]; int k=0; for(;k<NAME_LEN && G.names.alb[e][k];k++) nb[k]=G.names.alb[e][k]; nb[k]=0;
    story_name(s, k? nb : NAME_STR(G.album.name[e])); s->form= G.album.form[e]<N_FORMS? G.album.form[e] : 0;
    s->info=(u8)(2|(E_WONDER<<2)); s->days=0; s->loved=0xFF; }
  j->raised=(u16)(G.life.retired>j->n? G.life.retired : j->n); j->wonders=j->raised;
}
static void backdrop_stars(void){   // phase 4: painted star world (gen/title.py), palette loaded by set_extra_palette
  O_DMACOPY(YARD_BG, STARWORLD_BG); G.tbg=1; G.bg_state=0;
}
static void backdrop_yard(int night){
  prerender_yard(night); G.bg_state=0; G.tbg=0;
  u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)YARD_BG; fill(0,0,240,28,C_OUTLINE); fill(0,132,240,28,C_OUTLINE); DRAW_TARGET=sv;
}
static void sky_key(int k){ for(int i=0;i<4;i++) PAL[C_SKY0+i]=SKY_KEYS[k].c[i]; PAL[C_CLOUD]=SKY_KEYS[k].c[4]; PAL[C_CLOUD2]=SKY_KEYS[k].c[5];
  PAL[C_SUN]=RGB(31,29,14); PAL[C_SUN2]=RGB(31,22,8); }
static void start_sendoff(void){
  int c=care_pct();
  G.sendoff = c>=80? E_WONDER : c>=60? E_GOOD : E_SAD;
  G.mode=M_SENDOFF; G.sendoff_t=0; G.sendoff_pending=0; G.outside=0; MSG_TIMER=0;
  PET[P_LIGHTS]=0; G.fx_light=0;   // lights on: with them off the original dims the low colours, and the farewell's glow with them
  SND_PTR(0)=0; SND_PTR(1)=0;
  G.so_special = (G.sendoff!=E_SAD && PET[P_STAGE]>=3 && PET[P_STAGE]<=7)? G.ext.special : 0;
  G.so_lost = (G.sendoff!=E_WONDER && G.life.retired)? 1 : 0;
  G.so_bonus = G.sendoff==E_WONDER? 35 : G.sendoff==E_GOOD? 20 : 0;
  if(G.sendoff==E_WONDER){ if(G.life.retired<255) G.life.retired++; album_add(PET[P_NAME], form_index()); }
  journal_end(G.sendoff);   // (saves)
  G.bg_state=0; refresh_tables();
  int sp=G.so_special;
  if(sp==SF_ZIPPIT || sp==SF_GLIMMER) backdrop_stars();
  else backdrop_yard(G.sendoff==E_SAD);
}
// v8 phase 6: every form rises the same way (the original five secret forms keep their own scenes): it glows, a beam
// of starlight comes down, it rises (the hardware pixelates and whitens the picture), and it shines in the night sky
// as a star in its own colour while the picture slowly zooms out
static int so_unified(void){ int sp=G.so_special; return !(sp==SF_SUNPUFF || sp==SF_CHONKLE || sp==SF_ZIPPIT || sp==SF_GLIMMER || sp==SF_NESTLE); }
static void rise_fx(u32 t){
  if(t==220) sfx(1,SEQ_BEAM);
  if(t<400) return;
  if(t<560){ G.mos_fix=(u8)((t-400)*12/160);
    if(t>=470){ G.fx_kind=FX_WHITE; G.fx_lvl=(u16)(((t-470)*16/90)<<8); G.fx_step=1; } return; }
  if(!G.tbg){ backdrop_stars(); G.mos_fix=0; fx_fade(FX_WHITE,16,50); if(t<600) G.zoom_t=120; sfx(0, G.sendoff==E_SAD? SEQ_FAINTSTAR : SEQ_NEWSTAR); }
}
static void sendoff_update(u16 pr, u16 keys){
  G.sendoff_t++;
  if(G.sendoff_t>120 && G.sendoff_t<780 && (pr&K_A)) G.sendoff_t=780;
  if(so_unified()) rise_fx(G.sendoff_t);
  if(G.sendoff_t>=820 && (pr&K_A)){ G.mos_fix=0; G.zoom_t=0; }
  if(G.sendoff_t>=820 && (pr&K_A)){ G.mode=M_NONE; G.suppress=keys; life_reset(G.sendoff==E_WONDER, G.so_bonus); new_egg(); }
}
static void sendoff_box(const char *a,const char *b){ O_BOX(10,118,220,36); textc(a,120,126,C_DTEXT); if(b) textc(b,120,138,C_DTEXT); }
static void three_boxes(u32 t,const char *a1,const char *a2,const char *b1,const char *b2,const char *c1,const char *c2){
  char l1[40]; char *p=scpy(l1,pet_name()); scpy(p,a1);
  if(t<220) sendoff_box(l1,a2); else if(t<560) sendoff_box(b1,b2); else if(t<780) sendoff_box(c1,c2);
}
static int rise_sc(int y){ return y>50? 3 : y>24? 2 : 1; }
static int story_kind(const Story *s); static int story_colour(const Story *s); static void jr_star(int x,int y,int c,int kind,int tw);
static void dot_ring(int cx,int cy,int r,int c,int ph){ for(int i=0;i<16;i++) pset(cx+SINE16[(i+4+ph)&15]*r/8, cy+SINE16[(i+ph)&15]*r/8, c); }
static const u8 SKY_OLD[9][2]={{24,36},{62,44},{96,32},{150,30},{186,42},{218,34},{40,74},{204,72},{170,86}};
// v8.2: the farewell star as hardware sprites: a soft halo and a heart of rays. The light gathers on the critter as
// it rises, stays sharp through the white-out (sprites are never a brightness target), glides to its place in the
// night sky, then breathes and slowly turns. A sad goodbye leaves a small, faint star.
static void star_sprites(u32 t,int k,int col){
  if(t<300) return;
  int sad= k==E_SAD, big= k==E_WONDER? 256 : k==E_GOOD? 216 : 150, cy, gs, rs, br;
  if(t<560){ int y=81-(int)((t-220)*72/340), sc=rise_sc(y); cy=y+8*sc;
    gs=64+(int)(t-300)*(big-64)/260; rs= t<400? 0 : (int)(t-400)*big*4/(5*160);
    br=clampi((int)(t-300)*256/120,0,256); }
  else { int u=(int)t-560; cy= u>=40? 58 : 17+(58-17)*(u*(80-u))/1600;
    int b=SIN64[(t>>1)&63]; gs=big+big*b/2560; rs=big*9/10+big*b/3200; br=256; }
  if(sad) br=br*150>>8;
  int lvl= G.fx_kind==FX_WHITE? G.fx_lvl>>8 : 0, washed= lvl>6;
  int b=spr_pal(); if(b<0) return;
  u16 sc=PAL[col];
  static const u8 SG[7]={0,14,32,56,90,135,190};
  for(int L=1;L<=6;L++) G.sp[b][L]=rgb_scale(sc,SG[L]*br>>8);
  for(int L=9;L<=12;L++){ u16 c=rgb_scale(rgb_mix(sc,0x7FFF,(L-8)*64),br); if(washed) c=rgb_mix(c,0x7FFF,lvl*16); G.sp[b][L]=c; }
  if(rs>=16) spr_add(120,cy,2,SPT_SRAYS,b,spr_aff(rs,(int)(t>>4)&63),1,washed);   // the rays, on top
  if(!washed) spr_add(120,cy,2,SPT_SGLOW,b,spr_aff(gs,0),1,0);                    // the halo
}
static void rise_draw(u32 t){
  int k=G.sendoff, sad= k==E_SAD; const char *nm=pet_name(); char l1[40], l2[40], *p;
  const void *spr=CRITTER_SPR(PET[P_STAGE]);
  const Story *me= G.jr.n? &G.jr.s[G.jr.n-1] : 0; int col= me? story_colour(me) : C_GOLD;
  if(t<560){
    if(sad){
      for(int i=0;i<4;i++) draw_cloud((int)((i*71+(t>>2))%300u)-30,34+(i&1)*10);
      for(int i=0;i<14;i++){ int x=(int)((i*53+t*3)%250u)-5, y=(int)((i*37+t*7)%100u)+28; pset(x,y,C_PALEBLUE); pset(x-1,y+1,C_GRAY); } }
    else draw_cloud(40+(int)(t>>4)%200,40);
    int cx=120, y= t<220? 81 : 81-(int)((t-220)*72/340), sc= t<220? 3 : rise_sc(y), cy=y+8*sc;
    int g= t<200? (int)t/20 : 10, hc= sad? C_HINT : C_LTYEL, hc2= sad? C_SLATE : C_PALEYEL;
    if(t>=220){ int h=y+12*sc-28; if(h>0){   // the beam of starlight, see-through at its edges
        fill(cx-3,28,6,h,hc2); for(int yy=28;yy<28+h;yy++){ int o=yy&1; for(int x=cx-13+o;x<cx-3;x+=2) pset(x,yy,hc2); for(int x=cx+4-o;x<cx+14;x+=2) pset(x,yy,hc2); } } }
    if(g){ int R=8+g*2+((t>>4)&1);   // the glow: a see-through halo (every other pixel) that grows, and a ring of light
      for(int dy=-R;dy<=R;dy++){ int w=0; while((w+1)*(w+1)+dy*dy<=R*R) w++; int yy=cy+dy; if(yy<28 || yy>131) continue;
        for(int x=cx-w+((dy+cx-w)&1);x<=cx+w;x+=2) pset(x,yy,hc2); }
      dot_ring(cx,cy,R+3,hc,(int)(t>>3)); }
    sprite(spr, cx-8*sc, y, sc, 0, 99);
    star_sprites(t,k,col);
    int n= sad? 2 : 2+g/2;
    for(int i=0;i<n;i++){ int q=((int)t*2+i*23)&63; art(ART_SPARK,5,5,cx-24+((i*37+q)%48),cy+20-(q>>1),1,5); }
    if(t<220){ p=scpy(l1,nm); scpy(p, k==E_WONDER? " HAS GROWN UP SO WELL!" : k==E_GOOD? " IS ALL GROWN UP." : " LOOKS UP AT THE SKY...");
      sendoff_box(l1, sad? "ITS GLOW IS FAINT." : "IT STARTS TO GLOW..."); }
    else if(t<400){ if(sad) sendoff_box("IT DRIFTS UP, SLOW AND QUIET,","INTO THE NIGHT SKY...");
      else sendoff_box("A BEAM OF STARLIGHT COMES DOWN,","AND IT RISES INTO THE SKY..."); }
    return;
  }
  // the night sky: the critters before it, and its new star
  for(int i=0;i<9 && i+1<G.jr.n;i++){ const Story *o=&G.jr.s[G.jr.n-2-i]; jr_star(SKY_OLD[i][0],SKY_OLD[i][1],story_colour(o),story_kind(o),((G.sky_t+i*29)>>3)%10==0); }
  star_sprites(t,k,col);   // v8.2: its new star is a hardware sprite
  if(k==E_WONDER){ for(int i=0;i<4;i++){ int q=((int)t+i*16)&63; pset(120+SINE16[(i*4+2)&15]*(8+q/4)/8, 58+SINE16[(i*4+6)&15]*(8+q/4)/8, C_WHITE); } }
  if(t>=680 && t<780){
    O_BOX(10,108,220,44);
    p=scpy(l1,nm); scpy(p," ROSE TO BECOME A STAR,"); textc(l1,120,115,C_DTEXT);
    scpy(l2, k==E_WONDER? "THE BRIGHTEST ONE OVER THE MEADOW." : k==E_GOOD? "TWINKLING OVER THE MEADOW." : "A FAINT ONE, FAR AWAY...");
    textc(l2,120,126,C_DTEXT);
    textc(k==E_WONDER? "- A WONDERFUL FAREWELL -" : k==E_GOOD? "- A HAPPY FAREWELL -" : "- A SAD GOODBYE -",120,139, k==E_WONDER? C_GREEN : k==E_GOOD? C_ORANGE : C_GRAY);
  }
}
static void crit_draw(int f,int pose,int x,int y,int sc,int flip,int night);
// v8.2c: a critter whose health ran out leaves to find a home in the wild (it leaves no star). Six shots, each fading
// in from black: (0) dawn in its room: an empty bowl, and it packs its bindle; (1) sunrise in the yard: one look back
// at the house; (2) out along a path through the meadow, smaller and smaller; (3) a stream, and a family of wild
// critters by their burrow under the willow take it in; (4) that night, the place its star would have been stays
// empty; (5) the end card. New art: gen/meadow.py -> meadow.h (its own palette from MD_FIRST).
#include "meadow.h"
static void quick_box(int x,int y,int w,int h); static void build_bg(int want);
#define RA_T1 300
#define RA_T2 580
#define RA_T3 880
#define RA_T4 1220
#define RA_T5 1460
static int ra_shot(void){   // which shot is showing, or -1 when not in the runaway
  if(!(G.mode==M_NONE && SCENE==9) || !G.gone_bg) return -1;
  u32 t=FRAMES-G.sendoff_t; return t<RA_T1? 0 : t<RA_T2? 1 : t<RA_T3? 2 : t<RA_T4? 3 : t<RA_T5? 4 : 5; }
static u16 rgb_warm(u16 c){   // a touch of dawn: warmer, a little darker
  int r=c&31, g=(c>>5)&31, b=(c>>10)&31; r=r*7/8+4; g=g*13/16+1; b=b*5/8+1; if(r>31) r=31; return (u16)RGB(r,g,b); }
static void runaway_palette(int s){   // called from farewell_palette, before anything is drawn
  if(s==0){ for(int i=0;i<BGX_N;i++) PAL[BGX_FIRST+i]=rgb_warm(BGX_ROOM_DAYPAL[i]); decor_palettes(0);
    for(int i=208;i<244;i++) PAL[i]=rgb_warm(PAL[i]); }
  else if(s==1){ for(int i=0;i<BGX_N;i++) PAL[BGX_FIRST+i]=BGX_YARD_DAYPAL[i]; decor_palettes(0); sky_key(0); garden_palette(); }
  else if(s==2 || s==3){ for(int i=0;i<MD_N;i++) PAL[MD_FIRST+i]=MD_PAL[i]; }
}
static void ra_bg_md(const u8 *src){   // a meadow picture into the background, with the dark bars round it
  u8 *d=YARD_BG; for(int i=0;i<28*240;i++) d[i]=C_OUTLINE;
  for(int i=0;i<MD_H*MD_W;i++) d[28*240+i]=src[i];
  for(int i=(28+MD_H)*240;i<160*240;i++) d[i]=C_OUTLINE;
}
static void ra_box(const char *a,const char *b){   // the words, down in the bottom bar (the picture stays clear)
  quick_box(8,135,224,24); textc(a,120,138,C_DTEXT); if(b) textc(b,120,148,C_DTEXT); }
static void ra_bindle(int x,int y,int sc,int dir){   // a stick over the shoulder (x,y: the shoulder; dir: +1 leans right, -1 left), and a spotted bundle
  int L=6*sc; for(int i=0;i<L;i++) pset(x+i*dir,y-i,C_WOOD_DK);
  int bx=x+(L+1)*dir, by=y-L-1, r=sc+2; disc(bx,by,r,C_OUTLINE); disc(bx,by,r-1,C_RED);
  pset(bx-1,by-1,C_WHITE); pset(bx+1,by+1,C_WHITE); if(sc>2){ pset(bx+2,by-1,C_WHITE); pset(bx-2,by+1,C_WHITE); } pset(bx,by-r,C_OUTLINE); }
static void ra_crit(int pose,int fx,int feet,int sc,int flip){   // the critter by where its feet are
  int f=G.gone_form<N_FORMS? G.gone_form : 2; crit_draw(f,pose,fx-8*sc,feet-15*sc,sc,flip,0); }
static void ra_heart(int x,int y){ art(ART_HEART,7,6,x,y,1,7); }
static void runaway_draw(void){
  DRAW_TARGET=O_BACKBUF();
  if(!G.gone_bg){ G.gone_bg=1; G.sendoff_t=FRAMES; PET[P_LIGHTS]=0; G.fx_light=0; }   // (time it by frames: the original redraws this scene only now and then)
  u32 t=FRAMES-G.sendoff_t; if(t>4000) t=4000;
  int s=ra_shot();
  if(G.gone_t!=(u16)(s+1)){   // a new shot: build its background once, and fade in from black
    G.gone_t=(u16)(s+1); fx_fade(FX_BLACK,16,26); G.fx_hold=2;   // (black until both pages hold the new picture)
    if(s==0 || s==1){ build_bg(s==0? 1 : 3); u8 *d=YARD_BG; for(int i=0;i<28*240;i++) d[i]=C_OUTLINE; for(int i=134*240;i<160*240;i++) d[i]=C_OUTLINE;   // no icon bars: a cutscene
      if(s==1) sfx(0,SEQ_RUNAWAY); }
    else if(s==2) ra_bg_md(MD_BG_WALK);
    else if(s==3) ra_bg_md(MD_BG_STREAM);
    else if(s==4) backdrop_stars();
    else { u8 *d=YARD_BG; for(int i=0;i<160*240;i++) d[i]=C_OUTLINE; }
    G.bg_state=0;
  }
  O_DMACOPY(DRAW_TARGET, YARD_BG);
  sk_x0=0; sk_x1=239; sk_y0=28; sk_y1=95; sk_dx=0; sk_dy=0;
  const char *nm=pet_name(); char l1[40]; char *p;
  int walk=(int)((t>>3)&1);
  if(s==0){   // dawn in its room
    int u=(int)t;
    { int bx=150, by=122; ellipse(bx+10,by+3,11,4,C_OUTLINE); ellipse(bx+10,by+3,10,3,C_WHITE); ellipse(bx+10,by+3,7,2,C_GRAY); }   // its empty bowl
    ra_crit(u<150? POSE_SAD : POSE_IDLE, 104, 129, 3, 0);
    if(u>=150) ra_bindle(120,108,3,1);
    p=scpy(l1,nm); scpy(p," DIDN'T FEEL CARED FOR...");
    if(u<150) ra_box(l1,"ITS BOWL WAS EMPTY AGAIN.");
    else ra_box("SO AT DAWN, IT PACKED","ITS LITTLE BINDLE.");
  }
  else if(s==1){   // sunrise in the yard: from the door, one look back, then away
    int u=(int)t-RA_T1;
    draw_sun(92,58);
    for(int i=0;i<3;i++) draw_cloud((int)((i*90+(u>>2))%300u)-30,38+(i&1)*8);
    int x, flip=0, pose=walk? POSE_WALK1 : POSE_WALK2, bob=0;
    if(u<120){ x=196-u*3/4; bob=walk; }
    else if(u<200){ x=106; flip=1; pose=u<130? POSE_IDLE : POSE_SAD; }   // it turns and looks back at the house
    else { x=106-(u-200)*2; bob=walk; }
    if(x>-40){ ra_crit(pose,x,129-bob,3,flip); if(!flip) ra_bindle(x+12,108-bob,3,1); }
    ra_box("IT LOOKED BACK AT THE HOUSE","ONE LAST TIME...");
  }
  else if(s==2){   // along the path, up into the meadow, smaller and smaller
    int u=(int)t-RA_T2; if(u>280) u=280;
    int fy=104-u*44/280;                                     // its feet, in the picture's own rows (0 = screen row 28)
    int tt=(fy-52)*256/54, sc= tt>150? 3 : tt>70? 2 : 1;     // (tt: 0 far .. 256 near, as gen/meadow.py lays the path)
    int cx=128+((30*SIN64[(tt*131/1000+4)&63])>>8)-((70*tt*tt)>>16);   // the path's centre: 128 + 30 sin(3.3 tt + .4) - 70 tt^2
    int y=28+fy;
    ra_crit(walk? POSE_WALK1 : POSE_WALK2,cx,y-((t>>3)&1),sc,1);
    if(sc>=2) ra_bindle(cx-4*sc,y-11*sc,sc,-1);
    ra_box("IT WALKED FAR OUT","INTO THE MEADOW...");
  }
  else if(s==3){   // the stream: a wild one of its own kind, looking after two little Bliplets by the burrow; it stays
    int u=(int)t-RA_T3;
    // the wild grown-up (its own kind: the same art and colours), watching over the little ones
    ra_crit(u>200? (((u>>4)&1)? POSE_WAVE1 : POSE_WAVE2) : (((u>>5)&3)==1? POSE_BLINK : POSE_IDLE),186,121,3,0);
    // two Bliplets: shy behind the grown-up at first, then one hops over to say hello
    { int hop=(u>>3)&1, x1, f1, p1;
      if(u<150){ x1=156; f1=0; p1= ((u>>4)&3)==2? 1 : 0; }                              // peeking
      else if(u<210){ x1=156-(u-150)*2/3; f1=0; p1= hop? 3 : 4; }                         // hopping over
      else { x1=116; f1=0; p1= u>240? 5 : 2; }                                           // hello!
      spr_bliplet(x1,121-(u>=150 && u<210? hop*2 : 0),p1,f1);
      spr_bliplet(222,122-(u>230? (((u>>3)&1)? 2 : 0) : 0), u>230? 2 : (((u>>5)&1)? 1 : 0), 1); }
    int x= u<130? -26+u : 100, pose= u<130? (walk? POSE_WALK1 : POSE_WALK2) : u<210? POSE_IDLE : POSE_LOVE;
    ra_crit(pose,x,121-(u<130? walk : 0),3,1);
    if(u<130) ra_bindle(x-12,92-walk,3,-1);
    if(u>=215){ for(int i=0;i<3;i++){ int q=((u-215)*2+i*22)%60; ra_heart(98+i*22,84-q/2); } }
    if(u<215) ra_box("...WHERE A WILD ONE, JUST LIKE IT,","WAS LOOKING AFTER TWO LITTLE ONES.");
    else ra_box("IT STAYED, AND HELPED","TAKE CARE OF THEM.");
  }
  else if(s==4){   // that night: its star's place stays empty
    int u=(int)t-RA_T4;
    for(int i=0;i<9 && i+1<G.jr.n;i++){ const Story *o=&G.jr.s[G.jr.n-2-i]; jr_star(SKY_OLD[i][0],SKY_OLD[i][1],story_colour(o),story_kind(o),((G.sky_t+i*29)>>3)%10==0); }
    if(u<200){ int bk=spr_pal();   // a faint light where it would have shone: it flickers, dims and goes out
      if(bk>=0){ static const u8 RG[7]={0,14,32,56,90,135,190}; int br=(200-u)*190/200; if(((u>>3)&3)==0) br=br*2/3;
        for(int L=1;L<=6;L++) G.sp[bk][L]=rgb_scale(RGB(20,22,31),RG[L]*br>>8);
        spr_add(120,58,2,SPT_SGLOW,bk,spr_aff(120,0),1,0); } }
    ra_box("IT FOUND A NEW HOME,","BUT IT LEFT NO STAR BEHIND.");
  }
  else {   // the end card
    textc("- A SAD GOODBYE -",120,48,C_GRAY);
    textc("NOT EVERY CRITTER BECOMES A STAR.",120,66,C_LTEXT);
    textc("TAKE BETTER CARE NEXT TIME!",120,80,C_LTEXT);
    if((t>>4)&1) textc("PRESS START FOR A NEW SEED",120,116,C_GOLD);
  }
}
static void sendoff_draw(void){
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, YARD_BG);
  u32 t=G.sendoff_t; const char *nm=pet_name();
  char l1[40], *p;
  const void *spr=CRITTER_SPR(PET[P_STAGE]);
  int sp=G.so_special;
  sk_x0=0; sk_x1=239; sk_y0=28; sk_y1=95; sk_dx=0; sk_dy=0;
  if(so_unified()) rise_draw(t);
  else if(sp==SF_SUNPUFF){
    // sunrise: a sunbeam carries it up into the sun
    sky_key(0);
    int sy = t<200? 100-(int)t/4 : 50;
    draw_sun(120, sy);
    int y = t<240? 81 : 81 - (int)((t-240)*90/300); if(y<sy-8) y=sy-8;
    if(t>=200 && t<620){ for(int yy=sy+10; yy<132; yy++){ int w=(yy&2)? 12 : 16; fill(120-w/2,yy,w,1,(yy&2)? C_PALEYEL : C_GOLD); } }
    if(t<540){ int sc=rise_sc(y); sprite(spr, 120-8*sc, y, sc, 0, 99); }
    else if(t<620 && (t&4)) art(ART_SPARK,5,5,118,sy-2,2,5);
    for(int i=0;i<3;i++){ int k=((int)t+i*11)&31; art(ART_SPARK,5,5,104+i*14,sy+14+k,1,5); }
    three_boxes(t," BASKS IN THE","MORNING SUN...","A SUNBEAM CARRIES IT UP,","UP INTO THE SKY!","THANK YOU FOR ALL","THE SUNNY DAYS!");
  }
  else if(sp==SF_CHONKLE){
    // one last snack, then a bunch of balloons floats it away
    sky_key(1); draw_cloud(40+(int)(t>>3)%200,38); draw_cloud(160-(int)(t>>4)%160,52);
    static const u8 BAL0[8]={0,C_RED,C_PINK,C_RED2,0,C_OUTLINE,C_WHITE,C_RED2}, BAL1[8]={0,C_GOLD,C_LTYEL,C_ORANGE,0,C_OUTLINE,C_WHITE,C_ORANGE},
                    BAL2[8]={0,C_SKY,C_PALEBLUE,C_BLUE,0,C_OUTLINE,C_WHITE,C_ROYAL};
    static const u8 *const BCOL[3]={BAL0,BAL1,BAL2};
    int x=96, y=81;
    if(t>=240){ y=81-(int)((t-240)*150/360); x=96+(SINE16[(t>>3)&15]>>1); }
    int by = t<120? -30 : t<220? -30+(int)((t-120)*60/100) : y-30;
    if(t>=120){ for(int i=0;i<3;i++){ int bx=x+12+i*12, byy=by-(i==1? 8 : 0);
        for(int k=byy+6;k<y+4;k+=2) pset(x+24+((bx-x-24)*(y+4-k))/(y+4-byy),k,C_OUTLINE);
        mini_art(MA_BALLOON,bx-6,byy-6,BCOL[i],0); } }
    if(t<220){ fill(x+40,100,10,8,C_OUTLINE); fill(x+41,101,8,6,C_TAN); pset(x+43,102,C_WOOD_DK); pset(x+46,104,C_WOOD_DK); }
    if(y>-40){ int sc=rise_sc(y); if(y>=2) sprite(spr, x+24-8*sc, y+(3-sc)*12, sc, 0, 99); }
    if(t>=300 && t<420){ int cy=(int)(y+40+(t-300)); if(cy<130) fill(x+20,cy,2,2,C_TAN); }
    three_boxes(t," HAS ONE LAST","BIG, BIG SNACK...","THEN A BUNCH OF BALLOONS","FLOATS IT AWAY!","UP AND UP IT GOES,","ALL THE WAY TO THE STARS!");
  }
  else if(sp==SF_ZIPPIT){
    // zooms round and round, then streaks off across the night sky
    int cx=104, cy=74, sc=3;
    if(t<220){ cx+= (t&2)? 1 : -1; }
    else if(t<560){ u32 k=(t-220); int ph=(int)((k*(4+k/60))>>3)&15; int r=k<120? (int)k/2 : 60;
      cx=104+(SINE16[(ph+4)&15]*r)/8; cy=60+(SINE16[ph]*r)/16; sc=2;
      for(int j=1;j<=3;j++){ int ph2=(ph-j*2)&15; int tx=104+(SINE16[(ph2+4)&15]*r)/8+12, ty=60+(SINE16[ph2]*r)/16+12; art(ART_SPARK,5,5,tx,ty,1,5); } }
    else { u32 k=t-560; cx=104+(int)k*3; cy=60-(int)k*2; sc=1;
      for(int j=0;j<40;j+=2){ int tx=cx-j*3/2+8, ty=cy+j+8; if(tx>=0 && ty>=28 && ty<132) pset(tx,ty,(j&4)? C_GOLD : C_LTYEL); } }
    if(cx<236 && cy>2) sprite(spr, cx, cy, sc, 0, 99);
    three_boxes(t," CAN'T SIT STILL","TONIGHT...","IT ZOOMS AROUND AND AROUND,","FASTER AND FASTER...","AND STREAKS ACROSS THE SKY!","LOOK, A SHOOTING STAR!");
  }
  else if(sp==SF_GLIMMER){
    // glows brighter and brighter until it becomes a star
    int n = t<520? 2+(int)t/60 : 0;
    if(t<520 && !(t>=300 && (t&2))) sprite(spr, 96, 74, 3, 0, 99);
    for(int i=0;i<n && i<12;i++){ int k=((int)t*2+i*23)&63; art(ART_SPARK,5,5,90+((i*37+k)%60),66+((i*29)%56)-(k>>2),1,5); }
    if(t>=300){ int st = t<520? 1+(int)(t-300)/70 : (((t>>3)&1)? 4 : 3); int y = t<520? 92 : 92-(int)((t-520)/3); if(y<14) y=14;
      sprite(SPR_STAR, 120-(5*st)/2, y-(5*st)/2, st, 0, 99); }
    three_boxes(t," STARTS TO GLOW...",0,"BRIGHTER AND BRIGHTER,","UNTIL IT SHINES LIKE A STAR!","LOOK UP ON A CLEAR NIGHT -","IT'S TWINKLING AT YOU!");
  }
  else if(sp==SF_NESTLE){
    // builds a cozy nest high in the old tree and curls up in it
    sky_key(3);
    int twigs = t<100? 0 : t<220? (int)(t-100)/12 : 10;
    for(int i=0;i<twigs;i++){ int k=i*7; fill(8+(k%28),60+((i*3)%4),8,1,(i&1)? C_WOOD_DK : C_BROWN2); }
    int x, y, sc=3;
    if(t<220){ x=120; y=81; }
    else if(t<380){ x=120-(int)((t-220)*112/160); y=81; }
    else if(t<460){ int k=(int)t-380; x=8; y=81-k*47/80; sc= y>50? 3 : 2; }
    else { x=8; y=34; sc=2; }
    if(x<8) x=8;
    sprite(spr, x, y+((t>>4)&1&&t<380), sc, 0, 99);
    if(twigs>=10){ ellipse(24,64,15,4,C_OUTLINE); ellipse(24,64,14,3,C_BROWN2); for(int i=0;i<6;i++) fill(12+i*4,62+(i&1),4,1,C_WOOD_DK); }
    if(t>=480 && t<780){ int k=(int)(t>>2)&31; text("Z",44+(k>>3),40-k,C_WHITE); }
    three_boxes(t," GATHERS TWIGS","AND SOFT LEAVES...","IT BUILDS A COZY NEST","HIGH UP IN THE OLD TREE.","AND A NEW STAR SHINES","RIGHT ABOVE IT.");
  }
  if(t>=780){
    O_BOX(20,24,200,112);
    const char *ti = G.sendoff==E_WONDER? "A WONDERFUL FAREWELL" : G.sendoff==E_GOOD? "A HAPPY FAREWELL" : "A SAD GOODBYE";
    textbig(ti,120,30,G.sendoff==E_SAD? C_DTEXT : C_ORANGE,1);
    p=scpy(l1,nm); p=scpy(p," THE "); scpy(p,RAM_NAMES[10+(PET[P_STAGE]&7)]); textc(l1,120,44,C_RED);
    { int ag=P16(0x18); p=scpy(l1,"AGE: "); p=sint(p,ag); scpy(p, ag==1? " YEAR" : " YEARS"); textc(l1,120,56,C_DTEXT); }   // v8.1: in years, like the status card
    p=scpy(l1,"CARE: "); p=sint(p,care_pct()); scpy(p,"%"); textc(l1,120,66,C_DTEXT);
    if(G.so_bonus){ p=scpy(l1,"BONUS: +"); p=sint(p,G.so_bonus); scpy(p," COINS"); textc(l1,120,78,C_ORANGE); }
    else textc("NO BONUS COINS",120,78,C_DTEXT);
    if(G.sendoff==E_WONDER){ p=scpy(l1,"WONDERFUL IN A ROW: "); sint(p,G.life.retired); textc(l1,120,88,C_LILAC); }
    else if(G.sendoff==E_GOOD) textc("WONDERFUL NEEDS 80% CARE",120,88,C_LILAC);
    else textc("GIVE MORE CARE NEXT TIME...",120,88,C_LILAC);
    if(G.so_lost) textc("YOUR WONDERFUL RUN ENDS HERE.",120,98,C_RED);
    textc("EVERYTHING ELSE STARTS FRESH",120,110,C_HINT);
    if(t>=820 && ((t>>4)&1)) textc("A: NEW SEED",120,122,C_RED);
  }
}

// ================================================================== PAUSE MENU
static const char *const MENU_ITEMS[6]={"RESUME","SPEED:","SOUND:","MUSIC:","HOW TO PLAY","SAVE AND QUIT"};   // (v8.1: JOURNAL moved to SELECT)
static void enter_menu(void){ G.mode=M_MENU; G.cur = G.from_help? 4 : 0; G.from_help=0; G.confirm=0; }
static void dex_open(int from);
static void jr_garden_keys(u16 pr);
static void dex_update(u16 pr,u16 keys);
static void menu_update(u16 pr, u16 keys){
  if(G.confirm){
    if(pr&K_A){
      G.more.pad1=(u16)((G.more.pad1&~1)|(G.outside? 1 : 0)); save_all();   // (v8.2b: remember the yard)
      pet_save(); silence(); sfx(0,SEQ_CONFIRM);
      MSG_TIMER=0; TITLE_CUR=0; SCENE=0; G.mode=M_NONE; G.suppress=keys; G.confirm=0; G.outside=0;
    } else if(pr&K_B){ G.confirm=0; sfx(0,SEQ_BLIP); }
    return;
  }
  if(pr&K_UP){ G.cur=(u8)(G.cur? G.cur-1:5); sfx(0,SEQ_BLIP); }
  if(pr&K_DOWN){ G.cur=(u8)(G.cur>=5?0:G.cur+1); sfx(0,SEQ_BLIP); }
  if(pr&(K_B|K_START)){ sfx(0,SEQ_BLIP); to_room(keys); return; }
  if(pr&K_A){
    switch(G.cur){
      case 0: sfx(0,SEQ_BLIP); to_room(keys); break;
      case 1: PET[P_FAST]=(u8)!PET[P_FAST]; sfx(0,SEQ_BLIP); break;
      case 2: PET[P_SOUNDON]=(u8)!PET[P_SOUNDON]; if(!PET[P_SOUNDON]) silence(); else sfx(0,SEQ_BLIP); break;
      case 3: G.life.music_off=(u8)!G.life.music_off; life_save(); sfx(0,SEQ_BLIP); break;
      case 4: sfx(0,SEQ_BLIP); HELP_PAGE=0; G.help_page=0; SCENE=7; G.mode=M_NONE; G.suppress=keys; G.from_help=1; break;
      case 5: G.confirm=1; sfx(0,SEQ_BLIP); break;
    }
  }
}
static void menu_draw(void){
  O_BOX(52,34,136,92);
  textc("MENU",120,40,C_RED);
  for(int i=0;i<6;i++){
    int y=53+i*12;
    if(G.cur==i) text(">",62,y,C_RED);
    text(MENU_ITEMS[i],72,y,C_DTEXT);
    if(i==1) text(PET[P_FAST]?"FAST":"NORMAL",120,y,C_LILAC);
    if(i==2) text(PET[P_SOUNDON]?"ON":"OFF",120,y,C_LILAC);
    if(i==3) text(G.life.music_off?"OFF":"ON",120,y,C_LILAC);
  }
  if(G.confirm){
    O_BOX(30,56,180,50);
    textc("SAVE AND GO TO THE TITLE?",120,66,C_DTEXT);
    textc("A: YES    B: NO",120,86,C_RED);
  }
}

// ================================================================== SELECT MENU
static int can_go_outside(void){ return PET[P_STAGE]>=2 && PET[P_STAGE]<=7; }
static void enter_shop(void);
static void enter_bag(void);
static void story_open(void);
static int story_ok(void);
static int garden_ok(void);
static void garden_open(void);
static void garden_close(u16 keys);
static void garden_screen_draw(void);
static int extras_off(int i){   // greyed out
  if(G.ex_yard) return i==1 || i==2 || i==4;   // over the night yard: just GO INSIDE and JOURNAL
  return (i==0 && !G.outside && !can_go_outside() && !garden_ok()) || (i==4 && !story_ok());
}
static void extras_close(u16 keys){ G.suppress=keys; if(G.ex_yard){ G.ex_yard=0; G.mode=M_GARDEN; G.gui_key=0; } else G.mode=M_NONE; }
static void extras_update(u16 pr, u16 keys){
  if(pr&K_UP){ G.cur=(u8)(G.cur?G.cur-1:4); sfx(0,SEQ_BLIP); }
  if(pr&K_DOWN){ G.cur=(u8)(G.cur>=4?0:G.cur+1); sfx(0,SEQ_BLIP); }
  if(pr&(K_B|K_SELECT)){ sfx(0,SEQ_BLIP); extras_close(keys); return; }
  if(pr&K_A){
    G.suppress=keys;
    if(G.ex_yard){   // v8.1: the menu over the night yard
      if(G.cur==0){ G.ex_yard=0; G.mode=M_GARDEN; garden_close(keys); }
      else if(G.cur==3){ G.ex_yard=0; sfx(0,SEQ_JOURNAL); dex_open(2); }
      else sfx(0,SEQ_DENY);
      return; }
    if(G.cur==3){ sfx(0,SEQ_JOURNAL); dex_open(3); return; }   // v8.1: the journal lives here now
    if(G.cur==0){
      u8 st=PET[P_STAGE];
      G.mode=M_NONE;
      if(G.outside){ go_inside(0); set_msg("BACK INSIDE!"); }
      else if(garden_ok()) garden_open();
      else if(st==0){ set_msg("IT'S STILL AN EGG!"); sfx(0,SEQ_DENY); }
      else if(st<2){ set_msg("TOO LITTLE TO GO OUTSIDE!"); sfx(0,SEQ_DENY); }
      else if(PET[P_ASLEEP]){ set_msg("SHH... IT'S ASLEEP."); sfx(0,SEQ_DENY); }
      else if(PET[P_SICK]){ set_msg("TOO SICK TO GO OUTSIDE..."); sfx(0,SEQ_DENY); }
      else if(PET[P_LIGHTS]){ set_msg("TURN THE LIGHTS ON FIRST!"); sfx(0,SEQ_DENY); }
      else go_outside();
    } else if(G.cur==1) enter_shop();
    else if(G.cur==2) enter_bag();
    else story_open();
  }
}
static void extras_draw(void){
  O_BOX(136,30,96,100);
  const char *n0 = G.outside? "GO INSIDE" : garden_ok()? "GARDEN" : "OUTSIDE";
  const char *n[5]={n0,"SHOP","BAG","JOURNAL","STORY"};
  for(int i=0;i<5;i++){
    int y=38+i*14;
    if(G.cur==i) text(">",144,y,C_RED);
    text(n[i],154,y,extras_off(i)? (G.ex_yard? C_HINT : C_GRAY) : C_DTEXT);   // (C_GRAY is a garden colour at night)
  }
  coin_icon(146,110); sint(G.sbuf,G.ext.coins); text(G.sbuf,157,110,C_DTEXT);
}

// ================================================================== SHOP
#define SHOP_N 5
static void bars(void){ fill(0,0,240,26,C_BAR); fill(0,26,240,2,C_BORDER); fill(0,132,240,2,C_BORDER); fill(0,134,240,26,C_BAR); }
static int owned(int it){ return IS_TOY(it)? (G.ext.toys>>TOYBIT(it))&1 : IS_DECOR(it)? (int)((G.deco.owned>>DECOBIT(it))&1u) : 0; }
// ---- per-critter shop unlocks: a random order (cheaper pieces tend to come first; toys and yard
// decor lean later, since only grown-ups go outside), with more of it opened as the critter grows
static const u8 SHOP_POOL[33]={ IT_BALL,IT_KITE,IT_BUBBLES,IT_FRISBEE,IT_SPRINKLER,IT_PINWHEEL,
  IT_BED,IT_FISHBOWL,IT_PLANT,IT_RADIO,IT_CLOCK,IT_BOOKS,IT_TOYBOX,IT_TV,IT_PORTRAIT,IT_MOBILE,IT_LANTERN,
  IT_SWING,IT_SANDBOX,IT_BIRDBATH,IT_GARDEN,IT_BEANBAG,IT_PIANO,IT_ARCADE,IT_FIREPLACE,IT_MIRROR,IT_GUITAR,
  IT_DISCO,IT_PLANE,IT_TRAMPOLINE,IT_SLIDE,IT_POND,IT_FLOWERS };
static const u8 UNLOCK_N[7]={0,4,8,13,18,23,27};   // how many a shop had opened by each level (older saves)
// six of the 33 are left out of a critter's shop entirely (at most two of a kind), so no two critters' shops match
static void left_out(u8 *ex){
  u32 s=(G.deco.seed*2654435761u)^0x5A17C0DEu; if(!s) s=1;
  u8 grp[7]={0,0,0,0,0,0,0}; int n=0;
  for(int i=0;i<33;i++) ex[i]=0;
  while(n<6){
    s^=s<<13; s^=s>>17; s^=s<<5;
    int i=(int)((s>>8)%33u), it=SHOP_POOL[i], g= IS_TOY(it)? 6 : DECOR_SLOT(it);
    if(ex[i] || grp[g]>=2) continue;
    ex[i]=1; grp[g]++; n++;
  }
}
static void unlock_order(u8 *out){
  u32 s=G.deco.seed|1u; u16 key[33]; u8 ex[33]; left_out(ex);
  for(int i=0;i<33;i++){
    int it=SHOP_POOL[i]; s^=s<<13; s^=s>>17; s^=s<<5;
    int bias = (IS_TOY(it) || YARD_SLOT(DECOR_SLOT_OK(it)))? 60 : 0;
    u16 k= ex[i]? 0xFFFF : (u16)(ITEMS[it].price + bias + (int)((s>>8)%100u));
    int j=i; while(j>0 && key[j-1]>k){ key[j]=key[j-1]; out[j]=out[j-1]; j--; }
    key[j]=k; out[j]=(u8)it;
  }
}
// ---- v8 phase 5: each critter unlocks its own shop. Every growth level opens a few more pieces (cheaper ones tend to
// come first; toys and yard decor lean later) from the 27 that are not left out this life, so every critter's shop
// looks a little different. A new egg starts over. (The set is kept as bits in Keep.)
static const u8 UNLOCK_ADD[7]={0,4,4,5,5,5,4};
static int keep_has(int i){ return i<32? (int)((G.keep.un0>>i)&1u) : (G.keep.un1&1); }
static void keep_set(int i){ if(i<32) G.keep.un0|=1u<<i; else G.keep.un1|=1; }
static int unlocked(int it){ for(int i=0;i<33;i++) if(SHOP_POOL[i]==it) return keep_has(i); return 0; }
static int keep_count(void){ int n=0; for(int i=0;i<33;i++) n+=keep_has(i); return n; }
static int grant_level(int lv){   // opens this level's pieces; returns how many
  u32 s=(G.deco.seed*2654435761u + (u32)lv*0x9E3779B9u)|1u; u16 key[33]; u8 ex[33]; left_out(ex);
  for(int i=0;i<33;i++){ int it=SHOP_POOL[i]; s^=s<<13; s^=s>>17; s^=s<<5;
    int bias = (IS_TOY(it) || YARD_SLOT(DECOR_SLOT_OK(it)))? 60 : 0;
    key[i]= (keep_has(i) || ex[i])? 0xFFFF : (u16)(ITEMS[it].price + bias + (int)((s>>8)%100u)); }
  int n=0;
  for(int k=0;k<UNLOCK_ADD[lv];k++){ int b=-1; for(int i=0;i<33;i++) if(key[i]!=0xFFFF && (b<0 || key[i]<key[b])) b=i;
    if(b<0) break; keep_set(b); key[b]=0xFFFF; n++; }
  return n;
}
static void keep_from_v9(void){   // an older save: what this critter's shop had opened becomes the kept set
  G.keep.un0=0; G.keep.un1=0; G.keep.best=0; G.keep.ok=1;
  int lv=G.deco.unlock_lvl;
  if(G.deco.seed && lv>=1 && lv<=6){ u8 ord[33]; unlock_order(ord);
    for(int k=0;k<UNLOCK_N[lv];k++) for(int i=0;i<33;i++) if(SHOP_POOL[i]==ord[k]) keep_set(i); }
  for(int i=0;i<33;i++) if(owned(SHOP_POOL[i])) keep_set(i);   // anything already bought stays in reach
}
static int shop_level(void){
  u8 st=PET[P_STAGE];
  if(st<=1) return 1;
  if(st==2) return 2;
  if(st>7) return G.deco.unlock_lvl>=1 && G.deco.unlock_lvl<=6? G.deco.unlock_lvl : 1;
  int l=3+grown_days()/2; return l>6? 6 : l;   // (v8.2b: a level every 2 grown-up days, so all of them open in 8)
}
static void new_unlock_seed(void){ G.deco.seed=rnd32()^(PET_CLOCK*2654435761u)^FRAMES; if(!G.deco.seed) G.deco.seed=1; G.deco.unlock_lvl=0;
  G.keep.un0=0; G.keep.un1=0; G.keep.best=0; G.keep.ok=1; deco_save(); }   // a new egg: its own shop, its own best stack
// called every live frame: notices when the critter's growth opens up more of the shop
static void shop_level_watch(void){
  if(!G.deco.seed) new_unlock_seed();
  int lv=shop_level(), quiet=0;
  if(G.deco.unlock_lvl==0xFF){ G.deco.unlock_lvl=0; quiet=1; }   // set from the critter's current age without a message
  if(G.deco.unlock_lvl>6) G.deco.unlock_lvl=6;
  if(lv>G.deco.unlock_lvl){
    int news=0;
    for(int l=G.deco.unlock_lvl+1;l<=lv;l++) news+=grant_level(l);
    G.deco.unlock_lvl=(u8)lv; G.keep.ok=1; deco_save();
    if(news && lv>1 && !quiet) G.shop_news=1;
  }
  if(G.shop_news && SCENE==1 && G.mode==M_NONE && !MSG_TIMER && !PET[P_ASLEEP]){ G.shop_news=0; set_msg("NEW TOYS AND DECOR AT THE SHOP!"); sfx(0,SEQ_COIN); }
}
static void build_stock(void){
  u32 s = (u32)(G.ext.day+1)*2654435761u ^ 0xA5A5u;
  #define SR() (s^=s<<13, s^=s>>17, s^=s<<5, s)
  int a=(int)(SR()%N_FOODS), b; do { b=(int)(SR()%N_FOODS); } while(b==a);
  int c; do { c=(int)(SR()%(u32)N_CONSUMABLE); } while(c==a||c==b);
  G.stock[0]=(u8)a; G.stock[1]=(u8)b; G.stock[2]=(u8)c;
  int un[IT_COUNT], nu=0; for(int i=0;i<33;i++) if(keep_has(i) && !owned(SHOP_POOL[i])) un[nu++]=SHOP_POOL[i];
  int d=-1, e=-1;
  if(nu){ d=un[SR()%(u32)nu]; }
  if(nu>1){ do { e=un[SR()%(u32)nu]; } while(e==d); }
  if(d<0){ do { d=8+(int)(SR()%3u); } while(d==c); }
  if(e<0){ do { e=8+(int)(SR()%3u); } while(e==c||e==d); }
  if(d>e){ int t=d; d=e; e=t; }
  G.stock[3]=(u8)d; G.stock[4]=(u8)e;
  #undef SR
}
static void card_price(int it,int x,int y){
  if(owned(it)) textc("OWNED",x+21,y+38,C_DTEXT);
  else { sint(G.sbuf,ITEMS[it].price); int w=slen(G.sbuf)*6; coin_icon(x+21-(w+9)/2,y+38); text(G.sbuf,x+21-(w+9)/2+9,y+38,C_DTEXT); }
}
static void shop_static(void){
  u16 *save=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG;
  bars();
  fill(0,28,240,104,C_MINT);
  for(int x=6;x<240;x+=16) fill(x,28,4,104,C_PALEBLUE);
  for(int x=0;x<240;x+=20){ fill(x,28,10,8,C_RED2); fill(x+10,28,10,8,C_WHITE); }
  fill(0,36,240,1,C_OUTLINE);
  for(int x=0;x<240;x+=20){ disc(x+5,36,5,C_RED2); disc(x+15,36,5,C_WHITE); }
  fill(0,28,240,1,C_OUTLINE);
  fill(0,96,240,36,C_WOOD); fill(0,96,240,3,C_WOOD3); fill(0,99,240,1,C_WOOD_DK);
  for(int x=0;x<240;x+=40) fill(x,100,1,32,C_WOOD_DK);
  texts("SHOP",10,8,C_ORANGE);
  char *p=scpy(G.sbuf,"DAY "); sint(p,G.ext.day+1); textc(G.sbuf,120,8,C_HINT);
  textc("LEFT/RIGHT: PICK   A: BUY   B: LEAVE",120,143,C_HINT);
  for(int i=0;i<SHOP_N;i++){
    int it=G.stock[i], x=6+i*46, y=42;
    O_BOX(x,y,42,50);
    icon_draw(it,x+21,y+22);
    card_price(it,x,y);
  }
  O_BOX(8,98,224,32);
  DRAW_TARGET=save;
}
static void enter_shop(void){ G.mode=M_SHOP; G.cur=0; G.t=0; build_stock(); shop_static(); sfx(0,SEQ_DOOR); }
static void shop_update(u16 pr, u16 keys){
  G.t++; if(MSG_TIMER) MSG_TIMER--;
  if(pr&K_LEFT){ G.cur=(u8)(G.cur?G.cur-1:SHOP_N-1); sfx(0,SEQ_BLIP); }
  if(pr&K_RIGHT){ G.cur=(u8)(G.cur==SHOP_N-1?0:G.cur+1); sfx(0,SEQ_BLIP); }
  if(pr&K_B){ MSG_TIMER=0; sfx(0,SEQ_DOOR); to_room(keys); return; }
  if(pr&K_A){
    int it=G.stock[G.cur]; const Item *I=&ITEMS[it];
    if(owned(it)){ set_msg("YOU ALREADY HAVE THAT!"); sfx(0,SEQ_DENY); }
    else if(it<N_CONSUMABLE && G.ext.inv[it]>=9){ set_msg("YOUR BAG IS FULL OF THOSE!"); sfx(0,SEQ_DENY); }
    else if(G.ext.coins < I->price){ set_msg("NOT ENOUGH COINS!"); sfx(0,SEQ_DENY); }
    else {
      G.ext.coins=(u16)(G.ext.coins-I->price);
      if(IS_TOY(it)){ G.ext.toys|=(u8)(1<<TOYBIT(it)); if(G.ext.active_toy==0xFF) G.ext.active_toy=(u8)it; }
      else if(IS_DECOR(it)){ G.deco.owned|=1u<<DECOBIT(it); if(it==IT_PLANT) G.deco.plant_day=(u8)G.ext.day; int s=DECOR_SLOT(it); if(G.deco.slot[s]==0xFF) G.deco.slot[s]=(u8)it; deco_save(); }
      else G.ext.inv[it]++;
      found_set(it);
      ext_save(); shop_static(); G.bg_state=0;
      char *p=scpy(G.sbuf2,"BOUGHT "); p=scpy(p,I->name); scpy(p,"!"); set_msg(G.sbuf2);
      sfx(1,SEQ_COIN);
    }
  }
}
static void shop_draw(void){
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG);
  coin_icon(180,8); sint(G.sbuf,G.ext.coins); texts(G.sbuf,190,8,C_LTEXT);
  { int i=G.cur, it=G.stock[i], x=6+i*46, y=42;
    fill(x+3,y+3,36,43,C_ORANGE);
    int bob = SINE16[(G.t>>2)&15]>>2;
    icon_draw(it,x+21,y+22+bob);
    card_price(it,x,y); }
  int it=G.stock[G.cur];
  text(ITEMS[it].name,16,104,C_RED);
  { int k=ITEM_KIND[it]; text(KIND_NAME[k],16+(slen(ITEMS[it].name)+1)*6,104,C_LILAC); }   // v8: its kind of care
  if(it<N_CONSUMABLE){ char *q=scpy(G.sbuf,"HAVE: "); sint(q,G.ext.inv[it]); text(G.sbuf,224-slen(G.sbuf)*6,104,C_LILAC); }
  else if(owned(it)) text("OWNED",224-5*6,104,C_LILAC);
  text(ITEMS[it].desc,16,116,C_DTEXT);
  if(MSG_TIMER) label((const char*)MSG_BUF);
}

// ================================================================== BAG
static void build_bag(void){
  G.bag_n=0;
  for(int i=0;i<N_CONSUMABLE;i++) if(G.ext.inv[i]) G.bag_list[G.bag_n++]=(u8)i;
  for(int i=0;i<6;i++) if(owned(SHOP_POOL[i])) G.bag_list[G.bag_n++]=SHOP_POOL[i];          // toys
  for(int sl=0;sl<N_SLOTS;sl++) for(int i=6;i<33;i++){ int it=SHOP_POOL[i]; if(DECOR_SLOT(it)==sl && owned(it)) G.bag_list[G.bag_n++]=(u8)it; }
  if(G.cur>=G.bag_n) G.cur=G.bag_n?G.bag_n-1:0;
}
static void enter_bag(void){ G.mode=M_BAG; G.cur=0; G.bag_top=0; build_bag(); sfx(0,SEQ_BLIP); }
static void apply_item(int it){
  const Item *I=&ITEMS[it];
  PET[P_HUNGER]=(u8)clampi(PET[P_HUNGER]+I->hunger,0,4);
  PET[P_HAPPY]=(u8)clampi(PET[P_HAPPY]+I->happy,0,4);
  PET[P_BOND]=(u8)clampi(PET[P_BOND]+I->bond,0,4);
  PET[P_WEIGHT]=(u8)clampi(PET[P_WEIGHT]+I->weight,min_weight(),99);
}
static const char *bag_status(int it){
  if(IS_TOY(it)) return G.ext.active_toy==it? "IN USE" : "TOY";
  if(IS_DECOR(it)) return G.deco.slot[DECOR_SLOT(it)]==it? "PLACED" : "DECOR";
  return 0;
}
static void bag_update(u16 pr, u16 keys){
  G.t++;
  if(pr&K_B){ sfx(0,SEQ_BLIP); to_room(keys); return; }
  if(!G.bag_n) return;
  if(pr&K_UP){ G.cur=(u8)(G.cur?G.cur-1:G.bag_n-1); sfx(0,SEQ_BLIP); }
  if(pr&K_DOWN){ G.cur=(u8)(G.cur+1>=G.bag_n?0:G.cur+1); sfx(0,SEQ_BLIP); }
  if(G.cur<G.bag_top) G.bag_top=G.cur; if(G.cur>=G.bag_top+5) G.bag_top=(u8)(G.cur-4);
  if(pr&K_A){
    int it=G.bag_list[G.cur]; const Item *I=&ITEMS[it];
    if(IS_TOY(it)){
      char *p;
      if(G.ext.active_toy==it){ G.ext.active_toy=0xFF; p=scpy(G.sbuf2,"PUT THE "); p=scpy(p,I->name); scpy(p," AWAY."); }
      else { G.ext.active_toy=(u8)it; toys_reset(150,60);
        p=scpy(G.sbuf2,"TOOK OUT THE "); p=scpy(p,I->name); scpy(p,G.outside? "!" : " (YARD TOY)"); }
      ext_save(); set_msg(G.sbuf2); sfx(0,SEQ_CONFIRM); to_room(keys); return;
    }
    if(IS_DECOR(it)){
      int s=DECOR_SLOT(it); char *p;
      if(G.deco.slot[s]==it){ G.deco.slot[s]=0xFF; p=scpy(G.sbuf2,"PUT AWAY THE "); scpy(p,I->name); }
      else { G.deco.slot[s]=(u8)it;
        p=scpy(G.sbuf2,"PLACED THE "); p=scpy(p,I->name); scpy(p, YARD_SLOT(s)&&!G.outside? " IN THE YARD!" : "!"); }
      ext_save(); deco_save(); G.bg_state=0; set_msg(G.sbuf2); sfx(0,SEQ_CONFIRM); to_room(keys); return;
    }
    const char *deny=0;
    if(PET[P_STAGE]==0) deny="IT'S STILL AN EGG!";
    else if(PET[P_ASLEEP]) deny="SHH... IT'S ASLEEP.";
    else if(it==IT_VITAMIN && !PET[P_SICK] && G.deco.vit_day==(u8)(G.ext.day+1)) deny="IT'S PROTECTED FOR TODAY!";
    else if(it==IT_SOAP && !MESS_HERE) deny="IT'S ALREADY CLEAN!";
    else if(it<N_FOODS && I->hunger && !I->happy && PET[P_HUNGER]>=4) deny="IT'S FULL!";
    if(deny){ set_msg(deny); sfx(0,SEQ_DENY); to_room(keys); return; }
    G.ext.inv[it]--; ext_save();
    if(it<N_FOODS){ G.mode=M_EAT; G.eat_item=(u8)it; G.t=0; SCENE=1; G.suppress=keys; sfx(0,SEQ_CONFIRM); return; }
    if(it==IT_VITAMIN){ int was=PET[P_SICK]; PET[P_SICK]=0; G.deco.vit_day=(u8)(G.ext.day+1); deco_save();
      set_msg(was? "ALL BETTER - AND PROTECTED TODAY!" : "PROTECTED FROM SICKNESS TODAY!"); sfx(1,SEQ_HAPPY); }
    else if(it==IT_SOAP){ MESS_HERE=0; G.poop_hold=0; set_msg_name(" LOVES ITS BUBBLE BATH!"); sfx(1,SEQ_HAPPY); }   // (v8.1: just clean)
    else { apply_item(it); set_msg_name(" LOVES THE GIFT!"); sfx(0,SEQ_FANFARE); }
    to_room(keys);
  }
}
static void bag_draw(void){
  int rows = G.bag_n>5? 5 : G.bag_n; if(rows<2) rows=2;
  O_BOX(24,29,192,36+rows*12);
  text("BAG",34,35,C_RED);
  coin_icon(166,35); sint(G.sbuf,G.ext.coins); text(G.sbuf,176,35,C_DTEXT);
  if(!G.bag_n){ textc("NOTHING HERE YET!",120,50,C_DTEXT); textc("BUY THINGS AT THE SHOP.",120,62,C_LILAC); return; }
  for(int r=0;r<5 && G.bag_top+r<G.bag_n;r++){
    int idx=G.bag_top+r, it=G.bag_list[idx], y=46+r*12;
    if(idx==G.cur) text(">",32,y+2,C_RED);
    item_art(it,40,y,1);
    text(ITEMS[it].name,54,y+2,C_DTEXT);
    const char *st=bag_status(it);
    if(st) text(st,202-slen(st)*6,y+2,C_LILAC);
    else { char *p=scpy(G.sbuf,"X"); sint(p,G.ext.inv[it]); text(G.sbuf,202-slen(G.sbuf)*6,y+2,C_LILAC); }
  }
  if(G.bag_top>0) text("^",206,48,C_LILAC);
  if(G.bag_top+5<G.bag_n) text("V",206,96,C_LILAC);
  int it=G.bag_list[G.cur];
  const char *d = IS_TOY(it)? (G.ext.active_toy==it? "A: PUT IT AWAY" : "A: TAKE IT TO THE YARD")
                : IS_DECOR(it)? (G.deco.slot[DECOR_SLOT(it)]==it? "A: PUT IT AWAY" : YARD_SLOT(DECOR_SLOT(it))? "A: PLACE IT IN THE YARD" : "A: PLACE IT IN THE ROOM")
                : ITEMS[it].desc;
  if(!IS_TOY(it) && !IS_DECOR(it)){ char *p=scpy(G.sbuf2,KIND_NAME[ITEM_KIND[it]]); p=scpy(p,": "); scpy(p,d); d=G.sbuf2; }   // v8: its kind of care
  textc(d,120,50+rows*12,C_DTEXT);
}
static void eat_update(u16 keys){
  G.t++;
  live_update(0);
  if(SCENE!=1 || G.t>=84){
    apply_item(G.eat_item);
    PET[P_CALL]=0;
    const char *m[4]={"YUM!","DELICIOUS!","MUNCH MUNCH!","TASTY!"}; set_msg(m[rnd(4)]); sfx(0,SEQ_HAPPY);
    G.mode=M_NONE; if(SCENE==1) G.suppress=keys;
  }
}
// v8 phase 5 fixes: food is drawn with its own 24 colours (palette 40-63, unused by the game), dimmed with the room
// v8.2b: in the yard the moonflowers keep 40-63, so the food borrows the wall picture's 96-119 (no wall outside)
static int food_base(void){ return G.outside? C_PIC : FOOD_PAL0; }
static void food_palette(int f){
  int dim = PAL[C_WALL]!=RGB(30,27,22);
  for(int i=0;i<24;i++){ u16 c=FOOD_PAL[f][i];
    if(dim){ int r=c&31, g=(c>>5)&31, b=(c>>10)&31; r=r*5/8+1; g=g*5/8+1; b=b*5/8+3; if(b>31) b=31; c=(u16)(r|(g<<5)|(b<<10)); }
    PAL[food_base()+i]=c; }
}
// keep10: how much is left (10 = all); the bites come off the side facing the critter, with a scalloped edge
static void food_draw(int f,int x,int y,int keep10,int bite_left){
  const Icon *I=&FOOD_ART[f]; int keep=I->w*keep10/10;
  static const s8 BITE[6]={0,1,2,2,1,0};
  food_palette(f);
  const u8 *pix=FOOD_PIX+I->pix, *rows=FOOD_ROWS+2*I->row;
  for(int r=0;r<I->h;r++, pix+=I->w){ int a=rows[2*r], z=rows[2*r+1]; if(a==255) continue;
    int nb= keep10<10? BITE[r%6] : 0;
    int lo= bite_left? I->w-keep+nb : 0, hi= bite_left? I->w-1 : keep-1-nb;
    if(a<lo) a=lo; if(z>hi) z=hi;
    for(int c=a;c<=z;c++) if(pix[c]) pset(x+c,y+r,food_base()-1+pix[c]); }
}
static void eat_draw(void){
  int left = G.t<28? 10 : G.t<56? 7 : 4;
  int x = G.room_cx<140? G.room_cx+56 : G.room_cx-24, y = G.room_cy+42;
  if(!G.room_cy){ x=146; y=106; }
  if(G.eat_item<11){ const Icon *I=&FOOD_ART[G.eat_item]; food_draw(G.eat_item, x+10-I->w/2, y+10-I->h/2, left, x>G.room_cx); }
  else art(ITEMS[G.eat_item].art,10,10,x,y,2,left);
}
// the FEED icon's MEAL and SNACK: the original's 8x8 art is blanked (patch.py) and this draws in its place
static void feed_draw(void){
  int f= ANIM_KIND==1? FOOD_SNACK : FOOD_MEAL, q=(int)(ANIM_T/44u);
  int keep= q==0? 10 : q==1? 7 : q==2? 5 : 3;
  const Icon *I=&FOOD_ART[f];
  food_draw(f, G.room_cx+80-I->w/2, G.room_cy+63-I->h, keep, 1);
}
const char age_year[6]=" YEAR", age_years[7]=" YEARS";   // v8.1: the status card gives its age in years (patch.py)
const char blank_food_art[64]="................................................................";   // (patch.py)

// ================================================================== MINIGAMES
// v8 phase 5: the PLAY icon opens a picker. Four games, each tied to a kind of care: PANCAKE TOWER (food), HOP HOP and
// LEFT OR RIGHT? (play) and STARGAZER (rest). BERRY CATCH retired. (BUBBLE BATH and DUST BUNNIES were tried for clean
// and dropped after playtests: CLEAN care has the CLEAN icon, SOAP and clean decor.)
enum { G_PANCAKE, G_HOP, G_GUESS, G_STARS, N_GAMES };
enum { PH_PICK, PH_READY, PH_PLAY, PH_RESULT };
static const char *const GAME_NAMES[N_GAMES]={"PANCAKE TOWER","HOP HOP","LEFT OR RIGHT?","STARGAZER"};
static const char *const GAME_HINTS[N_GAMES]={"A: DROP!   B: QUIT","A: JUMP!   B: QUIT","LEFT/RIGHT: GUESS!   B: QUIT",
  "D-PAD: THAT STAR!   B: QUIT"};
static const u8 GAME_KIND[N_GAMES]={KF,KP,KP,KR};
static const char *const GAME_BOOST[N_GAMES]={"NICE OR BETTER: +1 HUNGER","EVERY ROUND: -1 WEIGHT","NICE OR BETTER: +1 MORE HAPPY",
  "NICE OR BETTER: HEALS IT"};
static const char *const GAME_SCORE[N_GAMES]={"STACK: ","SCORE: ","SCORE: ","SONGS: "};
static const u8 GAME_TUNE[N_GAMES]={T_MINI_CATCH,T_MINI_HOP,T_MINI_GUESS,T_MINI_STARS};
#define GAME_FRAMES 600   // 10 seconds
static void build_bg(int want);
static int decor_has_portrait(void){ for(int i=0;i<4;i++) if(G.deco.slot[i]==IT_PORTRAIT) return 1; return 0; }
// the PORTRAIT is part of the room background, so redraw it when the critter changes form
static void portrait_check(void){ if(!G.outside && G.pstage!=PET[P_STAGE] && decor_has_portrait()) G.bg_state=0; }
// ---- STARGAZER: four stars, one per D-pad direction, under a night sky (whatever the time)
static const u8 STAR_X[4]={120,70,170,120}, STAR_Y[4]={56,78,78,100};       // UP, LEFT, RIGHT, DOWN
static const Note STAR_NOTE[4][2]={{{784,16,0},{0,0,0}},{{523,16,0},{0,0,0}},{{1047,16,0},{0,0,0}},{{659,16,0},{0,0,0}}};
static const u8 COL_LIT[8]  ={0,C_SUN,C_STAR,C_SUN2,C_GREEN,C_OUTLINE,C_STAR,C_SUN2};
static const u8 COL_UNLIT[8]={0,C_HINT,C_LTEXT,C_BAR,C_GREEN,C_OUTLINE,C_LTEXT,C_BAR};
static void stars_sky(void){
  fill(0,28,240,104,C_BORDER);
  for(int y=62;y<112;y++) for(int x=0;x<240;x++){ int k=(y-62)>>3; if(k>=4 || ((x+(y&1)*2+((y>>1)&1))&3)<k) pset(x,y,C_DTEXT); }
  u32 sd=4242; for(int i=0;i<46;i++){ sd=sd*1103515245u+12345u; int x=(int)((sd>>16)%236u)+2, y=30+(int)((sd>>8)%70u); pset(x,y,(i%5)? C_HINT : C_LTEXT); }
  disc(210,44,8,C_MOON); disc(214,41,7,C_BORDER);
  for(int k=0;k<4;k++){ static const u8 A[4]={0,1,3,2}, B[4]={1,3,2,0}; int x0=STAR_X[A[k]], y0=STAR_Y[A[k]], x1=STAR_X[B[k]], y1=STAR_Y[B[k]];
    for(int t=4;t<28;t+=3) pset(x0+(x1-x0)*t/32, y0+(y1-y0)*t/32, C_HINT); }
  for(int x=0;x<240;x++){ int a=104+(x-40)*(x-40)/300, b=110+(x-185)*(x-185)/500, top= a<b? a : b; if(top>131) top=131;
    fill(x,top,1,132-top,C_NGRASS); pset(x,top,C_DEEPGREEN); }
}
// ---- PANCAKE TOWER: the plate and table under the stack
#define PK_TOP 15
#define PK_SLIDE_Y 31
static int pk_y(int i){ return 114-5*i; }
static void pk_table(void){
  fill(52,124,136,8,C_OUTLINE); fill(53,125,134,7,C_WOOD); fill(53,125,134,1,C_WOOD3);
  fill(66,119,108,5,C_OUTLINE); fill(67,120,106,3,C_WHITE); fill(69,122,102,1,C_GRAY);
  fill(186,102,13,22,C_OUTLINE); fill(187,103,11,21,C_BROWN2); fill(187,109,11,6,C_PALEYEL2); fill(189,97,7,6,C_OUTLINE); fill(190,98,5,5,C_WHITE);   // syrup
}
static const char *const PICK_HINT="UP/DOWN: PICK   A: PLAY   B: BACK";
static void prerender_mini(void){
  u16 *save=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG;
  int want = G.outside? 3+is_night() : (is_night()? 2 : 1);
  portrait_check();
  if(G.bg_state!=want){ DRAW_TARGET=save; build_bg(want); DRAW_TARGET=(u16*)SCRATCH_BG; }
  O_DMACOPY(SCRATCH_BG, YARD_BG);
  // v8.1: while a game is on, the room or yard behind it is faded (every other pixel dark), so the game stands out.
  // Drawn once here, before the game's own pieces; Stargazer has its own night sky
  if(G.phase!=PH_PICK && G.game!=G_STARS)
    for(int y=28;y<134;y++){ volatile u16 *row=(volatile u16*)SCRATCH_BG+y*120; u16 m=(y&1)? 0xFF00 : 0x00FF, d=(u16)((y&1)? (C_OUTLINE<<8) : C_OUTLINE);
      for(int x=0;x<120;x++) row[x]=(u16)((row[x]&~m)|d); }
  if(G.phase!=PH_PICK && G.game==G_STARS) stars_sky();
  if(G.phase!=PH_PICK && G.game==G_PANCAKE) pk_table();
  bars();
  textc(G.phase==PH_PICK? PICK_HINT : GAME_HINTS[G.game],120,143,C_HINT);
  DRAW_TARGET=save;
}
static void pk_start(void);
static void st_start(void);
static void enter_minigame(void){
  G.mode=M_MINI; G.phase=PH_PICK; G.t=0; MSG_TIMER=0;
  G.pick=(u8)(G.ext.last_game<N_GAMES? G.ext.last_game : 0);
  prerender_mini();
}
static void start_game(int g){
  G.game=(u8)g; G.ext.last_game=(u8)g;
  G.phase=PH_READY; G.t=0; G.score=0; G.timer=GAME_FRAMES; G.stun=0;
  G.px=(g==G_HOP)?36:104; G.py=0; G.pvy=0; G.spawn=30; G.round=0; G.guess=0; G.look=0; G.reveal=0;
  G.lastv = g==G_HOP? 500 : 240; G.lastx=120;
  for(int i=0;i<10;i++) G.obj[i].active=0;
  if(g==G_PANCAKE) pk_start();
  if(g==G_STARS) st_start();
  MSG_TIMER=0;
  prerender_mini();
}
static Obj *new_obj(void){ for(int i=0;i<10;i++) if(!G.obj[i].active){ G.obj[i].active=1; return &G.obj[i]; } return 0; }
// coins go by the round's rating, the same in every game (Phase 7): BETTER LUCK 2, NICE 8, AMAZING 15
#define HOP_N 12   // HOP HOP: hurdles and berries in a round - clear them all for a perfect score
// v8.1: coins by score, set per game (the player's tables), and the result each score gets:
// 0 BETTER LUCK, 1 NICE!, 2 AMAZING!, 3 PERFECT! (the top score)
static const u8 COIN_PANCAKE[15]={0,0,0,0,3,3,3,3,6,6,6,10,10,10,15};
static const u8 COIN_HOP[13]   ={0,0,0,3,3,3,6,6,6,12,12,12,15};
static const u8 COIN_GUESS[6]  ={0,2,3,5,7,10};      // a luck game: one lucky guess still pays 2
static const u8 COIN_STARS[6]  ={0,0,4,8,12,15};
static const u8 RES_PANCAKE[15]={0,0,0,0,1,1,1,1,1,1,1,2,2,2,3};
static const u8 RES_HOP[13]    ={0,0,0,1,1,1,1,1,1,2,2,2,3};
static const u8 RES_FIVE[6]    ={0,0,1,1,2,3};       // LEFT OR RIGHT? and STARGAZER
static const u8 GAME_TOP[N_GAMES]={14,HOP_N,5,5};
static int score_clamped(void){ int s=G.score, m=GAME_TOP[G.game]; return s<0? 0 : s>m? m : s; }
static int reward_coins(void){ int s=score_clamped();
  switch(G.game){ case G_PANCAKE: return COIN_PANCAKE[s]; case G_HOP: return COIN_HOP[s]; case G_GUESS: return COIN_GUESS[s]; default: return COIN_STARS[s]; } }
static int score_result(void){ int s=score_clamped(); return G.game==G_PANCAKE? RES_PANCAKE[s] : G.game==G_HOP? RES_HOP[s] : RES_FIVE[s]; }
static void mini_finish(void){
  int r=score_result();
  G.rating=(u8)r; G.phase=PH_RESULT; G.t=0;
  care_play(4); if(G.jr.games<65535) G.jr.games++; save_all();   // a minigame counts as a lot of play
  sfx(0, r>=2? SEQ_FANFARE : r==1? SEQ_HAPPY : SEQ_DENY);
}
static void mini_apply(u16 keys){
  int r=G.rating, g=G.game;
  if(g==G_HOP) PET[P_WEIGHT]=(u8)clampi(PET[P_WEIGHT]-1,min_weight(),99);
  if(r>=1) PET[P_HAPPY]=(u8)clampi(PET[P_HAPPY]+(r>2? 2 : r)+(g==G_GUESS),0,4);
  if(r>=2){ PET[P_BOND]=(u8)clampi(PET[P_BOND]+1,0,4); if(G.ext.stars<255) G.ext.stars++; }
  if(r>=1){   // what a good round does for its kind of care
    if(g==G_PANCAKE && PET[P_HUNGER]<4) PET[P_HUNGER]++;
    if(g==G_STARS){ u16 h=P16(0x16); P16(0x16)=(u16)(h+20>100? 100 : h+20); }
  }
  if(g==G_PANCAKE && G.score>G.keep.best) G.keep.best=(u8)G.score;
  if(r>=1 && care_win()==&G.care.adult) cw_add(&G.care.adult.good,2);
  add_coins(reward_coins());   // (saves)
  set_msg(r==3? "PERFECT! WHAT A STAR!" : r==2? "WHAT A STAR!" : r==0? "BETTER LUCK NEXT TIME" : g==G_PANCAKE? "YUM! THAT HIT THE SPOT!" :
          g==G_STARS? "WHAT A RESTFUL TUNE!" : "THAT WAS FUN!");
  G.last_happy=PET[P_HAPPY]; G.cry_delay=40;
  to_room(keys);
}
// ---- PANCAKE TOWER: a pancake slides back and forth over the stack; A drops it. Whatever hangs over the edge falls
// off (the critter eats what lands on its side) and the next pancake is only as wide as what stayed on.
static void pk_layer(int x,int y,int w){
  if(w<=0) return;
  if(w<3){ fill(x,y,w,5,C_OUTLINE); return; }
  fill(x+1,y,w-2,1,C_OUTLINE); fill(x+1,y+4,w-2,1,C_OUTLINE); fill(x,y+1,1,3,C_OUTLINE); fill(x+w-1,y+1,1,3,C_OUTLINE);
  fill(x+1,y+1,w-2,1,C_TAN); fill(x+1,y+2,w-2,1,C_LTYEL); fill(x+1,y+3,w-2,1,C_TAN);   // browned, fluffy, browned
  if(w>6){ pset(x+2,y+1,C_ORANGE); pset(x+w-3,y+3,C_BROWN2); }
}
static void pk_slider(void){   // the next one starts at a side, a little faster for every pancake on the stack
  int n=G.round, w=G.guess, v=256+36*(n-1);
  if(n&1){ G.px=(204-w)<<8; G.lastv=-v; } else { G.px=36<<8; G.lastv=v; }
  G.spawn=0;
}
static void pk_start(void){ G.pk_x[0]=96; G.pk_w[0]=48; G.round=1; G.guess=48; G.pk_eat=0; G.pk_land=0; pk_slider(); }
static void pk_chunk(int x,int w,int y,int dir){ Obj *o=new_obj(); if(!o) return; o->x=x<<8; o->y=y<<8; o->vx=dir*300; o->vy=-400; o->type=(u8)w; }
static void pancake_play(u16 pr){
  if(G.look) G.look--;
  if(G.pk_eat) G.pk_eat--;
  for(int i=0;i<10;i++){ Obj *o=&G.obj[i]; if(!o->active) continue;
    o->x+=o->vx; o->y+=o->vy; o->vy+=26; int x=o->x>>8, y=o->y>>8;
    if(x+o->type/2<46 && y>=100){ o->active=0; G.pk_eat=40; sfx(1,SEQ_CATCH); }   // into a happy mouth
    else if(y>134 || x>240 || x+o->type<0) o->active=0; }
  if(G.spawn==3){ if(--G.reveal<=0) mini_finish(); return; }
  if(G.spawn==0){
    G.px+=G.lastv; int x=G.px>>8, w=G.guess;
    if(x<36){ G.px=36<<8; G.lastv=-G.lastv; }
    if(x>204-w){ G.px=(204-w)<<8; G.lastv=-G.lastv; }
    if(pr&(K_A|K_DOWN)){ G.spawn=1; G.py=PK_SLIDE_Y<<8; G.pvy=256; sfx(0,SEQ_TICK); }
    return;
  }
  G.py+=G.pvy; G.pvy+=40;
  int n=G.round, land=pk_y(n);
  if((G.py>>8)<land) return;
  int x=G.px>>8, w=G.guess, tx=G.pk_x[n-1], tw=G.pk_w[n-1];
  int a= x>tx? x : tx, b= x+w<tx+tw? x+w : tx+tw;
  if(b-a<=0){ pk_chunk(x,w,land,x<tx? -1 : 1); G.spawn=3; G.reveal=70; G.pk_land=1; sfx(1,SEQ_BONK); return; }   // missed the stack
  if(x-tx<=2 && tx-x<=2){ a=tx; b=tx+tw; G.look=40; sfx(1,SEQ_STAR); }                                           // dead on: nothing trimmed
  else { if(x<a) pk_chunk(x,a-x,land,-1); if(x+w>b) pk_chunk(b,x+w-b,land,1); sfx(1,SEQ_CATCH); }
  G.pk_x[n]=(s16)a; G.pk_w[n]=(u8)(b-a); G.round=n+1; G.score=n; G.guess=b-a;
  if(G.round>=PK_TOP){ G.spawn=3; G.reveal=60; return; }
  pk_slider();
}
static void hop_play(u16 pr){
  if(G.stun) G.stun--;
  if((pr&(K_A|K_UP)) && G.py==0 && !G.stun){ G.pvy=-904; sfx(0,SEQ_JUMP); }
  if(G.pvy||G.py){ G.py+=G.pvy; G.pvy+=53; if(G.py>=0){ G.py=0; G.pvy=0; } }
  if(G.guess<HOP_N && --G.spawn<=0){   // (G.guess: how many have rolled in so far)
    Obj *o=new_obj(); if(o) G.guess++;
    if(o){
      // everything rolls at the current "lane speed", which only drifts a little between objects
      int el=GAME_FRAMES-G.timer, lo=470+el/2, hi=640+el/2;      // 1.8-2.5 px/frame at the start, up to 3.0-3.7 at the end
      int v = G.lastv + rnd(81) - 40; v=clampi(v,lo,hi);
      int last_on=0; for(int i=0;i<10;i++) if(G.obj[i].active && &G.obj[i]!=o && G.obj[i].x > (120<<8)) last_on=1;
      if(last_on && v>G.lastv) v=G.lastv;
      G.lastv=v;
      if(G.round){ v=G.lastv; G.round=0; o->type=0; }                // second of a close pair: same speed, always a hurdle
      else o->type=(u8)(rnd(4)==0? 2 : 0);
      G.lastv=v;
      o->x=240<<8; o->vx=-v; o->flip=0; o->y = o->type==2? -(34+rnd(14)) : 0; o->pad=0;
      if(o->type==0 && rnd(3)==0){ G.round=1; G.spawn=37+rnd(4); }  // a quick double hop is coming
      else G.spawn=38+rnd(24);
    } else G.spawn=38+rnd(24);
  }
  int pyp=G.py>>8;
  for(int i=0;i<10;i++){ Obj *o=&G.obj[i]; if(!o->active) continue;
    o->x+=o->vx; int x=o->x>>8;
    if(o->type==0){
      int hit = x<G.px+26 && x+18>G.px+6 && pyp>-16;
      if(hit && !G.stun && !o->pad){ o->pad=1; G.stun=60; sfx(1,SEQ_BONK); }
      else if(hit && !o->pad) o->pad=1;   // v8.1: still flashing from a hit - this one is a miss too (no second knock)
      if(!o->pad && !o->flip && x+18<G.px+6){ o->flip=1; G.score++; sfx(1,SEQ_CATCH); }
    } else {
      int by=o->y;
      if(x<G.px+28 && x+12>G.px+4 && by>pyp-24 && by<pyp+12){ o->active=0; G.score++; sfx(1,SEQ_STAR); continue; }
    }
    if(x<-20) o->active=0;
  }
}
static void guess_play(u16 pr){
  if(G.reveal){ if(--G.reveal==0){ G.round++; G.look=0; G.guess=0; G.t=0; if(G.round>=5){ mini_finish(); } } return; }
  int g = (pr&K_LEFT)? -1 : (pr&K_RIGHT)? 1 : 0;
  if(!g && G.t>=150) g=2;
  if(g){
    G.guess=g; G.look = rnd(2)? 1 : -1;
    if(g==G.look){ G.score++; sfx(1,SEQ_HAPPY); } else sfx(1,SEQ_DENY);
    G.reveal=60;
  } else if((G.t&31)==0) sfx(0,SEQ_TICK);
}
// ---- STARGAZER: the stars play a little tune; play it back on the D-pad to sing the critter to sleep.
// spawn: 0 showing, 1 your turn, 2 well done, 3 oops; px: the lit star (stun: how long), look: next note / the right star
static void st_start(void){
  for(int i=0;i<8;i++){ int d; do d=rnd(4); while(i>0 && d==G.st_seq[i-1] && rnd(2)); G.st_seq[i]=(u8)d; }
  G.round=0; G.spawn=0; G.look=0; G.reveal=50; G.px=-1; G.guess=0; G.stun=0; G.lastx=0;
}
static void st_light(int d,int t){ G.px=d; G.stun=(u8)t; sfx(0,STAR_NOTE[d]); }
static void stars_play(u16 pr){
  if(G.stun && --G.stun==0) G.px=-1;
  int len=2+G.round;
  if(G.spawn==0){
    if(--G.reveal>0) return;
    if(G.look<len){ st_light(G.st_seq[G.look],26); G.look++; G.reveal=36; }
    else { G.spawn=1; G.guess=0; G.timer=240; }
    return;
  }
  if(G.spawn==1){
    int d=(pr&K_UP)? 0 : (pr&K_LEFT)? 1 : (pr&K_RIGHT)? 2 : (pr&K_DOWN)? 3 : -1;
    if(d<0){ if(--G.timer<=0){ G.look=G.st_seq[G.guess]; G.lastx=1; G.spawn=3; G.reveal=80; sfx(1,SEQ_DENY); } return; }
    st_light(d,14);
    if(d==G.st_seq[G.guess]){ G.timer=240;
      if(++G.guess>=len){ G.score++; G.round++; G.spawn=2; G.reveal=64; } }
    else { G.look=G.st_seq[G.guess]; G.lastx=0; G.spawn=3; G.reveal=80; sfx(1,SEQ_DENY); }
    return;
  }
  if(G.reveal==44 && G.spawn==2) sfx(1,SEQ_HAPPY);
  if(--G.reveal>0) return;
  if(G.spawn==2 && G.round<5){ G.spawn=0; G.look=0; G.reveal=30; }
  else mini_finish();
}
static void mini_update(u16 keys, u16 pr){
  G.t++; if(MSG_TIMER) MSG_TIMER--;
  if(G.phase!=PH_RESULT && (pr&K_B)){ set_msg("OKAY, MAYBE LATER!"); sfx(0,SEQ_BLIP); to_room(keys); return; }
  if(G.phase==PH_PICK){
    if(pr&K_UP){ G.pick=(u8)(G.pick? G.pick-1 : N_GAMES-1); sfx(0,SEQ_BLIP); }
    if(pr&K_DOWN){ G.pick=(u8)(G.pick+1>=N_GAMES? 0 : G.pick+1); sfx(0,SEQ_BLIP); }
    if(pr&(K_A|K_START)){ sfx(0,SEQ_CONFIRM); start_game(G.pick); }
    return;
  }
  if(G.phase==PH_READY){
    if(G.t==1 || G.t==31) sfx(0,SEQ_TICK);
    if(G.t==20) cry();
    if(G.t==60) sfx(0,SEQ_GO);
    if(G.t>=80){ G.phase=PH_PLAY; G.t=0; }
    return;
  }
  if(G.phase==PH_PLAY){
    switch(G.game){
      case G_PANCAKE: pancake_play(pr); return;
      case G_HOP:     hop_play(pr);
        if(G.timer>1) G.timer--;   // the speed-up clock; the round ends once the last one has gone by
        { int left=0; for(int i=0;i<10;i++) left|=G.obj[i].active; if(G.guess>=HOP_N && !left) mini_finish(); }
        return;
      case G_GUESS:   guess_play(pr); return;
      default:        stars_play(pr); return;
    }
    if(--G.timer<=0){ G.timer=0; mini_finish(); }
    return;
  }
  if(G.t>20 && (pr&K_A)) mini_apply(keys);
}
static void pick_draw(void){
  O_BOX(20,30,200,106);
  text("WHICH GAME?",30,36,C_RED);
  for(int i=0;i<N_GAMES;i++){ int y=50+i*12; const char *k=KIND_NAME[GAME_KIND[i]];
    if(G.pick==i) text(">",30,y,C_RED);
    text(GAME_NAMES[i],40,y,C_DTEXT); text(k,210-slen(k)*6,y,C_LILAC); }
  fill(30,98,180,1,C_HINT);
  textc(GAME_BOOST[G.pick],120,102,C_DTEXT);
  textc("AMAZING! OR BETTER: +1 BOND",120,112,C_LILAC);
  { static const u8 TOP_COINS[N_GAMES]={15,15,10,15};
    char b[32]; char *p=scpy(b,"TOP PRIZE: "); p=sint(p,TOP_COINS[G.pick]); scpy(p," COINS");
    int w=slen(b)*6; coin_icon(120-w/2-10,123); text(b,120-w/2+1,123,C_ORANGE); }
}
static void mini_draw(void){
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG);
  char *p;
  if(G.phase==PH_PICK){ draw_sky(!G.outside); texts("PLAY",10,8,C_ORANGE);
    p=sint(G.sbuf,G.ext.coins); { int w=slen(G.sbuf)*6; coin_icon(222-w-10,8); texts(G.sbuf,230-w,8,C_LTEXT); }
    pick_draw(); if(MSG_TIMER) label((const char*)MSG_BUF); return; }
  if(G.game!=G_STARS) draw_sky(!G.outside);
  p=scpy(G.sbuf, G.game==G_PANCAKE? "STACK " : "SCORE "); sint(p,G.score); texts(G.sbuf,10,8,C_ORANGE);
  if(G.game==G_GUESS || G.game==G_STARS){ p=scpy(G.sbuf,"ROUND "); p=sint(p,G.round<5?G.round+1:5); scpy(p,"/5"); }
  else if(G.game==G_PANCAKE) G.sbuf[0]=0;          // no BEST counter: a full tower is 14, so it would just sit there
  else if(G.game==G_HOP){ int left=HOP_N-G.guess; for(int i=0;i<10;i++){ const Obj *q=&G.obj[i];
      if(q->active && (q->type==0? !(q->flip || q->pad) : (q->x>>8) > G.px-10)) left++; }
    p=scpy(G.sbuf,"LEFT "); sint(p,left); }
  else { p=scpy(G.sbuf,"TIME "); sint(p,(G.timer+59)/60); }
  if(G.sbuf[0]) texts(G.sbuf,230-slen(G.sbuf)*6,8,C_LTEXT);
  const void *spr=CRITTER_SPR(PET[P_STAGE]);
  int flash = G.stun && ((G.stun>>2)&1);
  if(G.game==G_PANCAKE){
    for(int i=0;i<G.round && i<16;i++) pk_layer(G.pk_x[i],pk_y(i),G.pk_w[i]);
    { int t=G.round-1, cx=G.pk_x[t]+G.pk_w[t]/2, y=pk_y(t); fill(cx-4,y-3,8,4,C_OUTLINE); fill(cx-3,y-2,6,2,C_PALEYEL2); }   // a pat of butter
    if(G.spawn<=1){ int x=G.px>>8; pk_layer(x, G.spawn? G.py>>8 : PK_SLIDE_Y, G.guess);
      if(G.spawn==0 && G.phase==PH_PLAY && ((G.t>>3)&1)){ text("<",x-8,PK_SLIDE_Y-1,C_LTEXT); text(">",x+G.guess+3,PK_SLIDE_Y-1,C_LTEXT); } }
    for(int i=0;i<10;i++){ Obj *o=&G.obj[i]; if(o->active) pk_layer(o->x>>8,o->y>>8,o->type); }
    G.cpose=(s8)(G.pk_eat? POSE_EAT+1 : G.pk_land && G.spawn==3? POSE_SAD+1 : G.look? POSE_HAPPY+1 : 0);
    sprite(spr,10,98,2,1,99); G.cpose=0;
    if(G.look) textbig_o("PERFECT!",204,76,C_ORANGE,1);
    if(G.spawn==3 && G.phase==PH_PLAY) label(G.pk_land? "OOPS! IT SLID OFF!" : "WHAT A TOWER!");
  } else if(G.game==G_HOP){
    G.cpose=(s8)(G.stun? POSE_SAD+1 : G.py<0? POSE_HAPPY+1 : G.phase==PH_PLAY? (((G.t>>2)&1)? POSE_WALK1 : POSE_WALK2)+1 : 0); if(!flash) sprite(spr, G.px, 98+(G.py>>8), 2, 0, 99); G.cpose=0;
    for(int i=0;i<10;i++){ Obj *o=&G.obj[i]; if(!o->active) continue;
      if(o->type==0) sprite(SPR_ROCK, o->x>>8, 112, 3, (G.t>>3)&1, 99);
      else sprite(SPR_BERRY, o->x>>8, 118+o->y, 2, 0, 99); }
  } else if(G.game==G_GUESS){
    int dx = G.look*28, fl = G.look>0;   /* critters face left unless mirrored: tail trails behind */
    G.cpose=(s8)(G.reveal && G.guess==G.look? POSE_HAPPY+1 : 0); sprite(spr, 96+dx, 84, 3, fl, 99); G.cpose=0;
    if(G.phase==PH_PLAY && !G.reveal){ int nudge=SINE16[(G.t>>2)&15]>>2;
      mini_art(MA_ARROW,46-nudge,92,COL_ARROW,0); mini_art(MA_ARROW,180+nudge,92,COL_ARROW,1); label("WHICH WAY WILL IT LOOK?"); }
    if(G.reveal){ textbig_o(G.guess==G.look? "YES!" : G.guess==2? "TOO SLOW!" : "NOPE!",120,44,G.guess==G.look?C_ORANGE:C_LTEXT,2);
      mini_art(MA_ARROW, G.look<0? 96+dx-16 : 96+dx+50, 94, COL_ARROWR, G.look>0);
      if(G.guess==G.look && G.reveal>20) art(ART_HEART,7,6,96+dx+18,76-(60-G.reveal)/4,2,7); }
  } else {
    G.cpose=(s8)(G.round>=3 || (G.phase==PH_RESULT && G.score>=3)? POSE_SLEEP+1 : G.spawn==2? POSE_HAPPY+1 : 0);
    sprite(spr,22,98,2,1,99); G.cpose=0;
    if(G.round>=3){ int z=(G.t>>4)%3; text("Z",56+z*3,92-z*6,C_LTEXT); }
    for(int d=0;d<4;d++){ int x=STAR_X[d], y=STAR_Y[d];
      int lit = (G.px==d && G.stun) || (G.spawn==3 && G.phase==PH_PLAY && G.look==d && ((G.t>>3)&1));
      if(lit){ ring(x,y,15,C_SUN); ring(x,y,12,C_SUN2); mini_art(MA_STAR4,x-10,y-10,COL_LIT,0); }
      else mini_art(MA_STAR3,x-7,y-7,COL_UNLIT,0); }
    if(G.phase==PH_PLAY) label(G.spawn==0? "WATCH THE STARS..." : G.spawn==1? "YOUR TURN!" : G.spawn==2? (G.round>=5? "FAST ASLEEP!" : "LOVELY!") : G.lastx? "TOO SLOW!" : "OOPS!");
  }
  if(G.phase==PH_READY){
    textbig(GAME_NAMES[G.game],120,36,C_ORANGE,1);
    textbig(G.t<60? "READY?" : "GO!",120,52,C_WHITE,4);
  }
  if(G.phase==PH_RESULT){
    O_BOX(30,30,180,82);
    p=scpy(G.sbuf,GAME_SCORE[G.game]); sint(p,G.score); textbig(G.sbuf,120,40,C_DTEXT,2);
    textc(G.rating==3? "PERFECT! OVER THE MOON!" : G.rating==2? "AMAZING! VERY HAPPY!" : G.rating==1? "NICE! IT HAD FUN." : "IT TRIED ITS BEST.",120,64,C_DTEXT);
    { int c=reward_coins(); if(c){ p=scpy(G.sbuf,"+"); p=sint(p,c); scpy(p, c==1? " COIN" : " COINS"); } else scpy(G.sbuf,"NO COINS"); }
    int w=slen(G.sbuf)*6; coin_icon(120-(w+10)/2,80); text(G.sbuf,120-(w+10)/2+10,80,C_ORANGE);
    for(int i=0;i<4;i++) mini_art(MA_STAR3, 120-35+i*18-7, 92, i<=G.rating? COL_STAR : COL_DIM, 0);   // 1-4 stars for the result
    if((G.t>>4)&1) text("A",198,98,C_RED);
  }
  if(MSG_TIMER) label((const char*)MSG_BUF);
}

// ================================================================== NAMING (v8 phase 5 fixes)
// When an egg hatches, the player names the critter (up to 6 letters). A name is suggested to start with; SELECT
// (or RANDOM) suggests another. The original's own texts pick the name up through RAM_NAMES (refresh_tables).
static const char *const NAME_IDEAS[]={"KIWI","BEAN","MOCHI","PIP","TOFU","NORI","SPRIG","DOT","BLIP","PIXEL",
  "PEANUT","NUGGET","BUTTON","PICKLE","MAPLE","PEBBLE","MUFFIN","WAFFLE","CLOVER","POPPY","HONEY","OLIVE","SUNNY",
  "BOBA","NOODLE","TATER","SOCKS","ZIGGY","FIGGY","PIPPIN","NIBBLE","SAGE","MOSS","ACORN","HAZEL","WILLOW","TOAST",
  "CRUMB","GIZMO","MUNCH","FERN","YUZU","MOMO","JELLY","QUILL","TWIGGY","BEEP","SODA","CHIP","LUMI","OTTO","PUDDLE"};
#define N_IDEAS ((int)(sizeof(NAME_IDEAS)/sizeof(NAME_IDEAS[0])))
static const char NAME_KEYS[40]="ABCDEFGHIJKLMNOPQRSTUVWXYZ-.' ";   // 3 rows of 10 (the last key is a space)
static const u8 NAME_BTN_X[3]={0,4,7};                               // RANDOM, DELETE, DONE on the bottom row
static int scmp(const char *a,const char *b){ while(*a && *a==*b){ a++; b++; } return (u8)*a-(u8)*b; }
static int nm_len(void){ int n=0; while(n<NAME_LEN && G.nm[n]) n++; return n; }
static void nm_suggest(void){ const char *s=NAME_IDEAS[rnd(N_IDEAS)]; if(G.nm[0] && !scmp(s,G.nm)) s=NAME_IDEAS[rnd(N_IDEAS)];
  memset(G.nm,0,8); for(int i=0;i<NAME_LEN && s[i];i++) G.nm[i]=s[i]; }
// a quick fill for even x and width: one DMA per row with a fixed source, far quicker than pixel loops
static void dma_fill(int x,int y,int w,int h,int c){
  *(volatile u16*)&G.dma_v=(u16)(c|(c<<8));   // (volatile: the DMA reads it)
  for(int r=0;r<h;r++){ REG32(0x040000DC)=0; REG32(0x040000D4)=(u32)&G.dma_v; REG32(0x040000D8)=(u32)(DRAW_TARGET+(y+r)*120+(x>>1));
    REG32(0x040000DC)=0x81000000u|(u32)(w>>1); (void)REG32(0x040000DC); }
}
// the original's message box, drawn our way (its own routine is too slow for one frame at this size)
static void quick_box(int x,int y,int w,int h){
  dma_fill(x,y,w,h,C_BORDER); dma_fill(x+2,y+2,w-4,h-4,C_CREAM); dma_fill(x+2,y+h-4,w-4,1,38);
  pset(x,y,pget(x-1,y)); pset(x+w-1,y,pget(x+w,y)); pset(x,y+h-1,pget(x-1,y+h-1)); pset(x+w-1,y+h-1,pget(x+w,y+h-1));
  pset(x+2,y+2,C_BORDER); pset(x+w-3,y+2,C_BORDER); pset(x+2,y+h-3,C_BORDER); pset(x+w-3,y+h-3,C_BORDER);
}
static const char *const NAME_BT[3]={"RANDOM","DELETE","DONE"}; static const u8 NAME_BX[3]={28,96,166};
static void name_key(int r,int c,int on){   // one key, plain or highlighted
  int x=27+c*19, y=78+r*12; char ch=NAME_KEYS[r*10+c]; if(!ch) return;
  if(on) fill(x-3,y-2,15,11,C_RED);
  if(ch==' ') text("SP",x-2,y,on? C_WHITE : C_DTEXT); else { char s1[2]={ch,0}; text(s1,x+2,y,on? C_WHITE : C_DTEXT); }
}
static void name_btn(int i,int on){ int w=slen(NAME_BT[i])*6; if(on) fill(NAME_BX[i]-3,115,w+6,11,C_RED); text(NAME_BT[i],NAME_BX[i],117,on? C_WHITE : C_DTEXT); }
// everything that doesn't change is drawn into the background once, a piece per frame (the original's text routine is
// slow, and a frame that runs long would shift the game clock against the random numbers)
static void name_prerender(int stage){
  u16 *save=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG;
  if(stage==0){ if(G.bg_state!=1 && G.bg_state!=2){ DRAW_TARGET=save; build_bg(is_night()? 2 : 1); DRAW_TARGET=(u16*)SCRATCH_BG; } }   // (1 and 2 are the same picture)
  else if(stage==1){   // DMA 3, 32-bit: much quicker than the original's copy loop for EWRAM to EWRAM
    REG32(0x040000DC)=0; REG32(0x040000D4)=(u32)YARD_BG; REG32(0x040000D8)=(u32)SCRATCH_BG; REG32(0x040000DC)=0x84000000u|(240*160/4);
    (void)REG32(0x040000DC); }
  else if(stage==2) bars();
  else if(stage==3) textc("A: TYPE   B: DELETE   START: DONE",120,143,C_HINT);
  else if(stage==4) texts("HELLO!",10,8,C_ORANGE);
  else if(stage==5) texts("SELECT: ANOTHER NAME",232-20*6,8,C_LTEXT);
  else if(stage==6) quick_box(12,30,216,100);
  else if(stage==7) textc("NAME YOUR CRITTER!",120,36,C_RED);
  else if(stage<=10){ for(int c=0;c<10;c++) name_key(stage-8,c,0); }
  else if(stage==11){ for(int i=0;i<3;i++) name_btn(i,0); }
  DRAW_TARGET=save;
}
#define NAME_READY 11
static void enter_naming(void){
  G.want_name=0; G.mode=M_NAME; G.t=0; MSG_TIMER=0; G.outside=0;
  nm_suggest(); G.nm_x=7; G.nm_y=3;    // start on DONE, so A takes the suggestion
  G.nm_pad=0; name_prerender(0); cry();
}
static void name_done(u16 k){
  if(!nm_len()) nm_suggest();
  while(nm_len() && G.nm[nm_len()-1]==' ') G.nm[nm_len()-1]=0;   // no trailing spaces
  if(!nm_len()) nm_suggest();
  memset(G.names.cur,0,8); for(int i=0;i<NAME_LEN;i++) G.names.cur[i]=G.nm[i];
  refresh_tables(); save_all();
  { volatile char *b=(volatile char*)0x03000381; char t[40]; char *p=scpy(t,"SAY HELLO TO "); p=scpy(p,G.names.cur); scpy(p,"!");   // the hatch box
    for(int i=0;i<40;i++){ b[i]=t[i]; if(!t[i]) break; } }
  G.mode=M_NONE; G.suppress=k; KEYS_PREV=k; sfx(0,SEQ_FANFARE); G.cry_delay=40;
}
static void name_update(u16 pr,u16 k){
  G.t++;
  if(G.nm_pad<NAME_READY){ G.nm_pad++; name_prerender(G.nm_pad); return; }   // still setting up the screen
  int n=nm_len();
  if(pr&K_UP){ G.nm_y=(u8)((G.nm_y+3)%4); sfx(0,SEQ_BLIP); }
  if(pr&K_DOWN){ G.nm_y=(u8)((G.nm_y+1)%4); sfx(0,SEQ_BLIP); }
  if(G.nm_y==3){ int b= G.nm_x>=7? 2 : G.nm_x>=4? 1 : 0;
    if(pr&K_LEFT){ b=(b+2)%3; sfx(0,SEQ_BLIP); } if(pr&K_RIGHT){ b=(b+1)%3; sfx(0,SEQ_BLIP); }
    if(pr&(K_LEFT|K_RIGHT)) G.nm_x=NAME_BTN_X[b]; }
  else { if(pr&K_LEFT){ G.nm_x=(u8)((G.nm_x+9)%10); sfx(0,SEQ_BLIP); } if(pr&K_RIGHT){ G.nm_x=(u8)((G.nm_x+1)%10); sfx(0,SEQ_BLIP); } }
  if(pr&K_SELECT){ nm_suggest(); sfx(0,SEQ_COIN); }
  if(pr&K_B){ if(n){ G.nm[n-1]=0; sfx(0,SEQ_BLIP); } else sfx(0,SEQ_DENY); }
  if(pr&K_START){ name_done(k); return; }
  if(pr&K_A){
    if(G.nm_y<3){ int i=G.nm_y*10+G.nm_x; char c=NAME_KEYS[i];
      if(!c || n>=NAME_LEN || (c==' ' && !n)) sfx(0,SEQ_DENY); else { G.nm[n]=c; sfx(0,SEQ_TICK); if(n+1>=NAME_LEN){ G.nm_x=7; G.nm_y=3; } } }
    else { int b= G.nm_x>=7? 2 : G.nm_x>=4? 1 : 0;
      if(b==0){ nm_suggest(); sfx(0,SEQ_COIN); }
      else if(b==1){ if(n){ G.nm[n-1]=0; sfx(0,SEQ_BLIP); } else sfx(0,SEQ_DENY); }
      else { name_done(k); return; } }
  }
}
static void name_draw(void){
  if(G.nm_pad<NAME_READY) return;    // (the hatch stays on screen for the few frames it takes to set this up)
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG);
  G.cpose=(s8)(POSE_HAPPY+1); sprite(CRITTER_SPR(PET[P_STAGE]),20,40,2,1,99); G.cpose=0;
  int n=nm_len();   // the name: six slots, the next one blinking
  for(int i=0;i<NAME_LEN;i++){ int x=78+i*16, y=50;
    if(i<n){ char c[2]={G.nm[i],0}; textbig(c,x+6,y-2,C_DTEXT,2); }
    fill(x,y+13,13,2, i==n && ((G.t>>4)&1)? C_RED : C_HINT); }
  if(G.nm_y<3) name_key(G.nm_y,G.nm_x,1);
  else name_btn(G.nm_x>=7? 2 : G.nm_x>=4? 1 : 0, 1);
}

// v8: the status card's corner (where BEST CATCH was) shows the care it is getting most of, and what it is missing
static const char *const NEED_NAME[N_RMISS]={"HUNGRY","UNHAPPY","MESSY","SICK","LIGHTS ON"};   // v8.1: what went wrong lately
static void card_care(void){
  const CareWin *w=care_win();
  u32 y,g,p; care_lean(w,&y,&g,&p);
  const char *most="BALANCED"; u32 best=250;              // a quarter of the way to a target already shows
  if(y>=best){ best=y; most="OUTSIDE"; }
  if(g>best){ best=g; most="PLANTS"; }
  if(p>best){ best=p; most="PLAY"; }
  int need=-1; u32 worst=14;                               // v8.1: something that went wrong in 15%+ of the last day's checks
  { const More *m=&G.more; u32 n=(u32)m->rn[0]+m->rn[1];
    if(n>=4) for(int k=0;k<N_RMISS;k++){ u32 sh=((u32)m->rmiss[0][k]+m->rmiss[1][k])*100u/n; if(sh>worst){ worst=sh; need=k; } } }
  int bg=pget(200,92);
  fill(158,101,68,22,bg);
  art(ART_HEART,7,6,160,103,1,7); text(most,170,102,C_GREEN);
  if(need>=0){ text("!",162,114,C_RED); text(NEED_NAME[need],170,114,C_RED); }
}

// ================================================================== HOOKS
static void flush_crit_palette(void);
// ================================================================== THE MOONFLOWER GARDEN (v8.1 B2)
// Three patches in the yard. At night, while the critter sleeps, SELECT > GARDEN goes out to them: plant a seed, water
// it each night, and it sprouts, buds and blooms on the fourth night (a missed night only slows it down). A bloom stays
// four nights, then drops a seed. Blooms are purely to look at; while they glow, night visitors come to see them.
// The colour comes from the care the critter gets; four rare forms come from how it was grown (or luck).
#include "garden.h"
#include "can.h"
#define GV_FIRST 9                 // Garden.found: bits 0-8 flowers, 9-18 visitors
#define G_FOUND_N (GK_N+GV_N)
// v8.1: the three patches are scattered through the middle of the yard, at different depths (B5c), behind the decor
// and the critter, so nothing stands in a flower
static const u8 PATCH_X[3]={80,132,152};
static const u8 PATCH_Y[3]={110,104,127};   // the bottom of each soil mound
#define GROUND_Y 128                         // where the ground visitors walk: in front of all three
static const char *const GK_NAME[GK_N]={"SILVER","ROSE","GOLD","SKY","MINT","STARBLOOM","MOON LILY","TWINGLOW","AURORA"};
static int moon_phase(void){ return G.ext.day&7; }                     // 0 full, 4 new: eight nights round
static int g_found(int b){ return (int)((G.gdn.found>>b)&1u); }
static int g_found_count(void){ int n=0; for(int i=0;i<G_FOUND_N;i++) n+=g_found(i); return n; }
static void g_find(int b){ G.gdn.found|=1u<<b; }
static int g_blooms(void){ int n=0; for(int i=0;i<3;i++) n+= G.gdn.p[i].stage==4; return n; }
static void garden_palette(void){ for(int i=0;i<GP_N;i++) PAL[GP_IDX[i]]=GP_PAL[i]; PAL[255]=RGB(31,15,5); }   // (255: the ember firefly)
static int garden_palette_on(void){ return G.mode==M_GARDEN || G.mode==M_FLY || (G.mode==M_MINI && G.outside) ||   // (v8.1: games in the yard show its flowers)
   G.ex_yard || (G.mode==M_DEX && (G.jtab==JT_GARDEN || G.jtab==JT_FLY)) || ra_shot()==1 ||
   // v8.2b: every screen over the yard keeps the flowers' colours (menus, the bag, feeding and petting too)
   (G.outside && SCENE>=1 && SCENE<=5 && (G.mode==M_NONE || G.mode==M_MENU || G.mode==M_EXTRAS || G.mode==M_BAG || G.mode==M_EAT || G.mode==M_SURE)); }
// the colour a flower takes from the care the critter is getting: a weighted pick, so the care it gets most shows most
static int care_colour(void){
  const CareWin *w= G.dex.phase>=PH_ADULT? &G.care.adult : &G.care.grow; u32 n=w->n? w->n : 1;
  u32 s[5];
  s[GK_SILVER]= (u32)w->good*100/n;
  s[GK_ROSE]  = (u32)w->play*300/n;
  s[GK_GOLD]  = (u32)(w->yard+w->garden)*300/n;
  s[GK_SKY]   = 100 - ((u32)w->miss[K_CLEAN]*300/n>100? 100 : (u32)w->miss[K_CLEAN]*300/n);
  s[GK_MINT]  = 100 - ((u32)w->miss[K_FOOD]*300/n>100? 100 : (u32)w->miss[K_FOOD]*300/n);
  u32 tot=0; for(int i=0;i<5;i++){ if(s[i]>100) s[i]=100; s[i]=s[i]*s[i]+1; tot+=s[i]; }
  u32 r=rnd(tot); for(int i=0;i<5;i++){ if(r<s[i]) return i; r-=s[i]; } return GK_SILVER;
}
static void garden_bloom(Patch *p){
  int wish = (u8)(G.more.spare[0]-1)==p->pday && G.more.spare[0], moon = (p->pday&7)==0;
  int k;
  if(rnd(20)==0) k=GK_AURORA;
  else if(moon && rnd(3)==0) k=GK_MOON;                     // v8.2b: the rare chances differ (lily 1/3, star 1/2, twin 1/5)
  else if(wish && rnd(2)==0) k=GK_STAR;
  else if(!(p->flags&2) && rnd(5)==0){ int a=(int)rnd(5), b=(int)rnd(4); if(b>=a) b++; k=GK_TWIN; p->col2=(u8)(a|(b<<4)); }
  else k= p->kind<5? p->kind : care_colour();   // (the bud already showed its colour)
  p->kind=(u8)k; p->stage=4; p->left=4;
  if(G.gdn.blooms<65535) G.gdn.blooms++;
  if(!g_found(k)) G.gdn.news|=4;
  g_find(k); G.gdn.news|=1;
}
static void garden_morning(void){   // the night is over: watered plants grow, blooms fade, faded blooms drop a seed
  Garden *g=&G.gdn; int any=0;
  for(int i=0;i<3;i++){ Patch *p=&g->p[i];
    if(p->stage>=1 && p->stage<=3){ any=1;
      if(p->flags&1){ p->stage++; if(p->stage==3) p->kind=(u8)care_colour(); if(p->stage==4){ p->stage=3; garden_bloom(p); } }
      else p->flags|=2; }
    else if(p->stage==4){ any=1; if(p->left) p->left--;
      if(!p->left){ p->stage=0; p->flags=0; if(g->seeds<9) g->seeds++; g->news|=2; } }
    p->flags&=(u8)~1; }
  if(any) save_all();
}
// ---- drawing
static void gdraw(int id,int x,int y,const u8 *pet,int mono){
  const GSpr *s=&GSPR[id]; const u8 *p=GPIX+s->off;
  for(int r=0;r<s->h;r++) for(int c=0;c<s->w;c++){ int v=*p++; if(!v) continue;
    if(v<8) v= pet? pet[v-1] : GP_K;
    pset(x+c,y+r, mono? mono : v); }
}
static void gdraw_aurora(int x,int y,int t){   // the petals as slow bands of colour, rippling upward like an aurora's curtains
  const GSpr *s=&GSPR[GS_BLOOM0]; const u8 *p=GPIX+s->off;
  for(int r=0;r<s->h;r++) for(int c=0;c<s->w;c++){ int v=*p++; if(!v) continue;
    if(v<8){ int b=r*4 + (t>>1) + SINE16[(c*2+(t>>3))&15]; v=GP_AURORA[(b>>3)&7]; }
    pset(x+c,y+r,v); }
}
static void glints(int cx,int cy,int c,int t){   // a soft ring of glints that slowly turns and twinkles
  for(int k=0;k<14;k++){ if((k+(t>>2))%7==0) continue;
    int a=(k*16/14 + (t>>4))&15, r=(k&1)? 7 : 8;
    pset(cx+SINE16[(a+4)&15]*r/8, cy+SINE16[a]*r/8, c); }
}
// v8.2b: watering. Planting leaves the soil dry; watering brings a can that tips and showers from its rose, and the
// patch's soil turns dark (wet) for the rest of the night. Art: gen/can.py -> can.h (garden palette).
#define CAN_T   72      // frames the can is out: in (12), pouring (44), away (16)
#define CAN_WET 36      // the soil darkens here, mid-pour
#define SEQ_POUR_START SEQ_BLIP
static void soil_wet(int i,int x,int yb){   // darken a watered patch's soil (59 soil -> 41, 41 -> 40)
  const Patch *p=&G.gdn.p[i]; if(!(p->flags&1)) return;
  for(int y=yb-5;y<yb;y++) for(int xx=x-7;xx<=x+7;xx++){ int v=pget(xx,y); if(v==59) pset(xx,y,41); else if(v==41) pset(xx,y,40); }
}
static void can_pose(int tip,int x,int y){
  int w= tip? CAN_TIP_W : CAN_UP_W, h= tip? CAN_TIP_H : CAN_UP_H;
  for(int r=0;r<h;r++) for(int c=0;c<w;c++){ int v= tip? CAN_TIP[r][c] : CAN_UP[r][c]; if(v) pset(x+c,y+r,v); }
}
static void can_draw(int px,int py,int t){   // t: 0 .. CAN_T-1; px,py: the patch (bottom of its soil)
  int ry=py-20, tx=px-CAN_TIP_RX-1, ty=ry-CAN_TIP_RY;              // the tipped can, its rose's face above the patch
  int ux=tx+2, uy=ty+3;                                             // carried, about where it tips from
  if(t<12){ can_pose(0,ux-(12-t)*3,uy-(12-t)); return; }             // carried in from the left
  if(t>=56){ int k=t-56; can_pose(0,ux-k*3,uy-k*2); return; }        // and away again
  can_pose(1,tx,ty);
  int fx=tx+CAN_TIP_RX, fy=ty+CAN_TIP_RY, L=py-4-fy;                 // the shower: only from the rose's face
  if(L<4) L=4;
  int u=t-12;                                                       // pouring time
  for(int k=0;k<4;k++){ int x0=fx-1+k;
    for(int j=0;j<4;j++){ int ph=(u*2+j*(L/4+1)+k*3)%L; if(ph>u*2) continue;   // (the first drops have only just left)
      int x=x0+(ph*(k-1))/14, y=fy+1+ph; pset(x,y,(j+k)&1? GP_SKY1 : 245); if(ph+1<L) pset(x,y+1,GP_SKY2); } }
  if(u>8){ int s=(u>>1)&3; pset(px-4+s,py-5,245); pset(px+3-s,py-6,GP_SKY1); pset(px+s,py-5,GP_SKY2); }   // little splashes
}
static void patch_draw(int i,int glow,int t,int moving){   // moving 0: soil, stems and young plants (kept in the yard picture)
  const Patch *p=&G.gdn.p[i]; int x=PATCH_X[i], yb=PATCH_Y[i];
  u8 pet[4]={GP_PETAL[0][0],GP_PETAL[0][1],GP_PETAL[0][0],GP_PETAL[0][1]};
  if(p->kind<5){ pet[0]=GP_PETAL[p->kind][0]; pet[1]=GP_PETAL[p->kind][1]; }
  if(!moving) switch(p->stage){
    case 0: gdraw(GS_MOUND,x-6,yb-3,0,0); return;
    case 1: gdraw(GS_SEED,x-6,yb-5,0,0); soil_wet(i,x,yb); return;
    case 2: gdraw(GS_SPROUT,x-6,yb-GSPR[GS_SPROUT].h,0,0); soil_wet(i,x,yb); return;
    case 3: gdraw(GS_BUD,x-6,yb-GSPR[GS_BUD].h,pet,0); soil_wet(i,x,yb); return; }
  if(p->stage<4) return;
  int hx=x-5, hy=yb-3-8-10;   // head: 11 wide, on an 8-high stem above the mound
  int a=p->col2&15, b=p->col2>>4;
  if(!moving){   // the bloom as it rests, kept in the yard picture; only its glow and sparkle are drawn each frame
    gdraw(GS_MOUND,x-6,yb-3,0,0); gdraw(GS_STEM,hx,yb-3-8,0,0);
    if(p->kind==GK_TWIN){ u8 tw[4]={GP_PETAL[a][0],GP_PETAL[a][1],GP_PETAL[b][0],GP_PETAL[b][1]}; gdraw(GS_TWIN,x-6,hy+1,tw,0); }
    else if(p->kind==GK_STAR) gdraw(GS_STAR0,hx,hy,GP_PETAL[GK_GOLD],0);
    else if(p->kind==GK_MOON) gdraw(GS_MOON0,hx-1,hy-1,0,0);
    else if(p->kind<5) gdraw(GS_BLOOM0,hx,hy,pet,0);
    return; }
  if(p->kind==GK_TWIN){ if(glow) glints(x,hy+3,GP_PETAL[(t>>5)&1? a : b][1],t); return; }
  if(glow){ int c= p->kind==GK_STAR? GP_GOLD2 : p->kind==GK_AURORA? GP_AURORA[(t>>4)&7] : p->kind<5? GP_PETAL[p->kind][1] : 0;
    if(c) glints(x,hy+5,c,t); }
  if(p->kind==GK_STAR && ((t>>3)&1)) gdraw(GS_STAR1,hx,hy,GP_PETAL[GK_GOLD],0);
  else if(p->kind==GK_MOON && ((t>>3)&3)==0) gdraw(GS_MOON1,hx-1,hy-1,0,0);
  else if(p->kind==GK_AURORA) gdraw_aurora(hx,hy,t);
}
static void garden_draw(int glow){ for(int i=0;i<3;i++) patch_draw(i,glow,(int)G.sky_t,1); }   // the blooms, each frame
static void garden_still(void){ for(int i=0;i<3;i++) patch_draw(i,0,0,0); }                         // the rest, in the yard picture
// ---- the visitors: who comes tonight, and where each one likes to be
static void garden_pick_visitors(void){
  Garden *g=&G.gdn; if(g->vis_day==(u8)G.ext.day) return;
  g->vis_day=(u8)G.ext.day; g->vis_n=0;
  int b=g_blooms(); if(!b) return;
  int n=1+(b>=2)+(b>=3 && rnd(2)); u16 taken=0;
  if(rnd(8)==0){ g->vis[g->vis_n++]=8; taken|=1<<8; }                 // the luna moth: rare
  if(b>=2 && rnd(12)==0 && g->vis_n<n){ g->vis[g->vis_n++]=9; taken|=1<<9; }   // the silver fox: rarer
  while(g->vis_n<n){ int v=(int)rnd(8); if(taken&(1<<v)) continue; taken|=1<<v; g->vis[g->vis_n++]=(u8)v; }
  for(int i=0;i<g->vis_n;i++){ int v=g->vis[i]; if(!g_found(GV_FIRST+v)) G.gdn.news|=8; g_find(GV_FIRST+v); }
  if(g->visits<65535) g->visits=(u16)(g->visits+g->vis_n);
  save_all();
}
static int first_bloom(void){ for(int i=0;i<3;i++) if(G.gdn.p[i].stage==4) return i; return 1; }
static void luna_glow(int cx,int cy,int t){
  for(int k=0;k<16;k++){ if((k+(t/3))%4==0) continue;
    int a=(k + (t>>3))&15, r=10+(k&1)*2; pset(cx+SINE16[(a+4)&15]*r/8, cy+SINE16[a]*r*9/64, (k%3)? GP_GLOW_LUNA0 : GP_GLOW_LUNA1); }
}
static void visitor_draw(int v,int t){
  const GVis *V=&GVIS[v]; int fi=V->seq[(t/6)%V->seq_n], id=V->first+fi; const GSpr *s=&GSPR[id];
  int b=first_bloom(), bx=PATCH_X[b], hy=PATCH_Y[b]-21, x=0, y=0;   // (hy: the top of that bloom's head)
  switch(v){
    case 0: x=bx-6+SINE16[((t>>2)+4)&15]*12/8; y=hy-3+SINE16[(t>>2)&15]*6/8; break;            // moth: round a bloom
    case 1: x=110+SINE16[(t>>4)&15]*5; y=92+SINE16[((t>>3)+5)&15]; break;                      // firefly: drifting
    case 2: x=92; y=GROUND_Y-1-s->h+GHOP_FROG[(t/6)%8]; break;                                 // frog: by the path
    case 3: x=(int)((t>>2)%300)-30; y=GROUND_Y+1-s->h; break;                                   // hedgehog: trundling past
    case 4: x=(int)((t*2)%320)-40; y=36+SINE16[(t>>3)&15]/2; break;                            // bat: across the sky
    case 5: x=100; y=79-s->h; break;                                                           // owl: on a fence post
    case 6: x=40+(int)((t>>5)%120); y=GROUND_Y+2-s->h; break;                                   // snail: very slowly
    case 7: x=164; y=GROUND_Y-s->h+GHOP_CRICKET[(t/6)%8]; break;                                // cricket
    case 8: x=bx+10+SINE16[(t>>3)&15]/2; y=hy-19+SINE16[((t>>2)+3)&15]/3; luna_glow(x+7,y+7,t); break;   // luna moth
    case 9: x=128; y=GROUND_Y+1-s->h; break; }                                                  // silver fox
  gdraw(id,x,y,0,0);
}
// ---- the night garden screen
static void build_bg(int want);
static void garden_msg(const char *m){ int i=0; while(m[i] && i<33){ G.gmsg[i]=m[i]; i++; } G.gmsg[i]=0; G.gmsg_t=150; G.gmsg_n++; }
// rows (or a box) of a picture kept in SCRATCH_BG, copied a word at a time onto the screen being drawn
static void copy_rect(int x0,int y0,int x1,int y1){ x0&=~3; x1=(x1+3)&~3;
  for(int y=y0;y<y1;y++){ const u32 *s=(const u32*)(SCRATCH_BG+y*240+x0); u32 *d=(u32*)((u8*)DRAW_TARGET+y*240+x0); for(int x=x0;x<x1;x+=4) *d++=*s++; } }
static int garden_ok(void){ return is_night() && PET[P_ASLEEP] && PET[P_STAGE]<=7; }
static void garden_open(void){
  G.gui_key=0; G.mode=M_GARDEN; G.gcur=0; G.gt=0; G.gwater=0; G.gmsg_t=0; G.outside=1; G.bg_state=0; G.gpad2=0; sfx(0,SEQ_DOOR);
  garden_pick_visitors();
  Garden *g=&G.gdn;
  if(g->news&4) garden_msg("A NEW MOONFLOWER BLOOMED!");
  else if(g->news&1) garden_msg("THE MOONFLOWERS BLOOMED!");
  else if(g->news&2) garden_msg("A BLOOM FADED AND LEFT A SEED.");
  else if(g->news&8) garden_msg("SOMEONE NEW CAME TO VISIT!");
  else if(g->vis_n) garden_msg("VISITORS CAME TO SEE THE FLOWERS!");
  else if(!g->blooms && g->p[0].stage+g->p[1].stage+g->p[2].stage==0) garden_msg("PLANT A SEED AND WATER IT NIGHTLY");
  g->news=0;
}
static void garden_close(u16 keys){ G.outside=0; G.bg_state=0; sfx(0,SEQ_DOOR); to_room(keys); }
static void garden_update(u16 pr,u16 keys){
  live_update(0);   // v8.1: the clock keeps running out in the yard, so the night still ends
  if(SCENE!=1){ G.outside=0; G.bg_state=0; G.mode=M_NONE; return; }   // (e.g. a farewell)
  G.gt++; if(G.gmsg_t) G.gmsg_t--;
  if(G.gwater){ G.gwater--;   // v8.2b: the watering can is out: wait for it
    if(G.gwater==CAN_WET){ G.bg_state=0; garden_msg("WATERED IT!"); sfx(0,SEQ_COIN); }
    if(G.gwater) return; }
  wish_tick();
  if(wish_try(pr)) return;
  if(!is_night()){ garden_close(keys); return; }
  if(pr&K_SELECT){ sfx(0,SEQ_BLIP); G.ex_yard=1; G.mode=M_EXTRAS; G.cur=3; G.suppress=keys; return; }   // v8.1: the menu, over the yard
  // v8.1: the yard opens just for looking; A shows the cursor to tend the moonflowers (G.gpad2), B puts it away
  if(pr&K_R){ if(fly_rounds_left()>0) fly_start(); else { garden_msg("NO MORE FIREFLIES TONIGHT."); sfx(0,SEQ_DENY); } G.suppress=keys; return; }   // v8.1
  if(!G.gpad2){
    if(pr&K_B){ garden_close(keys); return; }
    if(pr&K_A){ G.gpad2=1; G.gui_key=0; sfx(0,SEQ_BLIP); }
    return; }
  if(pr&K_B){ G.gpad2=0; G.gui_key=0; sfx(0,SEQ_BLIP); return; }
  if(pr&K_LEFT){ G.gcur=(u8)(G.gcur? G.gcur-1 : 2); sfx(0,SEQ_BLIP); }
  if(pr&K_RIGHT){ G.gcur=(u8)(G.gcur>=2? 0 : G.gcur+1); sfx(0,SEQ_BLIP); }
  if(!(pr&K_A)) return;
  Patch *p=&G.gdn.p[G.gcur];
  if(p->stage==0){
    if(!G.gdn.seeds){ garden_msg("NO SEEDS. A FADED BLOOM DROPS ONE."); sfx(0,SEQ_DENY); return; }
    G.gdn.seeds--; memset(p,0,sizeof(Patch)); p->stage=1; p->flags=0; p->pday=(u8)G.ext.day; p->kind=0xFF;   // (v8.2b: planted dry; A again waters it)
    garden_msg("PLANTED A MOONFLOWER SEED!"); sfx(0,SEQ_STAR); save_all(); G.bg_state=0; G.gui_key=0; return; }
  if(p->stage<4){
    if(p->flags&1){ garden_msg("IT'S WATERED. IT GROWS BY MORNING."); sfx(0,SEQ_BLIP); return; }
    p->flags|=1; G.gwater=CAN_T; G.gui_key=0; sfx(0,SEQ_POUR_START); save_all(); return; }   // the can comes; the message waits for the pour
  { char b[40]; char *q;
    if(p->kind>=GK_STAR){ q=scpy(b,"A RARE "); scpy(q,GK_NAME[p->kind]); scpy(b+slen(b),"!"); }
    else { q=scpy(b,"A "); q=scpy(q,GK_NAME[p->kind]); scpy(q," MOONFLOWER."); }
    garden_msg(b); sfx(0,SEQ_STAR); }
}
static void garden_words(void){   // the bars, their words and any message: drawn once into SCRATCH_BG, copied each frame
  u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG; O_DMACOPY(SCRATCH_BG, YARD_BG);
  dma_fill(0,0,240,27,C_BAR); dma_fill(0,27,240,1,C_OUTLINE); dma_fill(0,135,240,25,C_BAR); dma_fill(0,134,240,1,C_OUTLINE);
  texts("THE YARD AT NIGHT",8,10,GP_Y);   // (garden colours: the lights are off, and the low colours dim with them)
  { char b[16]; char *q=scpy(b,"SEEDS "); sint(q,G.gdn.seeds); texts(b,232-slen(b)*6,10,C_LTEXT); }
  if(G.gmsg_t){ int w=slen(G.gmsg)*6+12; quick_box(120-w/2,32,w,16); textc(G.gmsg,120,36,C_DTEXT); }
  const Patch *p=&G.gdn.p[G.gcur]; const char *h;
  if(wish_open()) h="A SHOOTING STAR!   A: MAKE A WISH";
  else if(!G.gpad2) h= fly_rounds_left()>0? "A: TEND  R: FIREFLIES  B: INSIDE" : "A: TEND THE MOONFLOWERS   B: GO INSIDE";
  else h= p->stage==0? (G.gdn.seeds? "A: PLANT A SEED   B: BACK" : "NO SEEDS LEFT   B: BACK")
   : p->stage<4? ((p->flags&1)? "WATERED TONIGHT   B: BACK" : "A: WATER IT   B: BACK")
   : "A: LOOK CLOSER   B: BACK";
  textc(h,120,144,wish_open()? GP_Y : C_HINT);   // (a shooting star: in gold)
  DRAW_TARGET=sv;
}
static void yard_fireflies(void);
static void garden_screen_draw(void){
  if(G.bg_state!=4) build_bg(4);
  { const Patch *p=&G.gdn.p[G.gcur];
    u16 key=(u16)(1 | (G.gdn.seeds<<1) | (G.gcur<<5) | (p->stage<<7) | ((p->flags&1)<<10) | ((G.gmsg_t!=0)<<11) | ((G.gmsg_n&15)<<12));
    if(key!=G.gui_key){ garden_words(); G.gui_key=key; } }
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, YARD_BG);
  draw_sky(0); garden_draw(1); draw_decor(1); yard_poops();
  for(int i=0;i<G.gdn.vis_n;i++) visitor_draw(G.gdn.vis[i],(int)G.gt);
  if(G.gpad2 && !G.gwater){ int x=PATCH_X[G.gcur], y=PATCH_Y[G.gcur]-(G.gdn.p[G.gcur].stage==4? 36 : G.gdn.p[G.gcur].stage==3? 21 : 13)-((G.gt>>3)&1);   // the cursor (tending)
    for(int k=0;k<4;k++){ fill(x-3+k,y+k,7-2*k,1,GP_Y); pset(x-4+k,y+k,GP_K); pset(x+4-k,y+k,GP_K); } pset(x,y+4,GP_K); }
  if(G.gwater) can_draw(PATCH_X[G.gcur],PATCH_Y[G.gcur],CAN_T-G.gwater);   // v8.2b: the watering can
  copy_rect(0,0,240,28); copy_rect(0,134,240,160);
  if(G.gmsg_t){ int w=slen(G.gmsg)*6+12; copy_rect(120-w/2,32,120+w/2,48); }
  yard_fireflies();
}

static void dex_text(const char *s,int x,int y,int c);
// ================================================================== BEDTIME STORIES (v8.1)
// Three little stories of the meadow, read at night from the SELECT menu. The first one each night gives +1 bond
// when the critter wakes up (More.spare[1]: the night of the last story, spare[2]: a bond waiting for the morning;
// More.stories: one bit per story read). The pictures use their own 24 colours in 40-63 (the food's range).
#define ST_N 3
#define ST_PAGES 6
static const char *const ST_TITLE[ST_N]={"THE FIRST STAR-SEED","WHY THE MOON CHANGES","THE FAINT STARS"};
static const char *const ST_TEXT[ST_N][ST_PAGES]={
 {"LONG, LONG AGO, THE SKY OVER THE|MEADOW HAD JUST ONE STAR. IT|SHONE ALL NIGHT, EVERY NIGHT, BUT|NOBODY EVER SHONE BACK.",
  "IT WATCHED THE MEADOW FOLK BELOW:|THE OWLS, THE MOTHS, THE SLEEPY|FROGS. THEY ALL HAD SOMEONE.|THE STAR WAS LONELY.",
  "SO ONE NIGHT IT BROKE OFF THE|TINIEST PIECE OF ITSELF, MADE A|WISH, AND LET IT FALL. DOWN, DOWN|IT WENT, INTO THE SOFT GRASS.",
  "THE MEADOW FOLK FOUND A LITTLE|GLOWING SEED. THEY KEPT IT WARM,|AND ONE MORNING IT HATCHED! THEY|FED IT, PLAYED WITH IT, LOVED IT.",
  "IT GREW AND GREW, AND WHEN ITS|TIME CAME, IT FLOATED UP, UP,|UP... AND BECAME A SECOND STAR,|RIGHT BESIDE THE FIRST ONE.",
  "EVER SINCE, EVERY CRITTER THAT|IS TRULY LOVED GOES HOME TO THE|SKY. THAT'S WHY THERE ARE SO MANY|STARS. GOODNIGHT, LITTLE ONE."},
 {"THE MOON LOVES THE MEADOW. EVERY|NIGHT SHE LOOKS DOWN AT THE|FLOWERS, ASLEEP IN THE DARK, AND|WISHES THEY COULD SHINE TOO.",
  "SO SHE GIVES THEM A LITTLE OF HER|LIGHT, JUST A SLIVER, SOFT AS A|WHISPER. THE FLOWERS OPEN UP AND|GLOW: SILVER, ROSE AND GOLD.",
  "NIGHT AFTER NIGHT SHE GIVES A|LITTLE MORE, AND NIGHT AFTER|NIGHT SHE GROWS THINNER... UNTIL|ONE NIGHT, SHE IS GONE.",
  "THE MEADOW IS VERY DARK. BUT THE|MOONFLOWERS REMEMBER WHO GAVE|THEM THEIR LIGHT. ONE BY ONE,|THEY SEND A LITTLE GLOW BACK UP.",
  "PETAL BY PETAL, THE LIGHT FLOATS|HOME, AND THE MOON GROWS ROUND|AGAIN. MOTHS AND FIREFLIES|FOLLOW THE GLOW ALL NIGHT LONG.",
  "SO WHEN THE MOON LOOKS THIN,|DON'T WORRY. SHE'S ONLY LENDING|HER LIGHT, AND IT ALWAYS COMES|BACK. GOODNIGHT, LITTLE ONE."},
 {"LOOK UP ON A CLEAR NIGHT. SOME|STARS SHINE BRIGHT, AND SOME ONLY|GLIMMER. EVERY ONE OF THEM WAS|ONCE SOMEBODY'S CRITTER.",
  "THE BRIGHT ONES SAID A HAPPY|GOODBYE. THEY WAVED, THEY FLOATED|UP, AND THEY HAVE SHONE EVER|SINCE, WARM WITH ALL THAT LOVE.",
  "THE FAINT ONES LEFT IN A HURRY,|BEFORE THEY COULD SAY GOODBYE AT|ALL. SO THEY ONLY GLIMMER, LIKE|A WORD THAT WAS NEVER SAID.",
  "BUT THEY AREN'T LOST. THEY'RE|STILL UP THERE, WATCHING OVER THE|MEADOW, HOPING SOMEONE STILL|REMEMBERS THEM.",
  "AND SOMETIMES ONE COMES DOWN TO|VISIT, STREAKING ACROSS THE SKY.|THAT'S A WISHING STAR! MAKE A|WISH, AND IT GOES HOME BRIGHTER.",
  "SO IF YOU SEE ONE, MAKE A WISH.|SOMEWHERE UP THERE, A LITTLE|STAR WILL SHINE A BIT BRIGHTER.|GOODNIGHT, LITTLE ONE."} };
enum { S_SKYA=40, S_SKYB, S_SKYC, S_SKYD, S_STAR, S_STARY, S_GOLD, S_FAINT, S_FAINT2, S_MOON, S_MOONSH, S_HILL1, S_HILL2, S_GRASS,
       S_ROSE, S_LILAC, S_MINT, S_GLOW, S_FLY, S_MOTH, S_SEED, S_SEEDG, S_STEM, S_SPARE };
static const u16 ST_PAL[24]={ RGB(2,3,8),RGB(3,4,10),RGB(4,6,13),RGB(6,8,16), RGB(31,31,28),RGB(31,28,16),RGB(31,22,6),RGB(11,12,20),RGB(19,20,27),
  RGB(30,30,23),RGB(8,9,15), RGB(3,9,8),RGB(2,6,7),RGB(6,14,10), RGB(29,17,23),RGB(23,19,31),RGB(17,29,24),RGB(27,30,21),
  RGB(30,31,12),RGB(25,23,27),RGB(31,30,21),RGB(31,25,9),RGB(5,12,8),0 };
static void story_palette(void){ for(int i=0;i<24;i++) PAL[S_SKYA+i]=ST_PAL[i]; }
static int story_tonight(void);
static int story_ok(void){ return is_night() && PET[P_STAGE]>=1 && PET[P_STAGE]<=7 && !story_tonight(); }   // v8.1: one a night
static void story_open(void){
  G.mode=M_NONE;
  if(PET[P_STAGE]==0){ set_msg("IT'S STILL AN EGG!"); sfx(0,SEQ_DENY); return; }
  if(!story_ok()){ set_msg(story_tonight() && is_night()? "ONE STORY A NIGHT. SLEEP TIGHT!" : "STORIES ARE FOR BEDTIME!"); sfx(0,SEQ_DENY); return; }
  G.mode=M_STORY; G.st_page=0xFF; G.st_pad[0]=0xFF; if(G.st_cur>=ST_N) G.st_cur=0; sfx(0,SEQ_JOURNAL);
}
static int story_tonight(void){ return G.more.spare[1]==(u16)(G.ext.day+1); }
static void story_finish(u16 keys){
  G.more.stories|=(u16)(1u<<G.st_cur);
  if(!story_tonight()){ G.more.spare[1]=(u16)(G.ext.day+1); G.more.spare[2]=1; set_msg("SWEET DREAMS! +1 BOND IN THE MORNING"); }
  else set_msg("SWEET DREAMS!");
  save_all(); sfx(0,SEQ_HAPPY); to_room(keys);
}
static void story_update(u16 pr,u16 keys){
  if(G.st_page==0xFF){
    if(pr&K_UP){ G.st_cur=(u8)(G.st_cur? G.st_cur-1 : ST_N-1); sfx(0,SEQ_BLIP); }
    if(pr&K_DOWN){ G.st_cur=(u8)(G.st_cur+1>=ST_N? 0 : G.st_cur+1); sfx(0,SEQ_BLIP); }
    if(pr&K_A){ G.st_page=0; sfx(0,SEQ_CONFIRM); }
    else if(pr&(K_B|K_SELECT|K_START)){ sfx(0,SEQ_BLIP); to_room(keys); }
    return; }
  if(pr&(K_A|K_RIGHT)){ if(G.st_page+1>=ST_PAGES){ if(pr&K_A) story_finish(keys); } else { G.st_page++; sfx(0,SEQ_BLIP); } }
  else if((pr&K_LEFT) && G.st_page){ G.st_page--; sfx(0,SEQ_BLIP); }
  else if(pr&K_B){ G.st_page=0xFF; sfx(0,SEQ_BLIP); }
}
// ---- the pictures: a night sky over two hills, with whatever the page needs
static const u8 ST_STARS[18][2]={{14,22},{40,34},{62,18},{88,40},{104,24},{130,36},{152,20},{176,44},{198,26},{222,38},{28,50},
  {72,54},{118,52},{164,58},{210,54},{232,20},{6,40},{142,28}};
static int hill_y(int x){ int d1=x-60, d2=x-190, y1=62+d1*d1/180, y2=58+d2*d2/160; return y1<y2? y1 : y2; }
// each page is drawn once into SCRATCH_BG (the still parts, ST_A=0); every frame copies it and adds what moves (ST_A=1)
#define ST_A (G.st_pad[1])   // (no globals of our own: the extension runs from ROM)
static void st_sky(void){
  if(ST_A) return;
  static const u8 band[4]={S_SKYA,S_SKYB,S_SKYC,S_SKYD};
  for(int i=0;i<4;i++) dma_fill(0,16+i*17,240,17,band[i]);
  for(int x=0;x<240;x++){ int y=hill_y(x); if(y<84){ pset(x,y,S_GRASS); if(y+1<84) fill(x,y+1,1,83-y,S_HILL1); } }
  for(int x=0;x<240;x++){ int y=hill_y(x)+8+((x*7)%5); if(y<84) fill(x,y,1,84-y,S_HILL2); }
}
static void st_star(int x,int y,int c,int big){
  if(!ST_A){ pset(x,y,S_STAR); pset(x-1,y,c); pset(x+1,y,c); pset(x,y-1,c); pset(x,y+1,c); }
  else if(big){ pset(x-2,y,c); pset(x+2,y,c); pset(x,y-2,c); pset(x,y+2,c); } }
static void st_field(int n,int faint){   // n background stars; some twinkle
  for(int i=0;i<n && i<18;i++){ int x=ST_STARS[i][0], y=ST_STARS[i][1]; if(y>=hill_y(x)-2) continue;
    if(!ST_A) pset(x,y, faint? S_FAINT : S_FAINT2);
    else if(((G.sky_t>>4)+i*5)%9==0) pset(x,y, faint? S_FAINT2 : S_STAR); }
}
static void st_moon(int cx,int cy,int ph){   // ph 0 full .. 4 half .. 8 gone (shadow from the right)
  if(ST_A) return;
  int off=ph>=8? 0 : 18-ph*2;
  for(int dy=-8;dy<=8;dy++) for(int dx=-8;dx<=8;dx++){ if(dx*dx+dy*dy>66) continue;
    int sx=dx-off; int shade= ph>0 && (ph>=8 || sx*sx+dy*dy<=66);
    pset(cx+dx,cy+dy, shade? S_MOONSH : S_MOON); }
}
static void st_flower(int x,int c,int open,int glow){
  int y=hill_y(x);
  if(ST_A){ if(open && glow){ int g=(G.sky_t>>3)&1; pset(x-3,y-9,S_GLOW); pset(x+3,y-9,S_GLOW); pset(x,y-12-g,S_GLOW); } return; }
  fill(x,y-6,1,6,S_STEM); pset(x+1,y-3,S_STEM);
  if(!open){ pset(x,y-7,S_MOONSH); pset(x,y-8,S_MOONSH); return; }
  pset(x,y-9,S_SEED); pset(x-1,y-9,c); pset(x+1,y-9,c); pset(x,y-10,c); pset(x,y-8,c); pset(x-1,y-10,c); pset(x+1,y-10,c);
}
static void st_flies(int n,int x0,int y0){ if(!ST_A) return; for(int i=0;i<n;i++){ int t=G.sky_t+i*37; if(((t>>4)+i)%3==0) continue;
  int x=x0+(i*29)%70+SINE16[(t>>3)&15]/2, y=y0+(i*13)%24+SINE16[((t>>4)+i*3)&15]/3; pset(x,y,S_FLY); } }
static void st_moth(int x,int y){ if(!ST_A) return; int f=(G.sky_t>>2)&1; x+=SINE16[(G.sky_t>>3)&15]/2; pset(x,y,S_MOONSH); pset(x,y+1,S_MOONSH);
  if(f){ pset(x-1,y-1,S_MOTH); pset(x-2,y-1,S_MOTH); pset(x+1,y-1,S_MOTH); pset(x+2,y-1,S_MOTH); pset(x-1,y,S_MOTH); pset(x+1,y,S_MOTH); }
  else { pset(x-1,y,S_MOTH); pset(x-2,y+1,S_MOTH); pset(x+1,y,S_MOTH); pset(x+2,y+1,S_MOTH); } }
static void st_shoot(int hx,int hy,int len){ if(!ST_A) return; for(int j=len;j>=1;j--) pset(hx+j,hy-j*2/3, j<4? S_STAR : j<10? S_STARY : S_GOLD);
  pset(hx,hy,S_STAR); pset(hx-1,hy,S_STARY); pset(hx,hy+1,S_STARY); }
static void st_seed(int x,int y){
  if(ST_A){ int g=(G.sky_t>>3)&7; if(g<4) for(int i=0;i<16;i++) pset(x+SINE16[(i+4)&15]*(5+g)/8, y+SINE16[i&15]*(5+g)/8, S_SEEDG); return; }
  pset(x,y-2,S_STAR); fill(x-1,y-1,3,1,S_SEED); fill(x-2,y,5,2,S_SEED); fill(x-1,y+2,3,1,S_SEEDG); pset(x-1,y-1,S_STAR); }
static void story_picture(int s,int p){
  st_sky();
  if(s==0){
    st_field(p==5? 18 : 4,0);
    if(p<=4) st_star(150,30,S_STARY,((G.sky_t>>4)&7)!=0);   // the lonely star shines big
    if(p==1){ st_flies(6,30,50); st_moth(196,44); }
    if(p==2){ int t=(G.sky_t>>1)%60; st_shoot(150-t,30+t*2/3,14); }
    if(p==3){ st_seed(112,hill_y(112)-4); st_flies(5,80,50); }
    if(p==4){ st_star(176,24,S_STARY,((G.sky_t>>4)&7)!=3); if(ST_A) for(int j=0;j<10;j++) if(((G.sky_t>>2)+j)&1) pset(176-j*3,30+j*4,S_SEEDG); }
    if(p==5){ st_star(150,30,S_STARY,((G.sky_t>>4)&3)==0); st_star(176,24,S_STARY,0); st_star(62,30,S_GOLD,((G.sky_t>>4)&3)==2); st_star(208,40,S_STARY,0); st_star(100,18,S_GOLD,0); }
  } else if(s==1){
    static const u8 PH[6]={0,1,6,8,4,0};
    st_field(10,0); st_moon(184,32,PH[p]);
    static const u8 FX[5]={36,62,92,128,150}; static const u8 FC[5]={S_MOON,S_ROSE,S_GOLD,S_LILAC,S_MINT};
    for(int i=0;i<5;i++) st_flower(FX[i],FC[i],p>0,p>0);
    if(p==3 && ST_A){ for(int i=0;i<5;i++){ int t=(G.sky_t+i*20)%80; pset(FX[i]+((i&1)? 1 : -1)*(t/16),hill_y(FX[i])-12-t/2,S_GLOW); } }
    if(p==4){ st_flies(7,40,46); st_moth(70,48); st_moth(120,44); }
    if(p==5) st_flies(5,30,50);
  } else {
    st_field(18,1);
    static const u8 BX[5]={40,96,150,200,120}, BY[5]={30,22,36,26,44};
    if(p<=1 || p==5) for(int i=0;i<4;i++) st_star(BX[i],BY[i],S_STARY,p==1 && ((G.sky_t>>4)+i)%4==0);
    if(p>=2 && p<=4 && !ST_A) for(int i=0;i<4;i++){ pset(BX[i],BY[i],S_FAINT2); pset(BX[i]-1,BY[i],S_FAINT); pset(BX[i]+1,BY[i],S_FAINT); pset(BX[i],BY[i]-1,S_FAINT); pset(BX[i],BY[i]+1,S_FAINT); }
    if(p>=2 && p<=4 && ST_A) for(int i=0;i<4;i++) if(((G.sky_t>>5)+i)%3==0) pset(BX[i],BY[i],S_STARY);   // they glimmer
    if(p==4){ int t=(G.sky_t>>1)%70; st_shoot(190-t,20+t*2/3,16); }
    if(p==5) st_star(120,44,S_STAR,((G.sky_t>>3)&3)!=0);
    else if(p!=4 && !ST_A) pset(120,44,p>=2? S_FAINT : S_FAINT2);
  }
}
static void story_still(void){   // everything that doesn't move, into SCRATCH_BG
  u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG; ST_A=0;
  dma_fill(0,0,240,16,C_BAR); dma_fill(0,144,240,16,C_BAR); dma_fill(0,84,240,60,C_BORDER);
  if(G.st_page==0xFF){
    texts("BEDTIME STORIES",8,4,C_GOLD);
    story_picture(1,5);
    quick_box(20,88,200,52);
    for(int i=0;i<ST_N;i++){ int y=95+i*12; if(G.st_cur==i) text(">",28,y,C_RED);
      text(ST_TITLE[i],38,y,C_DTEXT); if((G.more.stories>>i)&1) text("*",204,y,C_ORANGE); }
    textc("A STORY TONIGHT: +1 BOND",120,148,C_HINT);
  } else {
    char b[8]; char *p=sint(b,G.st_page+1); p=scpy(p,"/"); sint(p,ST_PAGES);
    texts(ST_TITLE[G.st_cur],8,4,C_GOLD); texts(b,232-slen(b)*6,4,C_LTEXT);
    story_picture(G.st_cur,G.st_page);
    quick_box(6,86,228,56);
    const char *t=ST_TEXT[G.st_cur][G.st_page]; char ln[40]; int n=0, y=92;   // (lines up to 34 letters)
    for(;;t++){ if(*t=='|' || !*t){ ln[n]=0; text(ln,14,y,C_DTEXT); y+=10; n=0; if(!*t) break; } else if(n<38) ln[n++]=*t; }
    textc(G.st_page+1>=ST_PAGES? "A: THE END   LEFT: BACK" : G.st_page? "A: NEXT   LEFT: BACK   B: CLOSE" : "A: NEXT   B: CLOSE",120,148,C_HINT);
  }
  DRAW_TARGET=sv;
}
static void story_draw(void){
  u8 key=(u8)(G.st_page==0xFF? 0x80|G.st_cur : (G.st_cur<<3)|G.st_page);
  if(G.st_pad[0]!=key){ story_still(); G.st_pad[0]=key; }
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG);
  ST_A=1;
  if(G.st_page==0xFF) story_picture(1,5); else story_picture(G.st_cur,G.st_page);
  ST_A=0;
}

void hook_update(u32 keys){
  init_once();
  fx_watch();   // (v8 phase 6: near the start of the frame, where register changes don't tear)
  spr_sync();   // (v8.2: the sprites the last render asked for)
  weight_floor();   // (v8.3: growing up, old saves: never under its stage's lightest)
  flush_crit_palette();
  // the original reloads the palette every vblank (40-63 to black): put the food's colours back straight away, so
  // they are right for the whole frame on hardware, not only from wherever the draw gets to
  if(garden_palette_on()) garden_palette();   // (v8.2b: first, so the food can share the yard: it sits at 96-119 there)
  if(G.mode==M_EAT && G.eat_item<11) food_palette(G.eat_item);
  else if(G.mode==M_STORY) story_palette();
  else if(G.mode==M_NONE && SCENE==3 && ANIM_KIND<=1 && PET[P_STAGE]>=1 && PET[P_STAGE]<=7) food_palette(ANIM_KIND==1? FOOD_SNACK : FOOD_MEAL);
  u16 k=(u16)keys, pr=(u16)(k & ~G.prev); G.prev=k;
  G.suppress &= k;
  u16 kin=(u16)(k & ~G.suppress);
  if(SCENE==0) G.outside=0; else G.tdex=0;
  if(SCENE==1) dex_tick();
  switch(G.mode){
    case M_NONE: {
      if(SCENE==1 && (pr&K_SELECT) && PET[P_STAGE]!=8){ G.mode=M_EXTRAS; G.cur=0; sfx(0,SEQ_BLIP); return; }
      if(SCENE==0){
        // the original asks "START OVER WITH A NEW EGG?" - its YES now leads to a second, louder check
        if(TITLE_CONFIRM==1 && TITLE_CUR==1 && (pr&K_A)){ G.mode=M_SURE; G.sure_cur=1; sfx(0,SEQ_BLIP); break; }
        // a third title choice under NEW EGG: the bestiary (handled here, the original only knows two)
        if(!TITLE_CONFIRM && *(volatile u32*)PET==0x31504B43u){
          if(G.tdex){
            if(pr&K_UP){ G.tdex=0; sfx(0,SEQ_BLIP); }
            else if(pr&(K_A|K_START)) dex_open(0);
            G.suppress=k; live_update(0); break;
          }
          if(TITLE_CUR==1 && (pr&K_DOWN)){ G.tdex=1; sfx(0,SEQ_BLIP); G.suppress=k; live_update(0); break; }
        } else G.tdex=0;
      }
      if(SCENE==7){   // HOW TO PLAY: our five pages replace the original three
        if((pr&K_LEFT) && G.help_page>0){ G.help_page--; sfx(0,SEQ_BLIP); }
        if((pr&K_RIGHT) && G.help_page<HELP_PAGES-1){ G.help_page++; sfx(0,SEQ_BLIP); }
        if(pr&(K_A|K_B|K_START)){ sfx(0,SEQ_BLIP); SCENE=6; G.from_help=1; enter_menu(); G.suppress=k; break; }
        live_update(0); break;
      }
      // a new hatchling: the hatch box asks for a name, and its A opens the naming screen
      if(SCENE==5 && G.want_name && PET[P_STAGE]==1){
        volatile char *b=(volatile char*)0x03000381; const char *t="LET'S GIVE IT A NAME!";
        if(b[0]!='L') for(int i=0;i<40;i++){ b[i]=t[i]; if(!t[i]) break; }
        if(pr&(K_A|K_START)){ enter_naming(); G.suppress=k; break; }
      }
      wish_tick();
      if(wish_try(pr)){ G.suppress=k; kin=(u16)(k & ~G.suppress); }   // the A is the wish's, not the icon bar's
      // v8.2b: LIGHTS in the yard just says so (the original would switch them off, and the critter would go in)
      if(G.outside && SCENE==1 && ICON_CUR==1 && (pr&kin&K_A)){ set_msg("YOU'RE OUTSIDE!"); sfx(0,SEQ_DENY); G.suppress=k; kin=(u16)(k & ~G.suppress); }
      int title_new = SCENE==0 && TITLE_CUR==1 && TITLE_CONFIRM==1;
      u32 sc_was=SCENE;
      live_update(kin);
      if(title_new && SCENE!=0){ full_restart(); G.more.pad1&=(u16)~1; }
      else if(sc_was==0 && SCENE==1 && (G.more.pad1&1)){ G.more.pad1&=(u16)~1; save_all();   // v8.2b: it was saved in the yard: back out there
        u8 st=PET[P_STAGE]; if(st>=2 && st<=7 && !PET[P_ASLEEP] && !PET[P_SICK] && !PET[P_LIGHTS]) go_outside(); }
      u32 sc=SCENE;
      if(sc==6) enter_menu();
      else if(sc==8) enter_minigame();
      else if(sc==1 && G.sendoff_pending && !PET[P_ASLEEP] && !MSG_TIMER && PET[P_STAGE]>=3 && PET[P_STAGE]<=7) start_sendoff();
      break; }
    case M_MENU:   menu_update(pr,k); sound_tick(); break;
    case M_EXTRAS: extras_update(pr,k);
                   if(G.mode==M_EXTRAS){ live_update(0); if(SCENE!=1){ G.mode=M_NONE; G.suppress=k; } }
                   else if(G.mode==M_NONE) sound_tick();
                   break;
    case M_SHOP:   shop_update(pr,k); sound_tick(); break;
    case M_BAG:    bag_update(pr,k); sound_tick(); break;
    case M_EAT:    eat_update(k); break;
    case M_MINI:   mini_update(k,pr); sound_tick(); break;
    case M_SENDOFF: sendoff_update(pr,k); sound_tick(); break;
    case M_ALBUM: {
      int pages=(G.album.n+7)/8; if(pages<1) pages=1;
      if(pr&K_LEFT){ G.cur=(u8)(G.cur? G.cur-1 : pages-1); sfx(0,SEQ_BLIP); }
      if(pr&K_RIGHT){ G.cur=(u8)(G.cur+1>=pages? 0 : G.cur+1); sfx(0,SEQ_BLIP); }
      if(pr&(K_B|K_A|K_SELECT|K_START)){ G.mode=M_NONE; G.suppress=k; sfx(0,SEQ_BLIP); }
      sound_tick(); break; }
    case M_DEX: dex_update(pr,k); sound_tick(); break;
    case M_STORY: story_update(pr,k); sound_tick(); break;
    case M_GARDEN: garden_update(pr,k); sound_tick(); break;
    case M_FLY: fly_update(pr,k); sound_tick(); break;
    case M_NAME: name_update(pr,k); if(G.mode==M_NAME) live_update(0); sound_tick(); break;   // time goes on, as under the hatch box
    case M_SURE:
      if(pr&(K_LEFT|K_RIGHT)){ G.sure_cur^=1; sfx(0,SEQ_BLIP); }
      if((pr&K_B) || ((pr&K_A) && G.sure_cur==1)){ TITLE_CONFIRM=0; G.mode=M_NONE; G.suppress=k; sfx(0,SEQ_BLIP); }
      else if((pr&K_A) && G.sure_cur==0){
        G.mode=M_NONE; KEYS_PREV=0; live_update(K_A);
        if(SCENE!=0) full_restart();
        G.suppress=k; KEYS_PREV=k; return;
      }
      sound_tick(); break;
  }
  if(G.mode!=M_NONE) KEYS_PREV=k;
}


// ------------------------------------------------------------------ v7 critters: 48x48-class art, 6 poses, 4 native sizes
static int crit_form(const void *spr){
  for(int i=0;i<8;i++) if(spr==ROM_SPR_TABLE[i]) return i;
  for(int j=0;j<5;j++) if(spr==(const void*)&SPECIAL_SPR[j]) return 8+j;
  for(int j=0;j<10;j++) if(spr==(const void*)&NEW_SPR[j]) return 13+j;
  return -1;
}
static void crit_palette(const u16 *pal,int n,const u8 *idx,int first,int night){
  for(int i=0;i<n;i++){ u16 c=pal[i];
    if(night){ int r=c&31, g=(c>>5)&31, b=c>>10; r=(r*5)>>3; g=(g*5)>>3; b=((b*3)>>2)+2; if(b>31) b=31; c=(u16)(r|(g<<5)|(b<<10)); }
    if(idx) G.cpal[i]=c; else PAL[first+i]=c; }
  if(idx) G.cpal_n=(u8)n;
}
// the critter-slot colours are written once, after the whole frame is drawn: if two critters were drawn in one frame
// (the original title still draws one under ours), the screen could otherwise show the wrong one's colours
static void flush_crit_palette(void){ for(int i=0;i<G.cpal_n;i++) PAL[CR_SLOT_IDX[i]]=G.cpal[i]; }
// rows: (first,last) opaque column per source row; scales S -> D with nearest sampling; optional mirror
static void blit_rows(const u8 *pix,const u8 *rows,int S,int D,int x,int y,int flip){
  u32 step=((u32)S<<16)/(u32)D, sy=step>>1;
  for(int dy=0; dy<D; dy++, sy+=step){
    int yy=y+dy; if((unsigned)yy>=160u) continue;
    int r=sy>>16, a=rows[2*r], b=rows[2*r+1]; if(a==255) continue;
    const u8 *src=pix+r*S;
    int d0,d1;
    if(S==D){ d0=a; d1=b; } else { d0=(a*D)/S; d1=((b+1)*D+S-1)/S-1; }
    if(flip){ int t=D-1-d1; d1=D-1-d0; d0=t; }
    int X0=x+d0, X1=x+d1; if(X0<0) X0=0; if(X1>239) X1=239; if(X0>X1) continue;
    volatile u16 *row=DRAW_TARGET+yy*120;
    if(S==D && !flip){   // fast path: native size, no mirror
      const u8 *sp=src-x; int X=X0;
      if(X&1){ int c=sp[X]; if(c){ volatile u16 *p=row+(X>>1); *p=(u16)((*p&0x00FF)|(c<<8)); } X++; }
      volatile u16 *p=row+(X>>1);
      for(; X<X1; X+=2, p++){
        u32 c0=sp[X], c1=sp[X+1];
        if(c0&&c1) *p=(u16)(c0|(c1<<8));
        else if(c0) *p=(u16)((*p&0xFF00)|c0);
        else if(c1) *p=(u16)((*p&0x00FF)|(c1<<8));
      }
      if(X==X1){ int c=sp[X]; if(c) *p=(u16)((*p&0xFF00)|c); }
      continue;
    }
    for(int X=X0&~1; X<=X1; X+=2){
      int c0=0, c1=0;
      for(int k=0;k<2;k++){
        int X2=X+k; if(X2<X0||X2>X1) continue;
        int dx=X2-x; if(flip) dx=D-1-dx;
        int sx= S==D? dx : (int)(((u32)dx*step+(step>>1))>>16);
        if(k) c1=src[sx]; else c0=src[sx];
      }
      volatile u16 *p=row+(X>>1);
      if(c0&&c1) *p=(u16)(c0|(c1<<8));
      else if(c0) *p=(u16)((*p&0xFF00)|c0);
      else if(c1) *p=(u16)((*p&0x00FF)|(c1<<8));
    }
  }
}
static int pet_variant(int f){ return (G.dex.gender && f>=0 && f<CR_BASE_FORMS)? CR_GIRL[f] : f; }   // v8: boy/girl touches
static void crit_draw(int f,int pose,int x,int y,int sc,int flip,int night){
  if(sc<1) sc=1;
  int si= sc>=4? 3 : sc-1, S=16*(si+1), D=16*sc;
  const u16 *pal=CR_PAL[f];
  crit_palette(pal,CR_SLOTS,CR_SLOT_IDX,0,night);
  if(flip && sc==3){ si=4; flip=0; }   // facing right at room size: a mirrored copy drawn with the light still from the top-left
  blit_rows(CR_PIX+CR_PIXOFF[f][pose][si], CR_ROWS+2*CR_ROWOFF[f][pose][si], S, D, x, y-(sc*3)/4, flip);
}
static void album_critter(int f,int x,int y){   // the album shows many forms at once: shared palette in the room-shade range
  crit_palette(ALB_PAL,ALB_N,0,ALB_FIRST,0);
  blit_rows(ALB_PIX+f*1024, ALB_ROWS+f*64, 32, 32, x, y-1, 0);
}
static int auto_pose(void){ return (FRAMES%193u)<7u? POSE_BLINK : ((FRAMES/40u)&1)? POSE_BREATHE : POSE_IDLE; }   // blink, and a slow breath
// phase 3: the menu icons (12x12 drawn doubled) are swapped for full-size 24x24 versions in the same colour
static const u32 BAR_ICON_SPR[8]={0x0800ffecu,0x0800fff4u,0x0800fffcu,0x0800ffdcu,0x08010004u,0x0801000cu,0x08010014u,0x0800ffbcu};
// draws a small tone grid (0 clear, 1 colour, 2 highlight, 3 shade) using per-row extents, two pixels at a time
static void blit_tones(const u8 *g,const u8 *ext,int w,int h,int x,int y,int c,int hc,int sc){
  u8 col[4]={0,(u8)c,(u8)hc,(u8)sc};
  for(int r=0;r<h;r++, g+=w){
    int a=ext[2*r], z=ext[2*r+1], yy=y+r; if(a==255 || (unsigned)yy>=160u) continue;
    volatile u16 *row=DRAW_TARGET+yy*120;
    int X=x+a, X1=x+z; const u8 *sp=g-x;
    if(X&1){ int v=col[sp[X]]; if(v){ volatile u16 *p=row+(X>>1); *p=(u16)((*p&0x00FF)|(v<<8)); } X++; }
    volatile u16 *p=row+(X>>1);
    for(; X<X1; X+=2, p++){ int v0=col[sp[X]], v1=col[sp[X+1]];
      if(v0&&v1) *p=(u16)(v0|(v1<<8)); else if(v0) *p=(u16)((*p&0xFF00)|v0); else if(v1) *p=(u16)((*p&0x00FF)|(v1<<8)); }
    if(X==X1){ int v=col[sp[X]]; if(v) *p=(u16)((*p&0xFF00)|v); }
  }
}
static int mini_sprite(const void *spr,int x,int y,int sc,int flip){
  if(spr==SPR_STAR && sc>=1 && sc<=5){ mini_art(MA_STAR1+sc-1,x,y,COL_STAR,0); return 1; }
  if(spr==SPR_BERRY && sc==2){ mini_art(MA_BERRY,x,y,COL_BERRY,0); return 1; }
  if(spr==SPR_ROCK && sc==2){ mini_art(MA_ROCK2,x,y,COL_ROCK,0); return 1; }
  if(spr==SPR_ROCK && sc==3){ mini_art(flip? MA_ROCK3B : MA_ROCK3A,x,y,COL_ROCK,0); return 1; }
  return 0;
}
static int bar_icon(const void *spr,int x,int y,int sc){
  if(sc!=2) return 0;
  int c=CMAP[(u8)'x'], h=c, sh=c;
  if(c==C_LTEXT){ h=C_WHITE; sh=18; } else if(c==C_RED){ h=C_PINK; sh=C_RED2; }
  for(int k=0;k<8;k++) if((u32)spr==BAR_ICON_SPR[k]){ blit_tones(BAR_ICON[k],BAR_ICON_ROWS[k],24,24,x,y,c,h,sh); return 1; }
  if((u32)spr==0x0800ffe4u){ blit_tones(SM_HEART,SM_HEART_ROWS,14,12,x,y,c,h,sh); return 1; }
  if((u32)spr==0x0801001cu){ blit_tones(SM_HEART_EMPTY,SM_HEART_EMPTY_ROWS,14,12,x,y,c,h,sh); return 1; }
  if((u32)spr==0x0800ffc4u){ blit_tones(SM_SKULL,SM_SKULL_ROWS,14,14,x,y,c,h,sh); return 1; }
  return 0;
}
static void sprite(const void *spr,int x,int y,int sc,int flip,int a5){
  if(bar_icon(spr,x,y,sc) || mini_sprite(spr,x,y,sc,flip)) return;
  int f=crit_form(spr);
  if(f<0){ O_SPRITE(spr,x,y,sc,flip,a5); return; }
  f=pet_variant(f);
  if(title_showing()) return;   // the original title's critter: our painted title covers it anyway
  crit_draw(f, G.cpose>0? G.cpose-1 : auto_pose(), x, y, sc, flip, 0);
}
void hook_sprite(const void *spr,int x,int y,int sc,int flip,int a5){ sprite(spr,x,y,sc,flip,a5); }
static int room_pose(int flip){
  u8 h=PET[P_HAPPY];
  if(h>G.hp_prev) G.happy_t=100;
  G.hp_prev=h;
  if(G.happy_t) G.happy_t--;
  int asleep=PET[P_ASLEEP]!=0;
  if(G.was_asleep && !asleep) G.stretch_t=90;          // waking up: a big stretch and a yawn
  G.was_asleep=(u8)asleep;
  if(G.stretch_t) G.stretch_t--;
  if(asleep) return ((FRAMES>>6)&1)? POSE_SLEEP2 : POSE_SLEEP;   // slow sleepy breathing
  if(SCENE==3 && (ANIM_KIND==2 || ANIM_KIND==4)) return POSE_YUCK;   // v8.1: medicine tastes awful, sick or not (it used to show the praised face)
  if(SCENE==3) return flip? (((FRAMES>>3)&1)? POSE_EAT : POSE_HAPPY) : POSE_LOVE;   // flip=1: eating (chomp), flip=0: being praised
  if(G.mode==M_EAT) return ((G.t>>3)&1)? POSE_EAT : POSE_HAPPY;
  if(G.stretch_t) return POSE_STRETCH;
  if(G.happy_t) return G.happy_t>55? POSE_LOVE : POSE_HAPPY;
  if(PET[P_CALL] && (FRAMES%200u)<72u) return ((FRAMES>>3)&1)? POSE_WAVE1 : POSE_WAVE2;   // waving for attention
  if(G.walk_t) return ((FRAMES>>3)&1)? POSE_WALK1 : POSE_WALK2;
  if(PET[P_SICK] || PET[P_HUNGER]==0 || h==0) return POSE_SAD;   // the sad face shows whenever it stands still
  return auto_pose();
}
// the room's critter draw call is routed here so we know where the critter stands
void hook_room_critter(const void *spr,int x,int y,int sc,int flip,int a5){
  int f=crit_form(spr);
  if(f==0){   // the egg: the original jiggles it 12px every couple of frames; draw that as a slow, gentle rock instead
    if(x==G.prev_cx){ if(G.still<255) G.still++; } else { G.still=0; G.egg_t=50; }
    if(G.still>=6) G.egg_rest=(s16)x;
    G.prev_cx=x; G.walk_t=0; G.facing=0;
    int pose=room_pose(flip), ex=G.egg_rest? G.egg_rest : x;
    if(pose==POSE_BLINK) pose=POSE_IDLE;                 // no little twitch every few seconds
    if(G.egg_t || (G.egg_ph&31)){                        // finish the swing back to the middle before stopping
      if(G.egg_t) G.egg_t--;
      G.egg_ph++; int s=SINE16[(G.egg_ph>>2)&15];        // one full rock every 64 frames
      ex+=s>>2;
      if(pose==POSE_IDLE || pose==POSE_BREATHE) pose= s>3? POSE_WALK2 : s<-3? POSE_WALK1 : POSE_IDLE;
    }
    G.room_cx=ex; G.room_cy=y;
    crit_draw(0, pose, ex, y, sc, 0, PAL[C_WALL]!=RGB(30,27,22));
    return;
  }
  int dx=x-G.prev_cx;
  if(dx && dx>-8 && dx<8){ G.walk_t=12; G.facing=(u8)(dx>0); } else if(G.walk_t) G.walk_t--;   // the game moves the critter around: step, and face where it goes
  G.prev_cx=x;
  G.room_cx=x; G.room_cy=y;
  if(f<0){ O_SPRITE(spr,x,y,sc,flip,a5); return; }
  crit_draw(pet_variant(f), room_pose(flip), x, y, sc, G.facing, PAL[C_WALL]!=RGB(30,27,22));
}
// the room renderer's background copy is routed here: yard outside, sky in the window, decor inside
static void garden_still(void);
static void build_bg(int want){
  if(want>=3){ prerender_yard(want==4); u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)YARD_BG;
    garden_still();   // v8.1: the flowers first, so the decor (nearer the front) is drawn over them
    u8 o=G.outside; G.outside=1; draw_decor(0); G.outside=o; DRAW_TARGET=sv; }
  else { O_DMACOPY(YARD_BG, BGX_ROOM); G.bgx_yard=0; u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)YARD_BG;   // same picture day and night: the palette darkens it
    { int base=C_PIC; const u8 *q=PIC_PIX; for(int y=0;y<PIC_H;y++) for(int x=0;x<PIC_W;x++) pset(181+x,43+y,base+*q++); }   // the wall picture
    u8 o=G.outside; G.outside=0; draw_decor(0); G.outside=o; DRAW_TARGET=sv; }
  G.bg_state=(u8)want;
}
void hook_bg_copy(void *dst, const void *src){
  int want = G.outside? 3+is_night() : (src==(const void*)BG_ROOM_NIGHT? 2 : 1);
  portrait_check();
  if(G.bg_state!=want) build_bg(want);
  O_DMACOPY(dst, YARD_BG);
  if(G.outside){ draw_sky(0); if(SCENE!=3) garden_draw(G.yard_night); draw_decor(1); draw_outside_extras(); yard_poops(); }
  else { win_open_pre(); draw_sky(1); win_open_post(); draw_decor(1); lantern_draw(); }
}

// ---- HOW TO PLAY pages (drawn in the original's style)
static const char *const HELP_TITLE[HELP_PAGES]={"HOW TO PLAY","THE ICONS","GROWING UP","OUTSIDE AND THE SHOP","SAYING GOODBYE"};
static const char *const HELP_TEXT[HELP_PAGES][8]={
  {"RAISE A CRITTER FROM A STAR-SEED!","","LEFT/RIGHT/UP/DOWN: PICK AN ICON","A: USE IT     B: CLOSE / BACK",
   "START: MENU (SPEED, SOUND, SAVE)","SELECT: OUTSIDE, SHOP, BAG, JOURNAL","WHEN THE ! ICON LIGHTS UP,","YOUR CRITTER NEEDS SOMETHING."},
  {"FEED  - MEALS FILL HUNGER, SNACKS","        CHEER IT UP (NOT TOO MANY!)","LIGHTS- TURN THEM OFF AT BEDTIME","PLAY  - PICK 1 OF 4 GAMES, WIN COINS",
   "MEDS  - CURE IT WHEN IT'S SICK","CLEAN - WASH AWAY THE POOP","STATUS- SEE HOW IT'S FEELING","PET   - ANSWER WHEN IT CALLS YOU"},
  {"HOW WELL IT IS CARED FOR PICKS","ITS TEEN. THE CARE IT GETS MOST","(OR MISSES MOST) PICKS WHAT IT","GROWS INTO - SEE THE STATUS CARD.",
   "20 FORMS IN THE JOURNAL!","DIRTY ROOMS AND SNACK BINGES","MAKE IT SICK. NEGLECT IT TOO LONG","AND IT RUNS AWAY, WITH NO STAR."},
  {"ONCE IT'S A CHILD, IT CAN GO OUT","TO THE YARD: TOYS, FRESH AIR AND","COINS IN THE GRASS. WHILE IT","SLEEPS, SELECT: GARDEN TO GROW",
   "MOONFLOWERS. R: CATCH FIREFLIES.","THE SHOP SELLS FOOD, TOYS AND","DECOR, AND MORE COMES AS IT GROWS.","BAG: PLACE DECOR AND USE ITEMS."},
  {"AFTER ABOUT 8 YEARS AS A GROWN-UP","IT RISES TO BECOME A STAR. CARE","DECIDES HOW BRIGHTLY IT SHINES:","80%+ WONDERFUL: +35 COINS",
   "60%+ HAPPY: +20 COINS","UNDER 60% SAD: DIM, NO BONUS","THEN A NEW STAR-SEED FALLS. SEE","EVERY STAR IN THE JOURNAL."},
};
static void help_draw(void){
  int pg=G.help_page; if(pg>=HELP_PAGES) pg=0;
  fill(0,0,240,160,C_BAR); fill(0,22,240,1,C_BORDER);
  texts(HELP_TITLE[pg],120-(slen(HELP_TITLE[pg])*6-1)/2,8,C_ORANGE);
  for(int i=0;i<8;i++) text(HELP_TEXT[pg][i],12,33+i*13,C_LTEXT);
  char b[16]; char *p=b; *p++='<'; *p++=' '; p=sint(p,pg+1); *p++='/'; p=sint(p,HELP_PAGES); *p++=' '; *p++='>'; *p=0;
  textc(b,120,148,C_HINT);
}
// ---- title extras: the star album and the second NEW EGG check
static void album_draw(void){
  DRAW_TARGET=O_BACKBUF();
  fill(0,0,240,160,C_BAR); fill(0,26,240,2,C_BORDER); fill(0,140,240,2,C_BORDER);
  u32 sd=4242; for(int i=0;i<50;i++){ sd=sd*1103515245u+12345u; int x=(int)((sd>>16)%240u), y=28+(int)((sd>>8)%112u); pset(x,y,(i&3)? C_HINT : C_LTYEL); }
  sprite(SPR_STAR,8,7,2,0,99); texts("STAR ALBUM",22,9,C_GOLD);
  char b[24]; char *p=scpy(b,"STARS: "); sint(p,G.life.retired); texts(b,232-slen(b)*6,9,C_LTEXT);
  int n=G.album.n, pages=(n+7)/8; if(pages<1) pages=1; if(G.cur>=pages) G.cur=0;
  for(int i=0;i<8;i++){
    int e=G.cur*8+i; if(e>=n) break;
    int col=i&3, row=i>>2, cx=30+col*60, y=32+row*54, f=G.album.form[e];
    if(f>=N_FORMS) f=0;
    const char *fn = form_name(f);
    album_critter(f, cx-16, y);
    { char nb[8]; int k=0; for(;k<NAME_LEN && G.names.alb[e][k];k++) nb[k]=G.names.alb[e][k]; nb[k]=0;
      textc(k? nb : NAME_STR(G.album.name[e]),cx,y+34,C_LTEXT); } textc(fn,cx,y+43,C_HINT);
  }
  if(!n) textc("YOUR STARS FROM EARLIER",120,70,C_LTEXT);
  int older=G.life.retired-n; if(n && older>0){ p=scpy(b,"+"); p=sint(p,older); scpy(p," EARLIER STARS"); textc(b,120,132,C_HINT); }
  p=b; if(pages>1){ p=scpy(b,"PAGE "); p=sint(p,G.cur+1); p=scpy(p,"/"); p=sint(p,pages); scpy(p,"  LEFT/RIGHT   "); } else *p=0;
  char l[48]; p=scpy(l,b); scpy(p,"B: BACK"); textc(l,120,148,C_HINT);
}
// ================================================================== BESTIARY
static void blit_mono(const u8 *pix,const u8 *rows,int S,int x,int y,int c){   // a silhouette for critters not met yet
  for(int r=0;r<S;r++){ int a=rows[2*r], b=rows[2*r+1]; if(a==255) continue;
    for(int X=a;X<=b;X++) if(pix[r*S+X]) pset(x+X,y+r,c); }
}
static void dex_text(const char *s,int x,int y,int c){   // '|' starts a new line
  char b[32]; int n=0, yy=y;
  for(;;s++){ if(*s=='|' || !*s){ b[n]=0; text(b,x,yy,c); yy+=10; n=0; if(!*s) break; } else if(n<31) b[n++]=*s; }
}
static int dex_count(void){ int n=0; for(int i=0;i<DEX_N;i++) if((G.dex.seen>>DEX_ORDER[i])&1) n++; return n; }
static int dex_secrets(int *total){ int n=0, t=0; for(int i=0;i<DEX_N;i++){ int f=DEX_ORDER[i]; if(FORMS[f].stage!=ST_SECRET) continue; t++; if((G.dex.seen>>f)&1) n++; } *total=t; return n; }
static const u8 FORM_STAR[23]={C_GOLD,C_GOLD,C_GREEN,C_PINK,C_TEAL,C_GOLD,C_GREEN,C_BLUE,88,89,7,C_LTGREEN,C_GOLD,
  C_PINK,C_LILAC,C_GREEN,30,C_LILAC,12,70,C_PINK,C_PALEBLUE,C_RED};
// ================================================================== JOURNAL (v8 phase 6): NIGHT SKY, FIELD GUIDE, RECORDS
static const char *const JTAB[N_JTAB]={"SKY","GUIDE","ITEMS","GARDEN","FIREFLIES","RECORDS"};
// v8.1: the tab strip scrolls when the tabs don't all fit; G.jt_first is the first one shown, with arrows at the ends
#define JTAB_GAP 14
static int jtab_last(void){   // the last tab that fits after jt_first
  int x= G.jt_first? 16 : 8, last=G.jt_first;
  for(int i=G.jt_first;i<N_JTAB;i++){ int w=slen(JTAB[i])*6; if(x+w > (i<N_JTAB-1? 226 : 232)) break; last=i; x+=w+JTAB_GAP; }
  return last;
}
static void jtab_view(void){
  if(G.jt_first>=N_JTAB) G.jt_first=0;
  if(G.jtab<G.jt_first) G.jt_first=G.jtab;
  while(G.jtab>jtab_last()) G.jt_first++;
}
static const char *const LOVED_NAME[4]={"A BIT OF ALL","OUTSIDE","PLANTS","PLAY"};
static void dex_open(int from){
  G.mode=M_DEX; G.dex_from=(u8)from; G.dex_cur=0; int f=form_index(); if(PET[P_STAGE]<=7) for(int i=0;i<DEX_N;i++) if(DEX_ORDER[i]==f) G.dex_cur=(u8)i;
  G.jtab=(u8)(from==2? JT_GARDEN : from || !G.jr.n? 1 : 0); G.jt_first=0; G.jstar=(u8)(G.jr.n? G.jr.n-1 : 0); G.jcare=0; sfx(0,SEQ_JOURNAL); }
static void dex_update(u16 pr,u16 keys){
  if(pr&(K_L|K_R)){ G.jtab=(u8)((G.jtab+((pr&K_R)? 1 : N_JTAB-1))%N_JTAB); sfx(0,SEQ_BLIP); }
  else if(G.jtab==0){ int n=G.jr.n;
    if(n && (pr&K_LEFT)){ G.jstar=(u8)(G.jstar? G.jstar-1 : n-1); sfx(0,SEQ_BLIP); }
    if(n && (pr&K_RIGHT)){ G.jstar=(u8)(G.jstar+1>=n? 0 : G.jstar+1); sfx(0,SEQ_BLIP); } }
  else if(G.jtab==1){
    if(pr&K_LEFT){ G.dex_cur=(u8)(G.dex_cur? G.dex_cur-1 : DEX_N-1); sfx(0,SEQ_BLIP); }
    if(pr&K_RIGHT){ G.dex_cur=(u8)(G.dex_cur+1>=DEX_N? 0 : G.dex_cur+1); sfx(0,SEQ_BLIP); }
    if(pr&K_UP){ int i=G.dex_cur; do i=i? i-1 : DEX_N-1; while(!((G.dex.seen>>DEX_ORDER[i])&1)); G.dex_cur=(u8)i; sfx(0,SEQ_BLIP); }      // skip to met ones
    if(pr&K_DOWN){ int i=G.dex_cur; do i=i+1>=DEX_N? 0 : i+1; while(!((G.dex.seen>>DEX_ORDER[i])&1)); G.dex_cur=(u8)i; sfx(0,SEQ_BLIP); }
    if(pr&K_A){ G.jcare^=1; sfx(0,SEQ_BLIP); } }
  else if(G.jtab==JT_GARDEN){ if(G.gdetail && (pr&K_B)){ G.gdetail=0; sfx(0,SEQ_BLIP); return; } jr_garden_keys(pr); }
  else if(G.jtab==JT_FLY) jr_fly_keys(pr);
  else if(G.jtab==JT_RECORDS){   // v8.2c: it scrolls
    if((pr&K_UP) && G.jrec){ G.jrec--; sfx(0,SEQ_BLIP); }
    if((pr&K_DOWN) && G.jrec<REC_ROWS-REC_SHOW){ G.jrec++; sfx(0,SEQ_BLIP); } }
  else if(G.jtab==JT_ITEMS){
    if(pr&K_LEFT){ G.jitem=(u8)(G.jitem? G.jitem-1 : IT_COUNT-1); sfx(0,SEQ_BLIP); }
    if(pr&K_RIGHT){ G.jitem=(u8)(G.jitem+1>=IT_COUNT? 0 : G.jitem+1); sfx(0,SEQ_BLIP); }
    if((pr&(K_UP|K_DOWN)) && found_count()){ int i=G.jitem, d=(pr&K_UP)? IT_COUNT-1 : 1;   // skip to found ones
      do i=(i+d)%IT_COUNT; while(!found_has(COL_ORDER[i])); G.jitem=(u8)i; sfx(0,SEQ_BLIP); } }
  if(pr&(K_B|K_SELECT|K_START)){ sfx(0,SEQ_BLIP);
    if(G.dex_from==2){ G.mode=M_GARDEN; G.gui_key=0; G.bg_state=0; }   // v8.1: back out to the yard
    else if(G.dex_from==3){ G.mode=M_EXTRAS; G.cur=3; }               // v8.1: back to the SELECT menu
    else if(G.dex_from){ G.mode=M_MENU; G.cur=5; } else G.mode=M_NONE;
    G.suppress=keys; }
}
static void jr_frame(int x,int y,int w,int h,int c){ fill(x,y,w,1,c); fill(x,y+h-1,w,1,c); fill(x,y,1,h,c); fill(x+w-1,y,1,h,c); }
static void jr_header(const char *right,const char *hint){
  dma_fill(0,0,240,27,C_BAR); dma_fill(0,139,240,21,C_BAR); dma_fill(0,28,240,110,C_BORDER); dma_fill(0,27,240,1,C_OUTLINE); dma_fill(0,138,240,1,C_OUTLINE);
  u32 sd=909; for(int i=0;i<34;i++){ sd=sd*1103515245u+12345u; pset((int)((sd>>16)%236u)+2, 30+(int)((sd>>8)%104u), (i%5)? C_HINT : C_LTEXT); }
  texts("JOURNAL",8,3,C_ORANGE); if(right) texts(right,232-slen(right)*6,3,C_LTEXT);
  jtab_view();
  { int last=jtab_last(), x= G.jt_first? 16 : 8;
    for(int i=G.jt_first;i<=last;i++){ int w=slen(JTAB[i])*6;
      if(i==G.jtab){ fill(x-4,13,w+7,13,C_OUTLINE); fill(x-3,14,w+5,11,C_CREAM); }
      text(JTAB[i],x,16,i==G.jtab? C_RED : C_LTEXT); x+=w+JTAB_GAP; }
    if(G.jt_first) text("<",2,16,C_ORANGE);
    if(last<N_JTAB-1) text(">",233,16,C_ORANGE); }
  textc(hint,120,147,C_HINT);
}
static int story_kind(const Story *s){ return (s->info>>2)&3; }
static int story_colour(const Story *s){ return s->form<23? FORM_STAR[s->form] : C_GOLD; }
static void jr_star(int x,int y,int c,int kind,int tw){   // a wonderful farewell shines brightest, a sad one dim
  if(kind==E_SAD){ pset(x,y,c); pset(x-1,y,C_HINT); pset(x+1,y,C_HINT); pset(x,y-1,C_HINT); pset(x,y+1,C_HINT); return; }
  int r= kind==E_WONDER? 5 : 3;
  for(int k=1;k<=r;k++){ int cc= k>r-2? C_HINT : c; pset(x-k,y,cc); pset(x+k,y,cc); pset(x,y-k,cc); pset(x,y+k,cc); }
  fill(x-1,y-1,3,3,c); pset(x,y,C_WHITE);
  if(kind==E_WONDER){ pset(x-2,y-2,C_HINT); pset(x+2,y-2,C_HINT); pset(x-2,y+2,C_HINT); pset(x+2,y+2,C_HINT);
    pset(x-1,y,C_WHITE); pset(x+1,y,C_WHITE); pset(x,y-1,C_WHITE); pset(x,y+1,C_WHITE);
    if(tw){ pset(x-6,y,C_WHITE); pset(x+6,y,C_WHITE); pset(x,y-6,C_WHITE); pset(x,y+6,C_WHITE); } }
}
static int jr_sx(int i){ return 18+(i%10)*23; }
static int jr_sy(int i){ return 42+(int)(((u32)i*37u+11u)*29u%42u); }
static void jr_sky(void){
  int n=G.jr.n; char b[24]; char *p=sint(b,n); scpy(p, n==1? " STAR" : " STARS");
  jr_header(b, n? "LEFT/RIGHT: STAR   L/R: TAB   B: BACK" : "L/R: TAB   B: BACK");
  if(!n){ textc("NO STARS YET",120,58,C_LTEXT); textc("EVERY CRITTER THAT SAYS FAREWELL",120,74,C_HINT); textc("RISES TO BECOME A STAR HERE.",120,84,C_HINT); return; }
  if(G.jstar>=n) G.jstar=(u8)(n-1);
  int pg=G.jstar/10, a=pg*10, z=a+10<n? a+10 : n;
  for(int i=a;i+1<z;i++){ int x0=jr_sx(i), y0=jr_sy(i), x1=jr_sx(i+1), y1=jr_sy(i+1);   // joined in order, like a constellation
    for(int t=5;t<=27;t+=3) pset(x0+(x1-x0)*t/32, y0+(y1-y0)*t/32, C_SLATE); }
  for(int i=a;i<z;i++){ const Story *s=&G.jr.s[i]; jr_star(jr_sx(i),jr_sy(i),story_colour(s),story_kind(s),((G.sky_t+i*29)>>3)%10==0); }
  { int x=jr_sx(G.jstar), y=jr_sy(G.jstar); if((G.sky_t>>4)&1 || 1) jr_frame(x-8,y-8,17,17,C_LTEXT); }
  if(n>10){ char *q=sint(b,pg+1); q=scpy(q,"/"); sint(q,(n+9)/10); text(b,232-slen(b)*6,30,C_HINT); }
  // its story
  const Story *s=&G.jr.s[G.jstar]; int f=s->form<N_FORMS? s->form : 0, k=story_kind(s);
  quick_box(6,94,228,42);
  album_critter(f,12,99);
  char nm[NAME_LEN+1]; int c=0; for(;c<NAME_LEN && s->name[c];c++) nm[c]=s->name[c]; nm[c]=0;
  text(nm,50,100,C_DTEXT); text(form_name(f),50+7*6,100,C_RED);
  if(!(s->info&2) && f) text((s->info&1)? "GIRL" : "BOY",50+17*6,100,(s->info&1)? C_RED : C_BLUE);
  if(s->days){ p=sint(b,s->days); p=scpy(p, s->days==1? " YEAR - LOVED: " : " YEARS - LOVED: "); scpy(p, LOVED_NAME[s->loved&3]); text(b,50,111,C_DTEXT); }
  else text("A STAR FROM BEFORE THE JOURNAL",50,111,C_GRAY);
  text(k==E_WONDER? "A WONDERFUL FAREWELL" : k==E_GOOD? "A HAPPY FAREWELL" : "A SAD GOODBYE",50,122, k==E_WONDER? C_GREEN : k==E_GOOD? C_ORANGE : C_GRAY);
}
// the grown-ups' secret forms, for the field guide's care page
static int adult_secret(int f){ switch(f){ case F_OAKHART: return 8; case F_DUNEROSE: return 11; case F_FLUFFBUD: return 10; case F_FRILLO: return 12;
  case F_THORNHOG: return F_JAMBLE; case F_SMOGWOOL: return F_PETALAMB; case F_GUNKBILL: return F_LILYBILL; } return -1; }
static void jr_guide(void){
  char b[40]; char *p=scpy(b,"MET "); p=sint(p,dex_count()); p=scpy(p,"/"); sint(p,DEX_N);
  jr_header(b,"LEFT/RIGHT: TURN  A: CARE  L/R: TAB");
  int n=G.dex_cur, f=DEX_ORDER[n], met=(G.dex.seen>>f)&1; const Form *d=&FORMS[f];
  for(int yy=0;yy<9;yy++){ int w=34-(yy*yy)/3; fill(46-w,122+yy,2*w,1,yy<3? C_SLATE : C_OUTLINE); }   // a little spotlight
  if(met){ G.cpose=0; crit_draw(f, auto_pose(), 14, 68, 4, 0, 0); }
  else blit_mono(CR_PIX+CR_PIXOFF[f][POSE_IDLE][3], CR_ROWS+2*CR_ROWOFF[f][POSE_IDLE][3], 64, 14, 65, C_SLATE);
  quick_box(88,32,148,104);
  p=scpy(b,"NO."); if(n+1<10) p=scpy(p,"0"); sint(p,n+1); text(b,96,38,C_HINT);
  text(met? form_name(f) : "???",134,38,C_RED);
  p=scpy(b,STAGE_NAME[d->stage]); scpy(p,SIDE_NAME[d->side]); text(b,96,49,d->side==2? C_BROWN2 : d->side==1? C_GREEN : C_LILAC);
  if(f){ int by=(G.dex.boy>>f)&1, gi=(G.dex.girl>>f)&1;
    text("RAISED:",96,60,C_DTEXT); text("BOY",144,60,by? C_BLUE : C_GRAY); text("GIRL",170,60,gi? C_RED : C_GRAY);
    if(!by && !gi) text("-",200,60,C_GRAY); }
  fill(96,70,132,1,C_HINT);
  if(!met){ if(RIDDLE[f]){ text("A RIDDLE:",96,74,C_LILAC); dex_text(RIDDLE[f],96,85,C_GRAY); } else dex_text(d->hint,96,74,C_GRAY); return; }
  if(!G.jcare){ dex_text(d->lore,96,74,C_DTEXT); return; }
  text("HOW IT GROWS:",96,74,C_GREEN); dex_text(d->hint[0]? d->hint : "A STORY FROM AN OLDER|SEED.",96,85,C_DTEXT);
  int sf=adult_secret(f);
  if(sf>=0){ int got=(G.dex.seen>>sf)&1; text("SECRET:",96,124,C_LILAC); text(got? form_name(sf) : "? ? ?",144,124,got? C_LILAC : C_GRAY); }
}
static char *jr_num(char *p,u32 v){   // 1,240
  if(v>=1000){ p=sint(p,(int)(v/1000)); *p++=','; u32 r=v%1000; *p++=(char)('0'+r/100); *p++=(char)('0'+r/10%10); *p++=(char)('0'+r%10); *p=0; return p; }
  return sint(p,(int)v); }
// v8.2c: the RECORDS tab scrolls (UP/DOWN): REC_SHOW of its REC_ROWS rows at a time, with arrows when there's more
static void jr_records(void){
  jr_header(0,"UP/DOWN: SCROLL   L/R: TAB");
  const Journal *j=&G.jr; char v[REC_ROWS][20]; const char *lab[REC_ROWS]; u8 col[REC_ROWS]; char *p; int t, n=0;
  #define ROW(L,C) (lab[n]=(L), col[n]=(u8)(C), v[n++])
  jr_num(ROW("CRITTERS RAISED",C_DTEXT),j->raised);
  sint(ROW("WONDERFUL IN A ROW",G.life.retired? C_RED : C_DTEXT),G.life.retired);
  p=sint(ROW("FORMS FOUND",C_DTEXT),dex_count()); p=scpy(p," / "); sint(p,DEX_N);
  { int sn=dex_secrets(&t); p=sint(ROW("SECRETS FOUND",C_DTEXT),sn); p=scpy(p," / "); sint(p,t); }
  p=sint(ROW("ITEMS FOUND",C_DTEXT),found_count()); p=scpy(p," / "); sint(p,IT_COUNT);
  jr_num(ROW("WONDERFUL FAREWELLS",C_DTEXT),j->wonders);
  jr_num(ROW("GAMES PLAYED",C_DTEXT),j->games);
  jr_num(ROW("WISHES MADE",C_DTEXT),G.more.wishes);
  { int m=-1, best=0; for(int k=1;k<4;k++) if(j->loved[k]>best){ best=j->loved[k]; m=k; } if(m<0 && j->loved[0]) m=0;
    scpy(ROW("MOST LOVED CARE",C_DTEXT), m>=0? LOVED_NAME[m] : "-"); }
  jr_num(ROW("COINS EARNED",C_DTEXT),j->coins);
  jr_num(ROW("FIREFLIES CAUGHT",C_DTEXT),G.more.spare[4]);
  sint(ROW("BEST FIREFLY NIGHT",C_DTEXT),G.more.spare[5]);
  #undef ROW
  int top= G.jrec>n-REC_SHOW? n-REC_SHOW : G.jrec;
  quick_box(14,28,212,106);
  for(int k=0;k<REC_SHOW;k++){ int i=top+k, y=32+k*10; text(lab[i],24,y,C_DTEXT); text(v[i],206-slen(v[i])*6,y,col[i]); }
  if(top>0) for(int r=0;r<4;r++) fill(216-r,32+r,1+2*r,1,C_HINT);                      // more above
  if(top+REC_SHOW<n) for(int r=0;r<4;r++) fill(216-r,129-r,1+2*r,1,C_HINT);             // more below
}
static void icon_big(int it,int cx,int cy,int mono){   // an item icon at double size (or its silhouette)
  const Icon *I=&ICONS[it]; const u8 *pix=ICON_PIX+I->pix, *rows=ICON_ROWS+2*I->row; int x0=cx-I->w, y0=cy-I->h;
  for(int r=0;r<I->h;r++, pix+=I->w){ int a=rows[2*r], z=rows[2*r+1]; if(a==255) continue;
    for(int x=a;x<=z;x++){ int c=pix[x]; if(!c) continue; if(mono) c=mono; int X=x0+2*x, Y=y0+2*r;
      pset(X,Y,c); pset(X+1,Y,c); pset(X,Y+1,c); pset(X+1,Y+1,c); } }
}
static const char *item_spot(int it){
  if(it<N_FOODS) return "FOOD - FROM THE BAG"; if(it<N_CONSUMABLE) return "CARE - FROM THE BAG"; if(IS_TOY(it)) return "YARD TOY";
  switch(DECOR_SLOT(it)){ case 0: case 1: return "ROOM - FLOOR"; case 2: return "ROOM - WALL"; case 3: return "ROOM - CEILING"; } return "YARD DECOR"; }
static void jr_items(void){
  char b[40]; char *p=scpy(b,"FOUND "); p=sint(p,found_count()); p=scpy(p,"/"); sint(p,IT_COUNT);
  jr_header(b,"LEFT/RIGHT: TURN   L/R: TAB");
  int n=G.jitem, it=COL_ORDER[n], got=found_has(it);
  for(int yy=0;yy<9;yy++){ int w=34-(yy*yy)/3; fill(46-w,122+yy,2*w,1,yy<3? C_SLATE : C_OUTLINE); }   // the same little spotlight
  icon_big(it,46,90,got? 0 : C_SLATE);
  quick_box(88,32,148,104);
  p=scpy(b,"NO."); if(n+1<10) p=scpy(p,"0"); sint(p,n+1); text(b,96,38,C_HINT);
  text(got? ITEMS[it].name : "???",134,38,C_RED);
  text(item_spot(it),96,49,C_GREEN);
  p=scpy(b,"CARE: "); scpy(p,KIND_NAME[ITEM_KIND[it]]); text(b,96,60,C_LILAC);
  if(got){ sint(b,ITEMS[it].price); int w=slen(b)*6, x=228-w-10;   // a plain coin: the usual icon's colours are lent to the item icons here
    fill(x+2,60,3,7,C_ORANGE); fill(x+1,61,5,5,C_ORANGE); fill(x,62,7,3,C_ORANGE); fill(x+3,62,1,3,C_BROWN2);
    text(b,228-w,60,C_ORANGE); }
  fill(96,70,132,1,C_HINT);
  dex_text(got? COL_TEXT[it] : "SOLD IN THE SHOP.|BUY ONE TO ADD ITS|PAGE TO THE JOURNAL.",96,74,got? C_DTEXT : C_GRAY);
}

// ---- the journal's GARDEN tab: every moonflower and visitor found (kept forever)
static const char *const GK_TEXT[GK_N]={
  "GROWS FOR A CRITTER|THAT IS WELL CARED|FOR, ALL ROUND.", "GROWS FOR A CRITTER|THAT PLAYS A LOT.",
  "GROWS FOR A CRITTER|THAT SPENDS LOTS OF|TIME OUTSIDE.", "GROWS FOR A CRITTER|KEPT NICE AND CLEAN.", "GROWS FOR A CRITTER|THAT IS WELL FED.",
  "PLANT IT ON A NIGHT|YOU WISHED ON A|SHOOTING STAR|(1 IN 2).", "PLANT IT ON THE|NIGHT OF A FULL MOON|(1 IN 3).",
  "WATER IT EVERY|NIGHT TILL IT BLOOMS|(1 IN 5). ITS TWO|COLOURS ARE NEW|EVERY TIME!", "ANY MOONFLOWER MAY|BLOOM AS AN AURORA:|1 IN 20." };
static const char *const GK_RIDDLE[4]={"WHAT FALLS, IS|WISHED ON, AND GROWS?","IT WAITS FOR THE|NIGHT WITH NO|MISSING PIECE.",
  "NOT ONE NIGHT|FORGOTTEN, NOT ONE|NIGHT DRY.","NO RIDDLE CAN TELL|YOU. ONLY LUCK."};
static const char *const GV_TEXT[GV_N]={"FLUTTERS ROUND AND|ROUND THE GLOW.","BLINKS ITS LITTLE|LANTERN BACK AT THE|FLOWERS.",
  "SITS BY THE PATH,|PUFFING AND HOPPING.","TRUNDLES BY ON ITS|NIGHT ROUNDS,|SNIFFING.","SWOOPS OVER THE|YARD AFTER MOTHS.",
  "KEEPS WATCH FROM|THE FENCE ALL NIGHT.","TAKES ALL NIGHT TO|CROSS THE GARDEN.","CHIRPS A SONG FOR|THE MOONFLOWERS.",
  "A RARE, PALE GREEN|MOTH THAT GLOWS|LIKE MOONLIGHT.","A RARE, SHY GUEST.|IT ONLY COMES WHEN|TWO OR MORE GLOW."};
static void flower_head(int k,int x,int y,int t){   // a flower's head, 11x11 at (x,y), for the journal
  if(k<5){ u8 pet[4]={GP_PETAL[k][0],GP_PETAL[k][1],0,0}; gdraw(GS_BLOOM0,x,y,pet,0); }
  else if(k==GK_STAR) gdraw(GS_STAR0,x,y,GP_PETAL[GK_GOLD],0);
  else if(k==GK_MOON) gdraw(GS_MOON0,x-1,y-1,0,0);
  else if(k==GK_AURORA) gdraw_aurora(x,y,t);
  else { int a=(t>>6)%5, b=(a+2)%5; u8 tw[4]={GP_PETAL[a][0],GP_PETAL[a][1],GP_PETAL[b][0],GP_PETAL[b][1]}; gdraw(GS_TWIN,x-1,y+1,tw,0); }   // its colours change
}
static void jr_garden_card(int i,int sel,int t){
  int x,y,w; if(i<GK_N){ x=12+i*24; y=40; w=22; } else { int j=i-GK_N; x=10+(j%5)*44; y= j<5? 79 : 108; w=42; }
  int got=g_found(i), moving= got && (i>=GK_N || i==GK_TWIN || i==GK_AURORA);
  if(!G.g_anim){
    fill(x,y,w,27,C_OUTLINE); fill(x+1,y+1,w-2,25,sel? C_HINT : C_BAR);
    if(sel){ fill(x-1,y-1,w+2,1,C_CREAM); fill(x-1,y+27,w+2,1,C_CREAM); fill(x-1,y,1,27,C_CREAM); fill(x+w,y,1,27,C_CREAM); } }
  if(moving!=G.g_anim) return;   // (still things go in the kept picture, moving ones are drawn each frame)
  if(i<GK_N){ if(got) flower_head(i,x+5,y+8,t); else text("?",x+8,y+10,C_LTEXT); return; }
  int v=i-GK_N; const GSpr *s=&GSPR[GVIS[v].first]; int fi= got? GVIS[v].seq[(t/8)%GVIS[v].seq_n] : 0, id=GVIS[v].first+fi;
  const GSpr *f=&GSPR[id]; (void)s;
  gdraw(id, x+w/2-f->w/2, y+26-f->h - (f->h<20? (26-f->h)/2 : 0), 0, got? 0 : C_SLATE);
}
static void jr_garden_big(int n,int t);
static void jr_garden_page(void){
  char b[40]; char *p=scpy(b,"FOUND "); p=sint(p,g_found_count()); p=scpy(p,"/"); sint(p,G_FOUND_N);
  int n=G.gpad, t=(int)G.sky_t;
  if(!G.gdetail){
    if(!G.g_anim){ jr_header(b,"D-PAD: PICK   A: LOOK   L/R: TAB"); text("MOONFLOWERS",12,30,GP_Y); text("NIGHT VISITORS",10,69,GP_Y); }
    for(int i=0;i<G_FOUND_N;i++) jr_garden_card(i,i==n,t);
    return; }
  if(G.g_anim){ jr_garden_big(n,t); return; }
  jr_header(b,"LEFT/RIGHT: TURN   B: BACK   L/R: TAB");
  for(int yy=0;yy<9;yy++){ int w=34-(yy*yy)/3; fill(46-w,122+yy,2*w,1,yy<3? C_SLATE : C_OUTLINE); }
  int got=g_found(n);
  quick_box(88,32,148,104);
  p=scpy(b,"NO."); if(n+1<10) p=scpy(p,"0"); sint(p,n+1); text(b,96,38,C_HINT);
  if(n<GK_N){
    text(got? (n<5? "MOONFLOWER" : GK_NAME[n]) : "???",134,38,C_RED);
    if(n<5 && got) text(GK_NAME[n],96,49,GP_K+10); else text(n<5? "MOONFLOWER" : "RARE MOONFLOWER",96,49,n<5? GP_K+10 : C_LILAC);   // (GP_K+10: the garden's green - C_GREEN dims with the lights)
    if(n>=5 && got) text("RARE MOONFLOWER",96,49,C_LILAC);
    fill(96,60,132,1,C_HINT);
    if(n>=5 && !got){ text("A RIDDLE:",96,64,C_LILAC); dex_text(GK_RIDDLE[n-5],96,75,C_GRAY); }
    else dex_text(GK_TEXT[n],96,64,got? C_DTEXT : C_GRAY);
    if(!got) text("?",43,70,C_LTEXT);
    return; }
  int v=n-GK_N; const GVis *V=&GVIS[v];
  text(got? V->name : "???",134,38,C_RED);
  text(V->rare? "RARE VISITOR" : "NIGHT VISITOR",96,49,V->rare? C_LILAC : GP_K+10);
  fill(96,60,132,1,C_HINT);
  dex_text(got? GV_TEXT[v] : V->rare? "A RARE GUEST..." : "COMES WHEN THE|FLOWERS GLOW.",96,64,got? C_DTEXT : C_GRAY);
  if(!got){ int id=V->first; const GSpr *s=&GSPR[id]; const u8 *q=GPIX+s->off; int ox=46-s->w, oy=120-s->h*2;   // its silhouette
    for(int r=0;r<s->h;r++) for(int c=0;c<s->w;c++){ int c2=*q++; if(c2) fill(ox+c*2,oy+r*2,2,2,C_SLATE); } }
}
static void jr_garden_big(int n,int t){   // the moving picture on a GARDEN page, at double size
  int got=g_found(n); if(!got) return;
  if(n<GK_N){
    // the flower itself, at double size
    { int k=n, cx=46, cy=78;
      const u8 *pet0=GP_PETAL[k<5? k : GK_GOLD];
      int id= k==GK_STAR? GS_STAR0 : k==GK_MOON? GS_MOON0 : k==GK_TWIN? GS_TWIN : GS_BLOOM0;
      const GSpr *s=&GSPR[id]; const u8 *q=GPIX+s->off; int ox=cx-s->w, oy=98-s->h*2; (void)cy;   // (v8.2c: placed so its stem ends in the soil at 118)
      int a=(t>>6)%5, bb=(a+2)%5; u8 pet[4]={pet0[0],pet0[1],GP_PETAL[bb][0],GP_PETAL[bb][1]}; if(k==GK_TWIN){ pet[0]=GP_PETAL[a][0]; pet[1]=GP_PETAL[a][1]; }
      for(int r=0;r<s->h;r++) for(int c=0;c<s->w;c++){ int v=*q++; if(!v) continue;
        if(v<8){ if(k==GK_AURORA){ int bnd=r*4+(t>>1)+SINE16[(c*2+(t>>3))&15]; v=GP_AURORA[(bnd>>3)&7]; } else v=pet[v-1]; }
        fill(ox+c*2,oy+r*2,2,2,v); }
      gdraw(GS_STEM,cx-5,oy+s->h*2-2,0,0); fill(cx-1,oy+s->h*2+6,2,14,GP_K+9 /* g: the stem's green */); gdraw(GS_MOUND,cx-6,118,0,0); }
    return; }
  int v=n-GK_N; const GVis *V=&GVIS[v];
  { int id=V->first+(got? V->seq[(t/8)%V->seq_n] : 0); const GSpr *s=&GSPR[id]; const u8 *q=GPIX+s->off; int ox=46-s->w, oy=120-s->h*2;
    if(got && v==8) luna_glow(46,oy+s->h,t);
    for(int r=0;r<s->h;r++) for(int c=0;c<s->w;c++){ int c2=*q++; if(!c2) continue; fill(ox+c*2,oy+r*2,2,2,got? c2 : C_SLATE); } }
}
static void jr_garden(void){
  u16 key=(u16)(0x8000 | G.gpad | (G.gdetail<<5));
  if(key!=G.jg_key){ u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG; G.g_anim=0; jr_garden_page(); DRAW_TARGET=sv; G.jg_key=key; }
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG); G.g_anim=1; jr_garden_page(); G.g_anim=0;
}
static void jr_garden_keys(u16 pr){
  int n=G.gpad;
  if(G.gdetail){
    if(pr&K_LEFT){ n= n? n-1 : G_FOUND_N-1; sfx(0,SEQ_BLIP); }
    if(pr&K_RIGHT){ n= n+1>=G_FOUND_N? 0 : n+1; sfx(0,SEQ_BLIP); }
    if(pr&K_A){ G.gdetail=0; sfx(0,SEQ_BLIP); }
    G.gpad=(u8)n; return; }
  if(pr&K_LEFT){ n= n? n-1 : G_FOUND_N-1; sfx(0,SEQ_BLIP); }
  if(pr&K_RIGHT){ n= n+1>=G_FOUND_N? 0 : n+1; sfx(0,SEQ_BLIP); }
  if(pr&K_DOWN){ n= n<GK_N? GK_N+(n*5/GK_N) : n<GK_N+5? n+5 : (n-GK_N-5)*GK_N/5; sfx(0,SEQ_BLIP); }
  if(pr&K_UP){ n= n<GK_N? GK_N+5+(n*5/GK_N) : n<GK_N+5? (n-GK_N)*GK_N/5 : n-5; sfx(0,SEQ_BLIP); }
  if(pr&K_A){ G.gdetail=1; sfx(0,SEQ_CONFIRM); }
  G.gpad=(u8)n;
}
// ================================================================== FIREFLIES (v8.1)
// From the yard at night (R), up to three 20-second rounds: steer a jar and scoop fireflies while they glow. There
// are twelve kinds to find for the journal's field guide: three are out every night, two of five others each night,
// and four rare ones need something from the rest of the game. Catching gives nothing but the find itself: the
// night's catch glows in a lantern on the windowsill and flies home in the morning; all twelve found, and they
// dance at the window every night after. Saved in More.spare[3..7]: [3] kinds found (one bit each), [4] caught in
// all, [5] the most in one night, [6] (day+1) of tonight, [7] rounds used tonight (bits 0-1) | tonight's kinds << 4.
#define FK_N 12
#define FLY_SLOTS 8
#define FLY_ROUND 1200          // 20 s (v8.2b: was 30 s)
#define FLY_ROUNDS 3
#define FLY_ALL 0xFFFu
enum { FS_LOW=1, FS_HIGH=2, FS_ZIG=4, FS_ROSE=8, FS_STAR=16 };
typedef struct { const char *name, *note, *hint; u8 on, gap, pulses, off, speed, style, col, pad; } FKind;
static const FKind FK[FK_N]={
 {"AMBER GLOW","A STEADY, WARM GLOW.",0,              70,0,1,40,  6,0,       250,0},
 {"GREEN FLICKER","QUICK LITTLE BLINKS.",0,            10,10,3,50, 8,0,       254,0},
 {"LEMON DRIFTER","SLOW AND LAZY.",0,                  90,0,1,70,  4,0,       249,0},
 {"PINK TWINKLER","BLINKS TWICE, THEN RESTS.",0,       16,14,2,70, 7,0,       247,0},
 {"BLUE WINKER","SHY. WINKS ONCE, THEN HIDES.",0,      18,0,1,90,  7,0,       252,0},
 {"EMBER","FLIES LOW, NEAR THE GRASS.",0,              50,0,1,50,  6,FS_LOW,  255,0},
 {"MINT DANCER","ZIGZAGS ALL OVER.",0,                 40,0,1,40, 13,FS_ZIG,  253,0},
 {"LILAC DOZER","LONG, SLEEPY GLOWS.",0,               60,0,1,140, 3,0,       246,0},
 {"MOONGLOW","HIGH UP, BRIGHT AS THE MOON.","ONLY ON A FULL MOON.",       120,0,1,30, 5,FS_HIGH, 245,0},
 {"ROSE HEART","IT BEATS LIKE A LITTLE HEART.","LOVES A ROSE IN BLOOM.",   14,10,2,60, 6,FS_ROSE, 248,0},
 {"WISH SPARK","BORN FROM A WISH.","COMES AFTER YOU MAKE A WISH.",        12,0,1,40, 15,FS_ZIG,  251,0},
 {"STARFLY","A LITTLE FALLEN STAR!","VERY RARE. LOVES MANY BLOOMS.",      14,0,1,70, 17,FS_STAR, 249,0},
};
#define FLY_DARK 244
#define FLY_EMBER RGB(31,15,5)
static void fly_palette(void){ for(int i=24;i<GP_N;i++) PAL[GP_IDX[i]]=GP_PAL[i]; PAL[255]=FLY_EMBER; }   // (the room: 244-255 are free)
static u16 fly_found(void){ return (u16)(G.more.spare[3]&FLY_ALL); }
static int fly_count(void){ int n=0; for(u16 b=fly_found(); b; b&=(u16)(b-1)) n++; return n; }
static int rose_blooming(void){ for(int i=0;i<3;i++) if(G.gdn.p[i].stage==4 && G.gdn.p[i].kind==GK_ROSE) return 1; return 0; }
static int g_blooms(void);
static void fly_tonight(void){   // decides tonight's kinds the first time it's asked each night
  if(G.more.spare[6]==(u16)(G.ext.day+1)) return;
  G.more.spare[6]=(u16)(G.ext.day+1);
  u16 m=7;                                                  // the three common ones
  int a=3+(rnd(5)+G.ext.day*2)%5, b=3+(rnd(4)+G.ext.day)%4; if(b>=a) b++; m|=(u16)((1u<<a)|(1u<<b));   // two of the five uncommon ones
  if((G.ext.day&7)==0) m|=1u<<8;                            // a full moon: moonglow
  if(g_blooms()>=2 && rnd(10)==0) m|=1u<<11;               // starfly: about one night in ten, with two or more blooms
  G.more.spare[7]=(u16)(m<<4);
  if(G.fly_night!=(u16)(G.ext.day+1)){ G.fly_night=(u16)(G.ext.day+1); G.fly_n=0; G.fly_kinds=0; }
}
static u16 fly_out(void){   // who's out right now
  u16 m=(u16)(G.more.spare[7]>>4);
  if(rose_blooming()) m|=1u<<9;
  if(G.more.spare[0]==(u16)(G.ext.day+1)) m|=1u<<10;        // a wish made tonight
  return m;
}
// v8.3: fireflies all over the yard at night (sprites, like the release): 15 drift slowly and blink, in tonight's common
// and uncommon colours (never a rare one). Only on the garden screen, and not during a round (those are the catchable ones).
#define YF_N 15
static const u8 YF_BX[YF_N]={14,67,120,173,226,67,120,173,14,67,120,173,226,67,120}, YF_BY[YF_N]={54,83,112,71,100,59,88,117,76,105,64,93,122,81,110};
static void yard_fireflies(void){
  if(G.mode!=M_GARDEN || !is_night()) return;
  fly_tonight();
  u16 m=fly_out()&0xFF; if(!m) return;
  if(m!=G.yf_key){                                            // tonight's colours: a bright and a dim palette each (kept)
    G.yf_key=m; G.yf_nk=0;
    static const u8 GL[5]={0,18,50,100,170};
    for(int k=0;k<8 && G.yf_nk<5;k++) if((m>>k)&1){ u16 kc=PAL[FK[k].col];
      for(int d=0;d<2;d++){ int f= d? 110 : 256; u16 *q=G.yf_pal[G.yf_nk][d]; q[0]=0;
        for(int L=1;L<=4;L++) q[L]=rgb_scale(kc,GL[L]*f>>8);
        q[5]=rgb_scale(kc,f); q[6]=rgb_scale(rgb_mix(kc,0x7FFF,180),f); }
      G.yf_nk++; } }
  int nk=G.yf_nk, bank[5][2];
  for(int j=0;j<nk;j++) for(int d=0;d<2;d++){ int b=spr_pal(); bank[j][d]=b; if(b>=0) for(int L=0;L<7;L++) G.sp[b][L]=G.yf_pal[j][d][L]; }
  int t=(int)G.sky_t, j=0;
  for(int i=0;i<YF_N;i++, j= j+1<nk? j+1 : 0){
    int ph=((t>>1)+i*23)&127;                                 // its blink: bright, dim, a rest in the dark
    if(ph>=104) continue;
    int dim= ph<8 || ph>=88, b=bank[j][dim]; if(b<0) continue;
    int x=YF_BX[i]+((SIN64[((t>>2)+i*9)&63]-128)*18>>8), y=YF_BY[i]+((SIN64[((t>>1)+i*13)&63]-128)*7>>8);
    if(y<50) y=50; if(y>128) y=128;
    if(!dim) spr_add(x,y,0,((t>>3)+i)&1? SPT_FCORE : SPT_FCORE2,b,-1,0,0);   // the bright heart, on top
    spr_add(x,y,1,SPT_FGLOW,b,-1,0,0);                                       // its glow
  }
}
static int fly_rounds_left(void){ fly_tonight(); return FLY_ROUNDS-(G.more.spare[7]&3); }
static int fly_level(const Fly *f){   // 0 dark, 1 faint, 2 lit, 3 bright
  const FKind *K=&FK[f->k]; int per=K->on+K->gap, t=f->t;
  if(t>=per*K->pulses) return 0;
  int q=t%per; if(q>=K->on) return 0;
  if(q<3 || q>=K->on-3) return 1;
  return (q<6 || q>=K->on-6)? 2 : 3;
}
static int fly_cycle(const FKind *K){ return (K->on+K->gap)*K->pulses+K->off; }
static void fly_bounds(int k,int *y0,int *y1,int *x0,int *x1){
  int st=FK[k].style; *x0=10; *x1=230; *y0=36; *y1=124;
  if(st&FS_LOW){ *y0=104; *y1=128; }
  if(st&FS_HIGH){ *y0=34; *y1=70; }
  if(st&FS_ROSE) for(int i=0;i<3;i++) if(G.gdn.p[i].stage==4 && G.gdn.p[i].kind==GK_ROSE){
    *x0=PATCH_X[i]-26; *x1=PATCH_X[i]+26; *y0=PATCH_Y[i]-46; *y1=PATCH_Y[i]-6; }
}
static int fly_pick(void){   // the next one to come along
  u16 out=fly_out(), on=0; for(int i=0;i<FLY_SLOTS;i++) if(G.fly[i].live) on|=(u16)(1u<<G.fly[i].k);
  for(int k=8;k<FK_N;k++) if(((out>>k)&1) && !((on>>k)&1) && rnd(k==11? 10 : 6)==0) return k;   // a rare one, now and then
  if(rnd(3)==0){ int n=0, c[5]; for(int k=3;k<8;k++) if((out>>k)&1) c[n++]=k; if(n) return c[rnd(n)]; }
  return rnd(3);
}
static void fly_spawn(Fly *f){
  int k=fly_pick(), x0,x1,y0,y1; fly_bounds(k,&y0,&y1,&x0,&x1);
  f->k=(u8)k; f->live=1; f->x=(s16)((x0+rnd(x1-x0))<<4); f->y=(s16)((y0+rnd(y1-y0))<<4);
  f->t=(u8)rnd(fly_cycle(&FK[k])); f->vx=0; f->vy=0; f->turn=0;
}
static void fly_start(void){
  fly_tonight(); G.more.spare[7]=(u16)(G.more.spare[7]+1);
  G.mode=M_FLY; G.fly_state=0; G.fly_t=FLY_ROUND; G.fly_rn=0; G.fly_key=0; G.fly_cool=0; G.fly_pop=0; G.jx=114; G.jy=84;
  for(int i=0;i<FLY_SLOTS;i++){ Fly *f=&G.fly[i]; memset(f,0,sizeof(Fly)); if(i<6) fly_spawn(f); else { f->wait=(u8)(40+i*30); } }
  sfx(0,SEQ_JUMP);
}
static void fly_finish(void){
  G.fly_state=2; G.fly_key=0;
  if(G.fly_n>G.more.spare[5]) G.more.spare[5]=G.fly_n;
  save_all(); sfx(0,G.fly_rn? SEQ_HAPPY : SEQ_BLIP);
}
static void fly_back(u16 keys){ G.mode=M_GARDEN; G.gui_key=0; G.suppress=keys; sfx(0,SEQ_BLIP); }
static void fly_move(Fly *f){
  const FKind *K=&FK[f->k]; int sp=K->speed, x0,x1,y0,y1; fly_bounds(f->k,&y0,&y1,&x0,&x1);
  if(++f->t>=fly_cycle(K)) f->t=0;
  if(!f->turn--){ f->turn=(u8)(16+rnd(30)); f->vx=(s8)(rnd(2*sp+1)-sp); f->vy=(s8)((rnd(2*sp+1)-sp)*2/3); }
  if((K->style&FS_ZIG) && (G.gt&15)==0) f->vx=(s8)-f->vx;
  int x=f->x+f->vx, y=f->y+f->vy;
  if(x<(x0<<4)){ x=x0<<4; f->vx=(s8)(f->vx<0? -f->vx : f->vx); } if(x>(x1<<4)){ x=x1<<4; f->vx=(s8)(f->vx>0? -f->vx : f->vx); }
  if(y<(y0<<4)){ y=y0<<4; f->vy=(s8)(f->vy<0? -f->vy : f->vy); } if(y>(y1<<4)){ y=y1<<4; f->vy=(s8)(f->vy>0? -f->vy : f->vy); }
  f->x=(s16)x; f->y=(s16)y;
}
static void fly_catch(void){
  int cx=G.jx+6, cy=G.jy+8, best=-1, bd=999;
  for(int i=0;i<FLY_SLOTS;i++){ Fly *f=&G.fly[i]; if(!f->live || fly_level(f)<1) continue;   // (v8.2b: a faint glow counts too)
    int dx=(f->x>>4)-cx, dy=(f->y>>4)-cy; if(dx<0) dx=-dx; if(dy<0) dy=-dy;
    if(dx<=8 && dy<=9 && dx+dy<bd){ bd=dx+dy; best=i; } }
  if(best<0){ sfx(0,SEQ_TICK); return; }
  Fly *f=&G.fly[best]; int k=f->k; f->live=0; f->wait=(u8)(30+rnd(60));
  if(G.fly_n<255) G.fly_n++; G.fly_rn++; secret_lantern(); G.fly_kinds|=(u16)(1u<<k); if(G.more.spare[4]<65535) G.more.spare[4]++;
  G.fly_pop=30; G.fly_popk=(u8)k;
  if(!((fly_found()>>k)&1)){ G.more.spare[3]|=(u16)(1u<<k); G.fly_state=1; G.fly_card=(u8)k; G.fly_key=0; sfx(0,SEQ_NEWSTAR); save_all(); }
  else sfx(0,SEQ_CATCH);
}
static void fly_update(u16 pr,u16 keys){
  live_update(0);   // the clock runs out here too
  if(SCENE!=1){ G.outside=0; G.bg_state=0; G.mode=M_NONE; return; }
  if(!is_night()){ if(G.fly_state==0) fly_finish(); G.mode=M_GARDEN; garden_close(keys); return; }
  G.gt++;
  if(G.fly_state==1){ if(pr&(K_A|K_B)){ G.fly_state=0; G.fly_key=0; G.fly_cool=8; sfx(0,SEQ_BLIP); } return; }
  if(G.fly_state==2){
    if((pr&K_A) && fly_rounds_left()>0) fly_start();
    else if(pr&(K_B|K_SELECT)) fly_back(keys);
    return; }
  if(pr&K_B){ fly_finish(); return; }
  // (v8.2b: the jar moves 3 px a frame, was 2)
  if(keys&K_LEFT) G.jx=(u8)(G.jx>7? G.jx-3 : 4);   if(keys&K_RIGHT) G.jx=(u8)(G.jx<221? G.jx+3 : 224);
  if(keys&K_UP) G.jy=(u8)(G.jy>35? G.jy-3 : 32);    if(keys&K_DOWN) G.jy=(u8)(G.jy<115? G.jy+3 : 118);
  for(int i=0;i<FLY_SLOTS;i++){ Fly *f=&G.fly[i];
    if(f->live) fly_move(f); else if(f->wait && !--f->wait) fly_spawn(f); }
  if(G.fly_cool) G.fly_cool--;
  if(G.fly_pop) G.fly_pop--;
  if((pr&K_A) && !G.fly_cool){ G.fly_cool=10; fly_catch(); if(G.fly_state) return; }
  if(G.fly_t && !--G.fly_t) fly_finish();
}
// ---- drawing
static void fly_dot(int x,int y,int lvl,int k,int t){   // one firefly
  int c=FK[k].col;
  if(!lvl){ pset(x,y,FLY_DARK); return; }
  if(k==11){ pset(x,y,lvl>=2? 245 : 249); if(lvl>=2){ pset(x-1,y,249); pset(x+1,y,249); pset(x,y-1,249); pset(x,y+1,249); }
    if(lvl>=3 && (t&4)){ pset(x-3,y,250); pset(x+3,y,250); pset(x,y-3,250); pset(x,y+3,250); }
    if(lvl>=3 && !(t&4)){ pset(x-2,y-2,249); pset(x+2,y+2,249); pset(x+2,y-2,249); pset(x-2,y+2,249); }
    return; }
  pset(x,y, lvl>=3? 245 : c);
  if(lvl>=2){ pset(x-1,y,c); pset(x+1,y,c); pset(x,y-1,c); pset(x,y+1,c); }
  if(lvl>=3){ int o=(t>>3)&1; if(o){ pset(x-2,y-2,c); pset(x+2,y+2,c); pset(x+2,y-2,c); pset(x-2,y+2,c); }
    else { pset(x-3,y,c); pset(x+3,y,c); pset(x,y-3,c); pset(x,y+2+1,c); } }
}
static void fly_big(int cx,int cy,int k,int t){   // twice the size: the field guide and the new-kind card
  int c=FK[k].col, lvl= ((t>>4)&3)==3? 2 : 3;
  fill(cx-1,cy-1,2,2,lvl>=3? 245 : c);
  fill(cx-3,cy-1,2,2,c); fill(cx+1,cy-1,2,2,c); fill(cx-1,cy-3,2,2,c); fill(cx-1,cy+1,2,2,c);
  if(k==11){ fill(cx-6,cy-1,2,2,250); fill(cx+4,cy-1,2,2,250); fill(cx-1,cy-6,2,2,250); fill(cx-1,cy+4,2,2,250); }
  if(lvl>=3){ pset(cx-4,cy-4,c); pset(cx+3,cy-4,c); pset(cx-4,cy+3,c); pset(cx+3,cy+3,c); }
  fill(cx-2,cy-6+(k==11? -2 : 0),1,2,246); fill(cx+1,cy-6+(k==11? -2 : 0),1,2,246);   // little wings
}
static void fly_jar(int x,int y,int n,u16 kinds,int open){   // a glass jar 12x15 (x,y: top left), with what's in it
  if(!open){ fill(x+3,y,6,1,FLY_DARK); fill(x+2,y+1,8,2,250); fill(x+3,y+1,6,1,249); }
  for(int j=3;j<15;j++){ int w=(j>4 && j<14)? 12 : 10, x0=x+(12-w)/2; pset(x0,y+j,246); pset(x0+w-1,y+j,246); }
  fill(x+2,y+14,8,1,246); fill(x+2,y+3,8,1,246); pset(x+2,y+6,245); pset(x+2,y+7,245); pset(x+2,y+8,246);
  static const u8 SPOT[9][2]={{5,9},{8,7},{4,12},{7,11},{9,10},{3,8},{6,6},{8,12},{5,7}};
  int ks[FK_N], m=0; for(int k=0;k<FK_N;k++) if((kinds>>k)&1) ks[m++]=k;
  for(int i=0;i<n && i<9;i++){ int k= m? ks[i%m] : 0; int on=((G.sky_t>>3)+i*3)%5; if(on) pset(x+SPOT[i][0],y+SPOT[i][1],FK[k].col); }
}
static void lantern_glow(int cx,int cy,int n,u16 kinds,int t){   // soft dots of light round the lantern
  int r= n>=12? 12 : n>=6? 10 : 8, ks[FK_N], m=0; for(int k=0;k<FK_N;k++) if((kinds>>k)&1) ks[m++]=k; if(!m) return;
  for(int i=0;i<18;i++){ int a=(i*7+(t>>4))&15, rr=r-(i&3); int x=cx+SINE16[(a+4)&15]*rr/8, y=cy+SINE16[a]*rr/8;
    if(((i+(t>>3))%3)) pset(x,y,FK[ks[i%m]].col); }
}
static void fly_words(void){   // the bars and any box, drawn once into SCRATCH_BG
  u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG; O_DMACOPY(SCRATCH_BG, YARD_BG);
  dma_fill(0,0,240,27,C_BAR); dma_fill(0,27,240,1,C_OUTLINE); dma_fill(0,135,240,25,C_BAR); dma_fill(0,134,240,1,C_OUTLINE);
  texts("FIREFLIES",8,10,GP_Y);
  { char b[16]; char *q=scpy(b,"FOUND "); q=sint(q,fly_count()); q=scpy(q,"/"); sint(q,FK_N); texts(b,232-slen(b)*6,10,C_LTEXT); }
  const char *h="A: SCOOP   B: STOP";
  if(G.fly_state==1){
    int k=G.fly_card; quick_box(34,34,172,92); textc("NEW FIREFLY!",120,40,C_ORANGE);
    fill(103,50,34,30,C_OUTLINE); fill(104,51,32,28,FLY_DARK);
    textc(FK[k].name,120,86,C_DTEXT); textc(FK[k].note,120,98,C_DTEXT);
    { char b[20]; char *q=sint(b,fly_count()); q=scpy(q," OF "); q=sint(q,FK_N); scpy(q," FOUND"); textc(b,120,112,C_HINT); }
    h="A: OK";
  } else if(G.fly_state==2){
    int left=fly_rounds_left(); char b[36]; char *q;
    quick_box(20,36,200,50);
    q=sint(b,G.fly_n); scpy(q, G.fly_n==1? " FIREFLY IN THE LANTERN!" : " FIREFLIES IN THE LANTERN!"); textc(G.fly_n? b : "THE LANTERN IS EMPTY...",120,42,C_DTEXT);
    q=scpy(b,"KINDS FOUND: "); q=sint(q,fly_count()); q=scpy(q," OF "); sint(q,FK_N); textc(b,120,54,C_ORANGE);
    if(left>0){ q=sint(b,left); scpy(q,left==1? " MORE ROUND TONIGHT" : " MORE ROUNDS TONIGHT"); } else scpy(b,"NO MORE ROUNDS TONIGHT");
    textc(b,120,68,C_HINT);
    h= left>0? "A: AGAIN   B: BACK" : "B: BACK TO THE YARD";
  }
  textc(h,120,144,C_HINT);
  DRAW_TARGET=sv;
}
static void fly_screen_draw(void){
  if(G.bg_state!=4) build_bg(4);
  { u16 key=(u16)(1 | (G.fly_state<<1) | (G.fly_card<<3) | (fly_count()<<7) | ((G.fly_n&15)<<11));
    if(G.fly_state!=2) key&=0x07FF;
    if(key!=G.fly_key){ fly_words(); G.fly_key=key; } }
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, YARD_BG);
  draw_sky(0); garden_draw(1); draw_decor(1); yard_poops();
  int t=(int)G.gt;
  for(int i=0;i<FLY_SLOTS;i++){ const Fly *f=&G.fly[i]; if(f->live) fly_dot(f->x>>4,f->y>>4,fly_level(f),f->k,t+i*5); }
  fly_jar(G.jx,G.jy,G.fly_n,G.fly_kinds,0);
  if(G.fly_pop && G.fly_state==0){ int y=G.jy-8-(30-G.fly_pop)/3; text("+1",G.jx+1,y,FK[G.fly_popk].col); }
  copy_rect(0,0,240,28); copy_rect(0,134,240,160);
  if(G.fly_state==0){   // the time left
    int w=40*G.fly_t/FLY_ROUND; fill(84,9,42,10,C_OUTLINE); fill(85,10,40,8,FLY_DARK); fill(85,10,w,8,GP_Y);
    char b[8]; int s=(G.fly_t+59)/60; b[0]='0'; b[1]=':'; b[2]=(char)('0'+s/10); b[3]=(char)('0'+s%10); b[4]=0; text(b,130,10,C_LTEXT); }
  else if(G.fly_state==1){ copy_rect(34,34,206,126); fly_big(120,65,G.fly_card,t); }
  else copy_rect(20,36,220,86);
}
// ---- in the room: the lantern on the windowsill all night, the release in the morning, the dance once all are found
#define LANT_X 40
#define LANT_Y 64
static void fly_morning(void){ if(G.fly_n && G.fly_night==G.ext.day){ G.fly_rel=1; } else { G.fly_n=0; G.fly_kinds=0; } }
// v8.2: in the morning the window swings open, the cork pops and the fireflies float out as glowing hardware
// sprites, shrinking and fading into the distance (window 0 keeps them inside the open window); then it shuts.
#define FLY_GONE 112                       // frames from leaving the jar to gone
#define WIN_SWING 32                       // frames to open (and to shut) the window
static int fly_rel_n(void){ return G.fly_n<10? G.fly_n : 10; }
static int fly_rel_last(void){ int n=fly_rel_n(); return 40+(n>0? n-1 : 0)*12+FLY_GONE; }   // the last one is gone
static int win_open_amt(void){   // 0 shut .. WIN_SWING wide open
  if(!G.fly_rel || G.outside) return 0;
  int r=G.fly_rel, rl=fly_rel_last();
  return r<=WIN_SWING? r : r<rl? WIN_SWING : r<rl+WIN_SWING? rl+WIN_SWING-r : 0; }
static void win_pane_geom(int o,int *w,int *k){   // how wide a pane looks, and how much its far edge shrinks
  int a=o*13/WIN_SWING;                          // up to 13/64 of a turn (about 73 degrees)
  *w=(23*SIN64[(a+16)&63]+128)>>8; *k=(4*SIN64[a&63]+128)>>8; }
static void vline(int x,int y0,int y1,int c){ for(int y=y0;y<=y1;y++) pset(x,y,c); }
static void sline(int x0,int y0,int x1,int y1,int c){   // a short line (Bresenham: no divides)
  int dx=x1>x0? x1-x0 : x0-x1, dy=y1>y0? y0-y1 : y1-y0, sx=x0<x1? 1 : -1, sy=y0<y1? 1 : -1, e=dx+dy;
  for(;;){ pset(x0,y0,c); if(x0==x1 && y0==y1) break; int e2=2*e; if(e2>=dy){ e+=dy; x0+=sx; } if(e2<=dx){ e+=dx; y0+=sy; } } }
static void win_open_pre(void){   // before the sky: the middle frame bars swing away, leaving open sky
  int o=win_open_amt(); if(!o) return;
  for(int y=40;y<=79;y++){ int c=sky_band(y-10); if(y==59 || y==60) fill(22,y,48,1,c); else fill(45,y,2,1,c); }
}
static void win_open_post(void){  // after the sky: the two panes, seen edge-on as they swing outwards
  int o=win_open_amt(); if(!o) return;
  int fc=YARD_BG[50*240+45], w, k; win_pane_geom(o,&w,&k);
  for(int s=0;s<2;s++){ int h= s? 69 : 22, xf= s? 69-w : 22+w;
    sline(h,40,xf,40+k,fc); sline(h,79,xf,79-k,fc); vline(xf,40+k,79-k,fc); sline(h,59,xf,59,fc); sline(h,60,xf,60,fc); }
}
static void fly_rel_sprites(int r){
  int ks[FK_N], m=0; for(int k=0;k<FK_N;k++) if((G.fly_kinds>>k)&1) ks[m++]=k;
  int n=fly_rel_n(), t=(int)G.sky_t, w, kk; win_pane_geom(WIN_SWING,&w,&kk);
  int show= G.mode==M_NONE && SCENE==1;   // (not over a menu or a box)
  for(int i=0;i<n && show;i++){ int a=r-40-i*12; if(a<0 || a>=FLY_GONE) continue;
    int y=LANT_Y+2-a/4, x=LANT_X+6+SINE16[((a>>3)+i*5)&15]*(5+i%3)/8+(i-n/2)*3; if(y<44) continue;
    int far=clampi((a-12)*256/96,0,256), scale=256-far*3/4, fade=256-far*7/8;
    int pulse=192+(SINE16[((t>>2)+i*5)&15]*8);    // a gentle flicker, 128..256
    fade=fade*pulse>>8;
    int b=spr_pal(); if(b<0) break;
    u16 kc=PAL[FK[m? ks[i%m] : 0].col];
    static const u8 GL[5]={0,18,50,100,170};   // the glow's falloff: faint at the rim, so it has no hard edge
    for(int L=1;L<=4;L++) G.sp[b][L]=rgb_scale(kc,GL[L]*fade>>8);
    G.sp[b][5]=rgb_scale(kc,fade); G.sp[b][6]=rgb_scale(rgb_mix(kc,0x7FFF,180),fade);
    if(far<110) spr_add(x,y,0,far<50? SPT_FCORE : SPT_FCORE2,b,-1,0,0);   // the bright heart, on top
    spr_add(x,y,1,SPT_FGLOW,b,spr_aff(scale,0),0,0);                      // its glow
  }
  G.spr_win=1; G.win[0]=(u8)(22+w+1); G.win[1]=(u8)(69-w); G.win[2]=45; G.win[3]=80;
}
// v8.2b: the jar's light is a soft glow (a hardware sprite, added to the picture) instead of specks round it: its
// colour drifts slowly through the kinds inside, it breathes, and more fireflies make it bigger and brighter
// the box a menu over the room covers (x, y, w, h), or 0 when none: the jar's glow (a sprite) stays outside it
static int menu_box(int *r){
  switch(G.mode){
    case M_MENU:   r[0]=52; r[1]=34; r[2]=136; r[3]=92; return 1;
    case M_EXTRAS: if(G.ex_yard) return 0; r[0]=136; r[1]=30; r[2]=96; r[3]=100; return 1;
    case M_BAG:  { int rows=G.bag_n>5? 5 : G.bag_n; if(rows<2) rows=2; r[0]=24; r[1]=29; r[2]=192; r[3]=36+rows*12; return 1; }
    case M_SURE:   r[0]=14; r[1]=44; r[2]=212; r[3]=74; return 1;
    default: return 0;
  }
}
static void jar_glow(int t){
  // v8.3: it keeps glowing behind the menus too (in the room, outside the menu's box)
  int box[4], boxed=0;
  if(G.mode==M_NONE || G.mode==M_EAT){ if(SCENE!=1 && SCENE!=3) return; }
  else if(!(boxed=menu_box(box))) return;
  if(!G.fly_n) return;
  int ks[FK_N], m=0; for(int k=0;k<FK_N;k++) if((G.fly_kinds>>k)&1) ks[m++]=k; if(!m) return;
  int ph=t>>2, a=(ph>>6)%m, b=(a+1)%m, f=(ph&63)*4;                     // the colour drifts from one kind to the next
  u16 col=rgb_mix(PAL[FK[ks[a]].col],PAL[FK[ks[b]].col],f);
  int n=G.fly_n>9? 9 : G.fly_n, br=190+n*8, sc=170+n*12;               // more fireflies, a bigger, brighter glow
  int s=SIN64[(t>>1)&63]; br+=br*s/2048; sc+=sc*s/3072;                   // it breathes
  int bk=spr_pal(); if(bk<0) return;
  static const u8 JG[7]={0,18,40,66,100,145,200};
  for(int L=1;L<=6;L++) G.sp[bk][L]=rgb_scale(col,JG[L]*br>>8);
  spr_add(LANT_X+6,LANT_Y+9,2,SPT_SGLOW,bk,spr_aff(sc,0),1,0);
  if(boxed && !G.spr_win){   // window 0 = the menu's box, with no sprites inside it (spr_sync: spr_win 2)
    G.spr_win=2; G.win[0]=(u8)box[0]; G.win[1]=(u8)(box[0]+box[2]); G.win[2]=(u8)box[1]; G.win[3]=(u8)(box[1]+box[3]); }
}
static void lantern_draw(void){
  if(G.outside) return;
  int t=(int)G.sky_t;
  if(G.fly_rel){   // morning: the window opens, the cork pops and they drift out, home
    int r=G.fly_rel++;
    if(r==40){ set_msg("BYE, FIREFLIES! THEY FLEW HOME."); sfx(0,SEQ_POP); }
    if(r<40){ jar_glow(t); fly_jar(LANT_X,LANT_Y,G.fly_n,G.fly_kinds,0); return; }
    fly_jar(LANT_X,LANT_Y,0,0,1); fill(LANT_X-6,LANT_Y+12,6,2,250);   // the cork, set down beside it
    fly_rel_sprites(r);
    if(r>=fly_rel_last()+WIN_SWING || r>400){ G.fly_rel=0; G.fly_n=0; G.fly_kinds=0; }
    return; }
  if(!is_night() || !G.fly_n || G.fly_night!=(u16)(G.ext.day+1)) return;
  jar_glow(t);
  fly_jar(LANT_X,LANT_Y,G.fly_n,G.fly_kinds,0);
}
static void fly_dance(void){   // all twelve found: they dance outside the window every night (sky coordinates)
  if(fly_found()!=FLY_ALL) return;
  u32 t=PET_CLOCK; if(t<482 || t>596) return;
  int s=(int)G.sky_t;
  for(int i=0;i<6;i++){ int k=(i*2+((s>>9)&1))%FK_N, a=(s>>2)+i*11;
    int x=84+SINE16[(a>>1)&15]*(10+i*2)/8, y=48+SINE16[((a>>2)+i*3)&15]*(6+i)/8;
    if(((s>>3)+i)%4) sky_pset(x,y,FK[k].col); }
}
// ---- the journal's field guide
static void jr_fly_page(void){
  int n=G.jfly, k=n, got=(fly_found()>>k)&1, t=(int)G.sky_t;
  if(!G.g_anim){
    char b[40]; char *p=scpy(b,"FOUND "); p=sint(p,fly_count()); p=scpy(p,"/"); sint(p,FK_N);
    jr_header(b,"D-PAD: PICK   L/R: TAB");
    for(int i=0;i<FK_N;i++){ int x=10+(i%6)*38, y=31+(i/6)*34;
      jr_frame(x,y,34,30,i==n? C_RED : C_HINT); fill(x+1,y+1,32,28,FLY_DARK);
      if(!((fly_found()>>i)&1)) textc("?",x+17,y+11,C_HINT); }
    quick_box(10,100,220,36);
    p=scpy(b,"NO."); p=sint(p,k+1); p=scpy(p,"  "); scpy(p,got? FK[k].name : "???"); text(b,18,105,got? C_DTEXT : C_HINT);
    { const char *s= got? FK[k].note : FK[k].hint? FK[k].hint : k<3? "OUT MOST NIGHTS." : "NOT OUT EVERY NIGHT."; text(s,18,115,got? C_DTEXT : C_ORANGE); }
    return; }
  for(int i=0;i<FK_N;i++) if((fly_found()>>i)&1) fly_big(10+(i%6)*38+17,31+(i/6)*34+15,i,t+i*9);
}
static void jr_fly(void){
  u16 key=(u16)(0x4000 | G.jfly | (fly_count()<<4));
  if(key!=G.jg_key){ u16 *sv=DRAW_TARGET; DRAW_TARGET=(u16*)SCRATCH_BG; G.g_anim=0; jr_fly_page(); DRAW_TARGET=sv; G.jg_key=key; }
  O_DMACOPY(DRAW_TARGET, SCRATCH_BG); G.g_anim=1; jr_fly_page(); G.g_anim=0;
}
static void jr_fly_keys(u16 pr){
  int n=G.jfly;
  if(pr&K_LEFT){ n= n? n-1 : FK_N-1; sfx(0,SEQ_BLIP); }
  if(pr&K_RIGHT){ n= n+1>=FK_N? 0 : n+1; sfx(0,SEQ_BLIP); }
  if(pr&(K_UP|K_DOWN)){ n=(n+6)%FK_N; sfx(0,SEQ_BLIP); }
  G.jfly=(u8)n;
}
static void dex_draw(void){
  DRAW_TARGET=O_BACKBUF();
  if(G.jtab!=JT_GARDEN && G.jtab!=JT_FLY) G.jg_key=0;
  if(G.jtab==0) jr_sky(); else if(G.jtab==1) jr_guide(); else if(G.jtab==JT_ITEMS) jr_items(); else if(G.jtab==JT_GARDEN) jr_garden(); else if(G.jtab==JT_FLY) jr_fly(); else jr_records();
}
static void sure_draw(void){
  O_BOX(14,44,212,74);
  textbig("ARE YOU SURE?",120,52,C_RED,1);
  textc("COINS, ITEMS AND DECOR GO.",120,68,C_DTEXT);
  textc("YOUR JOURNAL IS KEPT.",120,78,C_DTEXT);
  int y=91;
  text("YES",72,y,G.sure_cur==0? C_RED : C_GRAY); text("NO",150,y,G.sure_cur==1? C_RED : C_GRAY);
  text(">",G.sure_cur==0? 64 : 142,y,C_RED);
  textc("LEFT/RIGHT, THEN A",120,102,C_HINT);
}
// v8 phase 6: past critters shine over the title in their own colours, and now and then a shooting star drops a
// star-seed into the meadow

static const u8 TSTAR[12][2]={{10,12},{28,30},{12,48},{34,52},{58,68},{76,82},{166,92},{188,58},{208,40},{228,22},{232,60},{180,74}};
static void tiny_star(int x,int y,int c,int big){
  pset(x,y,C_WHITE); pset(x-1,y,c); pset(x+1,y,c); pset(x,y-1,c); pset(x,y+1,c);
  if(big){ pset(x-2,y,c); pset(x+2,y,c); pset(x,y-2,c); pset(x,y+2,c); } }
static void title_sky(void){
  int n=G.jr.n<12? G.jr.n : 12;   // the latest critters, each in its own colour (a sad goodbye shines dim)
  for(int i=0;i<n;i++){ const Story *s=&G.jr.s[G.jr.n-1-i]; int c= s->form<23? FORM_STAR[s->form] : C_GOLD;
    if(((s->info>>2)&3)==2){ pset(TSTAR[i][0],TSTAR[i][1],c); continue; }
    int tw=((G.sky_t+i*23)>>3)%12; tiny_star(TSTAR[i][0],TSTAR[i][1],c,tw==0); }
  // the shooting star: 44 frames falling from the top right, then the seed glows where it landed
  int t=G.sky_t%420;
  if(t<44){ int hx=228-t*58/44, hy=6+t*112/44;
    for(int j=1;j<24;j++){ int x=hx+j*58/126, y=hy-j*112/126; if(y<2) break; pset(x,y, j<4? C_WHITE : j<10? C_LTYEL : j<16? C_GOLD : C_ORANGE);
      if(j<8) pset(x+1,y,C_LTYEL); }
    tiny_star(hx,hy,C_LTYEL,1); }
  else if(t<200){ int g=t-44, x=170, y=117;
    if(g<12){ ring(x,y,2+g,(g&1)? C_WHITE : C_LTYEL); }
    // the star-seed: a little glowing egg
    pset(x,y-2,C_WHITE); fill(x-1,y-1,3,1,C_LTYEL); fill(x-2,y,5,2,C_LTYEL); fill(x-1,y+2,3,1,C_GOLD); pset(x-1,y-1,C_WHITE);
    if(((g>>3)&1) && g<130){ pset(x-4,y-3,C_WHITE); pset(x+4,y-2,C_WHITE); pset(x,y-5,C_WHITE); } }
}
static void title_draw(void){
  DRAW_TARGET=O_BACKBUF();
  O_DMACOPY(DRAW_TARGET, TITLE_BG);
  title_sky();
  int bob=SINE16[(G.sky_t>>3)&15]>>2;
  crit_draw(1, auto_pose(), 88, 60+bob, 4, 0, 0);
  if(G.mode==M_SURE) return;
  if(TITLE_CONFIRM==1){
    O_BOX(30,64,180,50);
    textc("START OVER WITH A NEW SEED?",120,73,C_DTEXT); textc("YOUR OLD CRITTER WILL BE GONE.",120,85,C_DTEXT);
    textc("A: YES     B: NO",120,99,C_RED);
  } else if(*(volatile u32*)PET==0x31504B43u){
    static const char *const OPT[3]={"CONTINUE","NEW SEED","JOURNAL"};
    for(int i=0;i<3;i++){ int y=127+i*11, sel= G.tdex? i==2 : TITLE_CUR==(u32)i;
      textc(OPT[i],121,y+1,C_OUTLINE); textc(OPT[i],120,y, sel? C_GOLD : C_LTEXT);
      if(sel){ int w=slen(OPT[i])*6; mini_art(MA_STAR2,120-w/2-14,y-2,COL_STAR,0); mini_art(MA_STAR2,120+w/2+3,y-2,COL_STAR,0); } }
  } else if((G.sky_t>>5)&1 || 1){ textc("PRESS START",121,141,C_OUTLINE); textc("PRESS START",120,140,((G.sky_t>>5)&1)? C_WHITE : C_LTYEL); }
}
void hook_render(void){
  init_once();
  spr_begin();
  G.sky_t++;
  if((G.sky_t&63)==1) check_window_panes();
  // v8.3: the runaway's cuts: black at once, before this frame's palette is the next shot's (the old picture would
  // otherwise show for a frame in the new shot's colours)
  { int s=ra_shot(); if(s>=0 && G.gone_t!=(u16)(s+1)){ REG_BLDCNT=(u16)(0x0034|(FX_BLACK<<6)); REG_BLDY=16; } }
  set_extra_palette(); set_special_cmap();
  int help = G.mode==M_NONE && SCENE==7;    // our HOW TO PLAY pages cover the whole screen
  if(!help && (G.mode==M_NONE || G.mode==M_MENU || (G.mode==M_EXTRAS && !G.ex_yard) || G.mode==M_BAG || G.mode==M_EAT || G.mode==M_SURE)){
    u32 sc=SCENE;
    G.yp_n=G.outside? G.more.pad0 : 0;      // (v8.2c: the yard draws its own, at free spots: yard_poops)
    if(G.mode==M_MENU) SCENE=1;              // our menu replaces the original's box
    O_RENDER();
    SCENE=sc;
    // v8.3: asleep and sick, the sick symbol shows beside the Zs (the original only draws it while it is awake).
    // In colour 11, which the lights-off dimming leaves bright.
    if(sc==1 && G.mode==M_NONE && !G.outside && PET[P_ASLEEP] && PET[P_SICK] && PET[P_STAGE]>=1 && PET[P_STAGE]<=7){
      u8 keep=CMAP['x']; CMAP['x']=11;
      hook_sprite((const void*)0x0800FFC4u, 126, 42+(int)((FRAMES>>3)&2), 2, 0, 99);   // (the original's sick picture)
      CMAP['x']=keep; }
  }
  if(help){ DRAW_TARGET=O_BACKBUF(); help_draw(); }
  if(G.mode==M_NONE && SCENE==9) runaway_draw(); else { G.gone_t=0; G.gone_bg=0; }
  if(G.mode==M_NONE && SCENE==3 && ANIM_KIND<=1 && PET[P_STAGE]>=1 && PET[P_STAGE]<=7){ DRAW_TARGET=O_BACKBUF(); feed_draw(); }
  if(G.mode==M_NONE && SCENE==4 && PET[P_STAGE]>=1 && PET[P_STAGE]<=7){  // the status card shows boy/girl
    text(G.dex.gender? "GIRL" : "BOY",G.dex.gender? 128 : 131,41,G.dex.gender? C_RED : C_BLUE);
    // (v8.3: the weight line says pounds, "WEIGHT 70 LB": the original's own drawing in orig_render.c writes it)
    card_care(); }
  if(title_showing()) title_draw();        // phase 4: our painted title replaces the original's
  switch(G.mode){
    case M_MENU:   menu_draw(); break;
    case M_EXTRAS: if(G.ex_yard) garden_screen_draw(); extras_draw(); break;
    case M_SHOP:   shop_draw(); break;
    case M_BAG:    bag_draw(); break;
    case M_EAT:    eat_draw(); break;
    case M_MINI:   mini_draw(); break;
    case M_SENDOFF: { G.cpose=(s8)(G.sendoff==E_SAD? POSE_SAD+1 : (((FRAMES>>4)&1)? POSE_WAVE1 : POSE_WAVE2)+1); sendoff_draw();   /* waving goodbye */ G.cpose=0; } break;
    case M_ALBUM:  album_draw(); break;
    case M_DEX:    dex_draw(); break;
    case M_SURE:   sure_draw(); break;
    case M_NAME:   name_draw(); break;
    case M_STORY:  story_draw(); break;
    case M_GARDEN: garden_screen_draw(); break;
    case M_FLY: fly_screen_draw(); break;
    default: break;
  }
  // background music
  if(!PET[P_SOUNDON] || G.life.music_off) music_stop();
  else {
    int tn, v=2, night=is_night();
    if(G.mode==M_SENDOFF) tn = G.sendoff==E_WONDER? T_FAREWELL_WONDER : G.sendoff==E_GOOD? T_FAREWELL_GOOD : T_FAREWELL_SAD;
    else if(G.mode==M_MINI && G.phase!=PH_PICK){ tn=GAME_TUNE[G.game]; if(G.game==G_STARS) v=3; }
    else if(G.mode==M_SHOP) tn=T_SHOP;
    else if(G.mode==M_ALBUM || G.mode==M_DEX) tn=T_ALBUM;
    else if(G.mode==M_STORY){ tn=T_LULLABY; v=3; }
    else if(G.mode==M_GARDEN || G.mode==M_FLY || G.ex_yard){ tn=T_OUTSIDE_NIGHT; v=3; }
    else if(SCENE==0 || G.mode==M_SURE) tn=T_TITLE;
    else if(PET[P_STAGE]==8) tn=T_FAREWELL_SAD;
    else if(PET[P_ASLEEP]){ tn=T_LULLABY; v=3; }
    else if(G.outside){ tn= night? T_OUTSIDE_NIGHT : T_OUTSIDE_DAY; if(night) v=3; }
    else { tn= night? T_INSIDE_NIGHT : T_INSIDE_DAY; if(night) v=3; }
    music_play(tn,v);
  }
  // the music keeps time by the hardware clock, not by how often we get to draw: a screen that takes two frames
  // to draw (the menus, the bestiary) would otherwise play it at half speed. Timer 3 counts 16.78 MHz / 1024;
  // one frame is 274.3 of its ticks (4389 in 1/16ths).
  { if(REG16(0x0400010E)!=0x0083){ REG16(0x0400010E)=0; REG16(0x0400010C)=0; REG16(0x0400010E)=0x0083; G.mus_last=REG16(0x0400010C); G.mus_acc=4389; }
    u16 now=REG16(0x0400010C); u32 acc=G.mus_acc+(u32)(u16)(now-G.mus_last)*16u; G.mus_last=now;
    int n=0; while(acc>=4389 && n<8){ music_tick(); acc-=4389; n++; }
    if(acc>=4389) acc=0;
    G.mus_acc=(u16)acc; }
  flush_crit_palette();
}


// debug: field offsets for the test harness
const u32 DBG_OFFS[] = { 0x0203E000u, __builtin_offsetof(State,mode), __builtin_offsetof(State,game), __builtin_offsetof(State,phase),
  __builtin_offsetof(State,score), __builtin_offsetof(State,timer), __builtin_offsetof(State,px), __builtin_offsetof(State,py),
  __builtin_offsetof(State,obj), sizeof(Obj), __builtin_offsetof(State,rng), __builtin_offsetof(State,ext), __builtin_offsetof(State,coin_timer),
  __builtin_offsetof(State,outside), __builtin_offsetof(Ext,coins), __builtin_offsetof(Ext,toys), __builtin_offsetof(Ext,decor), __builtin_offsetof(Ext,slot),
  __builtin_offsetof(Ext,active_toy), __builtin_offsetof(Ext,special), __builtin_offsetof(Ext,stars), __builtin_offsetof(Ext,adult_day), __builtin_offsetof(Ext,yard_mins),
  __builtin_offsetof(Ext,inv), __builtin_offsetof(Ext,day), sizeof(State), __builtin_offsetof(State,life), __builtin_offsetof(State,sendoff_pending), __builtin_offsetof(State,tune), __builtin_offsetof(State,sendoff_t), __builtin_offsetof(State,deco) };
