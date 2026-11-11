// Minimal GBA emulator for testing: ARM7TDMI (ARM+Thumb), modes 3/4 bitmap,
// vblank/hblank IRQ, timers, immediate DMA, SRAM, keypad. HLE BIOS (IRQ + a few SWIs).
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t u32; typedef uint16_t u16; typedef uint8_t u8; typedef int32_t s32;

static u8 *rom; static u32 romsize;
static u8 ewram[256*1024], iwram[32*1024], io[1024], pal[1024], vram[96*1024], oam[1024], sram[64*1024];
static u32 R[16], CPSR;
static u32 b13[6], b14[6], bspsr[6];
static int halted = 0;
static u16 keys = 0x3FF; // active low
static u32 vcount = 0;
static int dispstat_flags = 0;
static u32 next_pc; static int branched;
static unsigned long long total_cycles = 0;
static int in_hle_irq = 0; static u32 irq_save_R[16], irq_save_CPSR;
static int fault = 0;
static int prof_on=0; static unsigned prof[0x20000];
static u32 watch_lo=1, watch_hi=0; static u32 cur_pc; static u32 bp_addr=0xFFFFFFFF; static int bp_count=0;
static void wcheck(u32 a, u32 v, int sz){ if(a+sz>watch_lo && a<=watch_hi) printf("W%d [%08x]=%x pc=%08x lr=%08x vc=%u\n",sz,a,v,cur_pc,R[14],vcount); }

// timers
static u32 tm_counter[4]; static u32 tm_accum[4];

#define MAGIC_IRQ_RET 0xFFFFFF00u

static int midx(u32 m){ switch(m&0x1f){case 0x10:case 0x1f:return 0;case 0x12:return 1;case 0x13:return 2;case 0x17:return 3;case 0x1b:return 4;case 0x11:return 5;} return 0; }
static void set_cpsr(u32 n){ int o=midx(CPSR), k=midx(n); if(o!=k){ b13[o]=R[13]; b14[o]=R[14]; R[13]=b13[k]; R[14]=b14[k]; } CPSR=n; }
static u32 get_spsr(void){ return bspsr[midx(CPSR)]; }
static void set_spsr(u32 v){ int k=midx(CPSR); if(k) bspsr[k]=v; }

static u16 io_read16(u32 a){
  a &= 0x3FE;
  switch(a){
    case 0x004: return (io[4]|(io[5]<<8))&~7 | dispstat_flags | ((vcount==io[5])?4:0);
    case 0x006: return vcount;
    case 0x100: case 0x104: case 0x108: case 0x10C: return tm_counter[(a-0x100)/4];
    case 0x130: return keys;
  }
  return io[a]|(io[a+1]<<8);
}
static void do_dma(int ch);
static void io_write16(u32 a, u16 v){
  a &= 0x3FE;
  if(a==0x202){ u16 f=io[0x202]|(io[0x203]<<8); f&=~v; io[0x202]=f; io[0x203]=f>>8; return; }
  if(a==0x004){ v = (v & ~7) ; }
  if(a>=0x100 && a<0x110 && (a&3)==2){ // timer control
    int t=(a-0x100)/4; u16 old=io[a]|(io[a+1]<<8);
    if(!(old&0x80) && (v&0x80)){ tm_counter[t]=io[0x100+t*4]|(io[0x101+t*4]<<8); tm_accum[t]=0; }
  }
  io[a]=v; io[a+1]=v>>8;
  if(a==0x0BA||a==0x0C6||a==0x0D2||a==0x0DE){ if(v&0x8000){ int ch=(a-0x0BA)/12; if(((v>>12)&3)==0) do_dma(ch);} }
}

static u8 rd8(u32 a);
static u32 rd32(u32 a);
static u16 rd16(u32 a){
  a&=~1;
  switch(a>>24){
    case 0x02: return *(u16*)&ewram[a&0x3FFFF];
    case 0x03: return *(u16*)&iwram[a&0x7FFF];
    case 0x04: return io_read16(a);
    case 0x05: return *(u16*)&pal[a&0x3FF];
    case 0x06: { u32 o=a&0x1FFFF; if(o>=0x18000) o-=0x8000; return *(u16*)&vram[o]; }
    case 0x07: return *(u16*)&oam[a&0x3FF];
    case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: { u32 o=a&0x1FFFFFF; if(o+1<romsize) return *(u16*)&rom[o]; return (a>>1)&0xFFFF; }
    case 0x0E: case 0x0F: { u8 b=sram[a&0xFFFF]; return b*0x101; }
  }
  return 0;
}
static u8 rd8(u32 a){
  switch(a>>24){
    case 0x0E: case 0x0F: return sram[a&0xFFFF];
  }
  u16 h=rd16(a&~1); return (a&1)? h>>8 : h&0xFF;
}
static u32 rd32(u32 a){
  a&=~3;
  if((a>>24)==0x0E||(a>>24)==0x0F){ u8 b=sram[a&0xFFFF]; return b*0x01010101u; }
  return rd16(a) | ((u32)rd16(a+2)<<16);
}
static void wr16(u32 a, u16 v){
  a&=~1; wcheck(a,v,2);
  switch(a>>24){
    case 0x02: *(u16*)&ewram[a&0x3FFFF]=v; return;
    case 0x03: *(u16*)&iwram[a&0x7FFF]=v; return;
    case 0x04: io_write16(a,v); return;
    case 0x05: *(u16*)&pal[a&0x3FF]=v; return;
    case 0x06: { u32 o=a&0x1FFFF; if(o>=0x18000) o-=0x8000; *(u16*)&vram[o]=v; return; }
    case 0x07: *(u16*)&oam[a&0x3FF]=v; return;
    case 0x0E: case 0x0F: sram[a&0xFFFF]=v>>((a&1)*8); return;
  }
}
static void wr8(u32 a, u8 v){
  wcheck(a,v,1);
  switch(a>>24){
    case 0x02: ewram[a&0x3FFFF]=v; return;
    case 0x03: iwram[a&0x7FFF]=v; return;
    case 0x04: { u32 o=a&0x3FF; if(o==0x202||o==0x203){ u16 m = (o&1)? (v<<8):v; io_write16(0x202,m); return;} if(o==0x301){ halted=1; return;} u16 h=io_read16(a&~1); if(a&1) h=(h&0xFF)|(v<<8); else h=(h&0xFF00)|v; io_write16(a&~1,h); return; }
    case 0x05: wr16(a&~1, v*0x101); return;           // palette: byte duplicated
    case 0x06: { u32 o=a&0x1FFFF; if(o>=0x18000) o-=0x8000; if(o<0x14000) wr16(a&~1, v*0x101); return; } // bitmap area: dup; OBJ area ignored
    case 0x07: return; // OAM byte writes ignored
    case 0x0E: case 0x0F: sram[a&0xFFFF]=v; return;
  }
}
static void wr32(u32 a, u32 v){
  a&=~3;
  if((a>>24)==0x0E||(a>>24)==0x0F){ sram[a&0xFFFF]=v; return; }
  wr16(a, v&0xFFFF); wr16(a+2, v>>16);
}

