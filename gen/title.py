# Phase 4: a painted title screen and a matching "star world" backdrop for the farewells.
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
# Both share one 112-colour palette that is loaded into 144-255 while they are on screen
# (no room is shown then). The last 4 entries twinkle (cycled by the game).
import math, random, sys
from PIL import Image, ImageDraw
sys.path.insert(0, _ROOT + '/gen')
W, H = 240, 160
random.seed(4)
def lerp(a, b, t): return tuple(int(round(a[i]+(b[i]-a[i])*t)) for i in range(3))
def grad(stops, t):
    for i in range(len(stops)-1):
        t0, c0 = stops[i]; t1, c1 = stops[i+1]
        if t <= t1: return lerp(c0, c1, (t-t0)/(t1-t0) if t1 > t0 else 0)
    return stops[-1][1]
BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]
SKY = [(0, (14, 12, 44)), (0.45, (40, 32, 102)), (0.75, (92, 66, 150)), (0.92, (176, 112, 170)), (1.0, (232, 150, 160))]
TW = [(255, 255, 255), (255, 240, 170), (190, 210, 255), (255, 200, 230)]    # twinkle colours (entries cycle)

def sky(img, horizon=112):
    p = img.load()
    for y in range(H):
        for x in range(W):
            t = min(1, y/horizon)
            # band the gradient into 14 steps, dithered between neighbours
            v = t*13; k = int(v); f = v-k
            if f*16 > BAYER[y & 3][x & 3]: k += 1
            p[x, y] = grad(SKY, min(1, k/13))
def stars(img, n, ymax, tw):
    p = img.load(); rnd = random.Random(7)
    out = []
    for i in range(n):
        x = rnd.randrange(2, W-2); y = rnd.randrange(2, ymax)
        c = lerp(p[x, y], (255, 255, 255), 0.55+0.45*rnd.random())
        if i % 9 == 0 and tw:
            out.append((x, y, i % 4))                                      # twinkling: marked, coloured later
        elif i % 7 == 0:
            for dx, dy in ((0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)): p[x+dx, y+dy] = c if (dx, dy) == (0, 0) else lerp(p[x+dx, y+dy], c, 0.5)
        else: p[x, y] = c
    return out
def moon(img, cx, cy, r):
    p = img.load()
    for y in range(cy-r*3, cy+r*3):
        for x in range(cx-r*3, cx+r*3):
            if not (0 <= x < W and 0 <= y < H): continue
            d = math.hypot(x+0.5-cx, y+0.5-cy)
            if d <= r:
                d2 = math.hypot(x+0.5-cx+r*0.55, y+0.5-cy-r*0.35)
                if d2 > r*0.9: p[x, y] = lerp((255, 246, 210), (220, 200, 160), max(0, (x-cx+r)/(2*r))*0.6)
            elif d < r*2.2 and BAYER[y & 3][x & 3] < 16*(1-(d-r)/(r*1.2))*0.3:
                p[x, y] = lerp(p[x, y], (200, 180, 230), 0.35)
def hills(img, layers):
    p = img.load()
    for (cy, amp, freq, ph, col, rim) in layers:
        for x in range(W):
            top = cy + amp*math.sin(x*freq+ph) + amp*0.4*math.sin(x*freq*2.7+ph*1.7)
            for y in range(int(top), H):
                c = col
                if y-top < 1.5: c = rim
                p[x, y] = c
def dome(img, cx, cy, rx, ry, flowers=True):
    p = img.load(); rnd = random.Random(3)
    for y in range(H):
        for x in range(W):
            u = (x+0.5-cx)/rx; v = (y+0.5-cy)/ry
            if u*u+v*v > 1: continue
            # lit from the upper right (the moon), darker toward the bottom
            nx, ny = u, v; lit = 0.55 + 0.35*nx - 0.55*ny
            depth = (y-(cy-ry))/ (H-(cy-ry))
            c = grad([(0, (70, 160, 80)), (0.35, (48, 128, 76)), (1, (28, 82, 70))], depth)
            c = lerp(c, (150, 220, 120), max(0, min(1, lit-0.55))*0.6)
            if u*u+v*v > 0.985: c = lerp(c, (190, 240, 150), 0.6)                     # rim light
            if hash((x*92821) ^ (y*68917)) % 17 == 0 and depth > 0.05: c = lerp(c, (20, 60, 50), 0.35)   # grass texture
            p[x, y] = c
    if flowers:
        for i in range(26):
            x = rnd.randrange(8, 232); y = rnd.randrange(118, 158)
            u = (x-cx)/rx; v = (y-cy)/ry
            if u*u+v*v > 0.92: continue
            if 96 <= x <= 144 and y > 130: continue                                 # keep the menu area calm
            col = rnd.choice([(255, 180, 210), (255, 240, 160), (240, 240, 255), (200, 170, 255)])
            p[x, y] = col; p[x+1, y] = lerp(col, (40, 100, 70), 0.4); p[x, y+1] = (40, 110, 60)
