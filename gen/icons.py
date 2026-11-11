# Phase 3: shop icons (up to 32x32) for all 44 items, plus 20px food art for the eating animation.
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
import math, sys
sys.path.insert(0, _ROOT + '/gen')
from critters import Canvas, ramp, mix, OUTLINE
import decor

F = {}
def food(name, ramps):
    def d(fn): F[name] = (fn, ramps); return fn
    return d
W = (255, 255, 255)

@food('APPLE', {'r': ramp((232, 64, 72)), 'leaf': ramp((100, 190, 90)), 'stem': ramp((120, 80, 50))})
def APPLE(c):
    c.ellipse(24, 28, 15, 14, 'r', wob=lambda a: 0.06*math.cos(2*a))
    c.ellipse(24, 15.5, 3, 1.6, 'r', bias=-0.3)
    c.line(24, 16, 25.5, 9, (110, 70, 40), 1.6)
    c.ellipse(30, 11, 5, 2.4, 'leaf', rot=-0.4)
    c.ellipse(17, 21, 2.6, 3.6, 'r', bias=0.6, rot=0.5)
@food('FISH', {'b': ramp((110, 170, 230)), 'bel': ramp((226, 240, 250)), 'fin': ramp((80, 130, 210))})
def FISH(c):
    c.poly([(36, 24), (46, 15), (44, 24), (46, 33)], 'fin')
    c.ellipse(22, 24, 16, 10, 'b')
    c.ellipse(22, 28, 12, 4.5, 'bel', shade=False)
    c.poly([(18, 15), (28, 11), (26, 17)], 'fin')
    for x in (20, 26, 32): c.line(x, 18, x-2, 30, mix((80, 130, 210), OUTLINE, 0.2))
    c.ellipse(11.5, 22, 2.4, 2.4, 'bel', shade=False); c.dot(11.5, 22, OUTLINE)
    c.line(6.5, 26, 9, 27, OUTLINE)
@food('CAKE', {'sp': ramp((250, 226, 170)), 'cr': ramp((255, 250, 244)), 'st': ramp((236, 70, 90)), 'leaf': ramp((100, 190, 90))})
def CAKE(c):
    c.poly([(6, 22), (40, 14), (42, 38), (8, 42)], 'sp', nc=(24, 28, 20, 16))
    for y0 in (28, 35):
        for x in range(6, 43):
            y = y0 + (x-6)*(-6/36)
            c.ellipse(x, y, 0.8, 1.2, 'cr', shade=False)
    c.poly([(6, 22), (40, 14), (42, 19), (8, 26)], 'cr', tone=1)
    c.ellipse(27, 13, 5, 4.6, 'st'); c.ellipse(27, 9, 2.6, 1.2, 'leaf')
    for (x, y) in ((25, 12), (29, 14), (27, 15)): c.dot(x, y, (255, 230, 150))
@food('CANDY', {'w': ramp((250, 140, 190)), 's': ramp((255, 255, 255))})
def CANDY(c):
    c.poly([(4, 14), (14, 22), (14, 26), (4, 34), (8, 24)], 'w'); c.poly([(44, 14), (34, 22), (34, 26), (44, 34), (40, 24)], 'w')
    c.ellipse(24, 24, 11, 9, 'w')
    for k in range(-2, 3):
        for t in range(-10, 11):
            x = 24+t*0.7+k*5; y = 24+t*0.6
            if ((x-24)/11)**2+((y-24)/9)**2 <= 0.85: c.ellipse(x, y, 1.0, 1.0, 's', shade=False)
@food('SALAD', {'bowl': ramp((90, 150, 230)), 'g': ramp((110, 200, 90)), 'g2': ramp((70, 160, 70)), 't': ramp((236, 70, 70))})
def SALAD(c):
    for (x, y, r) in ((14, 22, 7), (22, 18, 8), (31, 19, 7.5), (37, 24, 6), (11, 26, 5)): c.ellipse(x, y, r, r*0.8, 'g' if x % 2 else 'g2', wob=lambda a: 0.12*math.sin(a*6))
    c.ellipse(19, 22, 3.4, 3.4, 't'); c.ellipse(30, 23, 3, 3, 't')
    def ins(u, v): return v >= 26 and ((u-24)/20)**2+((v-26)/15)**2 <= 1
    c.shape(ins, (3, 25, 45, 42), 'bowl', (24, 26, 20, 16))
    c.line(5, 26, 43, 26, mix((90, 150, 230), W, 0.6))
