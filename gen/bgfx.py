# Phase 1 graphics: richer room + yard backgrounds.
# Inputs (gen/bg): the game's own background bitmaps (palette indices) and its day/night palettes.
# Output: ../bgfx.h with three 240x160 bitmaps and up to EXTRA_N extra palette colours (each with a
# day and a night value; the original game darkens the room by swapping palette colours at night).
import struct, random, math
import numpy as np
D='bg/'
W,H=240,160
def rd(n): return open(D+n,'rb').read()
PD=struct.unpack('<256H',rd('pal_day.bin')); PN=struct.unpack('<256H',rd('pal_night.bin'))
rgb=lambda v:((v&31)<<3,((v>>5)&31)<<3,((v>>10)&31)<<3)
DAYC=[rgb(v) for v in PD]; NIGHTC=[rgb(v) for v in PN]
ROOM=list(rd('bg_room_day.bin')); YD=list(rd('bg_yard_day.bin')); YN=list(rd('bg_yard_night.bin'))
# v8.3: the wall picture hangs 8 px higher, to make room for the fireplace under it (ext.c draws the picture at y 35)
PIC_UP=8
_pf=[(x,y) for y in range(28,100) for x in range(150,240) if ROOM[y*W+x] in (35,6)]
_py0=min(p[1] for p in _pf); _keep={p:ROOM[p[1]*W+p[0]] for p in _pf}
for (x,y) in _pf: ROOM[y*W+x]=ROOM[(_py0-3)*W+x]          # the wall behind it (its stripes run straight down)
for (x,y),v in _keep.items(): ROOM[(y-PIC_UP)*W+x]=v
EXTRA0, EXTRA_N = 144, 112
LOCKED=set(range(80,144))|{9}            # sky/clouds/sun/moon, wall picture, bar colour handled separately
def clamp(c): return tuple(max(0,min(255,int(round(v)))) for v in c)
def mix(c,d,t): return clamp(tuple(c[i]+(d[i]-c[i])*t for i in range(3)))
def dk(c,t): return mix(c,(24,22,48),t)
def lt(c,t): return mix(c,(255,250,236),t)
def snap(c): return tuple((v>>3)<<3 for v in c)

class Pic:
    """one background: per pixel either a kept palette index or a (day,night) colour pair"""
    def __init__(s,base): s.b=base[:]; s.day=[None]*(W*H); s.night=[None]*(W*H)
    def idx(s,x,y): return s.b[y*W+x]
    def set(s,x,y,fn):
        """fn(colour) -> colour, applied to the pixel's current day and night colour"""
        i=y*W+x; b=s.b[i]
        if b in LOCKED and b!=9: return
        d=s.day[i] if s.day[i] else DAYC[b]; n=s.night[i] if s.night[i] else NIGHTC[b]
        s.day[i]=fn(d,0); s.night[i]=fn(n,1)
    def put(s,x,y,day,night=None):
        i=y*W+x
        if s.b[i] in LOCKED and s.b[i]!=9: return
        s.day[i]=day; s.night[i]=night if night else day
# night version of a colour that is given as a day colour (follows how the game darkens its wall)
WD,WN=DAYC[3],NIGHTC[3]
def nightify(c): return clamp(tuple(c[i]*WN[i]/WD[i] for i in range(3)))
random.seed(11)

