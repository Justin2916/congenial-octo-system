# v8.2c: the runaway's new scenes -> ../meadow.h
#   MD_BG_WALK   the meadow at morning: hills, a lone tree, a winding path, wildflowers (240 x 106, screen rows 28-133)
#   MD_BG_STREAM a stream through the meadow: the far bank's trees, an old willow with a burrow (the wild critters'
#                home), reeds and stones on the near bank (240 x 106)
#   WC_*         the wild critters of the meadow: a parent and two little ones, two frames each (idle, wave)
# All share one palette (MD_N colours from palette index MD_FIRST), loaded while those scenes show.
# Run from the repo root or gen/: python3 gen/meadow.py [preview.png]
import math, os, random, sys
import numpy as np
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
W, H = 240, 106
MD_FIRST, MD_N = 144, 96
BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0 - 0.47

def lerp(a, b, t): return a + (b - a) * t
def mix(c0, c1, t): return np.array(c0, float) * (1 - t) + np.array(c1, float) * t

class Canvas:
    def __init__(s, w=W, h=H): s.w, s.h = w, h; s.px = np.zeros((h, w, 3)); s.yy, s.xx = np.mgrid[0:h, 0:w]
    def put(s, mask, col, alpha=1.0):
        col = np.asarray(col, float)
        if col.ndim == 1: col = np.broadcast_to(col, s.px.shape)
        a = np.where(mask, alpha, 0.0)[..., None]; s.px = s.px * (1 - a) + col * a
    def dither_gradient(s, mask, c0, c1, t, steps):
        """a gradient c0->c1 along t (0..1), cut into 'steps' bands with a Bayer dither between them, like pixel art"""
        b = BAYER[s.yy % 4, s.xx % 4]
        q = np.clip(np.floor(t * (steps - 1) + 0.5 + b * 0.9), 0, steps - 1) / (steps - 1)
        col = np.array(c0, float)[None, None] * (1 - q[..., None]) + np.array(c1, float)[None, None] * q[..., None]
        s.put(mask, col)

def ridge(x, base, amp, seeds):
    y = np.full_like(x, base, dtype=float)
    for k, (f, ph, a) in enumerate(seeds): y += amp * a * np.sin(x * f + ph)
    return y

