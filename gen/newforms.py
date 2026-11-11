# Three new family lines (v7). Like the original critters, each is an animal fused with a piece of nature,
# and each family keeps its signature piece from baby to grown-up so they are clearly related:
#   FOREST : OAKIT (acorn fawnling)  -> OAKFAWN (fawn in an acorn cap)      -> OAKHART (stag with oak-branch antlers)
#   NIGHT  : NOXLET (bat pup wrapped in its wings) -> NOXPUP (starry-winged bat pup) -> NOXWING (night-sky bat, moon crest)
#   DESERT : DUNELET (cactus-ball kit) -> DUNEKIT (fennec with cactus-pad ears) -> DUNEROSE (fennec with blooming ears)
# This file is exec'd inside critters.py so it shares its helpers (Canvas API, face, tube, body_xf, LIFT, ARMX ...).

def shade_by(c,test,delta):     # darken/lighten pixels of a material (soft-depth canvas keeps its light in c.d)
    for y in range(c.N):
        for x in range(c.N):
            m=c.mat[y][x]
            if m not in (None,'__flat__','__eye__') and test(x/c.k,y/c.k,m):
                if hasattr(c,'d'): c.d[y][x]+=delta
                else: c.tone[y][x]=max(0,min(4,c.tone[y][x]+(1 if delta<0 else -1)))
FUZZ=lambda a:0.09*math.sin(a*11)
def dots_on(c,mat,pts,col):
    for (x,y) in pts:
        X,Y=int(x*c.k),int(y*c.k)
        if 0<=X<c.N and 0<=Y<c.N and c.mat[Y][X]==mat: c.dot(x,y,col)

# ------------------------------------------------------------------ FOREST line
FOREST={'nut':ramp((200,132,76)),'cap':ramp((136,94,62)),'cream':ramp((252,236,206)),'leaf':ramp((122,186,86)),
        'hoof':ramp((96,66,54)),'body':ramp((192,128,80)),'inner':ramp((250,190,172)),'moss':ramp((112,168,86)),
        'shroom':ramp((234,86,74)),'twig':ramp((152,106,72)),'acorn':ramp((210,146,84))}
def acorn_cap(c,cx,cy,rx,ry,tilt=0.0,stem=True):
    c.ellipse(cx,cy,rx,ry,'cap',rot=tilt)
    def hatch(u,v,m):
        if m!='cap': return False
        a,b=(u-cx)+(v-cy),(u-cx)-(v-cy)
        return (a%2.6)<0.9 and (b%2.6)<0.9
    shade_by(c,hatch,-0.32)
    if stem: c.line(cx+math.sin(tilt)*ry,cy-ry+0.5,cx+1.6+math.sin(tilt)*ry,cy-ry-2.4,(104,72,48),1.3)
def oak_leaf(c,x,y,r,a,mat='leaf'):
    c.ellipse(x,y,r,r*0.55,mat,rot=a)
    for k in (-1,1): c.ellipse(x+math.cos(a)*r*0.25-math.sin(a)*k*r*0.4,y+math.sin(a)*r*0.25+math.cos(a)*k*r*0.4,r*0.45,r*0.3,mat,rot=a+k*0.6)
def F_OAKIT(c,pose):
    dy,sx,sy=body_xf(pose); droop=1.6 if pose in('sad','sleep') else 0; perk=-1.2 if pose=='happy' else 0
    oak_leaf(c,35,41,3.4,-0.7+(0.4 if pose=='happy' else 0)+{'walk1':0.3,'walk2':-0.3}.get(FULL,0))   # a little oak-leaf tail
    for x in (19.5,28.5): c.ellipse(x,45.6-LIFT(x),2.7,1.5,'hoof')
    for s in (-1,1):   # fawn ears poking out from under the cap
        a=s*(0.3+droop*0.25)
        c.ellipse(24+s*11.5,29+dy+droop+perk,4.4,2.0,'body',rot=a); c.ellipse(24+s*11.8,29.2+dy+droop+perk,2.5,0.9,'inner',rot=a,shade=False)
    def nut(u,v): return ((u-24)/(9.6*sx))**2+((v-35.5-dy)/(10*sy))**2<=1 and v>=26+dy
    c.shape(nut,(13,24+dy,35,47+dy),'nut',(24,35+dy,10,10))
    c.ellipse(24,40.5+dy,5.4,3.0,'cream',shade=False)
    acorn_cap(c,24,26.8+dy,11.2,5.2)
    for (x,y) in ((16,33.5),(32,33.5),(17,37),(31,37)): c.dot(x,y+dy,(255,240,214))   # fawn freckles
    face(c,pose,24,4.6,32.5+dy,24,37+dy,style='big',mo='smile',blush=True)
form('OAKIT',FOREST,F_OAKIT)
def F_OAKFAWN(c,pose):
    dy,sx,sy=body_xf(pose); droop=2 if pose in('sad','sleep') else 0; perk=-1.5 if pose=='happy' else 0
    oak_leaf(c,33.5,35.5,3.0,-0.9+(0.5 if pose=='happy' else 0))
    c.ellipse(24,36.5+dy*0.5,9*sx,7*sy,'body',nc=(24,35,10,8))
    c.ellipse(24,37.5+dy*0.5,4.5,4.5,'cream',shade=False)
    dots_on(c,'body',((17.5,34),(19,37.5),(30.5,34),(29,37.5),(16.5,38)),(255,238,212))
    for s in (-1,1):
        a=s*(0.25+droop*0.2)
        c.ellipse(24+s*10.5,21+dy+droop+perk,5.2,2.3,'body',rot=a); c.ellipse(24+s*10.8,21.2+dy+droop+perk,3.0,1.0,'inner',rot=a,shade=False)
    c.ellipse(24,23+dy,8.5*sx,7.5*sy,'body',nc=(24,22+dy,9,8))
    c.ellipse(24,27.5+dy,4.4,3.0,'cream',shade=False)
    c.ellipse(24,26.4+dy,1.3,0.9,'hoof',shade=False)
    for s in (-1,1):   # twig antler nubs, each with a leaf
        c.line(24+s*3.5,16.5+dy,24+s*6,11.5+dy,(132,94,64),1.3)
        oak_leaf(c,24+s*7.4,10.5+dy+droop,2.2,-0.4 if s<0 else 0.4)
    acorn_cap(c,23,16.2+dy,6,2.8,-0.2)
    face(c,pose,24,4,22.5+dy,24,29.5+dy,style='round',mo='tiny',blush=True)