@food('PIZZA', {'ch': ramp((255, 210, 90)), 'cr': ramp((220, 150, 80)), 'pep': ramp((210, 60, 60)), 'leaf': ramp((90, 170, 80))})
def PIZZA(c):
    c.poly([(5, 10), (43, 10), (24, 44)], 'ch', nc=(24, 22, 20, 18))
    c.poly([(4, 6), (44, 6), (43, 12), (5, 12)], 'cr', nc=(24, 9, 22, 6))
    for (x, y) in ((16, 17), (29, 19), (23, 29), (33, 14)): c.ellipse(x, y, 3.2, 3.2, 'pep')
    c.ellipse(20, 23, 1.8, 1.0, 'leaf', rot=0.5)
    for (x, y) in ((12, 22), (31, 26)): c.line(x, y, x, y+3, (255, 230, 150))
@food('MILK', {'box': ramp((250, 250, 250)), 'lab': ramp((90, 150, 230)), 'top': ramp((230, 230, 240))})
def MILK(c):
    c.poly([(12, 16), (24, 6), (36, 16)], 'top', tone=1)
    c.poly([(16, 6), (32, 6), (24, 4)], 'top', tone=2)
    c.poly([(12, 16), (36, 16), (36, 44), (12, 44)], 'box', nc=(24, 30, 16, 18))
    c.poly([(12, 24), (36, 24), (36, 36), (12, 36)], 'lab', nc=(24, 30, 16, 10))
    c.ellipse(24, 30, 6, 3.6, 'box', shade=False)
    c.line(24, 6, 24, 16, mix((230, 230, 240), OUTLINE, 0.4))
@food('COOKIE', {'ck': ramp((222, 168, 100)), 'ch': ramp((100, 66, 50))})
def COOKIE(c):
    c.ellipse(24, 25, 17, 16, 'ck', wob=lambda a: 0.04*math.sin(a*7))
    for (x, y) in ((17, 18), (29, 16), (24, 26), (14, 30), (32, 31), (22, 35), (35, 22)): c.ellipse(x, y, 2.4, 2, 'ch')
    for (x, y) in ((20, 22), (28, 30), (19, 32)): c.dot(x, y, (190, 130, 70))
@food('VITAMIN', {'a': ramp((240, 90, 100)), 'b': ramp((250, 244, 230)), 'sp': ramp((255, 220, 90))})
def VITAMIN(c):
    def cap(u, v, half):
        x, y = u-24, v-26; a = -0.75
        X = x*math.cos(a)-y*math.sin(a); Y = x*math.sin(a)+y*math.cos(a)
        if abs(Y) > 8: return False
        if abs(X) > 16+math.sqrt(max(0, 64-Y*Y))-8: return False
        return (X < 0) == half
    c.shape(lambda u, v: cap(u, v, True), (4, 4, 44, 46), 'a', (24, 26, 18, 18))
    c.shape(lambda u, v: cap(u, v, False), (4, 4, 44, 46), 'b', (24, 26, 18, 18))
    for (x, y, r) in ((38, 10, 2.6), (8, 38, 2)):
        c.poly([(x, y-r*2), (x+r*0.6, y), (x, y+r*2), (x-r*0.6, y)], 'sp', tone=1); c.poly([(x-r*2, y), (x, y+r*0.6), (x+r*2, y), (x, y-r*0.6)], 'sp', tone=1)
@food('SOAP', {'s': ramp((250, 180, 210)), 'bub': ramp((210, 236, 255))})
def SOAP(c):
    def ins(u, v): return abs(u-22) <= 15 and abs(v-31) <= 9 and ((max(0, abs(u-22)-11))**2+(max(0, abs(v-31)-5))**2) <= 16
    c.shape(ins, (6, 21, 38, 41), 's', (22, 28, 18, 12))
    c.poly([(9, 27), (35, 27), (37, 30), (7, 30)], 's', tone=1)
    for (x, y, r) in ((34, 14, 6), (24, 10, 4), (41, 25, 3.5), (14, 16, 3)):
        c.ellipse(x, y, r, r, 'bub', shade=False)
        for yy in range(int((y-r)*c.k), int((y+r)*c.k)+1):
            for xx in range(int((x-r)*c.k), int((x+r)*c.k)+1):
                if 0 <= yy < c.N and 0 <= xx < c.N and c.mat[yy][xx] == 'bub':
                    d = math.hypot(xx/c.k-x, yy/c.k-y)/r
                    c.tone[yy][xx] = 0 if d > 0.82 else 1
        c.dot(x-r*0.4, y-r*0.4, W)