def tufts(img, cx, cy, rx, ry):
    p = img.load(); rnd = random.Random(9)
    for i in range(70):
        x = rnd.randrange(4, 236); y = rnd.randrange(108, 159)
        u = (x-cx)/rx; v = (y-cy)/ry
        if u*u+v*v > 0.95 or (90 <= x <= 150 and y > 128): continue
        base = p[x, y]
        p[x, y] = lerp(base, (20, 60, 50), 0.4); p[x+1, y-1] = lerp(base, (170, 230, 140), 0.45); p[x-1, y-1] = lerp(base, (20, 60, 50), 0.25)
def cottage(img, x0, y0):
    """a little round-roofed critter house with a warm window"""
    d = ImageDraw.Draw(img)
    wall = (236, 206, 170); wall2 = (200, 160, 130); roof = (200, 90, 110); roof2 = (150, 60, 90); ol = (30, 22, 50)
    d.rectangle([x0, y0+10, x0+26, y0+28], fill=ol)
    d.rectangle([x0+1, y0+11, x0+25, y0+27], fill=wall)
    d.rectangle([x0+20, y0+11, x0+25, y0+27], fill=wall2)
    d.polygon([(x0-3, y0+12), (x0+13, y0-2), (x0+29, y0+12)], fill=ol)
    d.polygon([(x0-1, y0+11), (x0+13, y0), (x0+27, y0+11)], fill=roof)
    d.polygon([(x0+13, y0), (x0+27, y0+11), (x0+20, y0+11)], fill=roof2)
    d.rectangle([x0+18, y0-1, x0+21, y0+5], fill=ol); d.rectangle([x0+19, y0, x0+20, y0+5], fill=(150, 110, 100))
    d.rectangle([x0+4, y0+14, x0+11, y0+21], fill=ol); d.rectangle([x0+5, y0+15, x0+10, y0+20], fill=(255, 214, 110))
    d.line([x0+5, y0+17, x0+10, y0+17], fill=(240, 160, 70)); d.line([x0+7, y0+15, x0+7, y0+20], fill=(240, 160, 70))
    d.rectangle([x0+15, y0+17, x0+21, y0+27], fill=ol); d.rectangle([x0+16, y0+18, x0+20, y0+27], fill=(150, 96, 80))
    d.point((x0+19, y0+23), fill=(255, 214, 110))
    p = img.load()   # window glow on the grass
    for y in range(y0+28, y0+33):
        for x in range(x0-2, x0+30):
            if 0 <= x < W and BAYER[y & 3][x & 3] < 6-(y-y0-28): p[x, y] = lerp(p[x, y], (255, 220, 140), 0.35)
    for i, (sx, sy) in enumerate(((x0+21, y0-5), (x0+23, y0-9), (x0+22, y0-14))):    # chimney puffs
        d.ellipse([sx-1-i//2, sy-1-i//2, sx+1+i//2, sy+1+i//2], fill=lerp((200, 190, 230), (90, 70, 150), i/3))


# ---- extra scenery (title fill): trees, bushes, a fence, a lamp post, distant pines, fireflies
LIGHT = (0.55, -0.62)          # the moon is up and to the right
def shade_ramp(stops, d, x, y):
    # 6-step ramp with a narrow dither band, like the critters
    v = max(0.0, min(0.999, d))*(len(stops)-1); k = int(v); f = v-k
    if f > 0.62 or (f > 0.38 and BAYER[y & 3][x & 3]/16 < (f-0.38)/0.24): k += 1
    return stops[min(k, len(stops)-1)]
def blobs(img, circles, ramp, rim, outline=(18, 22, 40)):
    """a clump of overlapping balls (tree canopy, bush), lit from the moon, with a dark outline"""
    p = img.load(); inside = {}
    for (cx, cy, r) in circles:
        for y in range(int(cy-r)-1, int(cy+r)+2):
            for x in range(int(cx-r)-1, int(cx+r)+2):
                if not (0 <= x < W and 0 <= y < H): continue
                u, v = (x+0.5-cx)/r, (y+0.5-cy)/r
                if u*u+v*v <= 1: inside[(x, y)] = (u, v)       # later balls sit in front
    for (x, y), (u, v) in inside.items():
        nz = math.sqrt(max(0, 1-u*u-v*v)); d = 0.25+0.55*(u*LIGHT[0]+v*LIGHT[1])+0.3*nz
        c = shade_ramp(ramp, d, x, y)
        edge = any((x+dx, y+dy) not in inside for dx, dy in ((1, 0), (0, -1), (1, -1)))
        if edge and u > 0.2 and v < 0.3: c = lerp(c, rim, 0.5)
        if hash((x*7919) ^ (y*104729)) % 11 == 0: c = lerp(c, ramp[0], 0.45)      # leafy speckle
        p[x, y] = c
    for (x, y) in inside:
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            q = (x+dx, y+dy)
            if q not in inside and 0 <= q[0] < W and 0 <= q[1] < H: p[q] = lerp(p[q], outline, 0.75)
    return inside
LEAF = [(16, 40, 46), (24, 62, 58), (34, 88, 66), (52, 118, 76), (84, 150, 92), (130, 190, 120)]
LEAF2 = [(20, 44, 58), (30, 66, 72), (40, 92, 84), (62, 124, 96), (96, 160, 112), (150, 204, 140)]
def tree(img, cx, base, h, seed=1, ramp=LEAF):
    d = ImageDraw.Draw(img); rnd = random.Random(seed)
    tw = max(3, h//9)
    d.rectangle([cx-tw//2-1, base-h//2, cx+tw//2+1, base], fill=(20, 16, 34))       # trunk
    d.rectangle([cx-tw//2, base-h//2, cx+tw//2, base], fill=(92, 62, 60))
    d.rectangle([cx+tw//2-1, base-h//2, cx+tw//2, base], fill=(130, 92, 80))
    d.line([cx, base-h//2+4, cx-h//5, base-h//2-2], fill=(70, 48, 52), width=2)          # a branch
    R = h*0.30; cy = base-h+R
    balls = [(cx-R*0.75, cy+R*0.35, R*0.72), (cx+R*0.8, cy+R*0.4, R*0.7), (cx, cy+R*0.55, R*0.75),
             (cx-R*0.45, cy-R*0.25, R*0.78), (cx+R*0.4, cy-R*0.3, R*0.74), (cx+R*0.05, cy-R*0.7, R*0.62)]
    blobs(img, balls, ramp, (190, 230, 200))
    return balls
def bush(img, cx, cy, r, berries=None, seed=2):
    rnd = random.Random(seed)
    balls = [(cx-r*0.7, cy+r*0.15, r*0.62), (cx+r*0.7, cy+r*0.2, r*0.6), (cx, cy-r*0.1, r*0.75)]
    inside = blobs(img, balls, LEAF2, (190, 230, 200))
    p = img.load()
    if berries:
        pts = [k for k in inside if (k[0]+k[1]) % 5 == 0]; rnd.shuffle(pts)
        for (x, y) in pts[:max(3, int(r))]:
            if (x+1, y) in inside and (x, y+1) in inside:
                p[x, y] = berries; p[x+1, y] = lerp(berries, (255, 255, 255), 0.45); p[x, y+1] = lerp(berries, (40, 20, 50), 0.4)
def pine(img, cx, base, h, col=(44, 58, 96), rim=(80, 96, 140)):
    p = img.load()
    for y in range(base-h, base+1):
        w = (y-(base-h))/h*h*0.32 + (1.2 if (y-(base-h)) % 5 >= 3 else 0)
        for x in range(int(cx-w), int(cx+w)+1):
            if 0 <= x < W: p[x, y] = rim if x > cx+w-1.5 else col
def fence(img, x0, x1, y):
    d = ImageDraw.Draw(img)
    for x in range(x0, x1+1, 7):
        d.rectangle([x-1, y-9, x+2, y+1], fill=(24, 20, 40)); d.rectangle([x, y-8, x+1, y], fill=(150, 110, 96))
        d.point((x+1, y-8), fill=(200, 160, 130))
    for yy in (y-6, y-3):
        d.line([x0, yy, x1, yy], fill=(24, 20, 40)); d.line([x0, yy+1, x1, yy+1], fill=(120, 86, 80))
def lamp(img, x, y):
    d = ImageDraw.Draw(img); p = img.load()
    for yy in range(y-30, y+6):
        for xx in range(x-16, x+17):
            if 0 <= xx < W and 0 <= yy < H:
                r = math.hypot(xx-x, (yy-(y-20))*1.2)
                if r < 15 and BAYER[yy & 3][xx & 3] < 10*(1-r/15): p[xx, yy] = lerp(p[xx, yy], (255, 214, 140), 0.35)
    d.rectangle([x-1, y-18, x+1, y], fill=(24, 20, 40)); d.line([x, y-18, x, y], fill=(90, 80, 110))
    d.rectangle([x-3, y-25, x+3, y-18], fill=(24, 20, 40)); d.rectangle([x-2, y-24, x+2, y-19], fill=(255, 220, 130))
    d.point((x-1, y-23), fill=(255, 250, 220)); d.line([x-4, y-26, x+4, y-26], fill=(24, 20, 40))
def mushrooms(img, pts):
    d = ImageDraw.Draw(img)
    for (x, y, c) in pts:
        d.rectangle([x, y-2, x+1, y], fill=(230, 220, 200)); d.ellipse([x-2, y-5, x+3, y-1], fill=(24, 20, 40))
        d.ellipse([x-1, y-4, x+2, y-2], fill=c); d.point((x, y-4), fill=(255, 240, 230))

# ---- the logo: a rounded bubble font drawn from strokes
STROKES = {
 'C': [[(4, 1), (3, 0), (1, 0), (0, 1), (0, 5), (1, 6), (3, 6), (4, 5)]],
 'R': [[(0, 6), (0, 0), (3, 0), (4, 1), (4, 2), (3, 3), (0, 3)], [(2.2, 3), (4, 6)]],
 'I': [[(2, 0), (2, 6)], [(0.8, 0), (3.2, 0)], [(0.8, 6), (3.2, 6)]],
 'T': [[(0, 0), (4, 0)], [(2, 0), (2, 6)]],
 'E': [[(4, 0), (0, 0), (0, 6), (4, 6)], [(0, 3), (3, 3)]],
 'K': [[(0, 0), (0, 6)], [(4, 0), (0.6, 3.4)], [(1.6, 2.6), (4, 6)]],
 'P': [[(0, 6), (0, 0), (3, 0), (4, 1), (4, 2), (3, 3), (0, 3)]],
 # v8 final: STARFALL MEADOW
 'S': [[(4, 1), (3, 0), (1, 0), (0, 1), (0, 2), (1, 3), (3, 3), (4, 4), (4, 5), (3, 6), (1, 6), (0, 5)]],
 'A': [[(0, 6), (0, 1.6), (1.3, 0), (2.7, 0), (4, 1.6), (4, 6)], [(0, 3.4), (4, 3.4)]],
 'F': [[(4, 0), (0, 0), (0, 6)], [(0, 3), (3, 3)]],
 'L': [[(0, 0), (0, 6), (4, 6)]],
 'M': [[(0, 6), (0, 0), (2.5, 3.2), (5, 0), (5, 6)]],
 'D': [[(0, 0), (0, 6), (2.4, 6), (4, 4.4), (4, 1.6), (2.4, 0), (0, 0)]],
 'O': [[(1.2, 0), (2.8, 0), (4, 1.2), (4, 4.8), (2.8, 6), (1.2, 6), (0, 4.8), (0, 1.2), (1.2, 0)]],
 'W': [[(0, 0), (0.9, 6), (2.5, 2.8), (4.1, 6), (5, 0)]],
}
CW = {'M': 5, 'W': 5}   # letters wider than the usual 4 units
def word_mask(word, s, t, gap):
    SS = 4; w = int((sum(CW.get(c, 4)*s+t for c in word)+(len(word)-1)*gap+2*t)*SS); h = int((6*s+2*t)*SS)
    m = Image.new('L', (w, h), 0); d = ImageDraw.Draw(m)
    x = t
    for ch in word:
        for st in STROKES[ch]:
            pts = [((x+px*s)*SS, (t+py*s)*SS) for px, py in st]
            d.line(pts, fill=255, width=int(t*SS), joint='curve')
            for q in pts: d.ellipse([q[0]-t*SS/2, q[1]-t*SS/2, q[0]+t*SS/2, q[1]+t*SS/2], fill=255)
        x += CW.get(ch, 4)*s+t+gap
    m = m.resize((w//SS, h//SS), Image.LANCZOS)
    return [[m.getpixel((xx, yy)) >= 128 for xx in range(m.width)] for yy in range(m.height)]
def put_word(img, word, cx, y, s, t, gap, top, bot, shine):
    mk = word_mask(word, s, t, gap); h = len(mk); w = len(mk[0]); x0 = cx-w//2
    p = img.load(); ol = (24, 18, 56); sh = (40, 26, 80)
    inside = lambda x, y: 0 <= y < h and 0 <= x < w and mk[y][x]
    def near(x, y, r):
        return any(inside(x+dx, y+dy) for dx in range(-r, r+1) for dy in range(-r, r+1) if dx*dx+dy*dy <= r*r+1)
    for yy in range(-3, h+5):                                  # drop shadow, then a 2px outline
        for xx in range(-3, w+5):
            X, Y = x0+xx, y+yy
            if not (0 <= X < W and 0 <= Y < H): continue
            if near(xx-2, yy-3, 2) and not near(xx, yy, 2): p[X, Y] = sh
    for yy in range(-2, h+2):
        for xx in range(-2, w+2):
            X, Y = x0+xx, y+yy
            if not (0 <= X < W and 0 <= Y < H): continue
            if inside(xx, yy):
                c = lerp(top, bot, yy/(h-1))
                if not inside(xx, yy-1) or not inside(xx, yy-2): c = lerp(c, shine, 0.7)        # glossy top edge
                elif not inside(xx, yy+1): c = lerp(c, (0, 0, 0), 0.25)
                p[X, Y] = c
            elif near(xx, yy, 2): p[X, Y] = ol

def title_image():
    img = Image.new('RGB', (W, H)); sky(img)
    tw = stars(img, 120, 104, True)
    tw += [(x, y, k) for (x, y, k) in ((16, 98, 1), (54, 110, 2), (88, 104, 3), (190, 120, 0), (150, 100, 2), (226, 132, 1))]   # fireflies
    moon(img, 224, 54, 8)
    hills(img, [(110, 3, 0.045, 0.6, (58, 48, 110), (96, 76, 150)), (116, 2.5, 0.07, 2.0, (42, 70, 92), (70, 110, 120))])
    for (x, b, h) in ((62, 112, 13), (70, 113, 9), (156, 113, 11), (166, 112, 15), (176, 114, 8)): pine(img, x, b, h)
    dome(img, 120, 232, 196, 116)
    tree(img, 30, 124, 58, 1)                     # big tree on the left
    tree(img, 228, 110, 30, 3, LEAF2)             # small one behind the house
    cottage(img, 196, 98)
    fence(img, 170, 194, 126)
    bush(img, 62, 126, 9, (236, 90, 120), 4)
    bush(img, 176, 136, 8, (130, 170, 255), 5)
    bush(img, 8, 140, 10, None, 6)
    lamp(img, 74, 122)
    mushrooms(img, ((48, 134, (236, 90, 80)), (53, 136, (236, 90, 80)), (212, 140, (200, 140, 255))))
    tufts(img, 120, 232, 196, 116)
    put_word(img, 'STARFALL', 120, 6, 3.0, 5.0, 2.0, (255, 232, 120), (248, 150, 50), (255, 255, 230))
    put_word(img, 'MEADOW', 120, 34, 3.0, 5.0, 2.0, (255, 255, 255), (214, 196, 246), (255, 255, 255))
    return img, tw
def starworld_image():
    img = Image.new('RGB', (W, H)); sky(img, 150)
    tw = stars(img, 150, 120, True)
    p = img.load()   # the critter's home planet: a round grassy hill
    for y in range(H):
        for x in range(W):
            u = (x+0.5-120)/70; v = (y+0.5-150)/70
            if u*u+v*v <= 1:
                lit = 0.5+0.4*u-0.5*v
                c = lerp((40, 110, 80), (120, 200, 120), max(0, min(1, lit-0.3)))
                if u*u+v*v > 0.94: c = lerp(c, (190, 240, 160), 0.55)
                if hash((x*92821) ^ (y*68917)) % 15 == 0: c = lerp(c, (20, 60, 50), 0.35)
                p[x, y] = c
            elif u*u+v*v <= 1.18 and BAYER[y & 3][x & 3] < 3: p[x, y] = lerp(p[x, y], (190, 240, 200), 0.25)
    return img, tw

def build():
    timg, ttw = title_image(); simg, stw = starworld_image()
    # palette: 108 colours by k-means over both pictures, + 4 twinkle entries at the end
    import numpy as np
    px = list(timg.get_flattened_data())+list(simg.get_flattened_data())
    hist = {}
    for c in px: hist[c] = hist.get(c, 0)+1
    ks = list(hist); X = np.array(ks, float); Wt = np.array([hist[k] for k in ks], float)
    K = 108; C = [X[Wt.argmax()]]
    for _ in range(K-1):
        d = np.min(((X[:, None, :]-np.array(C)[None])**2).sum(-1), 1)*np.sqrt(Wt); C.append(X[d.argmax()])
    C = np.array(C)
    for it in range(25):
        a = ((X[:, None, :]-C[None])**2).sum(-1).argmin(1)
        for j in range(K):
            m = a == j
            if m.any(): C[j] = (X[m]*Wt[m, None]).sum(0)/Wt[m].sum()
    lut = {k: 144+int(a[i]) for i, k in enumerate(ks)}
    def idx(img, tw):
        b = bytearray(lut[c] for c in img.get_flattened_data())
        for (x, y, k) in tw:
            b[y*W+x] = 144+108+k
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                if 0 <= x+dx < W and 0 <= y+dy < H and (k & 1): b[(y+dy)*W+x+dx] = 144+108+((k+2) & 3)
        return b
    tb = idx(timg, ttw); sb = idx(simg, stw)
    pal = [tuple(int(v) for v in c) for c in C]
    bgr = lambda c: (c[0] >> 3) | ((c[1] >> 3) << 5) | ((c[2] >> 3) << 10)
    o = ['// generated by gen/title.py\n#define TBG_FIRST 144\n#define TBG_N 112\n#define TBG_TWINKLE 108\n']
    o.append('static const u16 TBG_PAL[108]={%s};\n' % ','.join(str(bgr(c)) for c in pal))
    o.append('static const u16 TBG_TW[4]={%s};\n' % ','.join(str(bgr(c)) for c in TW))
    o.append('static const u8 TITLE_BG[38400] __attribute__((aligned(4)))={%s};\n' % ','.join(map(str, tb)))
    o.append('static const u8 STARWORLD_BG[38400] __attribute__((aligned(4)))={%s};\n' % ','.join(map(str, sb)))
    open(_ROOT + '/title.h', 'w').write(''.join(o))
    # previews
    def show(b):
        im = Image.new('RGB', (W, H)); q = im.load()
        for i, v in enumerate(b):
            c = pal[v-144] if v-144 < 108 else TW[v-144-108]; q[i % W, i//W] = c
        return im
    pv = Image.new('RGB', (W*2+4, H)); pv.paste(show(tb), (0, 0)); pv.paste(show(sb), (W+4, 0))
    pv.resize((pv.width*3, pv.height*3), Image.NEAREST).save('/tmp/title_prev.png')
if __name__ == '__main__': build()