static void do_dma(int ch){
  u32 base=0xB0+ch*12;
  u32 src = io[base]|(io[base+1]<<8)|(io[base+2]<<16)|(io[base+3]<<24);
  u32 dst = io[base+4]|(io[base+5]<<8)|(io[base+6]<<16)|(io[base+7]<<24);
  u32 cnt = io[base+8]|(io[base+9]<<8); u16 ctl=io[base+10]|(io[base+11]<<8);
  if(cnt==0) cnt = (ch==3)?0x10000:0x4000;
  int w32 = ctl & 0x400; int dm=(ctl>>5)&3, sm=(ctl>>7)&3; int sz=w32?4:2;
  for(u32 i=0;i<cnt;i++){
    if(w32) wr32(dst, rd32(src)); else wr16(dst, rd16(src));
    if(sm==0) src+=sz; else if(sm==1) src-=sz;
    if(dm==0||dm==3) dst+=sz; else if(dm==1) dst-=sz;
  }
  ctl &= ~0x8000; io[base+10]=ctl; io[base+11]=ctl>>8;
}

// ---------- flags helpers
#define FN 0x80000000u
#define FZ 0x40000000u
#define FC 0x20000000u
#define FV 0x10000000u
static inline void setNZ(u32 r){ CPSR = (CPSR & ~(FN|FZ)) | (r&FN) | (r?0:FZ); }
static inline void setC(int c){ if(c) CPSR|=FC; else CPSR&=~FC; }
static inline void setV(int v){ if(v) CPSR|=FV; else CPSR&=~FV; }
static inline u32 addf(u32 a, u32 b, u32 c, int s){ unsigned long long r=(unsigned long long)a+b+c; u32 res=(u32)r; if(s){ setNZ(res); setC(r>>32); setV(((a^res)&(b^res))>>31);} return res; }
static inline u32 subf(u32 a, u32 b, u32 c, int s){ // a - b - !c ; c=1 means no borrow
  return addf(a, ~b, c, s); }
static int cond(u32 c){
  int n=!!(CPSR&FN), z=!!(CPSR&FZ), cc=!!(CPSR&FC), v=!!(CPSR&FV);
  switch(c){case 0:return z;case 1:return !z;case 2:return cc;case 3:return !cc;case 4:return n;case 5:return !n;case 6:return v;case 7:return !v;
  case 8:return cc&&!z;case 9:return !cc||z;case 10:return n==v;case 11:return n!=v;case 12:return !z&&n==v;case 13:return z||n!=v;case 14:return 1;}
  return 0;
}
static void setpc(u32 v){ next_pc=v; branched=1; }
static void wreg(int n, u32 v){ if(n==15){ setpc(v & ~1); } else R[n]=v; }

static u32 shifter(int type, u32 v, u32 amt, int imm, int *carry){
  int c=!!(CPSR&FC);
  switch(type){
    case 0: if(amt==0){*carry=c;return v;} if(amt<32){*carry=(v>>(32-amt))&1; return v<<amt;} if(amt==32){*carry=v&1;return 0;} *carry=0; return 0;
    case 1: if(amt==0){ if(imm){amt=32;} else {*carry=c;return v;} } if(amt<32){*carry=(v>>(amt-1))&1; return v>>amt;} if(amt==32){*carry=v>>31;return 0;} *carry=0;return 0;
    case 2: if(amt==0){ if(imm){amt=32;} else {*carry=c;return v;} } if(amt<32){*carry=(v>>(amt-1))&1; return (u32)((s32)v>>amt);} *carry=v>>31; return (v>>31)?0xFFFFFFFF:0;
    case 3: if(amt==0){ if(imm){ *carry=v&1; return (v>>1)|((u32)c<<31);} *carry=c; return v; }
      amt&=31; if(amt==0){*carry=v>>31; return v;} *carry=(v>>(amt-1))&1; return (v>>amt)|(v<<(32-amt));
  }
  return v;
}

static void swi(u32 n);

