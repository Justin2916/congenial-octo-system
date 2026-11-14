# Phase 3: shaded decor art, drawn at the exact screen spots the game already uses,
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
# so the animated parts (fish, clock hands, TV/arcade screens, flames, the bird...) still line up.
import math, random, sys
sys.path.insert(0, _ROOT + '/gen')
from critters import Canvas, ramp, mix, clamp, OUTLINE
import volume
volume.Canvas3.MODE = 'soft'; volume.Canvas3.LINES = True

class SC(volume.Canvas3):
    def __init__(s):
        s.k = 1.0; s.N = 240
        s.mat = [[None]*240 for _ in range(240)]; s.tone = [[2]*240 for _ in range(240)]; s.flat = [[None]*240 for _ in range(240)]
        s.d = [[0.0]*240 for _ in range(240)]; s.pid = [[0]*240 for _ in range(240)]; s.part = 0; s.patches = set()
    def rgb(s, ramps, ground=False): return super().rgb(ramps, ground=False)
TONE_D = {0: 0.9, 1: 0.65, 2: 0.32, 3: 0.02, 4: -0.3}

def tp(c, x, y, m, t):
    if 0 <= x < 240 and 0 <= y < 160:
        t = max(0, min(4, t)); c.mat[y][x] = m; c.tone[y][x] = t; c.flat[y][x] = None
        c.d[y][x] = TONE_D[t]; c.pid[y][x] = c.part
def rect(c, x, y, w, h, m, t=2):
    c.part += 1
    for yy in range(y, y+h):
        for xx in range(x, x+w): tp(c, xx, yy, m, t)
def bevel(c, x, y, w, h, m, t=2, r=0, hi=1, lo=1):
    """flat-faced panel: lit top/left edge, shaded bottom/right edge, optional rounded corners"""
    c.part += 1
    for yy in range(y, y+h):
        for xx in range(x, x+w):
            dx = min(xx-x, x+w-1-xx); dy = min(yy-y, y+h-1-yy)
            if r and dx < r and dy < r and (r-dx-0.5)**2+(r-dy-0.5)**2 > r*r: continue
            tt = t
            if yy-y < hi or xx-x < hi: tt = t-1
            if y+h-1-yy < lo or x+w-1-xx < lo: tt = t+1
            tp(c, xx, yy, m, tt)
def line(c, x0, y0, x1, y1, col):
    n = int(max(abs(x1-x0), abs(y1-y0)))+1
    for i in range(n+1):
        t = i/n; c.px(int(round(x0+(x1-x0)*t)), int(round(y0+(y1-y0)*t)), col)
def hline(c, x0, x1, y, col):
    for x in range(x0, x1+1): c.px(x, y, col)
def vline(c, x, y0, y1, col):
    for y in range(y0, y1+1): c.px(x, y, col)
def _sync(c, x, y):
    c.d[y][x] = TONE_D[c.tone[y][x]]
def grain(c, x, y, w, h, m, seed, amt=0.10, streak=5):
    rnd = random.Random(seed)
    for yy in range(y, y+h):
        for xx in range(x, x+w):
            if c.mat[yy][xx] == m and rnd.random() < amt:
                for k in range(rnd.randint(1, streak)):
                    if xx+k < x+w and c.mat[yy][xx+k] == m: c.tone[yy][xx+k] = min(4, c.tone[yy][xx+k]+1); c.d[yy][xx+k] -= 0.22
def speckle(c, x, y, w, h, m, seed, amt=0.08, d=1):
    rnd = random.Random(seed)
    for yy in range(y, y+h):
        for xx in range(x, x+w):
            if 0 <= yy < 160 and c.mat[yy][xx] == m and rnd.random() < amt: c.tone[yy][xx] = max(0, min(4, c.tone[yy][xx]+d)); c.d[yy][xx] -= 0.22*d
def disc(c, cx, cy, r, m, t=None, shade=True):
    if t is None: return c.ellipse(cx, cy, r, r, m, shade=shade)
    for yy in range(int(cy-r)-1, int(cy+r)+2):
        for xx in range(int(cx-r)-1, int(cx+r)+2):
            if (xx+0.5-cx)**2+(yy+0.5-cy)**2 <= r*r: tp(c, xx, yy, m, t)

D = {}   # name -> (draw fn, ramps)
def item(name, ramps):
    def deco(fn): D[name] = (fn, ramps); return fn
    return deco
WOOD = ramp((196, 136, 84)); DKWOOD = ramp((140, 92, 60))

# ================================================================== slot 0: floor, left
# v8.3: the bed stands with its head against the back wall, under the window (3/4 size, its foot toward you): the
# headboard stays below the sill. (Before, it stood by the door, at the left edge of the room.)
def _floor_trap(c, y0, y1, xa0, xa1, xb0, xb1, m, tone_at):
    """a shape lying flat, seen from above: x range a at y0 widening to range b at y1"""
    c.part += 1
    for y in range(y0, y1+1):
        f = (y-y0)/max(1, y1-y0); x0 = round(xa0+(xb0-xa0)*f); x1 = round(xa1+(xb1-xa1)*f)
        for x in range(x0, x1+1): tp(c, x, y, m, tone_at(x, y))