@food('GIFT', {'box': ramp((110, 170, 240)), 'rib': ramp((236, 80, 100))})
def GIFT(c):
    c.poly([(8, 20), (40, 20), (40, 44), (8, 44)], 'box', nc=(24, 32, 18, 14))
    c.poly([(6, 15), (42, 15), (42, 22), (6, 22)], 'box', tone=1)
    c.poly([(21, 15), (27, 15), (27, 44), (21, 44)], 'rib', nc=(24, 30, 6, 16))
    c.ellipse(17, 11, 7, 4.5, 'rib', rot=0.4); c.ellipse(31, 11, 7, 4.5, 'rib', rot=-0.4)
    c.ellipse(24, 14, 3, 3, 'rib', bias=-0.3)
@food('BUBBLES', {'w': ramp((250, 150, 190)), 'bub': ramp((210, 236, 255))})
def BUBBLES(c):
    c.line(10, 44, 22, 24, (250, 150, 190), 2.4)
    c.ellipse(26, 18, 7, 7, 'w')
    c.ellipse(26, 18, 4.4, 4.4, 'bub', shade=False)
    for (x, y, r) in ((38, 10, 5), (40, 26, 3.4), (15, 9, 3)):
        c.ellipse(x, y, r, r, 'bub', shade=False); c.dot(x-r*0.4, y-r*0.4, W)

# v8 phase 5 fixes: the FEED icon's MEAL and SNACK, drawn in the same style as the bag foods
def _rtri(u, v, r=6.5):   # a rounded triangle (rice ball)
    A, B, C = (24, 7), (5, 41), (43, 41)
    def side(p, q): return (q[0]-p[0])*(v-p[1])-(q[1]-p[1])*(u-p[0])
    def dist(p, q):
        L = math.hypot(q[0]-p[0], q[1]-p[1]); return -side(p, q)/L
    # signed distance to each edge (positive inside); inside the rounded shape if inside the inset triangle grown by r
    d = [dist(A, B), dist(B, C), dist(C, A)]
    if min(d) >= r: return True
    if min(d) < 0: return False
    # near a corner: within r of the inset triangle
    import itertools
    best = 1e9
    ins = [(24, 7+r*2.1), (5+r*1.75, 41-r), (43-r*1.75, 41-r)]
    for (x0, y0), (x1, y1) in ((ins[0], ins[1]), (ins[1], ins[2]), (ins[2], ins[0])):
        dx, dy = x1-x0, y1-y0; t = max(0, min(1, ((u-x0)*dx+(v-y0)*dy)/(dx*dx+dy*dy)))
        best = min(best, math.hypot(u-(x0+t*dx), v-(y0+t*dy)))
    return best <= r or min(d) >= r
@food('MEAL', {'rice': ramp((250, 250, 244)), 'nori': ramp((50, 74, 62)), 'ume': ramp((226, 70, 90))})
def MEAL(c):
    c.shape(lambda u, v: _rtri(u, v), (3, 5, 45, 43), 'rice', (24, 26, 18, 18))
    c.shape(lambda u, v: _rtri(u, v) and 17 <= u <= 31 and v >= 31, (3, 5, 45, 43), 'nori', (24, 37, 7, 6))
    c.ellipse(24, 15.5, 3, 2.4, 'ume')
    for (x, y) in ((14, 30), (32, 22), (19, 21), (29, 30), (36, 34), (11, 36)): c.line(x, y, x+1.6, y+0.8, (226, 226, 214))
    for (x, y) in ((20, 34), (26, 38), (29, 33), (22, 40)): c.dot(x, y, (232, 220, 170))