static void exec_arm(u32 op){
  if(!cond(op>>28)) return;
  if((op&0x0FFFFFF0)==0x012FFF10){ u32 t=R[op&15]; if(t&1){ CPSR|=0x20; setpc(t&~1);} else { CPSR&=~0x20; setpc(t&~3);} return; }
  if((op&0x0FC000F0)==0x00000090){ int rd=(op>>16)&15, rn=(op>>12)&15, rs=(op>>8)&15, rm=op&15; u32 r=R[rm]*R[rs]; if(op&(1<<21)) r+=R[rn]; R[rd]=r; if(op&(1<<20)) setNZ(r); return; }
  if((op&0x0F8000F0)==0x00800090){ int hi=(op>>16)&15, lo=(op>>12)&15, rs=(op>>8)&15, rm=op&15; unsigned long long r;
    if(op&(1<<22)) r=(unsigned long long)((long long)(s32)R[rm]*(long long)(s32)R[rs]); else r=(unsigned long long)R[rm]*R[rs];
    if(op&(1<<21)) r+=((unsigned long long)R[hi]<<32)|R[lo];
    R[lo]=(u32)r; R[hi]=r>>32; if(op&(1<<20)){ CPSR=(CPSR&~(FN|FZ))|((r>>63)?FN:0)|(r?0:FZ);} return; }
  if((op&0x0FB00FF0)==0x01000090){ int rn=(op>>16)&15, rd=(op>>12)&15, rm=op&15; u32 a=R[rn];
    if(op&(1<<22)){ u8 t=rd8(a); wr8(a,R[rm]); R[rd]=t;} else { u32 t=rd32(a); wr32(a,R[rm]); R[rd]=t;} return; }
  if((op&0x0E000090)==0x00000090 && (op&0x60)){
    int P=(op>>24)&1,U=(op>>23)&1,I=(op>>22)&1,W=(op>>21)&1,L=(op>>20)&1; int rn=(op>>16)&15, rd=(op>>12)&15; int sh=(op>>5)&3;
    u32 off = I ? (((op>>4)&0xF0)|(op&15)) : R[op&15];
    u32 base=R[rn]; u32 addr = P ? (U?base+off:base-off) : base;
    if(L){ u32 v; if(sh==1) { v=rd16(addr); if(addr&1) v=(v>>8)|(v<<24);} else if(sh==2) v=(u32)(s32)(int8_t)rd8(addr); else { if(addr&1) v=(u32)(s32)(int8_t)rd8(addr); else v=(u32)(s32)(int16_t)rd16(addr);}
      if(!P){ R[rn]= U?base+off:base-off; } else if(W) R[rn]=addr; wreg(rd,v); }
    else { u32 v=R[rd]; if(rd==15) v+=4; if(sh==1) wr16(addr,v); if(!P){ R[rn]= U?base+off:base-off; } else if(W) R[rn]=addr; }
    return; }
  if((op&0x0FBF0FFF)==0x010F0000){ R[(op>>12)&15] = (op&(1<<22))? get_spsr() : CPSR; return; }
  if((op&0x0DB0F000)==0x0120F000){
    u32 v; if(op&(1<<25)){ u32 imm=op&0xFF, rot=((op>>8)&15)*2; v=(imm>>rot)|(imm<<((32-rot)&31)); } else v=R[op&15];
    u32 mask=0; if(op&(1<<16)) mask|=0xFF; if(op&(1<<17)) mask|=0xFF00; if(op&(1<<18)) mask|=0xFF0000; if(op&(1<<19)) mask|=0xFF000000;
    if(op&(1<<22)){ set_spsr((get_spsr()&~mask)|(v&mask)); }
    else { if(midx(CPSR)==0) mask&=0xFF000000; set_cpsr((CPSR&~mask)|(v&mask)); }
    return; }
  if((op&0x0C000000)==0x00000000){
    int opc=(op>>21)&15, S=(op>>20)&1, rn=(op>>16)&15, rd=(op>>12)&15; u32 b; int carry;
    if(op&(1<<25)){ u32 imm=op&0xFF, rot=((op>>8)&15)*2; b= rot? (imm>>rot)|(imm<<(32-rot)) : imm; carry = rot? (b>>31) : !!(CPSR&FC); }
    else { int type=(op>>5)&3; u32 rmv=R[op&15]; if(op&0x10){ if((op&15)==15) rmv+=4; b=shifter(type,rmv,R[(op>>8)&15]&0xFF,0,&carry);} else b=shifter(type,rmv,(op>>7)&31,1,&carry); }
    u32 a=R[rn]; if(rn==15 && !(op&(1<<25)) && (op&0x10)) a+=4;
    u32 res=0; int logical=0, wr=1; int s = S && rd!=15;
    switch(opc){
      case 0: res=a&b; logical=1; break; case 1: res=a^b; logical=1; break;
      case 2: res=subf(a,b,1,s); break; case 3: res=subf(b,a,1,s); break;
      case 4: res=addf(a,b,0,s); break; case 5: res=addf(a,b,!!(CPSR&FC),s); break;
      case 6: res=subf(a,b,!!(CPSR&FC),s); break; case 7: res=subf(b,a,!!(CPSR&FC),s); break;
      case 8: res=a&b; logical=1; wr=0; break; case 9: res=a^b; logical=1; wr=0; break;
      case 10: res=subf(a,b,1,1); wr=0; break; case 11: res=addf(a,b,0,1); wr=0; break;
      case 12: res=a|b; logical=1; break; case 13: res=b; logical=1; break;
      case 14: res=a&~b; logical=1; break; case 15: res=~b; logical=1; break;
    }
    if(logical && (S && (rd!=15 || !wr))){ setNZ(res); setC(carry); }
    if(wr){ if(rd==15){ if(S){ u32 sp=get_spsr(); set_cpsr(sp); setpc(res & ((sp&0x20)?~1u:~3u)); } else setpc(res&~1);} else R[rd]=res; }
    return; }
  if((op&0x0C000000)==0x04000000){
    int I=(op>>25)&1,P=(op>>24)&1,U=(op>>23)&1,B=(op>>22)&1,W=(op>>21)&1,L=(op>>20)&1; int rn=(op>>16)&15, rd=(op>>12)&15; u32 off; int c;
    if(!I) off=op&0xFFF; else off=shifter((op>>5)&3,R[op&15],(op>>7)&31,1,&c);
    u32 base=R[rn]; u32 addr=P?(U?base+off:base-off):base;
    if(L){ u32 v; if(B) v=rd8(addr); else { v=rd32(addr); int r=(addr&3)*8; if(r) v=(v>>r)|(v<<(32-r)); }
      if(!P) R[rn]=U?base+off:base-off; else if(W) R[rn]=addr; wreg(rd,v); }
    else { u32 v=R[rd]; if(rd==15) v+=4; if(B) wr8(addr,v); else wr32(addr,v); if(!P) R[rn]=U?base+off:base-off; else if(W) R[rn]=addr; }
    return; }
  if((op&0x0E000000)==0x08000000){
    int P=(op>>24)&1,U=(op>>23)&1,S=(op>>22)&1,W=(op>>21)&1,L=(op>>20)&1; int rn=(op>>16)&15; u32 list=op&0xFFFF;
    int n=__builtin_popcount(list); u32 base=R[rn]; u32 start;
    if(U) start = P? base+4 : base; else start = P? base-4*n : base-4*n+4;
    u32 nb = U? base+4*n : base-4*n;
    int usr = S && !(L && (list&0x8000)); int om=0; u32 savecpsr=CPSR;
    if(usr){ om=1; set_cpsr((CPSR&~0x1f)|0x1f); }
    u32 a=start;
    for(int i=0;i<16;i++) if(list&(1<<i)){
      if(L){ u32 v=rd32(a); if(i==15){ if(S){ u32 sp=get_spsr(); set_cpsr(sp); setpc(v&((sp&0x20)?~1u:~3u)); } else setpc(v&~3);} else R[i]=v; }
      else { u32 v=R[i]; if(i==15) v+=4; wr32(a,v); }
      a+=4; }
    if(om) set_cpsr((CPSR&~0x1f)|(savecpsr&0x1f));
    if(W && !(L && (list&(1<<rn)))) R[rn]=nb;
    return; }
  if((op&0x0E000000)==0x0A000000){ s32 off=(s32)(op<<8)>>6; if(op&(1<<24)) R[14]=cur_pc+4; setpc(R[15]+off); return; }
  if((op&0x0F000000)==0x0F000000){ swi((op>>16)&0xFF); return; }
  fprintf(stderr,"undefined ARM op %08x at %08x\n",op,cur_pc); fault=1;
}