@item('BED', {'wood': WOOD, 'mat': ramp((246, 238, 222)), 'quilt': ramp((236, 120, 140)), 'q2': ramp((250, 196, 120)), 'pil': ramp((250, 250, 250))})
def BED(c):
    bevel(c, 22, 88, 46, 14, 'wood', 2, r=4)                    # headboard, below the sill (the jar's window stays clear)
    rect(c, 26, 91, 38, 1, 'wood', 1)
    _floor_trap(c, 101, 109, 24, 65, 21, 68, 'quilt', lambda x, y: 2 if ((x//6)+(y > 104)) % 2 else 1)   # the quilt
    _floor_trap(c, 99, 101, 26, 63, 25, 64, 'mat', lambda x, y: 1)                                   # the turned-down sheet
    c.ellipse(36, 99, 7, 2.6, 'pil'); c.ellipse(54, 99, 7, 2.6, 'pil')                                # two pillows
    bevel(c, 20, 110, 50, 4, 'mat', 2)                          # the mattress's front
    bevel(c, 19, 113, 52, 3, 'wood', 2)                         # the frame
    rect(c, 20, 116, 3, 2, 'wood', 3); rect(c, 67, 116, 3, 2, 'wood', 3)
    grain(c, 19, 88, 52, 30, 'wood', 1)

# v8.3: the firefly jar's stand (it comes with the first firefly caught): in the middle under the window when nothing
# stands on the left, or just right of what does (the bed or another left piece). Not a decor piece: ext.c draws it
# in the room's own colours (STAND_PIX), so it needs no palette band.
STAND_X = (37, 73)                                   # its left edge: under the window / right of the left piece
STAND_RAMPS = {'wood': WOOD, 'mat': ramp((246, 238, 222))}
def stand(X):
    def f(c):
        for x in range(X, X+16): tp(c, x, 92, 'wood', 0)       # the top: lit, 2 px deep (the jar stands on y 91)
        for x in range(X, X+16): tp(c, x, 93, 'wood', 1)
        bevel(c, X+1, 94, 14, 13, 'wood', 2, r=1)              # the body
        bevel(c, X+3, 97, 10, 5, 'wood', 3, r=1)               # a drawer
        tp(c, X+7, 99, 'mat', 0); tp(c, X+8, 99, 'mat', 0)     # its knob
        rect(c, X+1, 107, 2, 3, 'wood', 3); rect(c, X+13, 107, 2, 3, 'wood', 3)
        grain(c, X, 92, 16, 18, 'wood', 7)
    return f
def render_fn(fn, ramps):
    c = SC(); fn(c); return c.rgb(ramps)

@item('FISHBOWL', {'wood': WOOD, 'glass': ramp((196, 232, 250)), 'water': ramp((110, 186, 236)), 'sand': ramp((240, 214, 150)), 'weed': ramp((90, 180, 110)), 'peb': ramp((170, 160, 200))})
def FISHBOWL(c):
    bevel(c, 10, 116, 30, 11, 'wood', 2, r=1)
    rect(c, 12, 120, 26, 1, 'wood', 3); rect(c, 23, 122, 4, 2, 'wood', 1)
    for yy in range(89, 116):
        for xx in range(12, 39):
            d = math.hypot(xx+0.5-25, yy+0.5-102)
            if d <= 13 and yy >= 89:
                if yy < 95: tp(c, xx, yy, 'glass', 1 if d > 11.5 else 0)
                else:
                    t = 1 + (yy-95)//6
                    tp(c, xx, yy, 'water', t if d < 11.5 else t+1)
    rect(c, 13, 89, 25, 2, 'glass', 1); hline(c, 14, 36, 88, (255, 255, 255))
    hline(c, 15, 35, 95, (230, 246, 255))               # water surface
    for yy in range(110, 115):
        for xx in range(14, 37):
            if math.hypot(xx+0.5-25, yy+0.5-102) <= 11.5: tp(c, xx, yy, 'sand', 2 if (xx+yy) % 3 else 3)
    for (x, y) in ((18, 111), (31, 110), (28, 112)): disc(c, x, y, 1.5, 'peb')
    for k, (x, h) in enumerate(((19, 9), (21, 6), (32, 8))):
        for i in range(h): tp(c, x+(1 if (i//2) % 2 else 0), 110-i, 'weed', 2 if i % 3 else 1)
    for (x, y) in ((17, 98), (16, 101), (17, 104)): c.px(x, y, (255, 255, 255))
    c.px(33, 99, (230, 246, 255))

@item('TOYBOX', {'box': ramp((222, 84, 84)), 'trim': ramp((250, 200, 80)), 'ball': ramp((90, 150, 240)), 'bear': ramp((200, 150, 100)), 'rkt': ramp((240, 240, 246))})
def TOYBOX(c):
    c.ellipse(16, 101, 5.5, 5.5, 'ball')
    for x in range(11, 22): tp(c, x, 101, 'trim', 1)
    c.ellipse(34, 99, 4.5, 4.5, 'bear'); disc(c, 30, 95.5, 2, 'bear'); disc(c, 38, 95.5, 2, 'bear')
    c.px(33, 99, OUTLINE); c.px(35, 99, OUTLINE); c.px(34, 101, OUTLINE)
    c.poly([(24, 104), (26, 92), (28, 104)], 'rkt')
    bevel(c, 6, 106, 40, 21, 'box', 2, r=1)
    bevel(c, 5, 103, 42, 5, 'trim', 2)
    for x in (10, 41): rect(c, x, 110, 2, 14, 'trim', 2)
    c.poly([(26, 110), (27.6, 113.5), (31.4, 113.8), (28.5, 116.2), (29.5, 120), (26, 118), (22.5, 120), (23.5, 116.2), (20.6, 113.8), (24.4, 113.5)], 'trim', tone=1)
    rect(c, 6, 124, 40, 1, 'box', 4)

@item('BEANBAG', {'bag': ramp((90, 140, 236)), 'seam': ramp((60, 100, 200))})
def BEANBAG(c):
    def ins(u, v):
        y = (v-104)/22
        if not 0 <= y <= 1: return False
        half = 13+8*math.sin(min(1, y*1.25)*math.pi/2) - (3*(y-0.85)/0.15 if y > 0.85 else 0)
        return abs(u-26) <= half
    c.shape(ins, (4, 103, 48, 127), 'bag', (26, 112, 20, 14))
    for yy in range(106, 113):                          # dimple where it's been sat on
        for xx in range(18, 34):
            if ((xx-26)/8)**2+((yy-109)/3.5)**2 <= 1: c.tone[yy][xx] = min(4, c.tone[yy][xx]+1); c.d[yy][xx] -= 0.25
    for (x0, y0, x1, y1) in ((26, 104, 22, 126), (14, 112, 10, 125), (38, 112, 42, 125)): line(c, x0, y0, x1, y1, mix((60, 100, 200), OUTLINE, 0.3))
    for x in range(8, 45): c.px(x, 126, mix((40, 70, 150), OUTLINE, 0.5))

@item('PIANO', {'wood': ramp((150, 70, 60)), 'key': ramp((250, 248, 240)), 'sheet': ramp((250, 244, 220)), 'brass': ramp((240, 196, 90))})
def PIANO(c):
    bevel(c, 5, 91, 42, 4, 'wood', 1, r=1)
    bevel(c, 6, 94, 40, 13, 'wood', 2)
    bevel(c, 15, 96, 22, 9, 'sheet', 1)
    for y in (98, 100, 102): hline(c, 17, 34 if y != 102 else 27, y, (150, 140, 170))
    for (x, y) in ((20, 99), (25, 97), (31, 101)): c.px(x, y, OUTLINE)
    bevel(c, 5, 107, 42, 3, 'wood', 1)
    bevel(c, 6, 110, 40, 5, 'key', 1)
    for x in range(9, 45, 4): vline(c, x, 112, 114, (190, 186, 200))
    for x in range(10, 44, 4):
        if ((x-10) >> 2) % 7 not in (2, 6): rect(c, x, 110, 2, 3, 'wood', 4)
    bevel(c, 6, 115, 40, 8, 'wood', 3)
    rect(c, 9, 118, 34, 1, 'wood', 2)
    rect(c, 8, 123, 3, 4, 'wood', 4); rect(c, 41, 123, 3, 4, 'wood', 4)
    disc(c, 40, 93, 1.2, 'brass', 1)
    grain(c, 6, 94, 40, 30, 'wood', 7, 0.05)

# ================================================================== slot 1: floor, right
def pot(c):
    bevel(c, 205, 108, 24, 4, 'terra', 1)
    for yy in range(112, 126):
        inset = (yy-112)//6
        for xx in range(206+inset, 228-inset): tp(c, xx, yy, 'terra', 2 if xx < 222-inset else 3)
    hline(c, 207, 227, 112, mix((210, 110, 70), OUTLINE, 0.4))
def leafy(c, x, y, ang, L, m='leaf'):
    for i in range(L):
        t = i/(L-1); w = math.sin(t*math.pi)*2.0
        cx = x+math.cos(ang)*i; cy = y+math.sin(ang)*i
        for o in range(-int(w), int(w)+1):
            tp(c, int(round(cx-math.sin(ang)*o)), int(round(cy+math.cos(ang)*o)), m, 1 if o < 0 else 2 if o == 0 else 3)
for g in range(3):
    def PLANT(c, g=g):
        top = (100, 88, 76)[g]
        for y in range(top, 109): tp(c, 216, y, 'stem', 2); tp(c, 217, y, 'stem', 3)
        n = (1, 3, 5)[g]
        for i in range(n):
            y = 106-i*5
            leafy(c, 216, y, math.pi+0.5-(i % 2)*0.2, 8+i % 3); leafy(c, 217, y-2, -0.5+(i % 2)*0.2, 8+(i+1) % 3)
        if g == 0: leafy(c, 216, top+3, -1.3, 5); leafy(c, 217, top+3, -1.9, 5)
        if g >= 1: c.ellipse(216.5, top, 2.6, 3.2, 'bud' if g == 1 else 'petal')
        if g == 2:
            for k in range(6):
                a = k*math.pi/3; c.ellipse(216.5+3.5*math.cos(a), top+3.5*math.sin(a), 2.6, 2.6, 'petal')
            disc(c, 216.5, top, 2, 'ctr', 1)
            for (x, y) in ((208, top+12), (225, top+16)):
                for k in range(5):
                    a = k*2*math.pi/5; disc(c, x+2*math.cos(a), y+2*math.sin(a), 1.3, 'petal', 1)
                c.px(x, y, (255, 214, 80))
        pot(c)
    item('PLANT%d' % g, {'terra': ramp((210, 110, 70)), 'stem': ramp((70, 140, 70)), 'leaf': ramp((100, 190, 90)), 'bud': ramp((120, 200, 100)), 'petal': ramp((250, 150, 190)), 'ctr': ramp((255, 210, 80))})(PLANT)

@item('RADIO', {'body': ramp((220, 80, 80)), 'grill': ramp((236, 220, 196)), 'dial': ramp((255, 236, 170)), 'knob': ramp((240, 196, 90)), 'metal': ramp((190, 196, 210))})
def RADIO(c):
    line(c, 229, 104, 238, 92, (190, 196, 210)); c.px(238, 91, (255, 255, 255))
    bevel(c, 200, 104, 34, 20, 'body', 2, r=3)
    bevel(c, 203, 107, 14, 14, 'grill', 2, r=2)
    for y in range(109, 120, 2): hline(c, 205, 214, y, mix((236, 220, 196), OUTLINE, 0.5))
    bevel(c, 220, 108, 10, 5, 'dial', 1)
    for x in range(221, 229, 2): c.px(x, 112, (200, 150, 90))
    c.ellipse(225, 117.5, 2.6, 2.6, 'knob')
    rect(c, 203, 124, 3, 3, 'metal', 3); rect(c, 228, 124, 3, 3, 'metal', 3)

@item('TV', {'case': ramp((140, 104, 76)), 'bezel': ramp((80, 84, 110)), 'knob': ramp((240, 196, 90)), 'metal': ramp((190, 196, 210))})
def TV(c):
    line(c, 214, 93, 206, 85, (190, 196, 210)); line(c, 218, 93, 226, 85, (190, 196, 210))
    c.px(206, 84, (255, 255, 255)); c.px(226, 84, (255, 255, 255))
    c.ellipse(216, 94, 4, 1.8, 'metal')
    bevel(c, 197, 94, 38, 28, 'case', 2, r=3)
    bevel(c, 199, 96, 28, 21, 'bezel', 3, r=2)
    rect(c, 200, 97, 26, 19, 'bezel', 4)
    c.ellipse(230.5, 101, 2.4, 2.4, 'knob'); c.ellipse(230.5, 108, 2.4, 2.4, 'metal')
    for y in range(113, 119, 2): hline(c, 229, 233, y, mix((140, 104, 76), OUTLINE, 0.5))
    grain(c, 197, 94, 38, 28, 'case', 3, 0.06)
    rect(c, 203, 122, 6, 4, 'case', 4); rect(c, 223, 122, 6, 4, 'case', 4)

@item('ARCADE', {'cab': ramp((80, 90, 210)), 'side': ramp((120, 150, 240)), 'mq': ramp((250, 200, 80)), 'pan': ramp((70, 76, 110)), 'btn': ramp((236, 80, 100))})
def ARCADE(c):
    bevel(c, 206, 74, 26, 53, 'cab', 2, r=2)
    rect(c, 206, 76, 2, 50, 'side', 1); rect(c, 230, 76, 2, 50, 'side', 3)
    bevel(c, 209, 76, 20, 7, 'mq', 1, r=1)
    for x in range(211, 228, 3): c.px(x, 79, (236, 80, 100)); c.px(x+1, 79, (90, 150, 240))
    bevel(c, 207, 83, 24, 20, 'pan', 3, r=1)
    rect(c, 208, 84, 22, 18, 'pan', 4)
    c.poly([(204, 103), (234, 103), (232, 110), (206, 110)], 'pan', tone=2)
    hline(c, 204, 234, 103, (150, 160, 200))
    vline(c, 212, 100, 105, OUTLINE); c.ellipse(212, 100, 1.8, 1.8, 'btn')
    for x, col in ((219, (236, 80, 100)), (223, (250, 200, 80)), (227, (120, 220, 170))): disc(c, x+1, 106, 1.3, 'btn', 1); c.px(x, 105, col); c.px(x+1, 105, col)
    bevel(c, 214, 113, 10, 9, 'pan', 2, r=1)
    rect(c, 218, 116, 2, 3, 'mq', 1)
    for y in range(88, 124, 6): c.px(207, y, (180, 200, 255))

# v8.3: on the back wall under the picture, at 3/4 size, facing straight out. It stands out from the wall by its mantel
# (lit top, dark underside), a hearth slab on the floor and a soft shadow round it on the wall (FP_SHADOW, applied by
# ext.c to the wall's own colours). Green stockings hang just inside the corners, toes to the right.
FP_X0, FP_Y0, FP_W, FP_H = 181, 72, 30, 30          # the front face (x 181-210, y 72-101); the floor starts at y 101
FP_SOCKS = (182, 204)
def _fp_sock_at(x, y): return y < 80 and any(sx-1 <= x <= sx+6 for sx in FP_SOCKS)
@item('FIREPLACE', {'brick': ramp((200, 100, 80)), 'brickin': ramp((120, 60, 52)), 'stone': ramp((210, 200, 190)), 'mantel': ramp((180, 120, 76)),
                    'soot': ramp((60, 52, 70)), 'log': ramp((140, 90, 60)), 'sock': ramp((70, 170, 90)), 'glow': ramp((255, 170, 90))})
def FIREPLACE(c):
    X0, Y0, W, H = FP_X0, FP_Y0, FP_W, FP_H; mort = mix((200, 100, 80), (240, 220, 200), 0.45)
    bevel(c, X0, Y0, W, H, 'brick', 2)
    for r_, y in enumerate(range(Y0+2, Y0+H, 3)):                 # brick courses (smaller bricks: it's further back)
        for x in range(X0+1, X0+W-1):
            if not _fp_sock_at(x, y): c.px(x, y, mort)
        for x in range(X0+1+(r_ & 1)*3, X0+W-1, 6):
            for yy in (y+1, y+2):
                if not _fp_sock_at(x, yy): c.px(x, yy, mort)
    speckle(c, X0, Y0, W, H, 'brick', 4, 0.12)
    bevel(c, 186, 79, 20, 23, 'stone', 2, r=3)                     # the stone arch
    for yy in range(82, 101):                                      # the recess: its inner right wall and a sooty back
        for xx in range(188, 204):
            if yy < 84 and (xx < 190 or xx > 201): continue
            inner = xx >= 201-(yy-82)//8
            tp(c, xx, yy, 'brickin' if inner else 'soot', 3 if inner else (4 if yy < 92 else 3))
    bevel(c, 190, 96, 13, 3, 'log', 2, r=1); bevel(c, 192, 94, 9, 2, 'log', 1, r=1)
    for x in range(176, 216): tp(c, x, 64, 'mantel', 0)            # the mantel: lit top, 2 px deep
    for x in range(176, 216): tp(c, x, 65, 'mantel', 1)
    bevel(c, 176, 66, 40, 4, 'mantel', 1, r=1); grain(c, 176, 66, 40, 4, 'mantel', 9, 0.15)
    for x in range(176, 216): tp(c, x, 70, 'mantel', 4)            # its underside
    for x in FP_SOCKS:                                             # stockings
        c.part += 1
        for y in range(71, 79):
            for xx in range(x, x+4): tp(c, xx, y, 'sock', 1 if xx == x else 2)
        for xx in range(x+4, x+6):
            for y in (77, 78): tp(c, xx, y, 'sock', 2)
        for xx in range(x, x+4): tp(c, xx, 71, 'stone', 0); tp(c, xx, 72, 'stone', 1); tp(c, xx, 75, 'stone', 1)
        tp(c, x, 77, 'stone', 1); tp(c, x+5, 78, 'stone', 1)
    for yy, (a, b) in zip(range(101, 105), ((178, 213), (177, 214), (176, 215), (175, 216))):   # the hearth slab
        for xx in range(a, b+1): tp(c, xx, yy, 'stone', 1)
    for xx in range(189, 204): tp(c, xx, 101, 'glow', 1); tp(c, xx, 102, 'glow', 2)
    for yy in (105, 106):
        for xx in range(175, 217): tp(c, xx, yy, 'stone', 3 if yy == 105 else 4)
def object_shadow(img, floor=False):
    """(x, y, level 0-2): a soft, even shadow on the wall round an object standing against it (both sides alike, a
    little deeper under it), and with floor=True one on the floor just under its front"""
    m = [[img[y][x] is not None for x in range(240)] for y in range(160)]
    ys = [y for y in range(160) for x in range(240) if m[y][x]]; xs = [x for y in range(160) for x in range(240) if m[y][x]]
    def ok(x, y): return 0 <= y < 160 and 0 <= x < 240 and m[y][x]
    out = []
    for y in range(max(28, min(ys)-4), min(134, max(ys)+5)):
        for x in range(max(0, min(xs)-4), min(240, max(xs)+5)):
            if m[y][x]: continue
            for lv, d in enumerate((1, 2, 3)):
                if y < 101 and ((ok(x-d, y) and y-0 < 101) or ok(x+d, y) or (ok(x, y-d-1) and y-d-1 < 101)): out.append((x, y, lv)); break
                if floor and y >= 101 and ok(x, y-d) and not (y-d < 101): out.append((x, y, lv)); break
    return out
def fireplace_shadow(img): return object_shadow(img)

# ================================================================== slot 2: wall, centre
@item('CLOCK', {'frame': WOOD, 'face': ramp((250, 244, 226)), 'bell': ramp((240, 196, 90))})
def CLOCK(c):
    for sx in (-1, 1): c.ellipse(146+sx*7, 37.5, 3, 2.4, 'bell')
    c.ellipse(146, 48, 11.5, 11.5, 'frame')
    for yy in range(36, 61):
        for xx in range(134, 159):
            if math.hypot(xx+0.5-146, yy+0.5-48) <= 9: tp(c, xx, yy, 'face', 1 if yy < 48 else 2)
    for h in range(12):
        a = h*math.pi/6
        c.px(int(round(146+7.5*math.sin(a)-0.5)), int(round(48-7.5*math.cos(a)-0.5)), OUTLINE if h % 3 == 0 else (190, 180, 200))
    for yy in range(37, 60):
        for xx in range(135, 158):
            d = math.hypot(xx+0.5-146, yy+0.5-48)
            if 9 < d <= 10.2: tp(c, xx, yy, 'frame', 3)

@item('BOOKSHELF', {'wood': WOOD, 'b1': ramp((220, 80, 90)), 'b2': ramp((90, 140, 230)), 'b3': ramp((250, 200, 80)), 'b4': ramp((110, 190, 120)), 'pot': ramp((210, 110, 70)), 'cac': ramp((110, 180, 100))})
def BOOKSHELF(c):
    bevel(c, 126, 58, 44, 4, 'wood', 1)
    for x in (129, 164):
        c.poly([(x, 62), (x+4, 62), (x, 67)], 'wood', tone=3)
    books = (('b1', 14), ('b2', 12), ('b3', 15), ('b4', 11), ('b1', 13), ('b2', 15))
    x = 130
    for i, (m, h) in enumerate(books):
        w = 5 if i != 2 else 6
        bevel(c, x, 58-h, w, h, m, 2)
        rect(c, x+1, 60-h, w-2, 1, m, 0); rect(c, x+1, 54-h//3, w-2, 1, m, 0)
        x += w
    c.poly([(x, 58), (x+3, 58), (x+9, 47), (x+6, 46)], 'b4')        # a leaning book
    bevel(c, 160, 52, 7, 6, 'pot', 2)
    c.ellipse(163.5, 47.5, 2.6, 4.5, 'cac'); c.ellipse(160.8, 48.5, 1.2, 2.2, 'cac'); c.ellipse(166, 47, 1.2, 2, 'cac')
    c.px(163, 42, (250, 150, 190))

@item('PORTRAIT', {'gold': ramp((240, 196, 90)), 'bg': ramp((250, 236, 216))})
def PORTRAIT(c):
    bevel(c, 127, 36, 30, 28, 'gold', 2, r=2)
    for (x, y) in ((127, 36), (153, 36), (127, 60), (153, 60)): bevel(c, x, y, 4, 4, 'gold', 1, r=1)
    bevel(c, 130, 39, 24, 22, 'gold', 3)
    for yy in range(40, 60):
        for xx in range(131, 153): tp(c, xx, yy, 'bg', 1 if yy < 46 else 2 if yy < 54 else 3)
    for x in range(131, 153, 2): c.px(x, 36, (255, 246, 200))

@item('MIRROR', {'gold': ramp((240, 196, 90)), 'glass': ramp((200, 226, 246))})
def MIRROR(c):
    vline(c, 146, 29, 31, OUTLINE)
    c.ellipse(146, 47, 12.5, 16.5, 'gold')
    c.ellipse(146, 31.5, 3, 2.2, 'gold')
    for yy in range(33, 62):
        for xx in range(136, 157):
            if ((xx+0.5-146)/9.5)**2+((yy+0.5-47)/13.5)**2 <= 1:
                tp(c, xx, yy, 'glass', 1 + (yy-33)//10)
    for k in range(7): c.px(139+k, 45-k, (255, 255, 255)); c.px(143+k, 52-k, (255, 255, 255)); c.px(144+k, 52-k, (255, 255, 255))

@item('GUITAR', {'body': ramp((232, 160, 80)), 'neck': DKWOOD, 'gold': ramp((240, 196, 90))})
def GUITAR(c):
    hline(c, 143, 153, 29, OUTLINE)
    bevel(c, 145, 30, 7, 6, 'neck', 2, r=1)
    for (x, y) in ((144, 31), (152, 31), (144, 34), (152, 34)): c.px(x, y, (240, 196, 90))
    rect(c, 147, 36, 3, 14, 'neck', 2); rect(c, 147, 36, 1, 14, 'neck', 1)
    for y in range(38, 48, 3): hline(c, 147, 149, y, (240, 196, 90))
    c.ellipse(148.5, 51.5, 5, 4.5, 'body'); c.ellipse(148.5, 59.5, 7, 6, 'body', nc=(148.5, 56, 7, 10))
    c.ellipse(148.5, 55.5, 2.2, 2.2, 'neck', shade=False)
    for yy in range(54, 58):
        for xx in range(147, 151):
            if math.hypot(xx+0.5-148.5, yy+0.5-55.5) <= 1.6: c.px(xx, yy, OUTLINE)
    rect(c, 145, 60, 7, 2, 'neck', 3)
    for x in (147, 149): vline(c, x+1 if x == 149 else x, 37, 60, (250, 240, 220))

# ================================================================== slot 3: ceiling
@item('MOBILE', {'wood': WOOD})
def MOBILE(c):
    vline(c, 101, 28, 33, OUTLINE)
    c.ellipse(101, 34.5, 14, 1.8, 'wood')
    for x in (88, 114): c.ellipse(x, 34.5, 1.6, 1.6, 'wood')
@item('LANTERN', {'paper': ramp((236, 84, 84)), 'cap': ramp((60, 52, 70)), 'gold': ramp((250, 200, 80))})
def LANTERN(c):
    vline(c, 101, 28, 35, OUTLINE)
    bevel(c, 97, 35, 9, 3, 'cap', 2)
    c.ellipse(101.5, 46, 9.5, 9, 'paper')
    for y in range(39, 55, 4):
        for xx in range(92, 112):
            if c.mat[y][xx] == 'paper': c.tone[y][xx] = min(4, c.tone[y][xx]+1); c.d[y][xx] -= 0.3
    for x in (97, 101, 106):
        for yy in range(38, 55):
            if c.mat[yy][x] == 'paper' and yy % 2: c.tone[yy][x] = max(0, c.tone[yy][x]-1); c.d[yy][x] += 0.25
    bevel(c, 97, 54, 9, 3, 'cap', 2)
    for y in range(57, 64): c.px(101, y, (250, 200, 80)); c.px(102, y, (240, 170, 60) if y > 59 else (250, 200, 80))
@item('DISCO', {'mir': ramp((200, 206, 226)), 'tint': ramp((170, 200, 250))})
def DISCO(c):
    vline(c, 101, 28, 34, OUTLINE)
    c.ellipse(101.5, 42.5, 8, 8, 'mir')
    for yy in range(34, 51):
        for xx in range(93, 110):
            if c.mat[yy][xx] == 'mir':
                if (yy-35) % 3 == 0 or (xx-94) % 3 == 0: c.tone[yy][xx] = min(4, c.tone[yy][xx]+1); c.d[yy][xx] -= 0.3
                elif ((xx//3)+(yy//3)) % 4 == 0: c.mat[yy][xx] = 'tint'
@item('PLANE', {})
def PLANE(c): vline(c, 101, 28, 31, OUTLINE)

# ================================================================== slot 4: yard, left
@item('SWING', {'bark': ramp((150, 100, 70)), 'leaf': ramp((90, 170, 80))})
def SWING(c):
    bevel(c, 28, 59, 37, 4, 'bark', 2, r=1)
    grain(c, 28, 59, 37, 4, 'bark', 5, 0.25, 3)
    for (x, y) in ((63, 58), (66, 56), (61, 56)): c.ellipse(x, y, 2.4, 1.6, 'leaf')
@item('SANDBOX', {'wood': WOOD, 'sand': ramp((240, 214, 150)), 'pail': ramp((236, 80, 90)), 'flag': ramp((236, 80, 90))})
def SANDBOX(c):
    for yy in range(115, 127):
        for xx in range(22, 66): tp(c, xx, yy, 'sand', 2)
    speckle(c, 22, 115, 44, 12, 'sand', 3, 0.18, 1); speckle(c, 22, 115, 44, 12, 'sand', 4, 0.10, -1)
    bevel(c, 20, 112, 48, 3, 'wood', 1); bevel(c, 20, 126, 48, 3, 'wood', 2)
    rect(c, 20, 115, 2, 11, 'wood', 2); rect(c, 66, 115, 2, 11, 'wood', 3)
    for (x, w, h) in ((34, 4, 12), (46, 4, 12)):
        bevel(c, x, 116-h, w, h, 'sand', 1)
        for k in range(0, w, 2): tp(c, x+k, 115-h, 'sand', 1)
    bevel(c, 37, 108, 10, 8, 'sand', 2)
    rect(c, 41, 112, 3, 4, 'sand', 4)
    vline(c, 42, 97, 107, OUTLINE); rect(c, 43, 97, 5, 3, 'flag', 1)
    c.poly([(54, 116), (62, 116), (61, 124), (55, 124)], 'pail')
    hline(c, 54, 62, 116, (255, 200, 200))
    line(c, 54, 116, 58, 112, OUTLINE); line(c, 58, 112, 62, 116, OUTLINE)
@item('TRAMPOLINE', {'frame': ramp((90, 140, 236)), 'mat': ramp((60, 64, 96)), 'leg': ramp((180, 186, 206))})
def TRAMPOLINE(c):
    for x in (21, 43, 65): bevel(c, x, 113, 3, 15, 'leg', 2)
    c.ellipse(44, 109, 27.5, 6.5, 'frame', shade=False)
    for yy in range(102, 117):
        for xx in range(16, 73):
            if c.mat[yy][xx] == 'frame':
                c.tone[yy][xx] = 1 if yy < 107 else 2 if yy < 112 else 3; c.d[yy][xx] = TONE_D[c.tone[yy][xx]]
    for yy in range(104, 114):
        for xx in range(20, 69):
            if ((xx+0.5-44)/22.5)**2+((yy+0.5-108.6)/3.6)**2 <= 1: tp(c, xx, yy, 'mat', 3 if yy < 108 else 2)
    for k in range(0, 360, 20):
        a = math.radians(k); x = 44+25*math.cos(a); y = 109+5.2*math.sin(a)
        c.px(int(x), int(y), (240, 246, 255))
@item('SLIDE', {'slide': ramp((250, 200, 70)), 'pole': ramp((120, 160, 220)), 'plat': ramp((236, 84, 84))})
def SLIDE(c):
    for x in (22, 31): bevel(c, x, 84, 2, 44, 'pole', 2)
    for y in range(90, 126, 6): rect(c, 24, y, 7, 1, 'pole', 1)
    bevel(c, 20, 81, 18, 4, 'plat', 1)
    for k in range(38):
        tp(c, 36+k, 82+k, 'slide', 0)
        for j in range(1, 5): tp(c, 36+k, 82+k+j, 'slide', 1 if j < 2 else 2 if j < 4 else 3)
    bevel(c, 74, 119, 10, 6, 'slide', 2)
    bevel(c, 56, 104, 2, 24, 'pole', 2)

# ================================================================== slot 5: yard, right
@item('BIRDBATH', {'stone': ramp((196, 196, 210)), 'water': ramp((120, 190, 240))})
def BIRDBATH(c):
    bevel(c, 184, 104, 7, 21, 'stone', 2)
    for y in range(107, 123, 5): hline(c, 185, 189, y, mix((196, 196, 210), OUTLINE, 0.3))
    c.ellipse(187.5, 125, 8.5, 2.8, 'stone')
    c.ellipse(187.5, 100, 16.5, 4.8, 'stone')
    for yy in range(96, 102):
        for xx in range(174, 202):
            if ((xx+0.5-187.5)/13.5)**2+((yy+0.5-98.5)/2.6)**2 <= 1: tp(c, xx, yy, 'water', 1 if yy < 98 else 2)
    c.px(180, 98, (255, 255, 255)); c.px(181, 98, (255, 255, 255))
@item('GARDEN', {'soil': ramp((130, 86, 60)), 'leaf': ramp((90, 180, 80)), 'tom': ramp((236, 70, 70)), 'fence': ramp((250, 250, 246))})
def GARDEN(c):
    bevel(c, 166, 113, 44, 13, 'soil', 2, r=1)
    speckle(c, 166, 114, 44, 12, 'soil', 6, 0.25, 1)
    for i in range(5):
        x = 170+i*8
        vline(c, x+1, 104, 114, (70, 130, 60))
        leafy(c, x+1, 110, math.pi+0.6, 5, 'leaf'); leafy(c, x+1, 107, -0.6, 5, 'leaf'); leafy(c, x+1, 105, -1.6, 4, 'leaf')
        c.ellipse(x+1.5, 111, 2.4, 2.4, 'tom' if i % 2 else 'leaf')
    for x in range(166, 210, 4):
        bevel(c, x, 120, 2, 8, 'fence', 1); c.px(x, 119, (250, 250, 246))
    rect(c, 166, 122, 44, 1, 'fence', 2); rect(c, 166, 125, 44, 1, 'fence', 3)
@item('POND', {'rock': ramp((180, 176, 190)), 'water': ramp((90, 160, 230)), 'pad': ramp((100, 190, 100)), 'reed': ramp((120, 100, 70)), 'stalk': ramp((90, 150, 70))})
def POND(c):
    c.ellipse(188, 120, 23.5, 8.5, 'rock')
    for yy in range(112, 129):
        for xx in range(166, 211):
            if ((xx+0.5-188)/21)**2+((yy+0.5-120)/6.2)**2 <= 1: tp(c, xx, yy, 'water', 2 if yy > 118 else 3)
    for yy in range(116, 121):
        for xx in range(172, 202):
            if ((xx+0.5-186)/13)**2+((yy+0.5-118.5)/1.8)**2 <= 1: tp(c, xx, yy, 'water', 1)
    for (x, y) in ((170, 113), (178, 128), (198, 112), (205, 126), (186, 112)): c.ellipse(x, y, 2.8, 1.8, 'rock')
    c.ellipse(176.5, 121.5, 3.5, 2, 'pad', shade=False); c.px(177, 121, (90, 160, 230)); c.px(178, 121, (90, 160, 230))
    c.ellipse(198, 124, 2.6, 1.4, 'pad', shade=False)
    for (x, top) in ((206, 102), (209, 105), (203, 107)):
        vline(c, x, top+4, 117, (90, 150, 70)); rect(c, x, top, 2, 4, 'reed', 2)
    c.px(196, 118, (255, 255, 255)); c.px(197, 118, (255, 255, 255))
@item('FLOWERS', {'box': WOOD, 'soil': ramp((110, 76, 56)), 'stem': ramp((80, 160, 80)), 'p1': ramp((250, 140, 180)), 'p2': ramp((255, 210, 80)), 'p3': ramp((176, 150, 240)), 'p4': ramp((250, 250, 250))})
def FLOWERS(c):
    pal = ('p1', 'p2', 'p3', 'p4', 'p1', 'p3', 'p2')
    for i in range(7):
        x = 170+i*6; top = 106+(i & 1)*4
        vline(c, x, top, 119, (80, 160, 80))
        leafy(c, x, top+8, -0.5, 4, 'stem'); leafy(c, x, top+10, math.pi+0.5, 4, 'stem')
        for k in range(5):
            a = k*2*math.pi/5-math.pi/2; disc(c, x+0.5+2*math.cos(a), top+0.5+2*math.sin(a), 1.4, pal[i], 1 if k < 2 else 2)
        c.px(x, top, (255, 214, 80) if pal[i] != 'p2' else (220, 120, 60))
    bevel(c, 166, 119, 44, 9, 'box', 2, r=1)
    rect(c, 167, 119, 42, 2, 'soil', 3)
    grain(c, 166, 121, 44, 7, 'box', 12, 0.15)

# ------------------------------------------------------------------ render
# ------------------------------------------------------------------ v8.3: where each indoor piece stands
# The room has five themed spots: under the window (comfort: bed, beanbag; the toy box goes at the bed's foot), under
# the picture (fun and cozy: fireplace, TV on a low cabinet, radio on a side table, arcade, piano), the right-hand
# corner (nature: the plant, or the fishbowl on a little table), the wall and the ceiling. The pieces were drawn for
# the old spots, so here each one is resampled (its material map, then shaded) to its new size and place.
# PLACE[name] = (scale, centre x, bottom y, stand or None, source rows to drop from y)  -- or ('shift', dx, dy)
PIC_DX = -12                                         # the picture and the spot under it moved 12 px left
WALL_DX = -8                                         # the wall spot moved 8 px left
STAND_R = {'stw': WOOD, 'stm': ramp((246, 238, 222))}
def cabinet(X, Wd, top, bot):                        # a low TV cabinet: lit top, two doors with small round knobs
    def f(c):
        for x in range(X, X+Wd): tp(c, x, top, 'stw', 0); tp(c, x, top+1, 'stw', 1)
        bevel(c, X+1, top+2, Wd-2, bot-top-3, 'stw', 2, r=1)
        h = (Wd-7)//2
        bevel(c, X+3, top+4, h, bot-top-8, 'stw', 3, r=1); bevel(c, X+4+h, top+4, h, bot-top-8, 'stw', 3, r=1)
        for kx in (X+3+h-2, X+4+h+1): tp(c, kx, top+6, 'stm', 1)
        rect(c, X+1, bot-1, 2, 2, 'stw', 3); rect(c, X+Wd-3, bot-1, 2, 2, 'stw', 3)
        grain(c, X, top, Wd, bot-top+1, 'stw', 3)
    return f
def side_table(X, Wd, top, bot):                     # a little side table: top, a low shelf, thin legs
    def f(c):
        for x in range(X, X+Wd): tp(c, x, top, 'stw', 0); tp(c, x, top+1, 'stw', 1)
        bevel(c, X, top+2, Wd, 2, 'stw', 2)
        for lx in (X+1, X+Wd-3): rect(c, lx, top+4, 2, bot-top-4, 'stw', 3)
        bevel(c, X+2, bot-5, Wd-4, 2, 'stw', 2)
        grain(c, X, top, Wd, bot-top+1, 'stw', 5)
    return f
PCX = 196 + PIC_DX                                    # the centre of the spot under the picture
NCX = 222                                             # the centre of the right-hand corner
PLACE = {
    'BEANBAG':   (0.75, 45, 108, None, None),
    'TV':        (0.75, PCX, 96, cabinet(PCX-16, 33, 96, 106), None),
    'RADIO':     (0.75, PCX, 93, side_table(PCX-13, 27, 93, 106), None),
    'ARCADE':    (0.75, PCX, 106, None, None),
    'PIANO':     (0.75, PCX, 106, None, None),
    'FIREPLACE': ('shift', PIC_DX, 0),
    'PLANT0':    (0.75, NCX, 106, None, None), 'PLANT1': (0.75, NCX, 106, None, None), 'PLANT2': (0.75, NCX, 106, None, None),
    'FISHBOWL':  (0.75, NCX, 93, side_table(NCX-10, 21, 93, 106), 116),    # (its own wooden base is dropped: it has a table)
    'CLOCK': ('shift', WALL_DX, 0), 'BOOKSHELF': ('shift', WALL_DX, 0), 'PORTRAIT': ('shift', WALL_DX, 0),
    'MIRROR': ('shift', WALL_DX, 12), 'GUITAR': ('shift', WALL_DX, 12),    # (they hung higher than the others)
}
XF = {}                                               # name -> (source x0, y0, new x0, y0, scale x4): for ext.c's animations
def _bbox(c):
    pts = [(x, y) for y in range(160) for x in range(240) if c.mat[y][x] is not None]
    return min(p[0] for p in pts), min(p[1] for p in pts), max(p[0] for p in pts), max(p[1] for p in pts)
def _copy(c, c2, sx, sy, X, Y, pid_add=0):
    c2.mat[Y][X] = c.mat[sy][sx]; c2.tone[Y][X] = c.tone[sy][sx]; c2.flat[Y][X] = c.flat[sy][sx]
    c2.d[Y][X] = c.d[sy][sx]; c2.pid[Y][X] = c.pid[sy][sx] + pid_add
def placed_canvas(name):
    """the piece's canvas at its new place (and its ramps), with any stand drawn under it"""
    fn, ramps = D[name]; c = SC(); fn(c); ramps = dict(ramps or {'_': ramp((0, 0, 0))})
    pl = PLACE.get(name)
    if pl is None: XF[name] = (0, 0, 0, 0, 4); return c, ramps
    c2 = SC()
    if pl[0] == 'shift':
        _, dx, dy = pl
        for y in range(160):
            for x in range(240):
                if c.mat[y][x] is not None and 0 <= x+dx < 240 and 0 <= y+dy < 160: _copy(c, c2, x, y, x+dx, y+dy)
        c2.part = c.part; XF[name] = (0, 0, dx, dy, 4); return c2, ramps
    scale, cx, base, stand, drop = pl
    if drop is not None:
        for y in range(drop, 160):
            for x in range(240): c.mat[y][x] = None
    x0, y0, x1, y1 = _bbox(c); w = x1-x0+1; h = y1-y0+1; W = round(w*scale); H = round(h*scale); nx = cx-W//2; ny = base-H+1
    off = 0
    if stand: stand(c2); ramps.update(STAND_R); off = c2.part + 1
    for y in range(H):
        for x in range(W):
            sx = x0+min(w-1, int((x+0.5)/scale)); sy = y0+min(h-1, int((y+0.5)/scale))
            if c.mat[sy][sx] is None: continue
            X, Y = nx+x, ny+y
            if 0 <= X < 240 and 0 <= Y < 160: _copy(c, c2, sx, sy, X, Y, off)
    c2.part = off + c.part
    XF[name] = (x0, y0, nx, ny, round(scale*4)); return c2, ramps
def render(name):
    if name == 'BEDTOY':                              # the bed with the toy box at its foot (half size), one palette
        c, ramps = placed_canvas('BED'); t, tr = D['TOYBOX'][1], None
        tc = SC(); D['TOYBOX'][0](tc); x0, y0, x1, y1 = _bbox(tc); w = x1-x0+1; h = y1-y0+1
        W = round(w*0.5); H = round(h*0.5); nx = 45-W//2; ny = 125-H+1; off = c.part + 1
        for y in range(H):
            for x in range(W):
                sx = x0+min(w-1, int((x+0.5)/0.5)); sy = y0+min(h-1, int((y+0.5)/0.5))
                if tc.mat[sy][sx] is not None: _copy(tc, c, sx, sy, nx+x, ny+y, off)
        ramps = dict(ramps); ramps.update(D['TOYBOX'][1]); XF['BEDTOY'] = (0, 0, 0, 0, 4)
        return c.rgb(ramps)
    c, ramps = placed_canvas(name)
    return c.rgb(ramps)
if __name__ == '__main__':
    from PIL import Image
    room = Image.open('/tmp/m_room.ppm').convert('RGB') if len(sys.argv) < 2 else None
    names = list(D)
    sets = [['BED', 'PLANT2', 'CLOCK', 'MOBILE'], ['FISHBOWL', 'RADIO', 'BOOKSHELF', 'LANTERN'], ['TOYBOX', 'TV', 'PORTRAIT', 'DISCO'],
            ['BEANBAG', 'ARCADE', 'MIRROR', 'PLANE'], ['PIANO', 'FIREPLACE', 'GUITAR', 'MOBILE'], ['PLANT0', 'PLANT1']]
    out = Image.new('RGB', (240*3, 160*2))
    base = Image.open('/tmp/m_roombg.png').convert('RGB')
    for i, s in enumerate(sets):
        im = base.copy(); p = im.load()
        for n in s:
            img = render(n)
            for y in range(160):
                for x in range(240):
                    if img[y][x] is not None: p[x, y] = img[y][x]
        out.paste(im, ((i % 3)*240, (i//3)*160))
    out.resize((out.width*2, out.height*2), Image.NEAREST).save('/tmp/decor_room.png')
    yb = Image.open('/tmp/m_yardbg.png').convert('RGB')
    out = Image.new('RGB', (240*2, 160*2))
    for i, s in enumerate((['SWING', 'BIRDBATH'], ['SANDBOX', 'GARDEN'], ['TRAMPOLINE', 'POND'], ['SLIDE', 'FLOWERS'])):
        im = yb.copy(); p = im.load()
        for n in s:
            img = render(n)
            for y in range(160):
                for x in range(240):
                    if img[y][x] is not None: p[x, y] = img[y][x]
        out.paste(im, ((i % 2)*240, (i//2)*160))
    out.resize((out.width*2, out.height*2), Image.NEAREST).save('/tmp/decor_yard.png')

# ================================================================== yard toys (small sprites, drawn every frame)
T = {}   # name -> (ramps, [frame fns])
def ball_frame(k):
    def f(c):
        c.ellipse(55, 55, 5.2, 5.2, 'w')
        for yy in range(49, 61):
            for xx in range(49, 61):
                if c.mat[yy][xx] == 'w':
                    a = (math.atan2(yy+0.5-55, xx+0.5-55)+k*math.pi/3) % (2*math.pi)
                    seg = int(a/(math.pi/3)) % 3
                    c.mat[yy][xx] = ('w', 'r', 'b')[seg]
        disc(c, 55, 55, 1.2, 'y', 1)
        c.px(53, 52, (255, 255, 255))
    return f
T['BALL'] = ({'w': ramp((250, 250, 246)), 'r': ramp((236, 80, 90)), 'b': ramp((90, 150, 240)), 'y': ramp((255, 210, 80))}, [ball_frame(k) for k in range(3)])
def kite_frame(k):
    def f(c):
        c.poly([(60, 41), (69, 51), (60, 64), (51, 51)], 'k1', nc=(60, 52, 12, 14))
        for yy in range(40, 65):
            for xx in range(50, 70):
                if c.mat[yy][xx] == 'k1' and ((xx >= 60) != (yy >= 51)): c.mat[yy][xx] = 'k2'
        line(c, 60, 41, 60, 64, (120, 90, 70)); line(c, 51, 51, 69, 51, (120, 90, 70))
        for i, (x, y) in enumerate(((60+k, 66), (59-k, 69))):
            c.poly([(x-2, y-1), (x+2, y+1), (x+2, y-1), (x-2, y+1)], 'k2' if i else 'k1', tone=2)
    return f
T['KITE'] = ({'k1': ramp((236, 84, 96)), 'k2': ramp((255, 210, 80))}, [kite_frame(k) for k in range(2)])
def fris(flying):
    def f(c):
        if flying:
            c.ellipse(55, 56, 5.5, 2.2, 'f')
            for xx in range(50, 61): tp(c, xx, 55, 'f', 1)
            c.ellipse(55, 55.6, 2.6, 0.9, 'f2', shade=False)
        else:
            c.ellipse(55, 57, 5.5, 1.6, 'f'); c.ellipse(55, 56.6, 2.6, 0.7, 'f2', shade=False)
    return f
T['FRISBEE'] = ({'f': ramp((236, 84, 96)), 'f2': ramp((255, 200, 210))}, [fris(1), fris(0)])
def spr(c):
    bevel(c, 50, 57, 10, 6, 'base', 2, r=2)
    rect(c, 52, 59, 6, 1, 'base', 3)
    bevel(c, 53, 54, 4, 3, 'met', 1); c.ellipse(55, 53.6, 1.6, 1.2, 'met')
T['SPRINKLER'] = ({'base': ramp((100, 190, 100)), 'met': ramp((196, 200, 214))}, [spr])
def pin(k):
    def f(c):
        cols = ('p1', 'p2', 'p3', 'p4')
        for i in range(4):
            a = k*math.pi/4 + i*math.pi/2
            p0 = (55, 55); p1 = (55+8*math.cos(a), 55+8*math.sin(a)); p2 = (55+7*math.cos(a+1.15)*0.8, 55+7*math.sin(a+1.15)*0.8)
            c.poly([p0, p1, p2], cols[i], tone=1 if i % 2 else 2)
        disc(c, 55, 55, 1.3, 'pin', 1)
    return f
T['PINWHEEL'] = ({'p1': ramp((236, 84, 96)), 'p2': ramp((255, 210, 80)), 'p3': ramp((100, 170, 240)), 'p4': ramp((250, 150, 190)), 'pin': ramp((250, 250, 250))}, [pin(k) for k in range(2)])
def render_toy(name, k):
    ramps, fr = T[name]; c = SC(); fr[k](c); return c.rgb(ramps)
TOY_ANCHOR = {'BALL': (50, 50), 'KITE': (50, 44), 'FRISBEE': (50, 54), 'SPRINKLER': (50, 54), 'PINWHEEL': (55, 55)}