@food('SNACK', {'cup': ramp((240, 120, 150)), 'cr': ramp((255, 236, 244)), 'ch': ramp((222, 40, 60)), 'cake': ramp((236, 180, 110))})
def SNACK(c):
    c.poly([(9, 28), (39, 28), (35, 45), (13, 45)], 'cup', nc=(24, 36, 16, 10))
    for x in (14, 19, 24, 29, 34): c.line(x, 29, x-(x-24)*0.15, 44, mix((240, 120, 150), OUTLINE, 0.35))
    c.ellipse(24, 27, 16, 5, 'cake', shade=False)
    c.ellipse(24, 24, 15, 6, 'cr'); c.ellipse(24, 18.5, 11, 5, 'cr'); c.ellipse(24, 13.5, 7, 4, 'cr')
    c.line(18, 24, 30, 22, mix((255, 236, 244), OUTLINE, 0.18)); c.line(20, 18.5, 28, 17, mix((255, 236, 244), OUTLINE, 0.18))
    for (x, y, col) in ((14, 23, (90, 170, 240)), (31, 25, (250, 210, 70)), (20, 19, (110, 200, 110)), (28, 15, (240, 110, 160)), (34, 21, (250, 210, 70)), (17, 26, (240, 110, 160))):
        c.line(x, y, x+1.4, y-0.8, col)
    c.line(24, 9, 27, 3.5, (110, 70, 40), 1.2)
    c.ellipse(24, 8.5, 3.6, 3.4, 'ch'); c.dot(22.8, 7.4, (255, 220, 220))

def render_food(name, size):
    import volume; volume.Canvas3.MODE = 'soft'; volume.Canvas3.LINES = True
    fn, ramps = F[name]; c = volume.Canvas3(size/48); fn(c); return c.rgb(ramps, ground=False)

def crop(img):
    pts = [(x, y) for y in range(len(img)) for x in range(len(img[0])) if img[y][x] is not None]
    x0 = min(p[0] for p in pts); x1 = max(p[0] for p in pts); y0 = min(p[1] for p in pts); y1 = max(p[1] for p in pts)
    return [row[x0:x1+1] for row in img[y0:y1+1]]
def upscale(img, k): return [[v for v in row for _ in range(k)] for row in img for _ in range(k)]
def downscale(img, f):
    """area-average shrink of pixel art (transparent where under half covered)"""
    h = len(img); w = len(img[0]); W2 = max(1, round(w*f)); H2 = max(1, round(h*f)); out = []
    for Y in range(H2):
        row = []
        for X in range(W2):
            xa, xb = X/f, (X+1)/f; ya, yb = Y/f, (Y+1)/f
            acc = [0, 0, 0]; cov = 0; tot = 0; dark = 0
            for y in range(int(ya), min(h, int(math.ceil(yb)))):
                for x in range(int(xa), min(w, int(math.ceil(xb)))):
                    a = (min(xb, x+1)-max(xa, x))*(min(yb, y+1)-max(ya, y)); tot += a
                    v = img[y][x]
                    if v is not None:
                        cov += a; acc = [acc[i]+v[i]*a for i in range(3)]
                        if sum(v) < 200: dark += a
            if cov < tot*0.45: row.append(None)
            elif dark > cov*0.5: row.append(OUTLINE)     # keep outlines crisp
            else: row.append(tuple(int(acc[i]/cov) for i in range(3)))
        out.append(row)
    return out

TOYMAP = {'BALL': ('BALL', 0, 2), 'KITE': ('KITE', 0, 1), 'FRISBEE': ('FRISBEE', 0, 2), 'SPRINKLER': ('SPRINKLER', 0, 2), 'PINWHEEL': ('PINWHEEL', 0, 2)}
ITEM_ORDER = ['APPLE', 'FISH', 'CAKE', 'CANDY', 'SALAD', 'PIZZA', 'MILK', 'COOKIE', 'VITAMIN', 'SOAP', 'GIFT', 'BALL', 'KITE', 'BUBBLES',
              'BED', 'FISHBOWL', 'PLANT1', 'RADIO', 'CLOCK', 'BOOKSHELF', 'TOYBOX', 'TV', 'PORTRAIT', 'MOBILE', 'LANTERN',
              'SWING', 'SANDBOX', 'BIRDBATH', 'GARDEN', 'FRISBEE', 'SPRINKLER', 'PINWHEEL',
              'BEANBAG', 'PIANO', 'ARCADE', 'FIREPLACE', 'MIRROR', 'GUITAR', 'DISCO', 'PLANE', 'TRAMPOLINE', 'SLIDE', 'POND', 'FLOWERS']