static void exec_thumb(u16 op){
  switch(op>>13){
    case 0: {
      if((op>>11)==3){ int rd=op&7, rs=(op>>3)&7; u32 b= (op&(1<<10))? ((op>>6)&7) : R[(op>>6)&7];
        R[rd] = (op&(1<<9)) ? subf(R[rs],b,1,1) : addf(R[rs],b,0,1); return; }
      int type=(op>>11)&3, rd=op&7, rs=(op>>3)&7, amt=(op>>6)&31; int c; u32 r=shifter(type,R[rs],amt,1,&c); R[rd]=r; setNZ(r); setC(c); return; }
    case 1: { int rd=(op>>8)&7; u32 imm=op&0xFF; switch((op>>11)&3){
        case 0: R[rd]=imm; setNZ(imm); break; case 1: subf(R[rd],imm,1,1); break;
        case 2: R[rd]=addf(R[rd],imm,0,1); break; case 3: R[rd]=subf(R[rd],imm,1,1); break; } return; }
    case 2: {
      if((op>>10)==0x10){ int rd=op&7, rs=(op>>3)&7; u32 a=R[rd], b=R[rs], r; int c;
        switch((op>>6)&15){
          case 0: r=a&b; R[rd]=r; setNZ(r); break; case 1: r=a^b; R[rd]=r; setNZ(r); break;
          case 2: r=shifter(0,a,b&0xFF,0,&c); R[rd]=r; setNZ(r); setC(c); break;
          case 3: r=shifter(1,a,b&0xFF,0,&c); R[rd]=r; setNZ(r); setC(c); break;
          case 4: r=shifter(2,a,b&0xFF,0,&c); R[rd]=r; setNZ(r); setC(c); break;
          case 5: R[rd]=addf(a,b,!!(CPSR&FC),1); break; case 6: R[rd]=subf(a,b,!!(CPSR&FC),1); break;
          case 7: r=shifter(3,a,b&0xFF,0,&c); R[rd]=r; setNZ(r); setC(c); break;
          case 8: setNZ(a&b); break; case 9: R[rd]=subf(0,b,1,1); break;
          case 10: subf(a,b,1,1); break; case 11: addf(a,b,0,1); break;
          case 12: r=a|b; R[rd]=r; setNZ(r); break; case 13: r=a*b; R[rd]=r; setNZ(r); break;
          case 14: r=a&~b; R[rd]=r; setNZ(r); break; case 15: r=~b; R[rd]=r; setNZ(r); break; }
        return; }
      if((op>>10)==0x11){ int o=(op>>8)&3; int rd=(op&7)|((op>>4)&8), rs=(op>>3)&15; u32 b=R[rs];
        switch(o){ case 0: wreg(rd, R[rd]+b); break; case 1: subf(R[rd],b,1,1); break; case 2: wreg(rd,b); break;
          case 3: if(b&1) setpc(b&~1); else { CPSR&=~0x20; setpc(b&~3);} break; }
        return; }
      if((op>>11)==9){ u32 a=((R[15])&~3)+((op&0xFF)<<2); R[(op>>8)&7]=rd32(a); return; }
      { int ro=(op>>6)&7, rb=(op>>3)&7, rd=op&7; u32 a=R[rb]+R[ro];
        if(!(op&(1<<9))){ switch((op>>10)&3){ case 0: wr32(a,R[rd]); break; case 1: wr8(a,R[rd]); break;
            case 2: { u32 v=rd32(a); int r=(a&3)*8; if(r) v=(v>>r)|(v<<(32-r)); R[rd]=v; } break; case 3: R[rd]=rd8(a); break; } }
        else { switch((op>>10)&3){ case 0: wr16(a,R[rd]); break; case 1: R[rd]=(u32)(s32)(int8_t)rd8(a); break;
            case 2: { u32 v=rd16(a); if(a&1) v=(v>>8)|(v<<24); R[rd]=v;} break; case 3: R[rd]= (a&1)? (u32)(s32)(int8_t)rd8(a) : (u32)(s32)(int16_t)rd16(a); break; } }
        return; } }
    case 3: { int rd=op&7, rb=(op>>3)&7; u32 off=(op>>6)&31; int B=(op>>12)&1, L=(op>>11)&1;
      if(B){ u32 a=R[rb]+off; if(L) R[rd]=rd8(a); else wr8(a,R[rd]); }
      else { u32 a=R[rb]+off*4; if(L){ u32 v=rd32(a); int r=(a&3)*8; if(r) v=(v>>r)|(v<<(32-r)); R[rd]=v;} else wr32(a,R[rd]); }
      return; }
    case 4: {
      if(!(op&(1<<12))){ int rd=op&7, rb=(op>>3)&7; u32 a=R[rb]+((op>>6)&31)*2; if(op&(1<<11)){ u32 v=rd16(a); if(a&1) v=(v>>8)|(v<<24); R[rd]=v;} else wr16(a,R[rd]); return; }
      { int rd=(op>>8)&7; u32 a=R[13]+(op&0xFF)*4; if(op&(1<<11)){ u32 v=rd32(a); int r=(a&3)*8; if(r) v=(v>>r)|(v<<(32-r)); R[rd]=v;} else wr32(a,R[rd]); return; } }
    case 5: {
      if(!(op&(1<<12))){ int rd=(op>>8)&7; u32 imm=(op&0xFF)*4; R[rd] = (op&(1<<11)) ? R[13]+imm : (R[15]&~3)+imm; return; }
      if((op&0x0F00)==0x0000){ u32 imm=(op&0x7F)*4; if(op&0x80) R[13]-=imm; else R[13]+=imm; return; }
      if((op&0x0600)==0x0400){ int L=(op>>11)&1, Rb=(op>>8)&1; u32 list=op&0xFF;
        if(!L){ int n=__builtin_popcount(list)+Rb; u32 a=R[13]-4*n; R[13]=a; for(int i=0;i<8;i++) if(list&(1<<i)){ wr32(a,R[i]); a+=4;} if(Rb){ wr32(a,R[14]); } }
        else { u32 a=R[13]; for(int i=0;i<8;i++) if(list&(1<<i)){ R[i]=rd32(a); a+=4;} if(Rb){ u32 v=rd32(a); a+=4; setpc(v&~1);} R[13]=a; }
        return; }
      break; }
    case 6: {
      if(!(op&(1<<12))){ int rb=(op>>8)&7; u32 list=op&0xFF; u32 a=R[rb];
        if(op&(1<<11)){ for(int i=0;i<8;i++) if(list&(1<<i)){ R[i]=rd32(a); a+=4;} if(!(list&(1<<rb))) R[rb]=a; }
        else { for(int i=0;i<8;i++) if(list&(1<<i)){ wr32(a,R[i]); a+=4;} R[rb]=a; }
        return; }
      int c=(op>>8)&15; if(c==15){ swi(op&0xFF); return; } if(c==14) break;
      if(cond(c)){ s32 off=(s32)(int8_t)(op&0xFF)*2; setpc(R[15]+off); } return; }
    case 7: {
      int h=(op>>11)&3;
      if(h==0){ s32 off=((s32)(op<<21))>>20; setpc(R[15]+off); return; }
      if(h==2){ s32 off=((s32)(op<<21))>>9; R[14]=R[15]+off; return; }
      if(h==3){ u32 t=R[14]+((op&0x7FF)<<1); R[14]=(cur_pc+2)|1; setpc(t); return; }
      break; }
  }
  fprintf(stderr,"undefined THUMB op %04x at %08x\n",op,cur_pc); fault=1;
}