form('OAKFAWN',FOREST,F_OAKFAWN)
def mushroom(c,x,y,r):
    c.ellipse(x,y+r*0.6,r*0.35,r*0.6,'cream')
    c.ellipse(x,y,r,r*0.62,'shroom'); c.dot(x-r*0.35,y-r*0.15,(255,246,236)); c.dot(x+r*0.3,y+r*0.05,(255,246,236))
def F_OAKHART(c,pose):
    dy,sx,sy=body_xf(pose); droop=2 if pose in('sad','sleep') else 0; perk=-1.5 if pose=='happy' else 0
    lift=-1.2 if pose=='happy' else 1.2 if pose in('sad','sleep') else 0
    for s in (-1,1):   # oak-branch antlers with leaf clusters (and one acorn)
        r=[(24+s*4,13+dy,1.4),(24+s*8,8+dy+lift,1.2),(24+s*13.5,5+dy+lift,1.0)]
        tube(c,r,'twig',(24+s*9,9,6,5))
        tube(c,[(24+s*8.4,8.6+dy+lift,1.0),(24+s*9,2.5+dy+lift,0.9)],'twig',(24+s*9,5,3,4))
        oak_leaf(c,24+s*14.5,4.5+dy+lift,2.6,s*0.4); oak_leaf(c,24+s*9.5,2+dy+lift,2.2,-1.2*s)
    c.ellipse(36.5,7.5+dy+lift,1.6,2.0,'acorn'); c.ellipse(36.5,6+dy+lift,1.9,1.0,'cap')
    for s in (-1,1):
        a=s*(0.25+droop*0.2)
        c.ellipse(24+s*10.5,17+dy+droop+perk,5,2.2,'body',rot=a); c.ellipse(24+s*10.8,17.2+dy+droop+perk,2.9,0.9,'inner',rot=a,shade=False)
    c.ellipse(24,37+dy*0.5,12*sx,8*sy,'body',nc=(24,35,13,9))
    dots_on(c,'body',((14.5,35),(16,39),(33.5,35),(32,39),(13.5,38.5),(34.5,38.5)),(255,238,212))
    c.ellipse(24,36+dy*0.5,5,5.5,'cream',shade=False)
    c.ellipse(24,28.5+dy,12.5,4.6,'moss',wob=FUZZ)                       # a mossy mantle over the shoulders
    mushroom(c,14,26.5+dy,2.2); mushroom(c,33.5,26+dy,1.8)
    c.ellipse(24,19.5+dy,8*sx,7*sy,'body',nc=(24,19+dy,9,8))
    c.ellipse(24,24+dy,4.4,3.0,'cream',shade=False)
    c.ellipse(24,22.8+dy,1.4,0.9,'hoof',shade=False)
    face(c,pose,24,4,19+dy,24,26+dy,style='round',mo='smile',blush=True)
form('OAKHART',FOREST,F_OAKHART)

# ------------------------------------------------------------------ NIGHT line
NIGHT={'fur':ramp((112,106,188)),'fur2':ramp((82,76,154)),'belly':ramp((228,218,252)),'wing':ramp((58,56,124)),
       'moon':ramp((255,234,150)),'inner':ramp((242,162,198))}
STAR=(255,244,190)
def crescent(c,x,y,r,tilt=0.35,bite='fur'):
    c.ellipse(x,y,r,r,'moon')
    # bite the inner curve out by painting it back in fur colour
    X0,Y0=x+r*0.55*math.cos(tilt),y-r*0.55*math.sin(tilt)
    for yy in range(int((y-r-1)*c.k),int((y+r+1)*c.k)+1):
        for xx in range(int((x-r-1)*c.k),int((x+r+1)*c.k)+1):
            if 0<=xx<c.N and 0<=yy<c.N and c.mat[yy][xx]=='moon' and math.hypot((xx+0.5)/c.k-X0,(yy+0.5)/c.k-Y0)<r*0.82:
                c.mat[yy][xx]=bite
def bat_ears(c,cx,y,gap,h,dy,droop=0):
    for s in (-1,1):
        bx=cx+s*gap
        c.poly([(bx-s*2.6,y+dy+2),(bx+s*1.5,y+dy-h+droop),(bx+s*3.4,y+dy+2.5)],'fur',nc=(bx,y+dy-h/2,4,h))
        c.poly([(bx-s*1.0,y+dy+1.2),(bx+s*1.4,y+dy-h+3+droop),(bx+s*2.4,y+dy+1.6)],'inner',tone=1)
def bat_wing(c,s,x0,y0,span,up,mat='wing',stars=True):
    """a bat wing seen from the front: straight top edge to the wrist, scalloped lower edge between the fingers"""
    tip=(x0+s*span,y0-3+up)
    pts=[(x0,y0-2),(x0+s*span*0.5,y0-5+up*0.6),tip]
    for i,f in enumerate((0.86,0.7,0.55,0.38,0.2)):
        depth=(8 if i%2 else 4.5)*(0.7+0.3*f)
        pts.append((x0+s*span*f,y0+depth+up*(f*0.6)))
    pts.append((x0,y0+7))
    c.poly(pts,mat,nc=(x0+s*span*0.5,y0+1,span*0.6,7))
    for f in (0.86,0.55,0.2):   # finger bones
        c.line(x0+s*1,y0-1,x0+s*span*f,y0+(8 if f!=0.86 else 4)*(0.7+0.3*f)+up*(f*0.6)-0.5,(92,88,170),0.8)
    if stars:
        for (fx,fy) in ((0.35,1),(0.62,-1),(0.8,2),(0.5,4)):
            c.dot(x0+s*span*fx,y0+fy+up*fx*0.6,STAR)