def icon(name, MAX=32):
    if name in F: return crop(render_food(name, 32))
    if name in TOYMAP:
        t, k, s = TOYMAP[name]; return upscale(crop(decor.render_toy(t, k)), s)
    if name == 'MOBILE': return crop(mobile_icon())
    if name == 'PLANE': return crop(plane_icon())
    fn, ramps = decor.D[name]; c = decor.SC(); fn(c); extra = dict(ramps); EXTRA.get(name, lambda c, r: None)(c, extra)
    img = crop(c.rgb(extra)); h = len(img); w = len(img[0])
    if max(w, h) > MAX: img = downscale(img, MAX/max(w, h))
    return img
def _swing(c, r):
    for k in range(38): c.px(36, 63+k, (150, 100, 70)); c.px(56, 63+k, (150, 100, 70))
    decor.bevel(c, 32, 101, 30, 3, 'seat', 2); r['seat'] = decor.WOOD
def _tv(c, r):
    r['scr'] = ramp((130, 200, 250)); r['sun'] = ramp((255, 210, 80)); r['gr'] = ramp((100, 190, 90))
    decor.rect(c, 200, 97, 26, 19, 'scr', 1); decor.rect(c, 200, 111, 26, 5, 'gr', 2); decor.disc(c, 219, 102, 3, 'sun', 1)
def _arcade(c, r):
    r['scr'] = ramp((60, 70, 170)); r['inv'] = ramp((120, 230, 170))
    decor.rect(c, 208, 84, 22, 18, 'scr', 2)
    for x in (211, 218, 225):
        for (dx, dy) in ((0, 0), (2, 0), (1, 1), (0, 2), (1, 2), (2, 2)): decor.tp(c, x+dx, 88+dy, 'inv', 1)
    decor.rect(c, 217, 98, 4, 2, 'inv', 2)
def _fire(c, r):
    r['fl'] = ramp((255, 150, 60)); r['fl2'] = ramp((255, 220, 90))
    for (x, h) in ((211, 12), (216, 15), (221, 11)):
        c.ellipse(x+1, 119-h/2, 2.8, h/2, 'fl', shade=False); c.ellipse(x+1, 119-h/3, 1.4, h/3, 'fl2', shade=False)
def _portrait(c, r):
    r['ht'] = ramp((236, 90, 120))
    c.ellipse(139, 47, 4, 4, 'ht'); c.ellipse(145, 47, 4, 4, 'ht'); c.poly([(135.2, 48.5), (148.8, 48.5), (142, 56)], 'ht', tone=2)
EXTRA = {'SWING': _swing, 'TV': _tv, 'ARCADE': _arcade, 'FIREPLACE': _fire, 'PORTRAIT': _portrait}
def mobile_icon():
    c = decor.SC(); decor.MOBILE(c)
    for (x, L) in ((89, 10), (101, 16), (113, 8)):
        for y in range(36, 36+L): c.px(x, y, OUTLINE)
        sy = 36+L+2
        c.poly([(x, sy-3), (x+1, sy-1), (x+3, sy-1), (x+1.5, sy+0.5), (x+2.2, sy+3), (x, sy+1.5), (x-2.2, sy+3), (x-1.5, sy+0.5), (x-3, sy-1), (x-1, sy-1)], 'g', tone=1)
    return c.rgb({'wood': decor.WOOD, 'g': ramp((255, 210, 80))})
def plane_icon():
    c = decor.SC()
    c.ellipse(60, 60, 12, 3.6, 'p')
    c.poly([(54, 57), (50, 50), (53, 50), (58, 57)], 'p', tone=2)
    c.poly([(58, 60), (64, 68), (67, 68), (64, 60)], 'p', tone=3)
    c.ellipse(70, 59, 2, 1.4, 'win', shade=False)
    for x in (62, 65): c.px(x, 59, (180, 220, 250))
    c.poly([(72, 55), (73, 55), (73, 65), (72, 65)], 'prop', tone=1)
    return c.rgb({'p': ramp((236, 84, 96)), 'win': ramp((180, 220, 250)), 'prop': ramp((200, 200, 210))})

if __name__ == '__main__':
    from PIL import Image
    im = Image.new('RGB', (11*40, 4*40), (240, 228, 200))
    for i, n in enumerate(ITEM_ORDER):
        img = icon(n); h = len(img); w = len(img[0]); ox = (i % 11)*40+20-w//2; oy = (i//11)*40+20-h//2
        for y in range(h):
            for x in range(w):
                if img[y][x] is not None: im.putpixel((ox+x, oy+y), img[y][x])
    im.resize((im.width*3, im.height*3), Image.NEAREST).save('/tmp/icons.png')