static void swi(u32 n){
  switch(n){
    case 0x02: halted=1; return;
    case 0x04: case 0x05: halted=2; return; // approximate: wait for next vblank
    case 0x06: { s32 a=R[0], b=R[1]; if(b==0){R[0]=0;R[1]=a;R[3]=0;return;} R[0]=a/b; R[1]=a%b; R[3]=abs(a/b); return; }
    case 0x07: { s32 a=R[1], b=R[0]; if(b==0){R[0]=0;return;} R[0]=a/b; R[1]=a%b; R[3]=abs(a/b); return; }
    case 0x0B: { u32 s=R[0], d=R[1], c=R[2]; u32 cnt=c&0x1FFFFF; int fill=(c>>24)&1, w=(c>>26)&1;
      if(w){ u32 v=rd32(s); for(u32 i=0;i<cnt;i++){ wr32(d, fill?v:rd32(s)); d+=4; if(!fill) s+=4; } }
      else { u16 v=rd16(s); for(u32 i=0;i<cnt;i++){ wr16(d, fill?v:rd16(s)); d+=2; if(!fill) s+=2; } } return; }
    case 0x0C: { u32 s=R[0], d=R[1], c=R[2]; u32 cnt=(c&0x1FFFFF); cnt=(cnt+7)&~7; int fill=(c>>24)&1; u32 v=rd32(s);
      for(u32 i=0;i<cnt;i++){ wr32(d, fill?v:rd32(s)); d+=4; if(!fill) s+=4; } return; }
  }
  fprintf(stderr,"unhandled SWI %02x at %08x\n",n,cur_pc);
}

static void step(void){
  cur_pc = next_pc; branched=0;
  if(cur_pc==bp_addr && bp_count>0){ bp_count--; printf("BP %08x r0=%08x r1=%d r2=%d r3=%d sp0=%d sp1=%d sp2=%d lr=%08x\n",cur_pc,R[0],R[1],R[2],R[3],rd32(R[13]),rd32(R[13]+4),rd32(R[13]+8),R[14]); }
  if(cur_pc==MAGIC_IRQ_RET){ // return from HLE IRQ
    set_cpsr(irq_save_CPSR); memcpy(R,irq_save_R,sizeof R); in_hle_irq=0; next_pc=R[15]; return; }
  if(prof_on){ prof[(cur_pc>>1)&0x1FFFF]++; }
  if(CPSR&0x20){ u16 op=rd16(cur_pc); R[15]=cur_pc+4; next_pc=cur_pc+2; exec_thumb(op); }
  else { u32 op=rd32(cur_pc); R[15]=cur_pc+8; next_pc=cur_pc+4; exec_arm(op); }
  if((cur_pc>>24)!=0x08 && (cur_pc>>24)!=0x02 && (cur_pc>>24)!=0x03){ fprintf(stderr,"PC in weird region %08x\n",cur_pc); fault=1; }
}