def blob_tree(cv, cx, cy, r, leaf, light, dark, trunk, rng, shadow_side=1):
    """a round, clumpy tree canopy with light from the top-left, and a trunk"""
    tw = max(2, r // 4); cv.put((abs(cv.xx - cx) <= tw // 2) & (cv.yy >= cy) & (cv.yy <= cy + int(r * 1.3)), trunk)
    cv.put((cv.xx == cx + tw // 2) & (cv.yy >= cy) & (cv.yy <= cy + int(r * 1.3)), np.array(trunk) * 0.7)
    clumps = [(cx + rng.uniform(-r * .7, r * .7), cy - r * .3 + rng.uniform(-r * .6, r * .5), r * rng.uniform(.45, .7)) for _ in range(9)]
    clumps.sort(key=lambda c: c[1])
    for (x, y, rr) in clumps:
        d = ((cv.xx - x) ** 2 + (cv.yy - y) ** 2) / rr ** 2; m = d <= 1
        lit = (-(cv.xx - x) * .6 - (cv.yy - y) * .8) / rr
        col = np.where((lit > .35)[..., None], np.array(light, float), np.where((lit < -.35)[..., None], np.array(dark, float), np.array(leaf, float)))
        cv.put(m, col)
    # a few bright leaf flecks
    for _ in range(int(r * 1.2)):
        x = int(cx + rng.uniform(-r, r * .3)); y = int(cy - r * .2 + rng.uniform(-r, 0))
        if 0 <= x < cv.w and 0 <= y < cv.h and cv.px[y, x].sum() > 0 and abs(cv.px[y, x] - np.array(leaf)).sum() < 60: cv.px[y, x] = light

def cloud(cv, cx, cy, w, top, body, shade):
    for k, (dx, dy, r) in enumerate(((-.3, .1, .32), (0, -.1, .42), (.32, .08, .3), (.15, .18, .3), (-.12, .2, .3))):
        x, y, rr = cx + dx * w, cy + dy * w * .6, r * w * .5
        d = ((cv.xx - x) / rr) ** 2 + ((cv.yy - y) / (rr * .62)) ** 2; m = d <= 1
        cv.put(m, np.where(((cv.yy - y) < -rr * .25)[..., None], np.array(top, float), np.array(body, float)))
    m = (cv.yy >= cy + w * .14) & (cv.yy <= cy + w * .2) & (abs(cv.xx - cx) < w * .42)
    cv.put(m, shade)

def flowers(cv, rng, y0, y1, n, avoid=None, near_big=True):
    cols = [(255, 236, 140), (255, 196, 210), (246, 246, 255), (190, 200, 255), (255, 170, 120)]
    for _ in range(n):
        y = int(rng.uniform(y0, y1)); x = int(rng.uniform(0, cv.w))
        if avoid is not None and avoid[min(y, cv.h - 1), min(x, cv.w - 1)]: continue
        c = cols[rng.randrange(len(cols))]; near = (y - y0) / max(1, y1 - y0)
        if near_big and near > .55 and 1 <= x < cv.w - 1 and y + 2 < cv.h:   # a near flower: petals round a centre, on a stem
            cv.px[y + 1, x] = (60, 130, 70); cv.px[y + 2, x] = (52, 116, 64)
            for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)): cv.px[y + dy, x + dx] = c
            cv.px[y, x] = (250, 210, 90) if c != (255, 236, 140) else (230, 140, 60)
        else:
            cv.px[y, x] = c
            if y + 1 < cv.h: cv.px[y + 1, x] = (70, 140, 74)

def grass_tufts(cv, rng, y0, y1, n, cols, avoid=None):
    for _ in range(n):
        y = int(rng.uniform(y0, y1)); x = int(rng.uniform(0, cv.w))
        if avoid is not None and avoid[min(y, cv.h - 1), min(x, cv.w - 1)]: continue
        near = (y - y0) / max(1, y1 - y0); h = 2 + int(near * 5)
        for k, dx in enumerate((-1, 0, 1)):
            hh = h - (1 if dx else 0)
            for t in range(hh):
                xx = x + dx + (1 if (dx > 0 and t > hh // 2) else -1 if (dx < 0 and t > hh // 2) else 0); yy = y - t
                if 0 <= xx < cv.w and 0 <= yy < cv.h: cv.px[yy, xx] = cols[min(2, t * 3 // max(1, hh))]

# ---------------------------------------------------------------------------------------------------- the meadow
def paint_walk():
    rng = random.Random(7); cv = Canvas(); X = cv.xx.astype(float); Y = cv.yy.astype(float)
    # morning sky
    cv.dither_gradient(Y < 60, (118, 168, 236), (255, 214, 176), np.clip(Y / 46, 0, 1), 6)
    sx, sy = 52, 20   # the sun, risen over the far hills
    for r, c, a in ((16, (255, 236, 196), .35), (12, (255, 238, 200), .5), (7, (255, 246, 214), 1.0)):
        cv.put(((X - sx) ** 2 + (Y - sy) ** 2) <= r * r, c, a)
    cv.put(((X - sx) ** 2 + (Y - sy) ** 2) <= 25, (255, 252, 232))
    cloud(cv, 150, 14, 40, (255, 250, 246), (246, 226, 222), (214, 188, 196)); cloud(cv, 214, 26, 26, (255, 250, 246), (246, 226, 222), (214, 188, 196))
    cloud(cv, 96, 30, 22, (255, 248, 240), (244, 222, 214), (220, 196, 200))
    # far hills (hazy blue-green), then nearer, greener ones
    far = ridge(X, 46, 4, ((.031, .4, 1), (.07, 1.9, .6), (.013, 3.0, 1.4)))
    cv.dither_gradient(Y >= far, (150, 190, 186), (126, 172, 150), np.clip((Y - far) / 20, 0, 1), 3)
    for x in range(0, W, 7):   # tiny trees along the far ridge
        y = int(far[0, x]) - 1
        if rng.random() < .5 and 2 <= y < H: cv.put(((X - x) ** 2 + (Y - y) ** 2 * 1.4) <= 4, (112, 156, 150))
    mid = ridge(X, 58, 5, ((.022, 2.2, 1), (.051, .3, .5), (.009, 1.1, 1.6)))
    cv.dither_gradient(Y >= mid, (130, 192, 112), (96, 168, 90), np.clip((Y - mid) / 24, 0, 1), 4)
    cv.put((Y >= mid) & (Y <= mid + 1) & (X % 3 != 0), (168, 214, 128))       # sunlit rim
    blob_tree(cv, 190, 44, 13, (74, 146, 84), (122, 190, 104), (46, 104, 70), (96, 70, 52), rng)
    # the near meadow
    near = 72 + 3 * np.sin(X * .02 + 1.0)
    cv.dither_gradient(Y >= near, (112, 186, 92), (70, 150, 70), np.clip((Y - near) / 34, 0, 1), 5)
    # the path: wide at the front left, narrowing as it winds up to the far hill
    path = np.zeros_like(X, bool); edge = np.zeros_like(X, bool)
    for y in range(52, H):
        t = (y - 52) / (H - 52)                        # 0 far .. 1 near
        cx = 128 + 30 * math.sin(t * 3.3 + .4) - 70 * t * t
        hw = 1 + 15 * t ** 1.4
        path[y, max(0, int(cx - hw)):int(cx + hw) + 1] = True
        for e in (int(cx - hw) - 1, int(cx + hw) + 1):
            if 0 <= e < W: edge[y, e] = True
    pm = path & (Y >= mid)
    cv.dither_gradient(pm, (226, 200, 150), (184, 146, 102), np.clip((Y - 52) / 54, 0, 1), 4)
    cv.put(edge & (Y >= mid), (90, 150, 76))
    for _ in range(60):                                 # pebbles and wheel-worn shades along the path
        ys, xs = np.nonzero(pm & (Y > 70));
        if len(ys) == 0: break
        i = rng.randrange(len(ys)); cv.px[ys[i], xs[i]] = rng.choice([(160, 124, 90), (240, 222, 180)])
    flowers(cv, rng, 76, H, 160, avoid=path); flowers(cv, rng, 62, 76, 60, avoid=path, near_big=False)
    grass_tufts(cv, rng, 80, H, 110, [(52, 120, 60), (78, 156, 72), (128, 196, 98)], avoid=path)
    return cv.px

# ---------------------------------------------------------------------------------------------------- the stream
STREAM_TOP, STREAM_BOT = 52, 70
def paint_stream():
    rng = random.Random(11); cv = Canvas(); X = cv.xx.astype(float); Y = cv.yy.astype(float)
    cv.dither_gradient(Y < 40, (126, 176, 236), (226, 236, 246), np.clip(Y / 30, 0, 1), 5)
    cloud(cv, 60, 10, 30, (255, 255, 255), (232, 238, 248), (196, 206, 228)); cloud(cv, 176, 16, 22, (255, 255, 255), (232, 238, 248), (196, 206, 228))
    # the far bank: a line of trees, darker toward the water
    for i, x in enumerate(range(-6, W + 12, 15)):
        blob_tree(cv, x + rng.randint(-3, 3), 26 + rng.randint(-3, 4), 10 + rng.randint(-1, 3), (66, 136, 90), (104, 172, 106), (40, 96, 72), (86, 64, 50), rng)
    cv.dither_gradient((Y >= 34) & (Y < STREAM_TOP), (104, 170, 94), (78, 146, 80), np.clip((Y - 34) / 18, 0, 1), 3)
    cv.put((Y >= STREAM_TOP - 2) & (Y < STREAM_TOP), (118, 104, 78))          # the far bank's earth edge
    # the water: a gentle bend, light from the sky, ripples and glints
    wt = STREAM_TOP + 1.5 * np.sin(X * .05); wb = STREAM_BOT + 2 * np.sin(X * .04 + 1)
    water = (Y >= wt) & (Y <= wb)
    cv.dither_gradient(water, (150, 200, 240), (78, 136, 206), np.clip((Y - wt) / (wb - wt + 1e-6), 0, 1), 4)
    for _ in range(46):
        x = rng.randrange(W - 8); y = rng.randint(STREAM_TOP + 2, STREAM_BOT - 2)
        if water[y, x]:
            L = rng.randint(3, 7); cv.put((Y == y) & (X >= x) & (X < x + L) & water, (226, 242, 255) if rng.random() < .5 else (186, 222, 250))
    for x in range(0, W, 2):   # reflections of the far trees, broken by ripples
        for y in range(int(wt[0, x]) + 1, int(wt[0, x]) + 5):
            if 0 <= y < H and (x + y) % 3 == 0: cv.px[y, x] = (84, 146, 128)
    # the near bank
    nb = (Y > wb)
    cv.dither_gradient(nb, (110, 184, 92), (72, 150, 72), np.clip((Y - STREAM_BOT) / 36, 0, 1), 5)
    cv.put((Y > wb) & (Y <= wb + 2), (132, 112, 82))                            # its muddy lip
    # stones at the water's edge
    for x, y, r in ((30, STREAM_BOT + 3, 4), (44, STREAM_BOT + 4, 3), (98, STREAM_BOT + 2, 3), (200, STREAM_BOT + 3, 5)):
        m = ((X - x) / r) ** 2 + ((Y - y) / (r * .6)) ** 2 <= 1
        cv.put(m, np.where(((X - x) + (Y - y) < -1)[..., None], np.array((206, 206, 214), float), np.array((150, 152, 166), float)))
    # reeds by the water, left and right
    for x in list(range(4, 26, 3)) + list(range(214, 238, 3)):
        h = rng.randint(10, 18); y0 = STREAM_BOT + rng.randint(1, 4)
        for t in range(h):
            xx = x + (1 if t > h * .7 else 0)
            if 0 <= xx < W and 0 <= y0 - t < H: cv.px[y0 - t, xx] = (64, 132, 66) if t < h - 3 else (150, 110, 70)
        if 0 <= y0 - h < H: cv.px[y0 - h, x + 1] = (120, 80, 56)
    # the old willow on the near bank, with the burrow between its roots: the wild critters' home
    wx = 168
    cv.put((abs(X - wx) <= 6) & (Y >= 24) & (Y <= 84), (110, 84, 62)); cv.put((abs(X - wx - 4) <= 2) & (Y >= 24) & (Y <= 84), (82, 62, 48))
    cv.put((abs(X - wx + 4) <= 1) & (Y >= 26) & (Y <= 80), (140, 110, 82))
    for dx, s in ((-14, -1), (-8, -1), (9, 1), (15, 1)):   # roots spreading into the bank
        for t in range(10):
            x = int(wx + dx * t / 9); y = 76 + t // 2
            if 0 <= x < W: cv.px[y:y + 3, x] = (96, 72, 54)
    bm = ((X - wx) / 7.5) ** 2 + ((Y - 82) / 6.5) ** 2 <= 1                       # the burrow's mouth
    cv.put(bm & (Y <= 84), (40, 30, 34)); cv.put(bm & (Y <= 84) & ((X - wx) ** 2 + (Y - 83) ** 2 <= 9), (64, 46, 44))
    cv.put(((X - wx) ** 2 + (Y - 82) ** 2 * 2) <= 3, (200, 140, 80)); cv.px[82, wx] = (255, 210, 130)   # a warm little light inside
    blob_tree(cv, wx, 18, 15, (86, 160, 96), (134, 204, 120), (58, 120, 80), (110, 84, 62), rng)
    for k in range(13):                                    # the willow's drooping fronds: leafy strands from the canopy's rim
        x0 = wx - 24 + k * 4 + rng.randint(-1, 1); y0 = 14 + int(6 * abs(math.sin(k * .5))); L = 22 + rng.randint(-3, 9)
        for t in range(L):
            x = int(round(x0 + 1.5 * math.sin(t / 7 + k))); y = y0 + t
            if not (0 <= x < W and 0 <= y < H) or y >= STREAM_TOP - 1: break
            leaf = (t + k) % 3 == 0
            cv.px[y, x] = (150, 212, 134) if leaf else (98, 176, 104)
            if x + 1 < W and (t % 2 == 0): cv.px[y, x + 1] = (72, 148, 88) if not leaf else (120, 194, 118)
    flowers(cv, rng, STREAM_BOT + 6, H, 90); grass_tufts(cv, rng, STREAM_BOT + 6, H, 70, [(52, 120, 60), (78, 156, 72), (128, 196, 98)])
    return cv.px

# ---------------------------------------------------------------------------------------------------- the wild critters
def paint_critter(w, h, body, light, dark, belly, wave, blush=(240, 150, 150)):
    """a round, fluffy wild critter with tufted ears, lit from the top-left; wave: one paw up"""
    cv = Canvas(w, h); X = cv.xx.astype(float); Y = cv.yy.astype(float)
    cx, cy, rx, ry = w / 2 - .5, h * .58, w * .42, h * .38
    for ex in (-1, 1):   # the ears: soft tufts
        ax = cx + ex * rx * .55; m = ((X - ax) / (w * .12)) ** 2 + ((Y - (cy - ry * 1.05)) / (h * .2)) ** 2 <= 1
        cv.put(m, dark if ex > 0 else body); cv.put(m & (Y < cy - ry * 1.1) & (abs(X - ax) < w * .05), light)
    d = ((X - cx) / rx) ** 2 + ((Y - cy) / ry) ** 2; m = d <= 1
    lit = (-(X - cx) / rx * .6 - (Y - cy) / ry * .8)
    cv.put(m, np.where((lit > .45)[..., None], np.array(light, float), np.where((lit < -.4)[..., None], np.array(dark, float), np.array(body, float))))
    cv.put(m & (((X - cx) / (rx * .6)) ** 2 + ((Y - cy - ry * .35) / (ry * .5)) ** 2 <= 1), belly)
    for f in range(0, w, 3):   # fluffy edge
        y = int(cy - ry * math.sqrt(max(0, 1 - ((f - cx) / rx) ** 2)))
        if 0 <= y - 1 < h and m[min(h - 1, y + 1), f]: cv.px[y - 1, f] = light if f < cx else body
    ey = int(cy - ry * .15)
    for ex in (-1, 1):   # eyes, with a glint; rosy cheeks
        x = int(round(cx + ex * rx * .38)); cv.px[ey, x] = (34, 30, 44); cv.px[ey + 1, x] = (34, 30, 44)
        if w > 16: cv.px[ey, x + 1 if ex < 0 else x - 1] = (34, 30, 44)
        cv.px[ey - (1 if w > 16 else 0), x] = (250, 250, 255) if w > 16 else cv.px[ey - 0, x]
        bx = int(round(cx + ex * rx * .62))
        if 0 <= bx < w: cv.px[ey + 2, bx] = blush
    mx = int(round(cx)); cv.px[ey + 3, mx] = (120, 60, 60)
    for ex in (-1, 1):   # paws: the right one raised high when it waves
        if wave and ex > 0:
            px_ = cx + rx * 1.02; py_ = cy - ry * 1.15
            cv.put((abs(X - (px_ - .5)) <= max(1, w * .06)) & (Y > py_) & (Y <= cy - ry * .2), light)      # the arm
            cv.put(((X - px_) / max(1.2, w * .09)) ** 2 + ((Y - py_) / max(1.2, h * .09)) ** 2 <= 1, light)  # the paw
        else: cv.put(((X - (cx + ex * rx * .95)) / (w * .07)) ** 2 + ((Y - (cy + ry * .2)) / (h * .1)) ** 2 <= 1, dark)
    for ex in (-1, 1):   # feet
        cv.put(((X - (cx + ex * rx * .45)) / (w * .13)) ** 2 + ((Y - (cy + ry * .95)) / (h * .07)) ** 2 <= 1, dark)
    # outline
    a = np.any(cv.px > 0, axis=2); o = cv.px.copy()
    for y in range(h):
        for x in range(w):
            if not a[y, x] and any(0 <= x + dx < w and 0 <= y + dy < h and a[y + dy, x + dx] for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))): o[y, x] = (40, 34, 52)
    return o, a | np.any(o != cv.px, axis=2)

CRITTERS = (   # name, size, body, light, dark, belly
    ('BIG', 26, 24, (226, 172, 128), (250, 214, 176), (178, 120, 88), (252, 236, 210)),
    ('KIT1', 17, 16, (236, 196, 150), (252, 228, 196), (190, 140, 104), (255, 244, 224)),
    ('KIT2', 17, 16, (196, 170, 210), (228, 210, 238), (150, 122, 170), (246, 238, 250)),
)

def quantize(images, n):
    """one palette of n colours for all the pictures (median cut over their pixels)"""
    allpx = np.concatenate([im.reshape(-1, 3) for im in images], axis=0).astype(np.uint8)
    strip = Image.fromarray(allpx.reshape(1, -1, 3))
    q = strip.quantize(colors=n, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[:3 * n]; idx = np.array(q).reshape(-1)
    out = []; o = 0
    for im in images:
        k = im.shape[0] * im.shape[1]; out.append(idx[o:o + k].reshape(im.shape[:2])); o += k
    return out, [tuple(pal[3 * i:3 * i + 3]) for i in range(n)]

def main():
    walk = paint_walk(); stream = paint_stream()
    crit = []
    for name, w, h, body, light, dark, belly in CRITTERS:
        for wave in (0, 1):
            im, a = paint_critter(w, h, body, light, dark, belly, wave); crit.append((f'{name}{wave}', im, a))
    imgs = [walk, stream] + [c[1] for c in crit]
    idx, pal = quantize([np.clip(i, 0, 255) for i in imgs], MD_N)
    L = ['// generated by gen/meadow.py - the runaway\'s meadow, stream and wild critters (palette MD_FIRST..)',
         f'#define MD_FIRST {MD_FIRST}\n#define MD_N {MD_N}\n#define MD_W {W}\n#define MD_H {H}']
    L.append('static const u16 MD_PAL[MD_N]={' + ','.join(str((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)) for r, g, b in pal) + '};')
    for name, arr in (('MD_BG_WALK', idx[0]), ('MD_BG_STREAM', idx[1])):
        L.append(f'static const u8 {name}[MD_H*MD_W]={{' + ','.join(str(MD_FIRST + int(v)) for v in arr.reshape(-1)) + '};')
    for (name, im, a), arr in zip(crit, idx[2:]):
        h, w = arr.shape
        vals = [(MD_FIRST + int(arr[y, x])) if a[y, x] else 0 for y in range(h) for x in range(w)]
        L.append(f'#define WC_{name}_W {w}\n#define WC_{name}_H {h}\nstatic const u8 WC_{name}[{h*w}]={{' + ','.join(map(str, vals)) + '};')
    open(os.path.join(ROOT, 'meadow.h'), 'w').write('\n'.join(L) + '\n')
    if len(sys.argv) > 1:   # a preview sheet
        P = np.array(pal, np.uint8)
        sheet = Image.new('RGB', (W * 3 + 20, H * 3 * 2 + 30 + 80), (24, 22, 40))
        for k, arr in enumerate(idx[:2]): sheet.paste(Image.fromarray(P[arr]).resize((W * 3, H * 3), Image.NEAREST), (10, 10 + k * (H * 3 + 10)))
        x = 10
        for (name, im, a), arr in zip(crit, idx[2:]):
            rgb = P[arr].copy(); rgb[~a] = (24, 22, 40); im2 = Image.fromarray(rgb); s = 3
            sheet.paste(im2.resize((im2.width * s, im2.height * s), Image.NEAREST), (x, 2 * (H * 3 + 10) + 10)); x += im2.width * s + 12
        sheet.save(sys.argv[1])
    print('meadow.h written')

if __name__ == '__main__':
    main()