def F_NOXLET(c,pose):
    dy,sx,sy=body_xf(pose); droop=2 if pose in('sad','sleep') else 0
    for x in (20.5,27.5): c.ellipse(x,45.5-LIFT(x),2.2,1.4,'fur2')
    bat_ears(c,24,27,6,9,dy,droop)
    c.ellipse(24,35.5+dy,9.5*sx,9.5*sy,'fur',wob=FUZZ,nc=(24,35+dy,10,10))
    crescent(c,24,26.8+dy,2.3)
    peek=-1.5 if pose=='happy' else 0
    for s in (-1,1):   # wings wrapped around it like a blanket
        c.ellipse(24+s*5.5,42+dy+peek,6.8,4.8,'wing',rot=-s*0.3,nc=(24,41,10,6))
        c.dot(24+s*10.5,38+dy+peek,(200,190,240))     # the little thumb claws
    dots_on(c,'wing',((19,43),(29,42.5),(23.5,45)),STAR)
    face(c,pose,24,4.2,33+dy,24,37+dy,style='sleepy' if pose=='idle' else 'round',mo='tiny',blush=True)
form('NOXLET',NIGHT,F_NOXLET)
def F_NOXPUP(c,pose):
    dy,sx,sy=body_xf(pose); droop=2 if pose in('sad','sleep') else 0
    up=-3 if pose=='happy' else 2 if pose in('sad','sleep') else 0
    for s in (-1,1): bat_wing(c,s,24+s*6,31+dy,12,up-1.5*ARMX(s))
    for x in (20.5,27.5): c.ellipse(x,45.5-LIFT(x),2.3,1.4,'fur2')
    bat_ears(c,24,22,6.5,10,dy,droop)
    c.ellipse(24,33+dy,9*sx,11*sy,'fur',wob=FUZZ,nc=(24,32+dy,10,12))
    c.ellipse(24,37+dy,5.5,6,'belly',wob=FUZZ,shade=False)
    crescent(c,24,23.5+dy,2.8)
    face(c,pose,24,4.4,29.5+dy,24,34+dy,style='round',mo='fang',blush=True)
form('NOXPUP',NIGHT,F_NOXPUP)
def F_NOXWING(c,pose):
    dy,sx,sy=body_xf(pose); droop=2 if pose in('sad','sleep') else 0
    up=-4 if pose=='happy' else 3 if pose in('sad','sleep') else 0
    for s in (-1,1): bat_wing(c,s,24+s*7,26+dy,16.5,up-2*ARMX(s))
    tube(c,[(29,42,1.6),(34,43,1.4),(38,40,1.0)],'fur2',(33,42,5,3)); c.ellipse(39,39.5,1.8,1.8,'moon')   # a little comet tail
    bat_ears(c,24,15,6.5,9,dy,droop)
    crescent(c,24,11.5+dy,4.6,0.6,None)                                  # a big moon crest
    c.ellipse(24,34+dy*0.6,10*sx,10.5*sy,'fur',wob=FUZZ,nc=(24,33,11,11))
    c.ellipse(24,28+dy,10,4,'belly',wob=FUZZ)
    c.ellipse(24,21.5+dy,8.5*sx,7.5*sy,'fur',wob=FUZZ,nc=(24,21+dy,9,8))
    face(c,pose,24,4,21+dy,24,25.5+dy,style='lash',mo='fang',blush=True)
form('NOXWING',NIGHT,F_NOXWING)

# ------------------------------------------------------------------ DESERT line
DESERT={'fur':ramp((246,206,134)),'fur2':ramp((224,172,100)),'cream':ramp((255,244,222)),'cac':ramp((128,192,114)),
        'cac2':ramp((96,158,96)),'flower':ramp((252,122,158)),'ctr':ramp((255,214,90)),'sun':ramp((255,172,72)),'nose':ramp((96,64,64))}
SPINE=(255,250,228)
def ribs(c,cx,mat='cac',step=3.2,delta=-0.28):
    shade_by(c,lambda u,v,m: m==mat and (abs(u-cx)%step)<0.8,delta)
def spines(c,mat,pts):
    dots_on(c,mat,pts,SPINE)
def bloom(c,x,y,r,n=5):
    for i in range(n):
        a=i*2*math.pi/n-math.pi/2; c.ellipse(x+math.cos(a)*r*0.75,y+math.sin(a)*r*0.75,r*0.62,r*0.62,'flower')
    c.ellipse(x,y,r*0.42,r*0.42,'ctr',shade=False)
def cactus_ear(c,x,y,rx,ry,rot):
    c.ellipse(x,y,rx,ry,'cac',rot=rot)
    ca,sa=math.cos(rot),math.sin(rot)
    shade_by(c,lambda u,v,m: m=='cac' and abs((u-x)*ca+(v-y)*sa)<0.55 and ((u-x)**2+(v-y)**2)<ry*ry,-0.3)
    spines(c,'cac',[(x+(-sa)*t*ry*0.6+ca*o*rx*0.55,y+ca*t*ry*0.6+sa*o*rx*0.55) for t in (-0.7,0,0.7) for o in (-1,1)])