static void check_irq(void){
  u16 ie=io[0x200]|(io[0x201]<<8), iff=io[0x202]|(io[0x203]<<8), ime=io[0x208];
  if(halted==1 && (ie&iff)) halted=0;
  if(in_hle_irq) return;
  if((ime&1) && (ie&iff) && !(CPSR&0x80)){
    memcpy(irq_save_R,R,sizeof R); irq_save_R[15]=next_pc; irq_save_CPSR=CPSR;
    set_cpsr((CPSR&~0x3F)|0x92); bspsr[1]=irq_save_CPSR;
    R[14]=MAGIC_IRQ_RET; u32 h=rd32(0x03007FFC); in_hle_irq=1; halted=0;
    if(h&1){ CPSR|=0x20; next_pc=h&~1; } else next_pc=h&~3;
  }
}

static void run_timers(int cyc){
  static const int pres[4]={1,64,256,1024};
  for(int t=0;t<4;t++){
    u16 ctl=io[0x102+t*4]|(io[0x103+t*4]<<8); if(!(ctl&0x80)) continue; if(ctl&4) continue; // cascade unsupported
    tm_accum[t]+=cyc; u32 p=pres[ctl&3];
    while(tm_accum[t]>=p){ tm_accum[t]-=p; tm_counter[t]++; if(tm_counter[t]>0xFFFF){ tm_counter[t]=io[0x100+t*4]|(io[0x101+t*4]<<8); if(ctl&0x40){ io[0x202]|=(8<<t); } } }
  }
}

static const int CPI = 2; // cycles per instruction (approx)
static void run_cycles(int n){
  int done=0;
  while(done<n && !fault){
    check_irq();
    if(halted){ run_timers(n-done); done=n; break; }
    step(); done+=CPI; total_cycles+=CPI;
    if((done & 63)==0) run_timers(64);
  }
}

typedef int16_t s16;
static u8 frame_rgb[240*160*3];
static void render_line(int y){   // scanline renderer: uses the palette as it is while this line is drawn
  u16 dc=io[0]|(io[1]<<8); int mode=dc&7;
  // v8 phase 6: BG2 affine (reference point + PA..PD), BG2 mosaic and BLDCNT brighten/darken, as the GBA does them
  s16 pa=(s16)(io[0x20]|(io[0x21]<<8)), pb=(s16)(io[0x22]|(io[0x23]<<8)), pcc=(s16)(io[0x24]|(io[0x25]<<8)), pd=(s16)(io[0x26]|(io[0x27]<<8));
  s32 rx=(s32)((io[0x28]|(io[0x29]<<8)|(io[0x2A]<<16)|(io[0x2B]<<24))<<4)>>4, ry=(s32)((io[0x2C]|(io[0x2D]<<8)|(io[0x2E]<<16)|(io[0x2F]<<24))<<4)>>4;
  u16 bg2cnt=io[0x0C]|(io[0x0D]<<8), mos=io[0x4C]|(io[0x4D]<<8); int mz=(bg2cnt&0x40)!=0, mh=(mos&15)+1, mv=((mos>>4)&15)+1;
  u16 bld=io[0x50]|(io[0x51]<<8); int eff=(bld>>6)&3, evy=io[0x54]&31; if(evy>16) evy=16;
  int sy=mz? y-(y%mv) : y;
  for(int x=0;x<240;x++){
    u16 c; int isbg=0;
    int sx=mz? x-(x%mh) : x;
    if(dc&0x80) c=0x7FFF;
    else if((mode==4||mode==3) && (dc&0x400)){
      s32 tx=(rx+pa*sx+pb*sy)>>8, ty=(ry+pcc*sx+pd*sy)>>8;
      if(tx<0||ty<0||tx>=240||ty>=160) c=*(u16*)&pal[0];
      else if(mode==4){ u32 base=(dc&0x10)?0xA000:0; u8 ix=vram[base+ty*240+tx]; c=*(u16*)&pal[ix*2]; isbg=ix!=0; if(!ix) c=*(u16*)&pal[0]; }
      else { c=*(u16*)&vram[(ty*240+tx)*2]; isbg=1; }
    }
    else c=*(u16*)&pal[0];
    if(eff>=2 && ((isbg && (bld&4)) || (!isbg && (bld&0x20)))){
      int r=c&31, g=(c>>5)&31, b=(c>>10)&31;
      if(eff==2){ r+=((31-r)*evy)>>4; g+=((31-g)*evy)>>4; b+=((31-b)*evy)>>4; } else { r-=(r*evy)>>4; g-=(g*evy)>>4; b-=(b*evy)>>4; }
      c=(u16)(r|(g<<5)|(b<<10));
    }
    u8 *p=&frame_rgb[(y*240+x)*3]; p[0]=((c&31)*255)/31; p[1]=(((c>>5)&31)*255)/31; p[2]=(((c>>10)&31)*255)/31;
  }
}
static void run_frame(void){
  for(int line=0; line<228; line++){
    vcount=line;
    u16 ds=io[4]|(io[5]<<8);
    if(line==160){ dispstat_flags|=1; if(ds&8) io[0x202]|=1; if(halted==2) halted=0; }
    if(line==0) dispstat_flags&=~1;
    if(line==io[5] && (ds&0x20)) io[0x202]|=4;
    run_cycles(960);
    if(line<160) render_line(line);
    dispstat_flags|=2; if(ds&0x10) io[0x202]|=2;
    run_cycles(272);
    dispstat_flags&=~2;
  }
}

static void save_ppm(const char *fn){ FILE *f=fopen(fn,"wb"); fprintf(f,"P6\n240 160\n255\n"); fwrite(frame_rgb,1,sizeof frame_rgb,f); fclose(f); }

static int keybit(const char *k){
  const char *names[]={"A","B","SELECT","START","RIGHT","LEFT","UP","DOWN","R","L"};
  for(int i=0;i<10;i++) if(!strcmp(k,names[i])) return 1<<i; fprintf(stderr,"bad key %s\n",k); return 0;
}