# ------------------------------------------------------------------ ROOM (one bitmap, palette does day/night)
R=Pic(ROOM)
WALLS={3,4}; FLOOR={5,35,6}; RUG={29,30,34}
floor_top=min(y for y in range(28,132) for x in range(100,140) if ROOM[y*W+x] in FLOOR)
for y in range(28,floor_top):
    for x in range(W):
        b=R.idx(x,y)
        if b not in WALLS: continue
        def f(c,n,x=x,y=y,b=b):
            if y<34 and ((x+y)&1 or y<31): c=dk(c,0.10+0.03*(33-y))
            if b==4 and (y-30)%10==0 and x%24 in (8,9): c=lt(c,0.30)
            return c
        R.set(x,y,f)
        P5d,P5n=DAYC[5],NIGHTC[5]
        if y==83: R.put(x,y,dk(P5d,0.30),dk(P5n,0.30))
        elif y in (84,86): R.put(x,y,lt(P5d,0.18),lt(P5n,0.10))
        elif y==85: R.put(x,y,P5d,P5n)
        elif y>86:
            R.set(x,y,lambda c,n,x=x: (dk(mix(c,(DAYC if not n else NIGHTC)[5],0.28),0.15) if x%30==0 else
                                       lt(mix(c,(DAYC if not n else NIGHTC)[5],0.28),0.18) if x%30==1 else
                                       mix(c,(DAYC if not n else NIGHTC)[5],0.28)))
# floor planks
for y in range(floor_top,132):
    for x in range(W):
        if R.idx(x,y) not in FLOOR: continue
        row=(y-floor_top)//6; off=(row*37)%60; plank=(x+off)//60
        k=(row*3+plank)%4; sp=random.random()
        def f(c,n,x=x,y=y,row=row,off=off,k=k,sp=sp):
            base=(DAYC if not n else NIGHTC)[5]
            c=[base,dk(base,0.06),lt(base,0.07),dk(base,0.11)][k]
            r=(y-floor_top)%6
            if r==0: c=dk(c,0.25)
            elif r==1: c=lt(c,0.12)
            if (x+off)%60==0: c=dk(c,0.30)
            if sp<0.04: c=dk(c,0.08)
            if y==floor_top: c=dk(c,0.35)
            elif y==floor_top+1: c=dk(c,0.20)
            return c
        R.set(x,y,f)
# rug: lit top edge, shaded bottom edge
for y in range(floor_top,132):
    for x in range(W):
        if R.idx(x,y) in RUG:
            up=R.idx(x,y-1); dn=R.idx(x,y+1)
            if up not in RUG: R.set(x,y,lambda c,n: lt(c,0.18))
            elif dn not in RUG: R.set(x,y,lambda c,n: dk(c,0.22))
