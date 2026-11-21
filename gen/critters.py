# Critter Keep v7 critters: 13 forms x 6 poses, 48x48, generated from simple shapes with lighting.
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
import math
W=48
L=(-0.55,-0.75,0.45); _n=math.sqrt(sum(v*v for v in L)); L=tuple(v/_n for v in L)
OUTLINE=(28,24,56)
def clamp(c): return tuple(max(0,min(255,int(round(v)))) for v in c)
def mix(a,b,t): return clamp(tuple(a[i]+(b[i]-a[i])*t for i in range(3)))
def ramp(c):   # highlight, light, base, shade, deep
    return [mix(c,(255,252,240),0.42),mix(c,(255,250,236),0.18),c,mix(c,(40,30,80),0.22),mix(c,(30,20,70),0.42)]
class Canvas:
    def __init__(s,k=1.0):
        s.k=k; s.N=int(round(48*k)); N=s.N
        s.mat=[[None]*N for _ in range(N)]; s.tone=[[2]*N for _ in range(N)]; s.flat=[[None]*N for _ in range(N)]
    def shade_px(s,x,y,col,nx,ny,shade=True,bias=0.0):
        if not(0<=x<s.N and 0<=y<s.N): return
        if shade:
            nz=math.sqrt(max(0.0,1-nx*nx-ny*ny)); d=nx*L[0]+ny*L[1]+nz*L[2]+bias
            t=0 if d>0.82 else 1 if d>0.55 else 2 if d>0.18 else 3 if d>-0.15 else 4
        else: t=2
        s.mat[y][x]=col; s.tone[y][x]=t; s.flat[y][x]=None
    def shape(s,inside,box,col,nc,shade=True,bias=0.0):
        k=s.k; x0,y0,x1,y1=box; ncx,ncy,nrx,nry=nc
        for y in range(int(y0*k)-1,int(y1*k)+2):
            for x in range(int(x0*k)-1,int(x1*k)+2):
                u,v=(x+0.5)/k,(y+0.5)/k
                if inside(u,v):
                    nx,ny=(u-ncx)/nrx,(v-ncy)/nry; m=math.hypot(nx,ny)
                    if m>0.98: nx,ny=nx/m*0.98,ny/m*0.98
                    s.shade_px(x,y,col,nx,ny,shade,bias)
    def ellipse(s,cx,cy,rx,ry,col,shade=True,rot=0.0,wob=None,bias=0.0,clip=None,nc=None):
        if nc is not None:
            ca,sa=math.cos(rot),math.sin(rot); R=max(rx,ry)+1
            def ins(u,v):
                dx,dy=u-cx,v-cy; a=(dx*ca+dy*sa)/rx; b=(-dx*sa+dy*ca)/ry; return a*a+b*b<=1
            return s.shape(ins,(cx-R,cy-R,cx+R,cy+R),col,nc,shade,bias)
        k=s.k; R=max(rx,ry)+3; ca,sa=math.cos(rot),math.sin(rot)
        for y in range(int((cy-R)*k)-1,int((cy+R)*k)+2):
            for x in range(int((cx-R)*k)-1,int((cx+R)*k)+2):
                dx,dy=(x+0.5)/k-cx,(y+0.5)/k-cy; u=(dx*ca+dy*sa)/rx; v=(-dx*sa+dy*ca)/ry
                r=math.hypot(u,v); lim=1.0 if wob is None else 1.0+wob(math.atan2(v,u))
                if r<=lim:
                    nx,ny=(dx/max(rx,1)),(dy/max(ry,1)); m=math.hypot(nx,ny)
                    if m>0.98: nx,ny=nx/m*0.98,ny/m*0.98
                    s.shade_px(x,y,col,nx,ny,shade,bias)
    def poly(s,pts,col,tone=None,nc=None):
        k=s.k; P=[(x*k,y*k) for x,y in pts]; xs=[p[0] for p in P]; ys=[p[1] for p in P]
        cx,cy=sum(xs)/len(xs),sum(ys)/len(ys)
        if nc is not None and tone is None:
            def ins(u,v):
                px,py=u*k,v*k; r=False; j=len(P)-1
                for i in range(len(P)):
                    xi,yi=P[i]; xj,yj=P[j]
                    if ((yi>py)!=(yj>py)) and px<(xj-xi)*(py-yi)/(yj-yi)+xi: r=not r
                    j=i
                return r
            return s.shape(ins,(min(xs)/k,min(ys)/k,max(xs)/k,max(ys)/k),col,nc)
        for y in range(int(min(ys))-1,int(max(ys))+2):
            for x in range(int(min(xs))-1,int(max(xs))+2):
                px,py=x+0.5,y+0.5; ins=False; j=len(P)-1
                for i in range(len(P)):
                    xi,yi=P[i]; xj,yj=P[j]
                    if ((yi>py)!=(yj>py)) and px<(xj-xi)*(py-yi)/(yj-yi)+xi: ins=not ins
                    j=i
                if ins and 0<=x<s.N and 0<=y<s.N:
                    if tone is not None: s.mat[y][x]=col; s.tone[y][x]=tone; s.flat[y][x]=None
                    else: s.shade_px(x,y,col,max(-.9,min(.9,(px-cx)/(12*k))),max(-.9,min(.9,(py-cy)/(12*k))))
    def dot(s,x,y,c):          # one 48-unit cell, scaled
        k=s.k; x0,y0=int(x*k),int(y*k); n=max(1,int(round(k)))
        for yy in range(y0,y0+n):
            for xx in range(x0,x0+n): s.px(xx,yy,c)
    def line(s,x0,y0,x1,y1,c,w=1):
        k=s.k; n=int(max(abs(x1-x0),abs(y1-y0))*2*k)+1; ww=max(1,int(round(w*k)))
        for i in range(n+1):
            t=i/n; x=(x0+(x1-x0)*t)*k; y=(y0+(y1-y0)*t)*k
            for ox in range(ww):
                for oy in range(ww): s.px(int(x)+ox-ww//2,int(y)+oy-ww//2,c)
    def px(s,x,y,c):           # one output pixel
        if 0<=x<s.N and 0<=y<s.N: s.mat[y][x]='__flat__'; s.flat[y][x]=c
    def rgb(s,ramps):
        N=s.N; out=[[None]*N for _ in range(N)]
        for y in range(N):
            for x in range(N):
                m=s.mat[y][x]
                if m is None or m=='__eye__': 
                    if m=='__eye__': out[y][x]=(34,28,64)
                    continue
                out[y][x]= s.flat[y][x] if m=='__flat__' else ramps[m][s.tone[y][x]]
        res=[row[:] for row in out]
        for y in range(N):
            for x in range(N):
                if out[y][x] is not None: continue
                ins=[(a,b) for a,b in ((x+1,y),(x-1,y),(x,y+1),(x,y-1)) if 0<=a<N and 0<=b<N and out[b][a] is not None]
                if ins:
                    a,b=ins[0]; m=s.mat[b][a]
                    lit = x<N*0.46 and y<N*0.54
                    res[y][x]= mix(ramps[m][4],OUTLINE,0.45) if (lit and m in ramps) else OUTLINE
        return res
# ------------------------------------------------------------------ faces (native pixel patterns, sized per output)
EYE=(34,28,64); HI=(255,255,255); MOUTHC=(60,30,60); TONG=(240,110,130); BLUSH=(250,150,175)
BEAK=(250,172,60); BEAKD=(196,104,40)
def P(c,x,y): return int(x*c.k), int(y*c.k)
def stamp(c,X,Y,rows,cmap,flip=False):
    h=len(rows); w=len(rows[0]); x0=X-w//2; y0=Y-h//2
    for j,r in enumerate(rows):
        if flip: r=r[::-1]
        for i,ch in enumerate(r):
            if ch!='.' and ch in cmap: c.px(x0+i,y0+j,cmap[ch])
def downscale(rows,f):
    h=len(rows); w=len(rows[0]); W2=max(1,round(w*f)); H2=max(1,round(h*f))
    out=[''.join(rows[min(h-1,int((y+0.5)/f))][min(w-1,int((x+0.5)/f))] for x in range(W2)) for y in range(H2)]
    if f>0.45 and not any('o' in r for r in out) and H2>=2 and W2>=2:
        out[0]=out[0][0]+'o'+out[0][2:] if W2>2 else 'o'+out[0][1:]
    return out
def eye_rows(style):
    if style in('round','big','lash'):
        w,h=(5,8) if style!='round' else (4,7)
        g=[['#']*w for _ in range(h)]
        for (x,y) in ((0,0),(w-1,0),(0,h-1),(w-1,h-1)): g[y][x]='.'
        g[1][1]='o'; g[1][2]='o'; g[2][1]='o'; g[h-2][w-2]='l'
        r=[''.join(q) for q in g]
        if style=='lash': r=['#......']+['.'+x+'.' for x in r]; r=[x.ljust(7,'.') for x in r]; r[1]='#'+r[1][1:]
        return r
    if style=='tall': return ['.#.','###','#o#','#o#','###','###','#l#','.#.']
    if style=='dot':  return ['.##.','#o##','####','.##.']
    if style=='sharp':return ['##....','####..','#####.','.#o###','..###.']
    if style=='sleepy':return ['######','######','.#o##.','..##..']
    if style=='owl':
        R=5.4; out=[]
        for y in range(11):
            row=''
            for x in range(11):
                d=math.hypot(x-5,y-5)
                ch='.' if d>R else '#' if d>4.6 else ('i' if d>2.9 else '#')
                if (x,y) in ((3,3),(4,3),(3,4),(4,4)): ch='o'
                if (x,y)==(6,6): ch='w'
                row+=ch
            out.append(row)
        return out
    if style=='glow': return ['.####.','######','#oo###','#oo###','######','#lllo#','#llll#','.llll.']
    return ['#']
def eye_open(c,x,y,style='round',sad=False,side=-1,iris=(255,186,60),light=(150,170,230)):
    X,Y=P(c,x,y); N=c.N
    if N<30:
        c.px(X,Y,EYE)
        if style in('owl','glow','big') and N>=20: c.px(X+1,Y,EYE)
        return
    rows=eye_rows(style)
    if N<60: rows=downscale(rows,N/64.0)
    cm={'#':EYE,'o':HI,'l':light,'i':iris,'w':HI}
    stamp(c,X,Y,rows,cm,flip=(side>0 and style in('sharp','lash')))
    if sad:
        w=len(rows[0]); h=len(rows)
        for i in range(w+1):
            yy=Y-h//2-2-(1 if (i<(w+1)//2)==(side<0) else 0)
            c.px(X-w//2-1+i if side<0 else X-w//2+i,yy,EYE)
def eye_line(c,x,y,curve,wide=False):
    X,Y=P(c,x,y); N=c.N
    if N<30: c.px(X,Y,EYE); return
    half=(4 if wide else 3) if N>=60 else (3 if wide else 2) if N>=44 else 1
    for dx in range(-half,half+1):
        e=abs(dx)==half
        if curve>0: dy=(1 if e else 0)-(1 if abs(dx)<=half-2 else 0) if N>=44 else (1 if e else 0)
        elif curve<0: dy=0 if e else 1
        else: dy=0
        c.px(X+dx,Y+dy,EYE)
HEART_L=['.##.##.','#w#####','#######','.#####.','..###..','...#...']
HEART_M=['##.##','#w###','#####','.###.','..#..']
HEART_S=['#.#','###','.#.']
MOUTHS_L={
 'smile':['#.....#','.#####.'], 'w':['#..#..#','.##.##.'], 'flat':['#####'], 'frown':['.###.','#...#'],
 'o':['.#.','#.#','.#.'], 'open':['#######','#######','.#ttt#.','..###..'], 'grin':['#######','.#ttt#.','..###..'],
 'fang':['#.....#','.#####.','..w....'], 'tooth':['#####','.ww..'], 'wavy':['#.#.#.#','.#.#.#.'],
 'smirk':['......#','#####.#'], 'tiny':['#.#.#','.#.#.'], 'beak':['bbbbb','.bbb.','..b..'], 'beak_open':['bbbbb','b###b','.bbb.'],
 'yuck':['#.#.#.#','.#.#.#.','...tt..','...tt..']}
MOUTHS_M={
 'smile':['#...#','.###.'], 'w':['#.#.#','.#.#.'], 'flat':['###'], 'frown':['.###.','#...#'], 'o':['.#.','#.#','.#.'],
 'open':['#####','#ttt#','.###.'], 'grin':['#####','.#t#.'], 'fang':['#...#','.###.','.w...'], 'tooth':['###','.w.'],
 'wavy':['#.#.#','.#.#.'], 'smirk':['....#','####.'], 'tiny':['#.#','.#.'], 'beak':['bbb','.b.'], 'beak_open':['bbb','b#b','.b.'],
 'yuck':['#.#.#','.#.#.','..t..']}
MOUTHS_S={'smile':['#.#','.#.'],'w':['#.#','.#.'],'flat':['##'],'frown':['.#.','#.#'],'o':['#'],'open':['##','##'],'grin':['###'],
 'fang':['#.#','.#.'],'tooth':['##'],'wavy':['#.#'],'smirk':['..#','##.'],'tiny':['#'],'beak':['bb','bb'],'beak_open':['bb','#b'],
 'yuck':['#.#','.t.']}
def mouth(c,x,y,kind):
    X,Y=P(c,x,y); N=c.N
    if N<30:
        if kind in('open','grin','beak','beak_open'): c.px(X,Y,BEAK if 'beak' in kind else MOUTHC)
        return
    tab=MOUTHS_L if N>=60 else MOUTHS_M if N>=44 else MOUTHS_S
    rows=tab[kind]; stamp(c,X,Y+len(rows)//2,rows,{'#':MOUTHC,'t':TONG,'w':HI,'b':BEAK} if 'beak' not in kind else {'#':MOUTHC,'b':BEAK})
def eye_squeeze(c,x,y,side):   # v8.1 yuck face: a squeezed-shut eye, > on the left and < on the right
    X,Y=P(c,x,y); N=c.N
    if N<30: c.px(X,Y,EYE); return
    h=2 if N>=60 else 1 if N>=44 else 1
    for i in range(-h,h+1):
        dx=(abs(i)-h)*side
        c.px(X+dx,Y+i,EYE)
        if N>=60: c.px(X+dx+side,Y+i,EYE)
def cheeks(c,x0,x1,y,big=False):
    N=c.N
    if N<30: return
    for xc in (x0,x1):
        X,Y=P(c,xc,y); rw=3 if N>=60 else 2; rh=2 if (N>=60 or big) else 1
        for dy in range(rh):
            for dx in range(-rw,rw+1):
                if ((dx+dy)&1 or abs(dx)<rw) and 0<=X+dx<N and 0<=Y+dy<N and c.mat[Y+dy][X+dx] not in (None,'__flat__'): c.px(X+dx,Y+dy,BLUSH)
def face(c,pose,cx,ex,ey,mx,my,style=('round','round'),mo='smile',blush=False,brow=False,iris=(255,186,60),light=(150,170,230)):
    if isinstance(style,str): style=(style,style)
    xl,xr=cx-ex,cx+ex; wide=style[0] in('owl','glow','big','lash'); beak=mo=='beak'
    if FACE=='love':
        if c.N>=30:
            rows=HEART_L if c.N>=60 else HEART_M if c.N>=44 else HEART_S
            for x in (xl,xr): X,Y=P(c,x,ey); stamp(c,X,Y,rows,{'#':(236,70,112),'w':(255,214,226)})
        else: eye_line(c,xl,ey,1,wide); eye_line(c,xr,ey,1,wide)
        mouth(c,mx,my,'beak_open' if beak else 'grin'); cheeks(c,xl-2,xr+2,ey+4); return
    if FACE=='call':
        eye_open(c,xl,ey,style[0],side=-1,iris=iris,light=light); eye_open(c,xr,ey,style[1],side=1,iris=iris,light=light)
        mouth(c,mx,my,'beak_open' if beak else 'open'); cheeks(c,xl-2,xr+2,ey+4); return
    if FACE=='yuck':   # v8.1: medicine tastes awful
        eye_squeeze(c,xl,ey,1); eye_squeeze(c,xr,ey,-1)
        mouth(c,mx,my+1,'beak_open' if beak else 'yuck'); return
    if FACE=='yawn':
        eye_line(c,xl,ey+1,-1,wide); eye_line(c,xr,ey+1,-1,wide); mouth(c,mx,my+1,'beak_open' if beak else 'open'); return
    if pose=='idle':
        eye_open(c,xl,ey,style[0],side=-1,iris=iris,light=light); eye_open(c,xr,ey,style[1],side=1,iris=iris,light=light); mouth(c,mx,my,mo)
        if blush: cheeks(c,xl-2,xr+2,ey+4)
        if brow and c.N>=30:
            X0,Y0=P(c,xl-2,ey-4); X1,Y1=P(c,xr+2,ey-4); t=2 if c.N>=60 else 1
            for x in range(X0,X1+1):
                yy=Y0+(1 if abs(x-(X0+X1)//2)<(X1-X0)//5 else 0)
                for q in range(t): c.px(x,yy+q,EYE)
    elif pose=='blink':
        eye_line(c,xl,ey+1,0,wide); eye_line(c,xr,ey+1,0,wide); mouth(c,mx,my,mo)
    elif pose=='happy':
        eye_line(c,xl,ey,1,wide); eye_line(c,xr,ey,1,wide); mouth(c,mx,my,'beak_open' if beak else 'grin'); cheeks(c,xl-2,xr+2,ey+4)
    elif pose=='sleep':
        eye_line(c,xl,ey+1,-1,wide); eye_line(c,xr,ey+1,-1,wide); mouth(c,mx,my+1,'beak' if beak else 'o')
    elif pose=='eat':
        eye_line(c,xl,ey,1,wide); eye_line(c,xr,ey,1,wide); mouth(c,mx,my,'beak_open' if beak else 'open'); cheeks(c,xl-2,xr+2,ey+4,True)
    elif pose=='sad':
        st=tuple('round' if s in('dot','sleepy','sharp') else s for s in style)
        eye_open(c,xl,ey+1,st[0],True,-1,iris,light); eye_open(c,xr,ey+1,st[1],True,1,iris,light); mouth(c,mx,my+1,'beak' if beak else 'frown')
        if c.N>=30:
            X,Y=P(c,xr+2,ey+5); c.px(X,Y,(140,200,255)); c.px(X,Y+1,(140,200,255)); c.px(X+1,Y+1,(200,230,255))
            if c.N>=60: c.px(X,Y+2,(140,200,255)); c.px(X+1,Y+2,(140,200,255))

POSES=['idle','blink','happy','sleep','eat','sad','walk1','walk2','breathe','sleep2','love','wave1','wave2','stretch','yuck']
# animation frames are drawn as one of the six base poses plus a modifier (legs, arms, breath, face)
BASE={'walk1':'idle','walk2':'idle','breathe':'idle','sleep2':'sleep','love':'happy','wave1':'idle','wave2':'idle','stretch':'sleep','yuck':'idle'}
FULL='idle'; BREATH=0; FACE=None
def LIFT(x):   # how far the foot at x is raised in a walk frame
    return 2.0 if (FULL=='walk1' and x<24) or (FULL=='walk2' and x>24) else 0.0
def ARMX(side,up=1):   # extra arm raise for walk/wave/stretch frames (up=-1 for angles measured downward)
    if FULL in('wave1','wave2') and side>0: return up*(1.7 if FULL=='wave1' else 2.2)
    if FULL=='stretch': return up*2.0
    if FULL=='walk1': return up*0.3*side
    if FULL=='walk2': return -up*0.3*side
    return 0.0
def QLIFT(x):  # four-legged walk: diagonal pairs
    return 1.6 if (FULL=='walk1' and x in(9,30)) or (FULL=='walk2' and x in(18,39)) else 0.0
def body_xf(pose):   # (dy, sx, sy)
    dy,sx,sy={'idle':(0,1,1),'blink':(0,1,1),'happy':(-3,0.96,1.05),'sleep':(2,1.05,0.92),'eat':(0,1.02,1),'sad':(1,1.02,0.97)}[BASE.get(pose,pose)]
    return (dy-0.9*BREATH, sx*(1-0.012*BREATH), sy*(1+0.035*BREATH))
FORMS={}
def form(name,ramps,draw): FORMS[name]=(ramps,draw)
def star(cx,cy,r1,r2,n=5,rot=-math.pi/2):
    return [(cx+(r1 if i%2==0 else r2)*math.cos(rot+i*math.pi/n), cy+(r1 if i%2==0 else r2)*math.sin(rot+i*math.pi/n)) for i in range(2*n)]
def sprout(c,x,y,dy,leaf='leaf',lean=0.0,droop=0.0,two=False):
    # the little sprout every critter keeps from its child form
    tx,ty=x+lean*5,y-5+droop*3
    c.line(x,y+dy,tx,ty+dy,(70,130,64))
    if two:
        c.ellipse(tx-3.2,ty-1+dy+droop*3,3.6,1.8,leaf,rot=-0.5-droop); c.ellipse(tx+3.2,ty-1+dy+droop*3,3.6,1.8,leaf,rot=0.5+droop)
    else: c.ellipse(tx+2.6,ty-0.6+dy+droop*3,3.4,1.7,leaf,rot=-0.5+droop*1.6)
def tube(c,pts,mat,nc):
    def ins(u,v):
        for (x0,y0,r0),(x1,y1,r1) in zip(pts,pts[1:]):
            dx,dy=x1-x0,y1-y0; L2=dx*dx+dy*dy; t=max(0,min(1,((u-x0)*dx+(v-y0)*dy)/L2)) if L2 else 0
            r=r0+(r1-r0)*t
            if (u-x0-dx*t)**2+(v-y0-dy*t)**2<=r*r: return True
        return False
    xs=[p[0] for p in pts]; ys=[p[1] for p in pts]; R=max(p[2] for p in pts)
    c.shape(ins,(min(xs)-R,min(ys)-R,max(xs)+R,max(ys)+R),mat,nc)
def retone(c,test,delta):
    for y in range(c.N):
        for x in range(c.N):
            m=c.mat[y][x]
            if m not in(None,'__flat__') and test(x/c.k,y/c.k,m): c.tone[y][x]=max(0,min(4,c.tone[y][x]+delta))
def recolor(c,test,newmat):
    for y in range(c.N):
        for x in range(c.N):
            m=c.mat[y][x]
            if m not in(None,'__flat__') and test(x/c.k,y/c.k,m): c.mat[y][x]=newmat

# ------------------------------------------------------------------ EGG: speckled egg with a zigzag band
def F_EGG(c,pose):
    tilt={'idle':0,'blink':-0.10,'happy':0.12,'sleep':0,'eat':0,'sad':-0.05}[pose]+{'walk1':-0.1,'walk2':0.1}.get(FULL,0); hop=-2 if pose=='happy' else 0
    c.ellipse(24,27+hop,14,18,'egg',rot=tilt)
    def band(u,v,m):
        if m!='egg': return False
        z=28+hop+(u-24)*-tilt + (1.8 if int((u+1)/3)%2 else -1.8)*(1-2*abs(((u+1)/3)%1-0.5))
        return abs(v-z)<1.7
    recolor(c,band,'band')
    for x,y,k in ((18,17,'pk'),(28,14,'bl'),(31,22,'pk'),(17,36,'bl'),(27,38,'pk'),(32,35,'bl')):
        c.ellipse(x+(y-27)*-tilt,y+hop,1.8,1.5,k)
    if pose=='sad':
        for (dx,dy) in ((0,0),(1,1),(0,2),(1,3),(2,4),(1,5)): c.dot(21+dx,14+dy,OUTLINE)
    if pose=='happy': c.dot(39,12,(255,240,160)); c.dot(8,16,(255,240,160))
form('EGG',{'egg':ramp((250,240,214)),'band':ramp((150,206,180)),'pk':ramp((246,160,186)),'bl':ramp((140,186,246))},F_EGG)

# ------------------------------------------------------------------ BLIPLET (baby): tiny gumdrop chick with a curl of sprout
def F_BLIPLET(c,pose):
    dy,sx,sy=body_xf(pose)
    sprout(c,24,24,dy,lean=0.25,droop=0.4 if pose in('sad','sleep') else 0)
    def ins(u,v):
        a=((u-24)/(11.5*sx))**2+((v-37-dy)/(8.5*sy))**2<=1
        b= 24+dy<=v<=37+dy and abs(u-24) < 11.5*sx*((v-23-dy)/14)**0.7
        return a or b
    flap=-3 if pose=='happy' else 0
    c.ellipse(12.5,36+dy+flap,2.6,3.6,'yel2',rot=0.6); c.ellipse(35.5,36+dy+flap,2.6,3.6,'yel2',rot=-0.6)
    c.shape(ins,(10,20+dy,38,46+dy),'yel',(24,34+dy,12,11))
    c.ellipse(24,40+dy,6,3.4,'bel',shade=False)
    face(c,pose,24,5.5,34+dy,24,38+dy,style='big',mo='tiny',blush=True)
form('BLIPLET',{'yel':ramp((255,220,104)),'yel2':ramp((250,190,80)),'bel':ramp((255,242,200)),'leaf':ramp((120,200,96))},F_BLIPLET)

# ------------------------------------------------------------------ SPROUTLE (child): upright bean on little legs, two leaves and a bud
def F_SPROUTLE(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.5 if pose in('sad','sleep') else 0
    c.line(24,18+dy,24,12+dy+droop*2,(70,130,64))
    c.ellipse(19,11+dy+droop*4,5.5,2.4,'leaf',rot=-0.45-droop); c.ellipse(29.5,10+dy+droop*4,5.5,2.4,'leaf',rot=0.55+droop)
    c.ellipse(24,11+dy+droop*2,1.8,2.2,'bud')
    c.ellipse(20,42-LIFT(20),2.6,4.2,'mint2'); c.ellipse(28,42-LIFT(28),2.6,4.2,'mint2')
    up=-0.9 if pose=='happy' else 0
    c.ellipse(13,30+dy,2,5.5,'mint2',rot=0.45+up); c.ellipse(35,30+dy,2,5.5,'mint2',rot=-0.45-up)
    c.ellipse(24,28+dy,10.5*sx,11*sy,'mint',nc=(24,30+dy,11.5,14)); c.ellipse(24,34+dy,11*sx,8*sy,'mint',nc=(24,30+dy,11.5,14))
    c.ellipse(24,35+dy,6,5.5,'bel',shade=False)
    face(c,pose,24,4.5,27+dy,24,31.5+dy,style='tall',mo='smile')
    if pose in('idle','blink'):
        for x in (17.5,19,29,30.5): c.dot(x,31+dy+(0.6 if x in(19,29) else 0),(96,170,110))
form('SPROUTLE',{'mint':ramp((156,222,166)),'mint2':ramp((120,196,140)),'leaf':ramp((92,180,84)),'bud':ramp((250,150,180)),'bel':ramp((226,248,222))},F_SPROUTLE)

# ------------------------------------------------------------------ FLUFFBUD: pink cotton-puff with long lop ears and a flower clip
def F_FLUFFBUD(c,pose):
    dy,sx,sy=body_xf(pose)
    for i in range(10):
        a=i*2*math.pi/10+0.3; c.ellipse(24+13*sx*math.cos(a),30+dy+12*sy*math.sin(a),5.6,5.6,'puff')
    c.ellipse(24,30+dy,14*sx,13*sy,'puff',nc=(24,30+dy,16,15))
    c.ellipse(24,32+dy,9.5,7.5,'patch',shade=False)
    lift=-5 if pose=='happy' else 2 if pose in('sad','sleep') else 0
    for sgn in (-1,1):
        x=24+sgn*12.5; c.ellipse(x,30+dy+lift,3.6,9.5,'ear',rot=-sgn*(0.18+(0.35 if pose=='happy' else 0)))
        c.ellipse(x+sgn*0.3,31+dy+lift,1.6,6.5,'inner',rot=-sgn*(0.18+(0.35 if pose=='happy' else 0)),shade=False)
    for i in range(5):
        a=i*2*math.pi/5-math.pi/2; c.ellipse(31+2.3*math.cos(a),15+dy+2.3*math.sin(a),1.7,1.7,'petal')
    c.ellipse(31,15+dy,1.2,1.2,'ctr',shade=False)
    face(c,pose,24,4.5,31+dy,24,35+dy,style='lash',mo='w',blush=True)
form('FLUFFBUD',{'puff':ramp((252,180,208)),'ear':ramp((236,140,180)),'inner':ramp((255,206,222)),'patch':ramp((255,234,240)),'hoof':ramp((200,120,150)),'petal':ramp((255,255,248)),'ctr':ramp((255,206,80))},F_FLUFFBUD)

# ------------------------------------------------------------------ GRUBBLE: grumpy segmented caterpillar curling off to one side
def F_GRUBBLE(c,pose):
    dy,sx,sy=body_xf(pose); w=-1 if pose=='happy' else 0
    segs=((43.5,30+w*2,3.6),(41,37,5),(34,39.5,6.3))
    for x,y,r in segs: c.ellipse(x,44.5,1.8,1.5,'feet')
    for i,(x,y,r) in enumerate(segs): c.ellipse(x,y+dy*0.5,r,r*0.95,'lav' if i%2 else 'lav2')
    c.ellipse(26,44.5,1.8,1.5,'feet'); c.ellipse(12,44.5,2,1.6,'feet')
    c.ellipse(17,31+dy,12*sx,12*sy,'lav')
    c.ellipse(17,38+dy,7,3.5,'bel',shade=False)
    for sgn in (-1,1):
        bx=17+sgn*5; tx=17+sgn*8+(sgn*2 if pose=='happy' else 0); ty=13+dy+(4 if pose in('sad','sleep') else 0)
        c.line(bx,21+dy,tx,ty,(110,90,150)); c.ellipse(tx,ty-1,2.2,1.6,'leaf',rot=sgn*0.5)
    for (x,y) in ((14,24),(20,25),(37,35),(33,35)): c.dot(x,y+dy,(240,200,120))
    face(c,pose,17,4.5,30+dy,17,36+dy,style='round',mo='tooth',brow=True)
form('GRUBBLE',{'lav':ramp((170,148,224)),'lav2':ramp((140,120,200)),'bel':ramp((224,212,248)),'feet':ramp((100,84,150)),'leaf':ramp((120,196,96))},F_GRUBBLE)

# ------------------------------------------------------------------ STARWING: a soft five-point star with little wings and gold tips
def F_STARWING(c,pose):
    dy,sx,sy=body_xf(pose); rot=-math.pi/2+(0.14 if pose=='happy' else -0.08 if pose=='sad' else 0)
    fl=-0.6 if pose=='happy' else 0.3 if pose in('sad','sleep') else 0
    for sgn in (-1,1):
        for j,(ox,oy,a) in enumerate(((10.5,34,0.35),(12,38,0.8))): c.ellipse(24+sgn*ox,oy+dy,4.6-j,2.2,'wing',rot=-sgn*(a+fl))
    pts=star(24,28+dy,19,10.5,rot=rot)
    c.poly(pts,'blue',nc=(24,28+dy,17,17))
    for i in range(0,10,2):
        x,y=pts[i]; x2,y2=24+(x-24)*0.86,28+dy+(y-28-dy)*0.86; c.ellipse(x2,y2,2.4,2.4,'gold')
    c.ellipse(24,32+dy,6.5,4.5,'bel',shade=False)
    for (x,y) in ((17,22),(31,24),(20,36),(29,37)): c.dot(x,y+dy,(255,236,150))
    face(c,pose,24,5,27+dy,24,32+dy,style='big',mo='smile')
form('STARWING',{'blue':ramp((110,170,250)),'wing':ramp((186,220,255)),'gold':ramp((255,210,70)),'bel':ramp((210,232,255))},F_STARWING)

# ------------------------------------------------------------------ MOSSMOO: low, wide mossy cow with a toadstool on its back
def F_MOSSMOO(c,pose):
    dy,sx,sy=body_xf(pose)
    for x in (9,18,30,39): u=QLIFT(x); c.ellipse(x,42.5-u,3,4,'hide2'); c.ellipse(x,45.3-u,3,1.2,'hoof',shade=False)
    for sgn in (-1,1): c.ellipse(24+sgn*19.5,29+dy+(2 if pose in('sad','sleep') else 0),4.2,2,'hide2',rot=sgn*(0.35 if pose!='happy' else -0.2))
    c.ellipse(24,32+dy,19*sx,10.5*sy,'hide')
    for x,y,r in ((10,26,4),(15,23.5,4.6),(21,22.5,4.8),(27,22.5,4.8),(33,23.5,4.6),(38,26,4)):
        c.ellipse(x,y+dy,r,r*0.75,'moss',wob=lambda a:0.12*math.sin(a*5))
    c.poly([(15,22+dy),(16,17+dy),(18,21+dy)],'horn'); c.poly([(33,22+dy),(32,17+dy),(30,21+dy)],'horn')
    c.ellipse(36,18+dy,1.2,2.4,'stem',shade=False); c.ellipse(36,16+dy,3.6,2.2,'cap')
    for x in (35,37.3): c.dot(x,15.5+dy,(255,250,240))
    sprout(c,12,22,dy,droop=0.3 if pose=='sad' else 0)
    c.ellipse(24,37.5+dy,8.5,5,'snout')
    for x in (21,27): c.dot(x,37+dy,(150,90,90))
    face(c,pose,24,8,30+dy,24,40.5+dy,style='sleepy',mo='flat')
form('MOSSMOO',{'hide':ramp((176,212,170)),'hide2':ramp((138,180,140)),'moss':ramp((96,160,80)),'horn':ramp((248,236,206)),'snout':ramp((250,206,196)),
                'hoof':ramp((110,90,80)),'cap':ramp((236,90,80)),'stem':ramp((250,240,220)),'leaf':ramp((140,214,110))},F_MOSSMOO)

# ------------------------------------------------------------------ GLOOMP: a melting puddle-blob with a wilted sprout
def F_GLOOMP(c,pose):
    dy,sx,sy=body_xf(pose); melt=1.5 if pose=='sad' else 0
    c.ellipse(24,43.5,19,3.6,'goo2')
    c.poly([(11,36+dy),(37,36+dy),(43,44),(5,44)],'goo',nc=(24,32,18,16))
    c.ellipse(24,29+dy+melt,12.5*sx,12.5*sy,'goo',nc=(24,32,18,16))
    for x,l in ((14,8),(32,10),(19,5)):
        for yy in range(int((28+dy)*c.k),int((28+dy+l)*c.k)):
            xx=int(x*c.k)
            if 0<=yy<c.N and c.mat[yy][xx]=='goo': c.tone[yy][xx]=min(4,c.tone[yy][xx]+1)
    c.ellipse(36,40.5,1.6,2.4,'goo'); c.ellipse(10.5,41,1.3,2,'goo')
    sprout(c,24,17.5+melt,dy,lean=0.6,droop=1.0 if pose!='happy' else 0.2)
    if pose!='happy': c.ellipse(36,23+dy,1.3,1.9,'drop')
    face(c,pose,24,5,28+dy+melt,24,34+dy+melt,style=('big','dot'),mo='wavy' if pose=='idle' else 'smile')
form('GLOOMP',{'goo':ramp((128,128,176)),'goo2':ramp((96,96,148)),'drop':ramp((170,210,255)),'leaf':ramp((150,170,110))},F_GLOOMP)

# ------------------------------------------------------------------ SUNPUFF: sunflower-maned lion cub on a small body
def F_SUNPUFF(c,pose):
    dy,sx,sy=body_xf(pose); spin=0.12 if pose=='happy' else 0; hy=22+dy
    c.ellipse(38,40,4,2.4,'fur2',rot=-0.6); c.ellipse(41,36.5,2.4,2.4,'pet2')
    c.ellipse(24,38+dy*0.5,8.5,7,'fur'); c.ellipse(19,44.5-LIFT(19),3,2,'fur2'); c.ellipse(29,44.5-LIFT(29),3,2,'fur2')
    for i in range(14):
        a=i*2*math.pi/14+spin; r=14.5 if i%2 else 13.5
        c.ellipse(24+r*math.cos(a),hy+r*math.sin(a),4.6,2.4,'pet1' if i%2 else 'pet2',rot=a)
    c.ellipse(24,hy,10.5,9.5,'fur')
    c.ellipse(24,hy+5,4.5,2.8,'muz',shade=False)
    c.poly([(22.6,hy+2.8),(25.4,hy+2.8),(24,hy+4.2)],'nose',tone=2)
    face(c,pose,24,4.5,hy-1,24,hy+5.2,style='round',mo='fang',blush=True)
form('SUNPUFF',{'fur':ramp((250,214,140)),'fur2':ramp((226,176,100)),'pet1':ramp((255,214,60)),'pet2':ramp((250,150,40)),'muz':ramp((255,240,210)),'nose':ramp((150,90,70))},F_SUNPUFF)

# ------------------------------------------------------------------ CHONKLE: pear-shaped hamster with stuffed cheeks, holding a seed
def F_CHONKLE(c,pose):
    dy,sx,sy=body_xf(pose); stuff=1.2 if pose=='eat' else 1.0
    c.ellipse(14,16+dy,3.6,3.6,'ham'); c.ellipse(34,16+dy,3.6,3.6,'ham')
    c.ellipse(14,16+dy,1.8,1.8,'inner',shade=False); c.ellipse(34,16+dy,1.8,1.8,'inner',shade=False)
    c.ellipse(17,45-LIFT(17),3,1.6,'inner'); c.ellipse(31,45-LIFT(31),3,1.6,'inner')
    c.ellipse(24,27+dy,13*sx,11*sy,'ham',nc=(24,32+dy,18,15)); c.ellipse(24,36+dy,17.5*sx,9.5*sy,'ham',nc=(24,32+dy,18,15))
    c.ellipse(24,37+dy,10.5,7.5,'cream',shade=False)
    for sgn in (-1,1): c.ellipse(24+sgn*11.5,29+dy,6.2*stuff,5.4*stuff,'cream',nc=(24+sgn*11.5,29+dy,7,6))
    if pose!='sleep':
        c.ellipse(24,35+dy,2,2.8,'seed'); c.dot(24,34+dy,(150,140,140))
        c.ellipse(21,35.5+dy,2.2,1.7,'inner'); c.ellipse(27,35.5+dy,2.2,1.7,'inner')
    else: c.ellipse(21,36+dy,2,1.5,'inner'); c.ellipse(27,36+dy,2,1.5,'inner')
    if c.N>=44:
        for sgn in (-1,1):
            for dd in (-1,1): c.line(24+sgn*15,29+dy+dd*0.7,24+sgn*19,28.5+dy+dd*1.7,(150,110,80))
    c.ellipse(24,28.3+dy,1.1,0.8,'inner',shade=False)
    face(c,pose,24,4,25+dy,24,29.5+dy,style='dot',mo='w')
form('CHONKLE',{'ham':ramp((240,176,110)),'cream':ramp((255,244,226)),'inner':ramp((246,156,166)),'seed':ramp((70,64,70))},F_CHONKLE)

# ------------------------------------------------------------------ ZIPPIT: lanky fox with tall ears and a big comet tail
def F_ZIPPIT(c,pose):
    dy,sx,sy=body_xf(pose); wag=-2 if pose=='happy' else 2 if pose in('sad','sleep') else 0
    tail=[(29,41.5,2.2),(35,40.5,3.8),(40,36,5),(42,29+wag*0.4,5.4),(41,22+wag*0.8,5),(38.5,16.5+wag*1.2,3.8),(36,13.5+wag*1.4,1.6)]
    tube(c,tail,'fox',(37,27,9,15))
    recolor(c,lambda u,v,m: m=='fox' and v<20+wag*1.1-(u-38)*0.7 and u>30,'tip')
    retone(c,lambda u,v,m: m=='fox' and u>29 and v>35,1)
    c.ellipse(20.5,42-LIFT(20.5),2.2,4.2,'fox2'); c.ellipse(27.5,42-LIFT(27.5),2.2,4.2,'fox2')
    c.ellipse(24,34+dy,8*sx,8.5*sy,'fox'); c.ellipse(24,33+dy,4.5,4.6,'chest',shade=False,wob=lambda a:0.12*math.sin(a*6))
    ear=3 if pose in('sad','sleep') else 0
    c.poly([(14,19+dy),(12.5+ear,4+dy+ear*1.5),(21,14+dy)],'fox'); c.poly([(34,19+dy),(35.5-ear,4+dy+ear*1.5),(27,14+dy)],'fox')
    c.poly([(15.5,17+dy),(14.5+ear,8+dy+ear*1.5),(19,15+dy)],'inner',tone=2); c.poly([(32.5,17+dy),(33.5-ear,8+dy+ear*1.5),(29,15+dy)],'inner',tone=2)
    c.poly([(14,20+dy),(10,27+dy),(18,25+dy)],'fox',nc=(24,22+dy,13,10)); c.poly([(34,20+dy),(38,27+dy),(30,25+dy)],'fox',nc=(24,22+dy,13,10))
    c.ellipse(24,22+dy,11*sx,8.5*sy,'fox',nc=(24,22+dy,13,10))
    c.ellipse(24,25.5+dy,5,3,'chest',shade=False)
    c.ellipse(24,23.6+dy,1.2,0.9,'nose',shade=False)
    face(c,pose,24,5,21+dy,24.5,26+dy,style='sharp',mo='smirk')
form('ZIPPIT',{'fox':ramp((84,200,196)),'fox2':ramp((60,160,164)),'tip':ramp((255,236,140)),'chest':ramp((236,252,248)),'inner':ramp((250,186,200)),'nose':ramp((40,40,70))},F_ZIPPIT)

# ------------------------------------------------------------------ GLIMMER: floating crystal jelly with trailing ribbons
def F_GLIMMER(c,pose):
    dy,sx,sy=body_xf(pose); fl=-2+dy
    for i,x0 in enumerate((15,20,24,28,33)):
        ph=i*1.3+(0.8 if pose=='happy' else 0)+(1.2 if FULL=='walk1' else 2.4 if FULL=='walk2' else 0); L2=10 if i in(1,3) else 8 if i==2 else 6
        pts=[(x0+1.0*math.sin(ph+t*0.8),31+fl+t) for t in range(0,L2+1)]
        for (a,b),(e,f) in zip(pts,pts[1:]): c.line(a,b,e,f,(196,170,250) if i%2 else (160,226,250),1.4)
        if pose!='sleep': c.dot(pts[-1][0],pts[-1][1]+0.5,(255,250,210))
    def ins(u,v): return ((u-24)/(14.5*sx))**2+((v-25-fl)/(13*sy))**2<=1 and v<=32+fl+0.9*math.sin(u*1.3)
    c.shape(ins,(8,10+fl,40,35+fl),'lil',(24,24+fl,15,14))
    for (x,a,h) in ((17,-0.4,6),(24,0,9.5),(31,0.4,6)):
        c.poly([(x-2.6,15+fl),(x+math.sin(a)*h,15+fl-h),(x+2.6,15+fl)],'crys')
    c.poly([(24,14+fl),(26.5,17+fl),(24,20+fl),(21.5,17+fl)],'gem')
    face(c,pose,24,5.5,25+fl,24,30.5+fl,style='glow',mo='smile',light=(150,230,255))
    if pose!='sleep':
        for (x,y) in ((6,12),(42,18),(7,33)):
            c.dot(x,y,(255,255,255)); c.dot(x-1,y,(255,240,160)); c.dot(x+1,y,(255,240,160)); c.dot(x,y-1,(255,240,160)); c.dot(x,y+1,(255,240,160))
form('GLIMMER',{'lil':ramp((206,178,250)),'crys':ramp((196,240,255)),'gem':ramp((90,220,240))},F_GLIMMER)

# ------------------------------------------------------------------ NESTLE: tufted owl wrapped in its wings, with a nest (and egg) on its head
def F_NESTLE(c,pose):
    dy,sx,sy=body_xf(pose)
    c.poly([(11,22+dy),(9,8+dy),(18,17+dy)],'brown'); c.poly([(37,22+dy),(39,8+dy),(30,17+dy)],'brown')
    c.ellipse(24,30+dy,14*sx,15*sy,'brown')
    c.ellipse(19.2,24+dy,6.8,6.4,'disc',shade=False); c.ellipse(28.8,24+dy,6.8,6.4,'disc',shade=False)
    c.ellipse(24,37+dy,7.5,6.5,'chest',shade=False)
    for (x,y) in ((21,35),(27,35),(24,38),(20.5,40),(27.5,40)):
        c.dot(x-0.8,y+dy,(176,130,90)); c.dot(x+0.8,y+dy,(176,130,90)); c.dot(x,y+dy+0.8,(176,130,90))
    op=-0.7 if pose=='happy' else 0; opl=op-0.5*ARMX(-1); opr=op-0.5*ARMX(1)
    c.ellipse(11.5,33+dy-2*ARMX(-1),4.6,10,'wing',rot=0.18+opl); c.ellipse(36.5,33+dy-2*ARMX(1),4.6,10,'wing',rot=-0.18-opr)
    for x in (19,29):
        for d in (-1.5,0,1.5): c.dot(x+d,45-LIFT(x),(250,160,60))
    c.ellipse(24,13+dy,8,2.8,'nest',wob=lambda a:0.15*math.sin(a*9))
    if pose!='sad': c.ellipse(24,10.5+dy,2.6,3,'egg')
    c.ellipse(29.5,10.5+dy,2.6,1.3,'leaf',rot=-0.6)
    face(c,pose,24,5,24+dy,24,29+dy,style='owl',mo='beak',iris=(255,190,70))
form('NESTLE',{'brown':ramp((170,120,84)),'wing':ramp((132,92,64)),'disc':ramp((250,234,206)),'chest':ramp((246,226,192)),'nest':ramp((190,140,80)),
               'egg':ramp((200,230,250)),'leaf':ramp((110,190,90))},F_NESTLE)


# ------------------------------------------------------------------ FRILLO (replaced GRUBBLE): axolotl with frilly gills and a curly tail
def F_FRILLO(c,pose):
    dy,sx,sy=body_xf(pose); wag=2 if pose=='happy' else -1 if pose=='sad' else 0
    tube(c,[(28,40,3.4),(35,41,2.8),(40,38+wag*0.5,2.2),(42,33+wag,1.4)],'body',(34,38,9,6))
    c.ellipse(18.5,43-LIFT(18.5),2.4,2.4,'body2'); c.ellipse(29.5,43-LIFT(29.5),2.4,2.4,'body2')
    c.ellipse(24,37+dy*0.5,8.5,6.5,'body')
    arm=-0.9 if pose=='happy' else 0.25 if pose in('sad','sleep') else 0
    for sgn in (-1,1):
        ax,ay=24+sgn*8.5,35+dy*0.5; a=arm+ARMX(sgn,-1); L=5.5 if a<-0.6 else 4
        ex,ey=ax+sgn*L*math.cos(0.5+a),ay+L*math.sin(0.5+a)
        tube(c,[(ax,ay,2.0),(ex,ey,1.7)],'body2',(ax,ay,4,4))
        c.ellipse(ex+sgn*0.6,ey+0.4,1.9,1.5,'body2')
        for k in (-0.9,0,0.9): c.dot(ex+sgn*1.6,ey+0.8+k,(206,120,120))
    perk=-0.25 if pose=='happy' else 0.35 if pose in('sad','sleep') else 0
    for sgn in (-1,1):
        for j,a in enumerate((-0.55,0.0,0.55)):
            ang=(math.pi if sgn<0 else 0)+sgn*(a+perk)
            x0,y0=24+sgn*11,25+dy; L=6.5 if j==1 else 5.5
            for t in range(4):
                px_=x0+math.cos(ang)*(1.5+t*1.4); py_=y0+math.sin(ang)*(1.5+t*1.4)-t*0.3*(1 if j==1 else 0)
                c.ellipse(px_,py_,1.5-t*0.15,1.5-t*0.15,'gill')
    c.ellipse(24,26+dy,13*sx,9.5*sy,'body')
    c.ellipse(24,31+dy,7.5,3,'belly',shade=False)
    for (x,y) in ((15,22),(33,22),(30,19)): c.dot(x,y+dy,(255,236,236))
    sprout(c,24,17,dy,droop=0.6 if pose in('sad','sleep') else 0)
    face(c,pose,24,7,25+dy,24,29+dy,style='dot',mo='smile',blush=True)
form('FRILLO',{'body':ramp((250,184,170)),'body2':ramp((230,150,140)),'gill':ramp((236,100,130)),'belly':ramp((255,224,214)),'leaf':ramp((110,190,90))},F_FRILLO)

# ------------------------------------------------------------------ DRIZZLE (replaced GLOOMP): a little rain cloud that rains on itself
def F_DRIZZLE(c,pose):
    dy,sx,sy=body_xf(pose); rain=pose!='happy'
    if rain:
        for i,(x,y) in enumerate(((14,38),(20,42),(27,40),(33,43),(17,45),(30,46),(36,38))): c.line(x,y,x-0.6,y+1.8,(150,190,240))
    c.ellipse(24,46,13,1.8,'pud',shade=False)
    if pose=='happy':   # the sun comes out: a little rainbow behind it
        for r,col in ((21.5,(240,110,120)),(20.2,(255,206,90)),(18.9,(130,206,140)),(17.6,(120,170,240))):
            for k in range(60):
                a=math.pi*(1.0+k/59.0); c.dot(24+r*math.cos(a),38+r*0.8*math.sin(a),col)
    for x,y,r in ((14,27,7),(34,27,7),(19,21,7.5),(29,20,8.5),(24,31,10)): c.ellipse(x,y+dy,r*sx,r*sy,'cloud',nc=(24,26+dy,18,13))

    sprout(c,29,12.5,dy,lean=0.6,droop=1.0 if pose!='happy' else 0.2)
    face(c,pose,24,5,26+dy,24,31+dy,style='sleepy',mo='wavy' if pose=='idle' else 'smile')
form('DRIZZLE',{'cloud':ramp((176,186,214)),'pud':ramp((140,170,220)),'leaf':ramp((150,170,110))},F_DRIZZLE)

ORDER=['EGG','BLIPLET','SPROUTLE','FLUFFBUD','FRILLO','STARWING','MOSSMOO','DRIZZLE','SUNPUFF','CHONKLE','ZIPPIT','GLIMMER','NESTLE']
exec(open(_ROOT + '/gen/newforms.py').read())   # the three new family lines
exec(open(_ROOT + '/gen/v8forms.py').read())    # v8: redrawn forms and boy/girl touches
ORDER+=NEW_ORDER
_gender_touches()
GIRL_ORDER=[n+'~G' for n in ORDER if n in GIRL_FORMS]   # rendered after ORDER (see critgen.py)
LIMBS=True
def breathe(g):
    # an in-breath: everything above the waist rises one pixel and the waist row is doubled,
    # so the shading stays exactly the same as the resting frame (re-drawing it would reshuffle every shaded pixel)
    N=len(g); ys=[y for y in range(N) if any(v is not None for v in g[y])]
    if not ys or N<32: return g
    top,bot=ys[0],ys[-1]; split=top+int((bot-top)*0.62)
    return [g[y+1] if y<split else g[y] for y in range(N)]
def render(name,pose,size=64,flat=False,mirror=False):
    if pose in('breathe','sleep2'): return breathe(render(name,'idle' if pose=='breathe' else 'sleep',size,flat,mirror))
    global FULL,BREATH,FACE
    FULL=pose; BREATH={'breathe':1,'sleep2':1,'stretch':2}.get(pose,0); FACE={'love':'love','wave1':'call','wave2':'call','stretch':'yawn','yuck':'yuck'}.get(pose)
    try: return _render(name,BASE.get(pose,pose),size,flat,mirror)
    finally: FULL='idle'; BREATH=0; FACE=None
def _render(name,pose,size,flat,mirror=False):
    ramps,draw=FORMS[name]
    if LIMBS:
        import limbs; ramps,draw=limbs.make(name)
    if flat: c=Canvas(size/48); draw(c,pose); return c.rgb(ramps)
    import volume                      # soft depth + inner part lines (gen/volume.py)
    volume.Canvas3.MODE='soft'; volume.Canvas3.LINES=True
    c=volume.Canvas3(size/48)
    if not mirror: draw(c,pose); return c.rgb(ramps,ground=False)
    # facing the other way: draw lit from the top-right, then mirror the drawing, so the light still comes from the top-left
    L0=volume.L; volume.L=(-L0[0],L0[1],L0[2])
    try: draw(c,pose)
    finally: volume.L=L0
    for a in (c.mat,c.tone,c.flat,c.d,c.pid):
        for row in a: row.reverse()
    return c.rgb(ramps,ground=False)

if __name__=='__main__':
    from PIL import Image, ImageDraw
    S=3; cell=64*S+8
    sheet=Image.new('RGB',(cell*7+8, cell*2+40+ (cell+20)*3 + 30),(236,226,206))
    d=ImageDraw.Draw(sheet)
    def put(img,x,y):
        n=len(img); im=Image.new('RGBA',(n,n),(0,0,0,0)); p=im.load()
        for yy in range(n):
            for xx in range(n):
                if img[yy][xx] is not None: p[xx,yy]=img[yy][xx]+(255,)
        sc=(64*S)//n; big=im.resize((n*sc,n*sc),Image.NEAREST); sheet.paste(big,(x,y),big)
    for i,n in enumerate(ORDER):
        x=8+(i%7)*cell; y=8+(i//7)*(cell+18)
        put(render(n,'idle'),x,y); d.text((x+4,y+64*S),n,fill=(40,30,70))
    y0=8+2*(cell+18)+10
    for r,n in enumerate(['FRILLO','DRIZZLE','NESTLE']):
        for j,p in enumerate(POSES):
            x=8+j*cell; y=y0+r*(cell+20)
            put(render(n,p),x,y); d.text((x+4,y+64*S),'%s %s'%(n,p),fill=(40,30,70))
    sheet.save('/tmp/critter_sheet.png'); print(sheet.size)