int main(int argc, char **argv){
  if(argc<3){ fprintf(stderr,"usage: emu rom.gba script [sram]\n"); return 1; }
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); romsize=ftell(f); fseek(f,0,SEEK_SET); rom=malloc(romsize+4); fread(rom,1,romsize,f); fclose(f);
  memset(sram,0xFF,sizeof sram);
  io[0x21]=1; io[0x27]=1;   // BG2PA/BG2PD reset to 1.0, as on the GBA
  const char *sramfn = argc>3? argv[3] : NULL;
  if(sramfn){ FILE *s=fopen(sramfn,"rb"); if(s){ fread(sram,1,sizeof sram,s); fclose(s);} }
  // post-BIOS state
  CPSR=0x1F; R[13]=0x03007F00; b13[1]=0x03007FA0; b13[2]=0x03007FE0; b13[0]=0x03007F00; next_pc=0x08000000;
  FILE *sc = strcmp(argv[2],"-")? fopen(argv[2],"r") : stdin; char line[256]; long frame=0;
  while(fgets(line,sizeof line,sc) && !fault){
    char cmd[32], a1[64]="", a2[64]="", a3[64]=""; int n=sscanf(line,"%31s %63s %63s %63s",cmd,a1,a2,a3); if(n<1||cmd[0]=='#') continue;
    if(!strcmp(cmd,"wait")){ int k=atoi(a1); for(int i=0;i<k && !fault;i++){ run_frame(); frame++; } }
    else if(!strcmp(cmd,"press")){ int b=keybit(a1); int k= n>2? atoi(a2):4; keys&=~b; for(int i=0;i<k&&!fault;i++){run_frame();frame++;} keys|=b; for(int i=0;i<4&&!fault;i++){run_frame();frame++;} }
    else if(!strcmp(cmd,"hold")){ keys&=~keybit(a1); }
    else if(!strcmp(cmd,"release")){ keys|=keybit(a1); }
    else if(!strcmp(cmd,"shot")){ save_ppm(a1); }
    else if(!strcmp(cmd,"savesram")){ FILE *s=fopen(a1,"wb"); fwrite(sram,1,sizeof sram,s); fclose(s); }
    else if(!strcmp(cmd,"peek")){ u32 a=strtoul(a1,0,16); int k=n>2?atoi(a2):16; printf("%08x:",a); for(int i=0;i<k;i++) printf(" %02x",rd8(a+i)); printf("\n"); }
    else if(!strcmp(cmd,"poke8")){ wr8(strtoul(a1,0,16), strtoul(a2,0,16)); }
    else if(!strcmp(cmd,"poke32")){ wr32(strtoul(a1,0,16), strtoul(a2,0,16)); }
    else if(!strcmp(cmd,"dump")){ FILE *d=fopen(a1,"wb"); fwrite(iwram,1,sizeof iwram,d); fwrite(ewram,1,sizeof ewram,d); fclose(d); }
    else if(!strcmp(cmd,"vdump")){ FILE *d=fopen(a1,"wb"); u16 dc=io[0]|(io[1]<<8); fwrite(&vram[(dc&0x10)?0xA000:0],1,38400,d); fwrite(pal,1,512,d); fclose(d); }
    else if(!strcmp(cmd,"waitscene")){ u32 v=strtoul(a1,0,0); int k=0; while(rd32(0x03000020)!=v && k<6000 && !fault){ run_frame(); frame++; k++; } if(k>=6000) printf("waitscene %u timeout (scene=%u)\n",v,rd32(0x03000020)); }
    else if(!strcmp(cmd,"waitw")){ u32 ad=strtoul(a1,0,16), v=strtoul(a2,0,0); int k=0; while(rd32(ad)!=v && k<20000 && !fault){ run_frame(); frame++; k++; } if(k>=20000) printf("waitw timeout\n"); }
    else if(!strcmp(cmd,"waitb")){ u32 ad=strtoul(a1,0,16), v=strtoul(a2,0,0); int k=0; while(rd8(ad)!=v && k<60000 && !fault){ run_frame(); frame++; k++; } if(k>=60000) printf("waitb timeout\n"); }
    else if(!strcmp(cmd,"scene")){ printf("frame %ld scene %u\n",frame,rd32(0x03000020)); }
    else if(!strcmp(cmd,"watch")){ watch_lo=strtoul(a1,0,16); watch_hi=n>2?strtoul(a2,0,16):watch_lo; }
    else if(!strcmp(cmd,"unwatch")){ watch_lo=1; watch_hi=0; }
    else if(!strcmp(cmd,"bp")){ bp_addr=strtoul(a1,0,16); bp_count=n>2?atoi(a2):10; }
    else if(!strcmp(cmd,"auto")){ int k=atoi(a1); static int ah=0;
      for(int i=0;i<k && !fault;i++){
        u32 G=0x0203E000; keys=0x3FF;
        if(rd8(G+8)==7 && rd8(G+0xed)==2){ int game=rd8(G+0xec); s32 px=rd32(G+0xf8), py=rd32(G+0x100);
          if(game==0){ int best=-1, by=-1; s32 tx=px;
            for(int j=0;j<10;j++){ u32 o=G+0x120+j*20; if(!rd8(o+17)) continue; int t=rd8(o+16); s32 x=rd32(o), y=rd32(o+4)>>8;
              if(t!=1 && y<112 && y>by){ by=y; best=j; tx=x-10; } }
            for(int j=0;j<10;j++){ u32 o=G+0x120+j*20; if(!rd8(o+17)||rd8(o+16)!=1) continue; s32 x=rd32(o), y=rd32(o+4)>>8;
              if(y>70 && y<112 && x+10>=tx+4 && x<=tx+28){ tx = (x>px)? x-34 : x+16; } }
            if(tx<px-1) keys&=~0x20; else if(tx>px+1) keys&=~0x10; }
          else if(game==1){ int want=0; for(int j=0;j<10;j++){ u32 o=G+0x120+j*20; int ty=rd8(o+16); if(!rd8(o+17)||(ty!=0&&ty!=2)) continue; s32 x=((s32)rd32(o))>>8; s32 vx=-(s32)rd32(o+8);
              if(ty==0 && x > px+20 && x < px+20+ (vx*10>>8)+6) want=1;
              if(ty==2 && x > px+4 && x < px+4+ (vx*9>>8)+4) want=1; }
            if(want && py==0 && !ah){ keys&=~1; ah=1; } else ah=0; }
          else { if((frame%50)==0) keys&=~0x20; }
        }
        run_frame(); frame++; }
      keys=0x3FF; }
    else if(!strcmp(cmd,"carebot")){   // balance tests: a player who fixes whatever it can see or hear, a2 frames after it shows
      int k=atoi(a1), R=atoi(a2), th=a3[0]? atoi(a3) : 0; static long fix=-1; u32 P=0x03000028;
      for(int i=0;i<k && !fault;i++){
        run_frame(); frame++;
        int h=rd8(P+6), p=rd8(P+7), sick=rd8(P+10), poop=rd8(P+11), asl=rd8(P+12), lt=rd8(P+13), call=rd8(P+14), st=rd8(P+4);
        if(st<1 || st>7 || rd32(0x03000020)<1 || rd32(0x03000020)>5) continue;
        int vis = (!asl && (h<=th || p<=th || poop || sick || call)) || (asl && !lt);
        if(!vis){ fix=-1; continue; }
        if(fix<0) fix=frame+R;
        if(frame>=fix){ fix=-1;
          if(h<=th) wr8(P+6,4); if(p<=th) wr8(P+7,4); wr8(P+11,0); wr8(P+10,0); wr8(P+14,0); if(asl) wr8(P+13,1); }
      } }
    else if(!strcmp(cmd,"cyc")){ int k=atoi(a1); unsigned long long c0=total_cycles, mx=0, over=0; for(int i=0;i<k&&!fault;i++){ unsigned long long c1=total_cycles; run_frame();frame++; unsigned long long d=total_cycles-c1; if(d>mx) mx=d; if(d>=280896) over++; } printf("%s avg busy cycles/frame: %llu (of 280896) max %llu, %llu full frames\n", a2, (total_cycles-c0)/k, mx, over); }
    else if(!strcmp(cmd,"prof")){ int k=atoi(a1); memset(prof,0,sizeof prof); prof_on=1; for(int i=0;i<k&&!fault;i++){run_frame();frame++;} prof_on=0; FILE *pf=fopen(a2,"wb"); fwrite(prof,4,0x20000,pf); fclose(pf); }
    else if(!strcmp(cmd,"fuzzt")){ int k=atoi(a1); unsigned r=strtoul(a2,0,0)|1; u32 ad= a3[0]? strtoul(a3,0,16) : 0x0203E00C, last=0;
      for(int i=0;i<k && !fault;i++){ if(i%6==0){ r^=r<<13; r^=r>>17; r^=r<<5; int b=r%100; keys=0x3FF;
          if(b<18) keys&=~1; else if(b<30) keys&=~2; else if(b<36) keys&=~4; else if(b<39) keys&=~8; else if(b<50) keys&=~16; else if(b<61) keys&=~32; else if(b<70) keys&=~64; else if(b<79) keys&=~128; }
        run_frame(); frame++; u32 v=rd32(ad); if(v!=last){ printf("f%ld rng %08x FRAMES %u scene %u mode %u msg %u\n",frame,v,rd32(0x03000018),rd32(0x03000020),rd8(0x0203E008),rd8(0x0300003a)); last=v; } }
      keys=0x3FF; }
    else if(!strcmp(cmd,"fuzz")){ int k=atoi(a1); unsigned r=strtoul(a2,0,0)|1; int mc[8]={0};
      for(int i=0;i<k && !fault;i++){ if(i%6==0){ r^=r<<13; r^=r>>17; r^=r<<5; int b=r%100; keys=0x3FF;
          if(b<18) keys&=~1; else if(b<30) keys&=~2; else if(b<36) keys&=~4; else if(b<39) keys&=~8; else if(b<50) keys&=~16; else if(b<61) keys&=~32; else if(b<70) keys&=~64; else if(b<79) keys&=~128; }
        run_frame(); frame++; mc[rd8(0x0203E008)&7]++; }
      keys=0x3FF; printf("fuzz done frame %ld scene %u mode %u | mode frames:",frame,rd32(0x03000020),rd8(0x0203E008)); for(int m=0;m<8;m++) printf(" %d",mc[m]); printf("\n"); }
    else if(!strcmp(cmd,"objlog")){ int k=atoi(a1); FILE *lf=fopen(a2,"a"); for(int i=0;i<k&&!fault;i++){ run_frame(); frame++; u32 G=0x0203E000; fprintf(lf,"%ld %d",frame,rd8(G+0xec)); for(int j=0;j<10;j++){ u32 o=G+0x120+j*20; if(rd8(o+17)) fprintf(lf," %d:%d:%d:%d",j,(s32)rd32(o),(s32)rd32(o+4),rd8(o+16)); } fprintf(lf,"\n"); } fclose(lf); }
    else if(!strcmp(cmd,"trace")){ int k=atoi(a1); u32 ad=strtoul(a2,0,16); u32 last=0xFFFFFFFF; for(int i=0;i<k&&!fault;i++){ run_frame(); frame++; u32 v=rd32(ad); if(v!=last){ printf("f%ld %08x scene %u mode %u\n",frame,v,rd32(0x03000020),rd8(0x0203E008)); last=v; } } }
    else if(!strcmp(cmd,"echo")){ printf("%s", line+5); }
  }
  if(sramfn){ FILE *s=fopen(sramfn,"wb"); fwrite(sram,1,sizeof sram,s); fclose(s); }
  if(fault){ fprintf(stderr,"FAULT at frame %ld pc=%08x\n",frame,cur_pc); for(int i=0;i<16;i++) fprintf(stderr,"r%d=%08x ",i,R[i]); fprintf(stderr,"cpsr=%08x\n",CPSR); return 2; }
  return 0;
}