# window: curtains on both sides, shadow under the sill
glass=[(x,y) for y in range(28,100) for x in range(W) if 80<=ROOM[y*W+x]<=83]
gx0=min(p[0] for p in glass); gx1=max(p[0] for p in glass); gy0=min(p[1] for p in glass); gy1=max(p[1] for p in glass)
frame=[(x,y) for y in range(28,100) for x in range(0,110) if ROOM[y*W+x]==8]
fx0=min(p[0] for p in frame); fx1=max(p[0] for p in frame); fy1=max(p[1] for p in frame)
CUR=[(222,120,140),(190,96,118),(150,70,95),(238,150,166)]
for side,(c0,c1) in enumerate(((fx0-3,fx0+7),(fx1-7,fx1+3))):
    for x in range(c0,c1+1):
        mid=(c0+c1)/2
        for y in range(gy0-6, fy1+5-int(abs(x-mid)//2)):
            if 80<=R.idx(x,y)<=83: continue
            if x in (c0,c1): c=CUR[2]
            elif (x-c0)%3==0: c=CUR[1]
            elif (x-c0)%3==1 and y<gy0+2: c=CUR[3]
            else: c=CUR[0]
            R.put(x,y,c,nightify(c))
    rod_y=gy0-7
for x in range(fx0-5,fx1+6): R.put(x,gy0-7,dk(DAYC[5],0.2),dk(NIGHTC[5],0.2)); R.put(x,gy0-8,lt(DAYC[5],0.2),lt(NIGHTC[5],0.1))
for x in range(fx0,fx1+1):
    for y in (fy1+1,fy1+2):
        if R.idx(x,y) in WALLS: R.set(x,y,lambda c,n,y=y: dk(c,0.28 if y==fy1+1 else 0.14))
# picture frame: bevel + shadow
pf=[(x,y) for y in range(28,100) for x in range(150,240) if ROOM[y*W+x] in (35,6)]
px0=min(p[0] for p in pf); px1=max(p[0] for p in pf); py0=min(p[1] for p in pf); py1=max(p[1] for p in pf)
for (x,y) in pf:
    if x<=px0+1 or y<=py0+1: R.set(x,y,lambda c,n: lt(c,0.30))
    elif x>=px1-1 or y>=py1-1: R.set(x,y,lambda c,n: dk(c,0.25))
for x in range(px0+2,px1+3):
    for y in (py1+1,py1+2):
        if R.idx(x,y) in WALLS: R.set(x,y,lambda c,n,y=y: dk(c,0.25 if y==py1+1 else 0.12))
for y in range(py0+2,py1+3):
    for x in (px1+1,px1+2):
        if R.idx(x,y) in WALLS: R.set(x,y,lambda c,n,x=x: dk(c,0.20 if x==px1+1 else 0.10))
# sunbeam (moonbeam at night): the window opening, slid down and to the right, so the shaft starts at the glass and
# lands on the floor as a patch the shape of the window; it thins out (sparser dither) toward its edges and far end
K=0.9
for y in range(gy0,132):
    xl=gx0+max(0,y-fy1)*K; xr=gx1+(y-gy0)*K
    far=(y-gy0)/(131-gy0)
    for x in range(int(xl),min(W-1,int(xr))+1):
        if R.idx(x,y) not in WALLS|FLOOR|RUG: continue
        edge=min(x-xl,xr-x)
        dense= ((x+y)&1)==0 if edge>=3 and far<0.75 else ((x+y)&3)==0 and (y&1)==0
        if dense:
            R.set(x,y,lambda c,n,f=far: mix(c,(255,246,220),0.27-0.08*f) if not n else mix(c,(176,190,255),0.19-0.06*f))

# ------------------------------------------------------------------ BARS (all three bitmaps share them)
import baricons
ICON_POS=[(18+60*k,2) for k in range(4)]+[(18+60*k,135) for k in range(4)]
def bars(P):
    for (ix,iy) in ICON_POS:          # phase 3: clear the old doubled 12x12 icons...
        for y in range(iy,iy+24):
            for x in range(ix,ix+24):
                if P.idx(x,y) in (11,12): P.b[y*W+x]=9
    for y in list(range(0,26))+list(range(134,160)):
        for x in range(W):
            if P.idx(x,y)==9:
                t=(y if y<26 else y-134)/25
                c=DAYC[9]
                c= lt(c,0.12*(1-t*2)) if t<0.5 else dk(c,0.14*(t-0.5)*2)
                if y in (0,134): c=lt(DAYC[9],0.22)
                P.put(x,y,c,c)
    for k,(ix,iy) in enumerate(ICON_POS):   # ...and draw the new full-size ones (fixed colours, same day and night)
        g=baricons.GRIDS[baricons.ORDER[k]]
        tone={'x':11,'h':2,'s':18} if k!=7 else {'x':12,'h':12,'s':12}
        for y in range(24):
            for x in range(24):
                ch=g[y][x]
                if ch!='.': i=(iy+y)*W+ix+x; P.b[i]=tone[ch]; P.day[i]=None; P.night[i]=None
bars(R)

# ------------------------------------------------------------------ YARD
# the tree trunk, found in the day drawing (where it has its own colour): columns holding trunk colour 6 low down
_tcols=[x for x in range(0,48) if sum(1 for y in range(76,98) if YD[y*W+x]==6)>=6]
TX0,TX1=min(_tcols),max(_tcols)
TRUNK=[0]*(W*H)
for _y in range(56,101):
    for _x in range(TX0,TX1+1):
        if YD[_y*W+_x] in (6,5): TRUNK[_y*W+_x]=1
for _y in range(98,102):      # the root flare
    for _x in range(4,36):
        if YD[_y*W+_x]==6: TRUNK[_y*W+_x]=1
_BD=((96,64,44),(56,42,48)); _BM=((78,52,38),(46,35,42)); _BK=((58,38,30),(34,27,34)); _BL=((118,82,56),(68,54,58))
def BARK(x,y):
    """day and night colours for a trunk pixel: a lit left edge, a shaded right edge, darker grooves"""
    import random as _r; h=_r.Random(x*977+y*131).random()
    left= not TRUNK[y*W+x-1]; right= not TRUNK[y*W+x+1]
    if y>=98: c=_BM if (x+y)%3 else _BK               # roots: a little grain
    elif left: c=_BL                                   # lit left edge
    elif right: c=_BK                                  # shaded right edge
    else: c=_BD if h>0.10 else _BM                     # one even bark colour, with a few flecks
    return c
def yard(base,night):
    P=Pic(base)
    GR = (66,67) if night else (21,64)            # grass, tufts
    HL = 67 if night else 68                       # hills
    FW = (69,67) if night else (25,5)              # fence wood, fence shading
    CAN= (67,66) if night else (65,21)             # canopy dark, canopy light
    TR = (69,67) if night else (6,5)               # trunk, trunk stripe
    def at(x,y): return P.idx(x,y)
    # sky: dither the band edges (indices only, so the sky still follows the clock)
    for y in range(29,96):
        for x in range(W):
            b=at(x,y)
            if 80<=b<=83:
                dn=at(x,y+2) if y+2<H else b; up=at(x,y-2)
                if 80<=dn<=83 and dn==b+1 and (x+y)&1: P.b[y*W+x]=dn
                elif 80<=up<=83 and up==b-1 and ((x+y)&1)==0 and (x//2+y)&1: P.b[y*W+x]=up
    # fence geometry from the yard drawing
    def is_post(x): return x<214 and (x-4)%18<=6
    def in_fence(x,y): return x<214 and ((79<=y<=83 or 89<=y<=93) or (is_post(x) and 71<=y<=97))
    hz=(150,170,215) if night else (170,215,235)
    for y in range(60,100):
        for x in range(W):
            b=at(x,y)
            if b==HL and not in_fence(x,y) and not (x<48 and y<72) and x<212:
                up=at(x,y-1); rr=random.random()
                def f(c,n,up=up,rr=rr):
                    if 80<=up<=83 or 80<=at(x,y-2)<=83: c=lt(c,0.28 if not night else 0.12)
                    elif rr<0.08: c=dk(c,0.12)
                    return mix(c,hz,0.16 if not night else 0.10)
                P.set(x,y,f)
    for y in range(70,99):
        for x in range(214):
            b=at(x,y)
            if in_fence(x,y) and b in FW:
                l=at(x-1,y) if x else b; u=at(x,y-1); r_=at(x+1,y); d=at(x,y+1); rr=random.random()
                def f(c,n,l=l,u=u,r_=r_,d=d,rr=rr,b=b):
                    if b==FW[1]: return dk(c,0.10)
                    if l not in FW or u not in FW: return lt(c,0.28 if not night else 0.14)
                    if r_ not in FW or d not in FW: return dk(c,0.22)
                    return dk(c,0.14) if rr<0.07 else c
                P.set(x,y,f)
    fy=97
    # grass: depth gradient, extra tufts, fence and tree shadows
    for y in range(96,132):
        t=(y-96)/36
        for x in range(212):
            if at(x,y)==GR[0]:
                if abs(t-0.5)<0.06 and (x+y)&1: continue
                P.set(x,y,lambda c,n,t=t: lt(c,(0.12 if not night else 0.05)*(1-t*2)) if t<0.5 else dk(c,0.16*(t-0.5)*2))
    for k in range(80):
        x=random.randrange(2,208); y=random.randrange(100,130)
        if at(x,y)!=GR[0] or at(x+1,y-1)!=GR[0] or at(x-1,y-1)!=GR[0]: continue
        P.set(x,y,lambda c,n: dk(c,0.30)); P.set(x+1,y-1,lambda c,n: lt(c,0.22 if not night else 0.08)); P.set(x-1,y-1,lambda c,n: dk(c,0.18))
    for y in range(98,102):
        for x in range(214):
            if at(x,y) in GR and ((x+y)&1 or y<100): P.set(x,y,lambda c,n,y=y: dk(c,0.24 if y<100 else 0.14))
    def shadow(cx,cy,rx,ry,amt):
        for y in range(cy-ry,cy+ry+1):
            for x in range(cx-rx,cx+rx+1):
                e=((x-cx)/rx)**2+((y-cy)/ry)**2
                if 0<=x<W and e<=1 and at(x,y) in GR and ((x+y)&1 or e<0.45): P.set(x,y,lambda c,n: dk(c,amt))
    shadow(24,103,24,3,0.30)
    # tree: lit leaf clusters, shade, bark
    for y in range(26,102):
        for x in range(0,50):
            b=at(x,y); rr=random.random()
            if TRUNK[y*W+x]:   # v8.1: the trunk gets its own darker bark, by day and night (it used to share the
                P.put(x,y,*BARK(x,y))   # fence's colours at night, and its stripe showed the hills' green)
            elif y<72 and b in CAN:
                u=at(x,y-1); l=at(x-1,y) if x else b
                def f(c,n,b=b,u=u,l=l,rr=rr,x=x,y=y):
                    if b==CAN[1]:
                        if u!=CAN[1] or l!=CAN[1]: return lt(c,0.32 if not night else 0.12)
                        return lt(c,0.14) if rr<0.12 else c
                    if (y>56 or x>32) : c=dk(c,0.18 if (x+y)&1 else 0.10)
                    return lt(c,0.18 if not night else 0.08) if rr<0.06 else c
                P.set(x,y,f)
    # house: siding shade, under-roof shadow, roof edges, bevelled door, knob glint
    for y in range(28,132):
        for x in range(212,240):
            b=at(x,y)
            if b in ((70,36) if night else (3,4)):
                def f(c,n,x=x,y=y,b=b):
                    if b==(36 if night else 4) and x+1<W and at(x+1,y)==(70 if night else 3): c=dk(c,0.12)
                    if y<42: c=dk(c,0.24 if y<38 else 0.12)
                    if x<217: c=dk(c,0.08)
                    return c
                P.set(x,y,f)
            elif y<38 and b in ((36,) if night else (20,)):
                P.set(x,y,lambda c,n,y=y: lt(c,0.25) if at(x,y-1) not in ((36,) if night else (20,)) else (dk(c,0.25) if at(x,y+1) not in ((36,) if night else (20,)) else c))
            elif y>=88 and b in ((69,) if night else (35,5)):
                D_=((69,) if night else (35,5))
                l=at(x-1,y); u=at(x,y-1); r_=at(x+1,y) if x+1<W else 1; d=at(x,y+1)
                P.set(x,y,lambda c,n,l=l,u=u,r_=r_,d=d: lt(c,0.26) if (l not in D_ or u not in D_) else (dk(c,0.20) if (r_ not in D_ or d not in D_) else c))
            elif b==22 and y>100: P.put(x,y,(255,248,200))
    bars(P)
    return P
YDP=yard(YD,0); YNP=yard(YN,1)

# ------------------------------------------------------------------ palette: existing static colours + new shared colours
STATIC=[i for i in range(64) if i not in (0,9)]+list(range(64,73))
EXTRA_N=64   # phase 3: room and yard each get their own 64 colours (144-207); 208-255 hold the decor palettes
exist={ (snap(DAYC[i]),snap(NIGHTC[i])):i for i in STATIC }
def build_set(pics):
    need={}
    for nm,P in pics:
        for i in range(W*H):
            if P.day[i] is not None:
                k=(snap(P.day[i]),snap(P.night[i])); need[k]=need.get(k,0)+1
    newk=[k for k in need if k not in exist]
    X=np.array([k[0]+k[1] for k in newk],dtype=float); Wt=np.array([need[k] for k in newk],dtype=float)
    K=min(EXTRA_N,len(newk)); rng=np.random.default_rng(1)
    C=X[rng.choice(len(X),K,replace=False)] if len(X)>K else X.copy()
    for it in range(60):
        d=((X[:,None,:]-C[None,:,:])**2).sum(-1); a=d.argmin(1)
        for j in range(len(C)):
            m=a==j
            if m.any(): C[j]=(X[m]*Wt[m,None]).sum(0)/Wt[m].sum()
    C=np.clip(np.round(C),0,255)
    CAND=[(np.array(DAYC[i]+NIGHTC[i],dtype=float),i) for i in STATIC]+[(C[j],EXTRA0+j) for j in range(len(C))]
    CA=np.array([c for c,_ in CAND]); CI=[i for _,i in CAND]; cache={}
    def pick(k):
        if k in exist: return exist[k]
        if k in cache: return cache[k]
        v=np.array(k[0]+k[1],dtype=float); j=((CA-v)**2).sum(1).argmin(); cache[k]=CI[j]; return CI[j]
    out={}
    for nm,P in pics:
        o=P.b[:]
        for i in range(W*H):
            if P.day[i] is not None: o[i]=pick((snap(P.day[i]),snap(P.night[i])))
        out[nm]=o
    return C,out
CR,outR=build_set([('ROOM',R)]); CY,outY=build_set([('YARD_DAY',YDP),('YARD_NIGHT',YNP)])
out={**outR,**outY}
def c15(c): return (int(c[0])>>3)|((int(c[1])>>3)<<5)|((int(c[2])>>3)<<10)
wall_idx=sorted(set(out['ROOM'][y*W+x] for y in range(28,132) for x in range(W) if ROOM[y*W+x] in WALLS))
wl=[0]*8
for i in wall_idx: wl[i>>5]|=1<<(i&31)
L=['// generated by gen/bgfx.py from the game\'s own backgrounds (gen/bg)','#define BGX_FIRST %d'%EXTRA0,'#define BGX_N %d'%EXTRA_N]
for nm,C in (('ROOM',CR),('YARD',CY)):
    C=list(C)+[C[-1]]*(EXTRA_N-len(C))
    L.append('static const u16 BGX_%s_DAYPAL[BGX_N]={%s};'%(nm,','.join(str(c15(c[:3])) for c in C)))
    L.append('static const u16 BGX_%s_NIGHTPAL[BGX_N]={%s};'%(nm,','.join(str(c15(c[3:])) for c in C)))
L.append('static const u32 BGX_WALLISH[8]={%s};'%','.join('0x%08xu'%v for v in wl))
for nm in ('ROOM','YARD_DAY','YARD_NIGHT'):
    L.append('static const u8 BGX_%s[38400] __attribute__((aligned(4)))={%s};'%(nm,','.join(map(str,out[nm]))))
open('../bgfx.h','w').write('\n'.join(L)+'\n')
# preview images
from PIL import Image
def show(o,night,C):
    im=Image.new('RGB',(W,H)); p=im.load()
    for y in range(H):
        for x in range(W):
            i=o[y*W+x]
            if i>=EXTRA0: c=C[i-EXTRA0][3:] if night else C[i-EXTRA0][:3]; c=tuple(int(v) for v in c)
            else: c=(NIGHTC if night else DAYC)[i]
            p[x,y]=snap(c)
    return im
G=Image.new('RGB',(W*2+4,H*2+4),(255,0,255))
G.paste(show(out['ROOM'],0,CR),(0,0)); G.paste(show(out['ROOM'],1,CR),(W+4,0)); G.paste(show(out['YARD_DAY'],0,CY),(0,H+4)); G.paste(show(out['YARD_NIGHT'],1,CY),(W+4,H+4))
G.resize(((W*2+4)*3,(H*2+4)*3),Image.NEAREST).save('/tmp/bgx_preview.png')
print('room colours',len(CR),'yard colours',len(CY),' wall indices:',len(wall_idx))