def F_DUNELET(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.25 if pose in('sad','sleep') else -0.15 if pose=='happy' else 0
    for x in (19.5,28.5): c.ellipse(x,45.4-LIFT(x),2.6,1.6,'fur2')
    for s in (-1,1): cactus_ear(c,24+s*7.5,25.5+dy,2.6,4.6,s*(0.45+droop))
    c.ellipse(24,36+dy,10*sx,9.5*sy,'cac',nc=(24,35+dy,11,10))
    ribs(c,24)
    spines(c,'cac',((16,33),(18.5,29),(31.5,33),(29.5,29),(15.5,38),(32.5,38)))
    c.ellipse(24,39.5+dy,5.4,3.6,'cream',shade=False)
    c.ellipse(24,37.4+dy,1.3,0.9,'nose',shade=False)
    bloom(c,24,26.5+dy,2.6)
    face(c,pose,24,4.4,33+dy,24,40+dy,style='big',mo='smile',blush=True)
form('DUNELET',DESERT,F_DUNELET)
def F_DUNEKIT(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.35 if pose in('sad','sleep') else -0.2 if pose=='happy' else 0
    wag=-0.4 if pose=='happy' else 0.3 if pose in('sad','sleep') else 0
    wag+={'walk1':-0.2,'walk2':0.2}.get(FULL,0)
    tube(c,[(29,40,2.6),(34,39,3.4),(37,34+wag*6,3.0),(36.5,30+wag*8,1.8)],'fur',(34,36,6,6))
    c.ellipse(36.5,29.5+wag*8,2.2,2.2,'cac')                                   # a cactus-pad tail tip
    c.ellipse(24,36.5+dy*0.5,8*sx,8*sy,'fur',nc=(24,35,9,9))
    c.ellipse(24,37.5+dy*0.5,4.5,5,'cream',shade=False)
    for s in (-1,1): cactus_ear(c,24+s*9,15+dy,4,8.5,s*(0.5+droop))
    bloom(c,33,9+dy+droop*6,2.6)
    c.ellipse(24,24.5+dy,9*sx,7.5*sy,'fur',nc=(24,24+dy,10,8))
    c.ellipse(24,28.5+dy,4.6,3.2,'cream',shade=False)
    c.ellipse(24,27.2+dy,1.3,0.9,'nose',shade=False)
    c.ellipse(24,19.2+dy,1.6,1.3,'sun',shade=False)                            # a little sun mark
    face(c,pose,24,4.2,23.5+dy,24,30+dy,style='round',mo='smile',blush=True)
form('DUNEKIT',DESERT,F_DUNEKIT)
def F_DUNEROSE(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.35 if pose in('sad','sleep') else -0.2 if pose=='happy' else 0
    wag=-0.4 if pose=='happy' else 0.3 if pose in('sad','sleep') else 0
    wag+={'walk1':-0.2,'walk2':0.2}.get(FULL,0)
    tube(c,[(30,41,3.0),(37,40,4.4),(41,33+wag*6,4.0),(40,26+wag*8,2.6)],'fur',(37,35,7,8))
    c.ellipse(40,25.5+wag*8,3.0,3.0,'sun')                                     # tail tip like a little sun
    c.ellipse(24,37+dy*0.5,10.5*sx,8.5*sy,'fur',nc=(24,36,11,10))
    c.ellipse(24,37+dy*0.5,5,5.5,'cream',shade=False)
    for s in (-1,1): cactus_ear(c,24+s*10.5,10+dy,5,10,s*(0.45+droop))
    bloom(c,13.5,3.5+dy+droop*8,3.0); bloom(c,34.5,3.5+dy+droop*8,3.0)
    for i in range(7):   # a collar of small cactus pads with pink buds
        a=math.pi*(0.1+0.8*i/6); x=24-math.cos(a)*11; y=28.5+dy+math.sin(a)*2.5
        c.ellipse(x,y,2.4,3.2,'cac',rot=(a-math.pi/2)*0.8)
        if i%2==0: c.ellipse(x,y-2.4,1.1,1.1,'flower')
    c.ellipse(24,20+dy,9*sx,7.5*sy,'fur',nc=(24,19+dy,10,8))
    c.ellipse(24,24+dy,4.6,3.2,'cream',shade=False)
    c.ellipse(24,22.6+dy,1.4,0.9,'nose',shade=False)
    c.ellipse(24,14.8+dy,1.6,1.6,'sun',shade=False)
    for i in range(0,8,2): c.dot(24+math.cos(i*math.pi/4)*2.4,14.8+dy+math.sin(i*math.pi/4)*2.4,(255,200,110))
    face(c,pose,24,4.3,19+dy,24,25.5+dy,style='lash',mo='smile',blush=True)
form('DUNEROSE',DESERT,F_DUNEROSE)

NEW_ORDER_OLD=['OAKIT','OAKFAWN','OAKHART','NOXLET','NOXPUP','NOXWING','DUNELET','DUNEKIT','DUNEROSE']
LINE_NAMES=['FOREST','NIGHT','DESERT']

# each line's egg is the same egg in that family's colours (the game swaps the egg palette)
EGG_LINES=[
  {'egg':ramp((246,236,214)),'band':ramp((176,116,66)),'pk':ramp((122,186,86)),'bl':ramp((234,86,74))},     # forest: brown zigzag
  {'egg':ramp((222,220,246)),'band':ramp((112,106,188)),'pk':ramp((255,234,150)),'bl':ramp((242,162,198))},  # night
  {'egg':ramp((255,240,214)),'band':ramp((255,160,64)),'pk':ramp((252,122,158)),'bl':ramp((128,192,114))},   # desert: orange zigzag
]

# ================================================================== v7 growth tree (proposal 3)
# TEENS stay generic, like Bliplet and Sproutle: the same green bean grown taller, cared-for or scruffy.
TEEN_G={'mint':ramp((150,220,156)),'mint2':ramp((112,192,128)),'leaf':ramp((88,182,80)),'petal':ramp((255,160,190)),'ctr':ramp((255,214,90)),'bel':ramp((228,248,222))}
def F_BLOOMLE(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.5 if pose in('sad','sleep') else 0
    c.line(24,14+dy,24,8+dy+droop*2,(70,130,64))
    c.ellipse(17.5,9+dy+droop*4,6.5,2.8,'leaf',rot=-0.4-droop); c.ellipse(30.5,8+dy+droop*4,6.5,2.8,'leaf',rot=0.45+droop)
    for i in range(5):   # the bud has opened into a little flower
        a=i*2*math.pi/5-math.pi/2; c.ellipse(24+math.cos(a)*2.1,6.5+dy+droop*2+math.sin(a)*2.1,1.7,1.7,'petal')
    c.ellipse(24,6.5+dy+droop*2,1.1,1.1,'ctr',shade=False)
    c.ellipse(24,23.5+dy,9.5*sx,10.5*sy,'mint',nc=(24,27+dy,11,15)); c.ellipse(24,31.5+dy,10*sx,8.5*sy,'mint',nc=(24,27+dy,11,15))
    c.ellipse(24,32.5+dy,5.6,6,'bel',shade=False)
    face(c,pose,24,4.4,22.5+dy,24,27+dy,style='lash',mo='smile',blush=True)
form('BLOOMLE',TEEN_G,F_BLOOMLE)
TEEN_B={'mint':ramp((150,178,112)),'mint2':ramp((118,146,88)),'leaf':ramp((96,128,70)),'thorn':ramp((128,92,70)),'bud':ramp((176,110,120)),'bel':ramp((222,228,196))}
def F_THORNLE(c,pose):
    dy,sx,sy=body_xf(pose); droop=0.5 if pose in('sad','sleep') else 0
    # a scraggly bramble sprout: jagged leaves, a closed spiky bud, a couple of thorns
    c.line(24,16+dy,23,8+dy+droop*2,(96,90,60),1.2); c.line(23.4,13.5+dy,18,12+dy+droop*4,(96,90,60),1.0); c.line(23.4,12.5+dy,30,11+dy+droop*4,(96,90,60),1.0)
    for s_,(x,y) in ((-1,(17,12)),(1,(30.5,11))):
        yy=y+dy+droop*4; tip=x+s_*6.5
        c.poly([(x-s_*1.5,yy),(x+s_*1.5,yy-3.2),(x+s_*2.8,yy-1.4),(x+s_*4.4,yy-3.4),(x+s_*5.2,yy-1.2),(tip,yy-2.2),(x+s_*4.5,yy+1.8),(x+s_*1,yy+2)],'leaf')
    c.ellipse(23,8.5+dy+droop*2,1.8,2.4,'bud'); c.poly([(21.6,7.5+dy),(23,3.5+dy),(24.4,7.5+dy)],'bud')
    c.ellipse(24,27+dy,10.5*sx,10.5*sy,'mint',nc=(24,30+dy,12,14)); c.ellipse(24,34+dy,11*sx,8.5*sy,'mint',nc=(24,30+dy,12,14))
    for (x,y) in ((13.5,26),(34.5,27),(15,35),(33,36)): c.poly([(x-1,y+dy),(x+(1.8 if x>24 else -1.8),y-0.6+dy),(x,y+1.2+dy)],'thorn')
    c.ellipse(24,35+dy,6,5.5,'bel',shade=False)
    c.ellipse(31,30+dy,2.6,1.2,'bel',rot=-0.6)                                   # a little bandage
    face(c,pose,24,4.6,26+dy,24,31+dy,style='sleepy',mo='wavy' if pose=='idle' else 'smile',blush=True)
form('THORNLE',TEEN_B,F_THORNLE)

# DRIZZLE's replacement on the less-care side, two candidates:
HOG={'hog':ramp((176,132,96)),'face':ramp((246,226,196)),'spine':ramp((92,140,74)),'spine2':ramp((70,112,62)),'berry':ramp((110,70,150)),'leaf':ramp((112,176,84)),'nose':ramp((70,50,60))}
def F_THORNHOG(c,pose):
    dy,sx,sy=body_xf(pose); bristle=-1 if pose=='happy' else 1 if pose in('sad','sleep') else 0
    for x in (17,31): c.ellipse(x,45-LIFT(x),3,1.6,'hog')
    # a coat of bramble: thorny green spikes all round its back
    for i in range(13):
        a=math.pi*(1.05+0.9*i/12); r=15+bristle
        x=24+math.cos(a)*r*1.05*sx; y=30+dy+math.sin(a)*r*0.95*sy
        n=(math.cos(a),math.sin(a)); t=(-n[1],n[0])
        c.poly([(x-t[0]*2.6,y-t[1]*2.6),(x+n[0]*5,y+n[1]*5),(x+t[0]*2.6,y+t[1]*2.6)],'spine2' if i%2 else 'spine')
    c.ellipse(24,32+dy,15*sx,12*sy,'spine',nc=(24,30+dy,16,14))
    for (x,y) in ((12,24),(34,22),(20,18),(30,29),(15,33)): c.ellipse(x,y+dy,1.9,1.9,'berry')   # blackberries
    c.ellipse(28,16+dy,3.4,1.6,'leaf',rot=0.5)
    c.ellipse(24,36+dy,9.5,8,'face',nc=(24,35+dy,10,9))
    c.ellipse(24,38.5+dy,3.2,2.4,'face'); c.ellipse(24,37.4+dy,1.4,1.0,'nose',shade=False)
    face(c,pose,24,4.6,34+dy,24,40.5+dy,style='round',mo='tiny',blush=True)
form('THORNHOG',HOG,F_THORNHOG)
TRAP={'stem':ramp((132,196,104)),'stem2':ramp((100,164,86)),'trap':ramp((112,186,92)),'jaw':ramp((226,82,88)),'tooth':ramp((250,246,220)),'bel':ramp((226,244,200))}
def F_SNAPWORT(c,pose):
    dy,sx,sy=body_xf(pose); gape={'happy':-2,'eat':-3}.get(pose,0)
    for x in (18,30): c.ellipse(x,45-LIFT(x),3,1.8,'stem2')
    c.ellipse(24,38+dy*0.5,9.5*sx,8*sy,'stem',nc=(24,37,10,9))
    c.ellipse(24,39+dy*0.5,5,4.5,'bel',shade=False)
    # its head sits inside its own flytrap: two lobes like a hood, red inside, with a fringe of soft 'teeth'
    c.ellipse(24,23+dy,14*sx,13*sy,'trap',nc=(24,23+dy,14,13))
    c.ellipse(24,24+dy,11.5,10.5,'jaw',shade=False)
    for i in range(11):
        a=math.pi*(1.0+i/10.0); c.poly([(24+math.cos(a)*12.5-1,23+gape+dy+math.sin(a)*11.6),(24+math.cos(a)*16.5,23+gape+dy+math.sin(a)*15.5),(24+math.cos(a)*12.5+1,23+gape+dy+math.sin(a)*11.6)],'tooth')
    c.ellipse(24,25.5+dy,9,8,'stem',nc=(24,25+dy,9,8))
    c.ellipse(24,28+dy,5.5,3.5,'bel',shade=False)
    face(c,pose,24,4.2,24+dy,24,29+dy,style='round',mo='fang',blush=True)
form('SNAPWORT',TRAP,F_SNAPWORT)

# The less-care side's pollution critter and its secret form: nature taking litter back.
COON={'fur':ramp((150,150,160)),'fur2':ramp((108,106,124)),'mask':ramp((70,66,84)),'pale':ramp((226,222,216)),'can':ramp((176,186,196)),
      'rust':ramp((196,120,80)),'weed':ramp((132,180,74)),'dand':ramp((255,216,64)),'nose':ramp((50,44,56)),'grime':ramp((126,120,96))}
def raccoon(c,pose,dy,sx,sy,droop,tailpal=('fur','mask')):
    wag={'walk1':-1.2,'walk2':1.2}.get(FULL,0)+(-1.5 if pose=='happy' else 0)
    for i,(x,y) in enumerate(((31,41),(35,39),(38.5,36),(40.5,32+wag*0.5),(41,28+wag))):   # ringed tail
        c.ellipse(x,y,3.2-i*0.2,3.0-i*0.2,tailpal[i%2])
    c.ellipse(24,37+dy*0.5,9.5*sx,8*sy,'fur',nc=(24,36,10,9))
    c.ellipse(24,38+dy*0.5,5,5.5,'pale',shade=False)
    dy=dy+2.5
    for s in (-1,1):   # round ears
        c.ellipse(24+s*8,17+dy+droop,3.2,3.4,'fur'); c.ellipse(24+s*8,17.4+dy+droop,1.7,1.9,'pale',shade=False)
    c.ellipse(24,24+dy,10*sx,8*sy,'fur',nc=(24,23+dy,10,9))
    c.ellipse(24,25+dy,8.6,3.4,'mask',shade=False)                      # bandit mask
    c.ellipse(24,28.5+dy,4.6,3.2,'pale',shade=False); c.ellipse(24,27.2+dy,1.4,1.0,'nose',shade=False)
def F_CANCOON(c,pose):
    dy,sx,sy=body_xf(pose); droop=1.5 if pose in('sad','sleep') else 0
    raccoon(c,pose,dy,sx,sy,droop)
    hy=dy+2.5
    for (x,y) in ((18,40),(30,34),(21,33)): c.ellipse(x,y+hy*0.5,1.4,1.0,'grime',shade=False)   # smudges
    # a dented tin can worn as a hat, with weeds and a dandelion growing out of it
    tilt=-0.15
    c.ellipse(24,15+hy,7.5,2.6,'can',rot=tilt)
    c.poly([(16.8,15+hy),(17.6,8+hy),(30.6,7+hy),(31.2,14+hy)],'can',nc=(24,11+hy,8,5))
    shade_by(c,lambda u,v,m: m=='can' and int(u)%3==0 and v<14.5+hy,-0.25)   # ribs
    c.ellipse(21,11+hy,1.6,1.2,'rust',shade=False); c.ellipse(28.5,9.5+hy,1.2,0.9,'rust',shade=False)
    c.ellipse(24,7.3+hy,6.6,1.6,'mask',rot=tilt)
    for (x,a,h) in ((20.5,-0.3,5),(24,0.1,7),(27.5,0.4,5)): c.line(x,7+hy,x+math.sin(a)*h,7+dy-h,(96,150,64),1.0)
    c.ellipse(24.7,0.2+hy,1.9,1.9,'dand')
    c.ellipse(19,3+hy,2.4,1.1,'weed',rot=-0.6); c.ellipse(29.6,3+hy,2.4,1.1,'weed',rot=0.6)
    face(c,pose,24,4.4,24.5+hy,24,30.5+hy,style='round',mo='fang',blush=True)
form('CANCOON',COON,F_CANCOON)
JAR={'fur':ramp((166,170,178)),'fur2':ramp((124,128,140)),'mask':ramp((78,76,96)),'pale':ramp((236,234,228)),'glass':ramp((196,232,236)),
     'moss':ramp((104,176,90)),'leaf':ramp((126,196,96)),'bloom':ramp((250,140,180)),'ctr':ramp((255,220,96)),'lid':ramp((214,170,90)),'nose':ramp((50,44,56)),'fly':ramp((255,246,150))}
def F_JARDEN(c,pose):
    dy,sx,sy=body_xf(pose); droop=1.5 if pose in('sad','sleep') else 0
    raccoon(c,pose,dy,sx,sy,droop,('fur','moss'))                          # its tail rings are moss now
    hy=dy+2.5
    # a glass jar full of garden, worn like a helmet: moss floor, ferns, a flower and a firefly inside
    c.ellipse(24,9+hy,9.5,8.5,'glass',nc=(24,8+hy,10,9))
    c.ellipse(24,14.5+hy,8.2,3.0,'moss')
    c.ellipse(20,9.5+hy,1.4,4,'leaf',rot=-0.3); c.ellipse(27.5,9+hy,1.3,4.4,'leaf',rot=0.35)
    for i in range(5):
        a=i*2*math.pi/5-math.pi/2; c.ellipse(23.5+math.cos(a)*1.6,7+dy+math.sin(a)*1.6,1.3,1.3,'bloom')
    c.ellipse(23.5,7+hy,0.8,0.8,'ctr',shade=False); c.dot(29,4.5+hy,(255,250,180))
    c.ellipse(24,1.2+hy,6,1.6,'lid')
    shade_by(c,lambda u,v,m: m=='glass' and u<20 and v<7+hy,0.35)          # a glint on the glass
    for s in (-1,1): c.dot(24+s*12.5,30+hy,(255,246,150)); c.dot(24+s*12.5,29+hy,(255,252,210))    # fireflies follow it
    face(c,pose,24,4.4,24.5+hy,24,30.5+hy,style='lash',mo='smile',blush=True)
form('JARDEN',JAR,F_JARDEN)

# ---- proposal 5: two pollution pairs to choose from (adult -> its secret form)
# A) a sheep whose wool has soaked up smog -> its wool cleaned and blooming
SMOG={'wool':ramp((150,146,150)),'wool2':ramp((118,114,122)),'face':ramp((92,86,96)),'muzzle':ramp((176,168,170)),'hoof':ramp((60,54,64)),
      'puff':ramp((184,180,186)),'horn':ramp((196,176,150))}
def sheep(c,pose,dy,sx,sy,wool,wool2,face_m,muzzle_m,horn=True):
    for x in (18,30): c.ellipse(x,45-LIFT(x),2.4,1.6,'hoof')
    for i in range(11):                     # a puffy coat of wool
        a=i*2*math.pi/11+0.2
        c.ellipse(24+12*sx*math.cos(a),31+dy+10*sy*math.sin(a),5.4,5.4,wool if i%3 else wool2)
    c.ellipse(24,31+dy,12.5*sx,11*sy,wool,nc=(24,30+dy,15,13))
    for i in range(7):                      # a woolly fringe on the head
        a=math.pi*(1.1+0.8*i/6); c.ellipse(24+6*math.cos(a),21+dy+4*math.sin(a),3.2,3.2,wool)
    if horn:
        for s in (-1,1): c.ellipse(24+s*8.5,24+dy,2.8,3.2,'horn'); c.ellipse(24+s*8.8,25+dy,1.3,1.5,'wool2',shade=False)
    c.ellipse(24,27+dy,7*sx,7.5*sy,face_m,nc=(24,26+dy,7,8))
    c.ellipse(24,31+dy,4.2,2.8,muzzle_m,shade=False)
def F_SMOGWOOL(c,pose):
    dy,sx,sy=body_xf(pose)
    sheep(c,pose,dy,sx,sy,'wool','wool2','face','muzzle')
    for (x,y) in ((13,32),(33,35),(18,38),(30,27),(15,26)): c.dot(x,y+dy,(80,76,84))             # soot specks
    rise={'happy':-2}.get(pose,0)
    for i,(x,y,r) in enumerate(((31,12,2.6),(34,7.5,2.0),(36,4,1.4))): c.ellipse(x,y+dy+rise,r,r*0.85,'puff')   # a wisp of smog
    face(c,pose,24,3.6,26.5+dy,24,31+dy,style='sleepy' if pose=='idle' else 'round',mo='wavy' if pose=='idle' else 'smile',blush=True)
form('SMOGWOOL',SMOG,F_SMOGWOOL)
LAMB={'wool':ramp((252,240,244)),'wool2':ramp((246,214,226)),'face':ramp((246,212,196)),'muzzle':ramp((255,236,226)),'hoof':ramp((150,110,120)),
      'petal':ramp((255,170,200)),'ctr':ramp((255,224,120)),'horn':ramp((240,206,160)),'leaf':ramp((126,196,110))}
def F_PETALAMB(c,pose):
    dy,sx,sy=body_xf(pose)
    sheep(c,pose,dy,sx,sy,'wool','wool2','face','muzzle',horn=False)
    def blossom(x,y,r):
        for i in range(5):
            a=i*2*math.pi/5-math.pi/2; c.ellipse(x+math.cos(a)*r*0.8,y+math.sin(a)*r*0.8,r*0.6,r*0.6,'petal')
        c.ellipse(x,y,r*0.35,r*0.35,'ctr',shade=False)
    for (x,y,r) in ((13,33,2.2),(35,30,2.2),(17,39,1.8),(31,39,1.8)): blossom(x,y+dy,r)       # blossoms in its wool
    for (x,r) in ((17.5,2.4),(24,2.8),(30.5,2.4)): blossom(x,18.5+dy-(1 if x==24 else 0),r)     # a flower crown
    c.ellipse(21,17.5+dy,1.8,0.9,'leaf',rot=-0.4); c.ellipse(27.5,17+dy,1.8,0.9,'leaf',rot=0.4)
    for (x,y) in ((8,18),(40,22),(6,30)): c.ellipse(x,y+dy,1.3,0.8,'petal',rot=0.6)            # drifting petals
    face(c,pose,24,3.6,26.5+dy,24,31+dy,style='lash',mo='smile',blush=True)
form('PETALAMB',LAMB,F_PETALAMB)

# B) a platypus from an oily pond -> a clean lily-pond platypus
GUNK={'fur':ramp((150,112,90)),'fur2':ramp((116,84,70)),'bill':ramp((112,104,116)),'slick1':ramp((120,96,170)),'slick2':ramp((80,150,160)),
      'slick3':ramp((200,170,90)),'belly':ramp((206,178,150)),'foot':ramp((86,78,88))}
def platypus(c,pose,dy,sx,sy,fur,belly,bill,foot):
    wag={'walk1':-0.25,'walk2':0.25}.get(FULL,0)+(-0.3 if pose=='happy' else 0)
    c.ellipse(37.5,39.5+wag*3,7.5,3.4,fur,rot=-0.7+wag)                     # flat beaver tail, sticking out behind
    for x in (17,31): c.ellipse(x,45.2-LIFT(x),3.8,1.6,foot)              # webbed feet
    c.ellipse(24,35+dy*0.5,11.5*sx,9.5*sy,fur,nc=(24,34,12,10))
    c.ellipse(24,37+dy*0.5,6.5,6,belly,shade=False)
    c.ellipse(24,22.5+dy,10.5*sx,7.5*sy,fur,nc=(24,22+dy,11,8))
    c.ellipse(24,28.6+dy,9.5,3.0,bill,nc=(24,28+dy,10,3.5))               # the wide duck bill
    c.dot(21.5,27.6+dy,(40,36,44)); c.dot(26.5,27.6+dy,(40,36,44))
def F_GUNKBILL(c,pose):
    dy,sx,sy=body_xf(pose)
    c.ellipse(24,46.2,13,1.8,'slick1',shade=False); c.ellipse(24,46.2,10,1.2,'bill',shade=False)   # standing in its own oily puddle
    platypus(c,pose,dy,sx,sy,'fur','belly','bill','foot')
    for side in (-1,1):          # a rainbow oil-slick sheen smeared across its sides
        for k,mat in enumerate(('slick1','slick2','slick3')):
            c.ellipse(24+side*(9.5-k*0.6),31+k*2.2+dy*0.5,2.0,1.1,mat,rot=side*0.5,shade=False)
    for (x,y,mat) in ((20,17,'slick2'),(22.5,16.4,'slick1'),(25,16.2,'slick3')): c.ellipse(x,y+dy,1.6,0.9,mat,shade=False)
    c.ellipse(13,40+dy*0.5,1.3,1.8,'slick1')                               # a drip
    face(c,pose,24,4.4,22+dy,24,32+dy,style='sleepy' if pose=='idle' else 'round',mo='tiny',blush=True)
form('GUNKBILL',GUNK,F_GUNKBILL)
LILY={'fur':ramp((110,170,186)),'fur2':ramp((86,140,160)),'bill':ramp((250,200,120)),'belly':ramp((226,240,236)),'foot':ramp((236,170,100)),
      'pad':ramp((96,176,96)),'petal':ramp((255,176,206)),'ctr':ramp((255,224,110)),'drop':ramp((180,226,250))}
def F_LILYBILL(c,pose):
    dy,sx,sy=body_xf(pose)
    platypus(c,pose,dy,sx,sy,'fur','belly','bill','foot')
    c.ellipse(24,15.5+dy,9.5,2.5,'pad',rot=-0.1)                            # a lily-pad hat
    for i in range(5):
        a=i*2*math.pi/5-math.pi/2; c.ellipse(27+math.cos(a)*1.8,12.5+dy+math.sin(a)*1.6,1.4,2.0,'petal',rot=a)
    c.ellipse(27,12.5+dy,0.9,0.9,'ctr',shade=False)
    for (x,y) in ((13,34),(35,32)): c.ellipse(x,y+dy*0.5,1.1,0.9,'fur2',shade=False)   # river-otter spots
    c.ellipse(43,44,1.4,1.9,'drop')                                         # a water drop off its tail
    face(c,pose,24,4.4,22+dy,24,32+dy,style='lash',mo='tiny',blush=True)
form('LILYBILL',LILY,F_LILYBILL)


# v8: THORNHOG's secret form (chosen from the Rosehog / Jamble / Maybloom mockup): kept happy, its berries ripen
def hog_blossom(c, x, y, r, petal='petal', ctr='ctr', n=5, rot=-math.pi/2):
    for i in range(n):
        a = rot + i*2*math.pi/n
        c.ellipse(x+math.cos(a)*r*0.78, y+math.sin(a)*r*0.78, r*0.6, r*0.6, petal)
    c.ellipse(x, y, r*0.36, r*0.36, ctr, shade=False)

def hog_body(c, pose, dy, sx, sy, coat, coat2, spike_fn, fur='hog', face_m='face', nose='nose', bristle=0):
    for x in (17, 31): c.ellipse(x, 45-LIFT(x), 3, 1.6, fur)
    for i in range(13):                                   # the coat's rim, all round its back
        a = math.pi*(1.05+0.9*i/12); r = 15+bristle
        x = 24+math.cos(a)*r*1.05*sx; y = 30+dy+math.sin(a)*r*0.95*sy
        spike_fn(c, i, a, x, y)
    c.ellipse(24, 32+dy, 15*sx, 12*sy, coat, nc=(24, 30+dy, 16, 14))
    c.ellipse(24, 36+dy, 9.5, 8, face_m, nc=(24, 35+dy, 10, 9))
    c.ellipse(24, 38.5+dy, 3.2, 2.4, face_m); c.ellipse(24, 37.4+dy, 1.4, 1.0, nose, shade=False)

# B) JAMBLE - the berries ripen: its thorny coat softens into moss, heavy with ripe berries, with a berry crown
JAM = {'hog': ramp((206, 156, 104)), 'face': ramp((255, 236, 206)), 'coat': ramp((110, 168, 92)), 'coat2': ramp((92, 146, 80)),
       'berry': ramp((226, 60, 96)), 'berry2': ramp((150, 70, 170)), 'ctr': ramp((255, 240, 220)), 'nose': ramp((110, 60, 60)),
       'leaf': ramp((126, 200, 96)), 'flower': ramp((255, 255, 246)), 'ctr2': ramp((255, 214, 90))}
def F_JAMBLE(c, pose):
    dy, sx, sy = body_xf(pose)
    def spike(c, i, a, x, y):      # soft moss tufts
        n = (math.cos(a), math.sin(a))
        c.ellipse(x+n[0]*1.6, y+n[1]*1.6, 3.6, 3.2, 'coat2' if i % 2 else 'coat')
    hog_body(c, pose, dy, sx, sy, 'coat', 'coat2', spike)
    def berry(x, y, mat):
        for (ox, oy) in ((-1.1, -0.9), (1.1, -0.9), (0, 0.7), (-1.1, 1.1), (1.1, 1.1), (0, -1.3)): c.ellipse(x+ox, y+oy, 1.5, 1.5, mat)
    for (x, y, m) in ((11.5, 27, 'berry'), (18, 21, 'berry2'), (31, 20, 'berry'), (36.5, 29, 'berry2'), (14, 35, 'berry'), (33, 35, 'berry')):
        berry(x, y+dy, m)
    hog_blossom(c, 24.5, 23+dy, 2.2, 'flower', 'ctr2')
    for i, x in enumerate((19.5, 24, 28.5)):                                     # a little berry crown
        berry(x, 15+dy-(1.2 if i == 1 else 0), 'berry' if i != 1 else 'berry2')
    c.ellipse(21.5, 16.5+dy, 2.0, 0.9, 'leaf', rot=-0.4); c.ellipse(26.5, 16.5+dy, 2.0, 0.9, 'leaf', rot=0.4)
    face(c, pose, 24, 4.6, 34+dy, 24, 40.5+dy, style='round', mo='smile', blush=True)
form('JAMBLE', JAM, F_JAMBLE)


# the v7 growth tree: forms 13..20 (0..12 keep their old numbers so old albums still show the right critter)
NEW_ORDER=['BLOOMLE','THORNLE','OAKHART','DUNEROSE','THORNHOG','SMOGWOOL','GUNKBILL','PETALAMB','LILYBILL','JAMBLE']   # v8: 21 LILYBILL, 22 JAMBLE
