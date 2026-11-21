# review sheet + animated preview for the B2 garden sprites
import sys, os, math
sys.path.insert(0, os.path.dirname(__file__))
from PIL import Image, ImageDraw
import garden_art as A

def q5(c): return tuple((v >> 3) * 255 // 31 for v in c)   # as the GBA shows it
SKY = [(14, 16, 38), (18, 22, 48), (22, 30, 58)]
GRASS = (26, 52, 50)

def draw(img, fr, x, y, s=1, recol=None):
    for j, row in enumerate(fr):
        for i, ch in enumerate(row):
            if ch == '.': continue
            c = (recol or {}).get(ch) or A.PAL.get(ch)
            if c is None: c = (255, 0, 255)
            img.paste(q5(c), (x + i * s, y + j * s, x + i * s + s, y + j * s + s))

def petals(name):
    l, m = A.FLOWER_COLS[name]; return {'1': l, '2': m}

def halo(img, cx, cy, col, t, s):   # a soft ring of glints that slowly turns and twinkles
    for k in range(14):
        a = (k + t / 16) * 2 * math.pi / 14
        r = 7.0 if k % 2 else 8.0
        if (k + t // 4) % 7 == 0: continue
        x = round(r * math.cos(a)); y = round(r * math.sin(a))
        img.paste(q5(col), (cx + x * s, cy + y * s, cx + x * s + s, cy + y * s + s))

AUR = list(A.FLOWER_COLS)
AUR_BANDS = 'LuUVpVUu'   # green, pale green, teal, violet, pink, violet, teal, pale green: the colours of a real aurora
def aurora_head(t):   # the bloom's petals as slow bands of colour, rippling and drifting upward like an aurora's curtains
    out = []
    for y, row in enumerate(A.BLOOM_HEAD[0]):
        r = ''
        for x, ch in enumerate(row):
            if ch in '12':
                v = y + t / 5 + 1.6 * math.sin((x + t / 7) * 0.9)
                r += AUR_BANDS[int(v / 2) % len(AUR_BANDS)]
            else: r += ch
        out.append(r)
    return out
def aurora_halo(img, cx, cy, t, s):   # its glow shimmers through the same colours, like curtains of light
    for k in range(14):
        a = k * 2 * math.pi / 14
        r = 7.0 + 1.2 * math.sin(a * 3 + t / 4)
        if (k + t // 3) % 5 == 0: continue
        x = round(r * math.cos(a)); y = round(r * math.sin(a))
        c = A.PAL[AUR_BANDS[(k // 2 + t // 6) % len(AUR_BANDS)]]
        img.paste(q5(c), (cx + x * s, cy + y * s, cx + x * s + s, cy + y * s + s))
def moon_glow(img, cx, cy, t, s):   # a soft dithered glow behind the moon, so its shadowed side only just shows
    for y in range(-9, 10):
        for x in range(-9, 10):
            d = math.hypot(x, y)
            if 5.0 < d < 7.6 + 0.4 * math.sin(t / 5) and ((x + y) & 1 or d < 6.3):
                img.paste(q5(A.PAL['m']), (cx + x * s, cy + y * s, cx + x * s + s, cy + y * s + s))
def luna_glow(img, cx, cy, t, s):   # the luna moth's pale green aura
    for k in range(16):
        a = (k + t / 10) * 2 * math.pi / 16
        r = 9.5 + (k % 2) * 1.5 + 0.6 * math.sin(t / 3 + k)
        if (k + t // 3) % 4 == 0: continue
        x = round(r * math.cos(a)); y = round(r * 1.1 * math.sin(a))
        img.paste(q5(A.PAL['u'] if k % 3 else A.PAL['U']), (cx + x * s, cy + y * s, cx + x * s + s, cy + y * s + s))
def flower(img, kind, x, y, t, s, glow=True):
    """kind: a colour name or STARBLOOM / MOONLILY / TWINGLOW / AURORA; (x,y) = top-left of an 11-wide head."""
    sway = 1 if (t // 8) % 2 else 0
    if kind.startswith('TWINGLOW'):
        c1, c2 = (kind.split(':')[1].split(',') if ':' in kind else ('ROSE', 'GOLD'))
        rc = {'1': A.FLOWER_COLS[c1][0], 'A': A.FLOWER_COLS[c1][1], '3': A.FLOWER_COLS[c2][0], 'B': A.FLOWER_COLS[c2][1]}
        if glow: halo(img, x + 6 * s, y + 3 * s, A.FLOWER_COLS[c1 if (t // 16) % 2 else c2][1], t, s)
        draw(img, A.TWIN, x - s, y, s, rc); draw(img, A.STEM[3:], x, y + 10 * s, s)
        draw(img, A.MOUND, x - s, y + 15 * s, s); return
    if glow:
        if kind == 'AURORA': aurora_halo(img, x + 5 * s, y + 5 * s, t, s)
        else:
            col = {'STARBLOOM': A.FLOWER_COLS['GOLD'][1], 'MOONLILY': A.FLOWER_COLS['SKY'][0]}.get(kind)
            if kind != 'MOONLILY': halo(img, x + 5 * s, y + 5 * s, col or A.FLOWER_COLS[kind][1], t, s)
    draw(img, A.STEM, x, y + 10 * s, s)
    if kind == 'STARBLOOM': draw(img, A.STARBLOOM[(t // 4) % 2], x, y, s, petals('GOLD'))
    elif kind == 'MOONLILY': draw(img, A.MOONLILY[1 if (t // 6) % 4 == 0 else 0], x - s, y - s, s)
    elif kind == 'AURORA': draw(img, aurora_head(t), x, y, s)
    else: draw(img, A.BLOOM_HEAD[sway], x, y, s, petals(kind))
    draw(img, A.MOUND, x - s, y + 17 * s, s)

def backdrop(W, H):
    img = Image.new('RGB', (W, H), SKY[0])
    for i, c in enumerate(SKY): img.paste(c, (0, H * i // 5, W, H * (i + 1) // 5))
    img.paste(GRASS, (0, H * 3 // 5, W, H))
    return img

def sheet():
    S = 4; W = 1040
    img = Image.new('RGB', (W, 1700), SKY[0]); d = ImageDraw.Draw(img)
    def title(y, t): d.text((14, y), t, fill=(255, 220, 90))
    y = 12; title(y, 'MOONFLOWERS - GROWTH: PLANTED, SPROUT, BUD, BLOOM (WITH ITS GLOW)'); y += 20
    img.paste(GRASS, (0, y, W, y + 110)); x = 30
    for fr in (A.SEED, A.SPROUT, A.BUD):
        draw(img, fr, x, y + 96 - len(fr) * S, S, petals('ROSE')); x += 90
    flower(img, 'ROSE', x + 4, y + 96 - 20 * S, 0, S); x += 110
    y += 128; title(y, 'FIVE COLOURS, FROM THE CARE IT GOT'); y += 20
    img.paste(GRASS, (0, y, W, y + 120)); x = 30
    for n in A.FLOWER_COLS:
        flower(img, n, x + 10, y + 106 - 20 * S, 4, S); d.text((x + 10, y + 108 - 2), '', fill=(0, 0, 0)); x += 120
    x = 30
    for n in A.FLOWER_COLS: d.text((x + 12, y + 108), n, fill=(230, 230, 245)); x += 120
    y += 140; title(y, 'RARE FORMS'); y += 20
    img.paste(GRASS, (0, y, W, y + 120)); x = 30
    for n, lab in (('STARBLOOM', 'STARBLOOM'), ('MOONLILY', 'MOON LILY'), ('AURORA', 'AURORA')):
        flower(img, n, x + 10, y + 106 - 20 * S, 4, S); d.text((x + 10, y + 108), lab, fill=(230, 230, 245)); x += 130
    for pair in ('ROSE,GOLD', 'SKY,MINT', 'SILVER,ROSE'):
        flower(img, 'TWINGLOW:' + pair, x + 10, y + 106 - 20 * S, 4, S); x += 110
    d.text((x - 300, y + 108), 'TWINGLOW: ANY TWO OF THE FIVE COLOURS, PICKED AT RANDOM', fill=(230, 230, 245))
    y += 140; title(y, 'OWL, FROG, HEDGEHOG: BEFORE (LEFT) AND THE NEW VERSIONS (RIGHT)'); y += 22
    for old, new in ((A.OWL_V1, A.OWL), (A.FROG_V1, A.FROG), (A.HEDGEHOG_V1, A.HEDGEHOG)):
        mh = max(len(f) for f in old + new)
        img.paste((20, 26, 54), (0, y - 4, W, y + mh * S + 4)); x = 14
        for fr in old[:2]: draw(img, fr, x, y + (mh - len(fr)) * S, S); x += len(fr[0]) * S + 14
        x = max(x + 40, 330)
        for fr in new: draw(img, fr, x, y + (mh - len(fr)) * S, S); x += len(fr[0]) * S + 18
        y += mh * S + 14
    y += 10; title(y, 'NIGHT VISITORS - EVERY ANIMATION FRAME'); y += 22
    for name, frames, seq, rare in A.VISITORS:
        mh = max(len(f) for f in frames)
        img.paste((20, 26, 54), (0, y - 4, W, y + mh * S + 4))
        d.text((14, y + mh * S // 2 - 6), name + ('  (RARE)' if rare else ''), fill=(180, 250, 200) if rare else (230, 230, 245))
        x = 170
        for k, fr in enumerate(frames):
            if name == 'LUNA MOTH': luna_glow(img, x + len(fr[0]) * S // 2, y + mh * S // 2, k * 5, S)
            draw(img, fr, x, y + (mh - len(fr)) * S, S); x += len(fr[0]) * S + (60 if name == 'LUNA MOTH' else 22)
        y += mh * S + (40 if name == 'LUNA MOTH' else 14)
    return img.crop((0, 0, W, y + 8))

def anim(path):
    """the yard at night at game size (x3): three patches in bloom and every visitor at once (a real night has a few)"""
    S = 3; W, H = 240, 160
    stars = [(13, 9), (40, 22), (71, 6), (102, 30), (130, 12), (158, 26), (190, 8), (221, 20), (58, 40), (176, 44), (230, 46), (20, 36)]
    frames = []
    def put(img, fr, x, y): draw(img, fr, x * S, y * S, S)
    for t in range(96):
        img = Image.new('RGB', (W * S, H * S), SKY[0])
        for i, c in enumerate(SKY): img.paste(c, (0, 16 * i * S, W * S, 16 * (i + 1) * S))
        for k, (x, y) in enumerate(stars):
            img.paste(q5((210, 214, 240)) if (t // 8 + k) % 5 else q5((120, 124, 170)), (x * S, y * S, x * S + S, y * S + S))
        img.paste(q5((40, 44, 70)), (0, 64 * S, W * S, 92 * S))            # fence
        for x in range(4, W, 20): img.paste(q5((52, 56, 86)), (x * S, 60 * S, (x + 6) * S, 92 * S))
        img.paste(GRASS, (0, 92 * S, W * S, H * S))
        for k in range(40): img.paste(q5((36, 70, 64)), (((k * 53) % 236) * S, (96 + (k * 29) % 60) * S, ((k * 53) % 236 + 1) * S, (97 + (k * 29) % 60) * S))
        flower(img, 'MOONLILY', 30 * S, 104 * S, t, S); flower(img, 'ROSE', 66 * S, 104 * S, t, S); flower(img, 'AURORA', 110 * S, 106 * S, t, S); flower(img, 'TWINGLOW:SKY,MINT', 150 * S, 104 * S, t, S); flower(img, 'STARBLOOM', 186 * S, 102 * S, t, S)
        seq = lambda fr, sq, d=3: fr[sq[(t // d) % len(sq)]]
        put(img, seq(A.OWL, A.OWL_FR, 4), 24, 42)                                         # on a fence post
        a = t * 2 * math.pi / 48
        put(img, seq(A.MOTH, A.MOTH_FR, 2), int(66 + 11 * math.cos(a)), int(96 + 7 * math.sin(a)))   # round the rose
        lx, ly = int(104 + 4 * math.sin(a)), int(78 + 3 * math.cos(a * 2)); luna_glow(img, (lx + 7) * S, (ly + 7) * S, t, S)
        put(img, seq(A.LUNA, A.LUNA_FR, 3), lx, ly)
        put(img, seq(A.FIREFLY, A.FIREFLY_FR, 3), int(150 + 20 * math.sin(a / 2)), int(90 + 5 * math.sin(a)))
        put(img, seq(A.FIREFLY, A.FIREFLY_FR, 4), int(80 + 15 * math.cos(a / 2)), int(84 + 6 * math.cos(a)))
        put(img, seq(A.BAT, A.BAT_FR, 2), (t * 3) % 280 - 30, int(30 + 6 * math.sin(a * 2)))
        hx = (t * 1) % 130 - 20
        put(img, A.HEDGEHOG[A.HEDGEHOG_FR[(t // 3) % 4]], hx, 136)
        fi = (t // 4) % len(A.FROG_FR); put(img, A.FROG[A.FROG_FR[fi]], 86, 126 + A.FROG_HOP[fi])
        put(img, seq(A.SNAIL, A.SNAIL_FR, 6), 128 + (t // 24), 142)
        ci = (t // 3) % len(A.CRICKET_FR); put(img, A.CRICKET[A.CRICKET_FR[ci]], 186, 140 + A.CRICKET_HOP[ci])
        fx = A.FOX[A.FOX_FR[(t // 6) % len(A.FOX_FR)]]; put(img, fx, 204, 141 - len(fx))
        frames.append(img)
    frames[0].save(path, save_all=True, append_images=frames[1:], duration=60, loop=0)

if __name__ == '__main__':
    sheet().save(sys.argv[1] if len(sys.argv) > 1 else '/tmp/garden_sheet.png')
    if len(sys.argv) > 2: anim(sys.argv[2])
