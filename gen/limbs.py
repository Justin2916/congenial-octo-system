# Mockup: the same critters with clearer arms and legs (drawn around the existing designs).
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
import math, sys
sys.path.insert(0, _ROOT + '/gen')
import critters as C
from critters import tube, body_xf, Canvas

def arm_angle(pose, base, side):
    full = C.FULL
    if full in ('wave1', 'wave2') and side > 0: return 1.85 if full == 'wave1' else 2.35
    if full == 'stretch': return 2.65
    sw = {'walk1': 0.3*side, 'walk2': -0.3*side}.get(full, 0)
    return base + sw + {'idle': 0, 'blink': 0, 'happy': -1.5, 'sleep': -0.2, 'eat': -0.7, 'sad': -0.25}[pose]
def arm(c, sx, sy, side, L, th, mat, pose, base=0.65, hand=None, hr=None):
    """side -1 = left, +1 = right. angle measured from straight down, swinging outward"""
    a = arm_angle(pose, base, side)
    if C.FULL in ('wave1', 'wave2') and side > 0: L *= 1.15
    ex = sx + side*math.sin(a)*L; ey = sy + math.cos(a)*L
    mx = sx + side*math.sin(a*0.6)*L*0.55; my = sy + math.cos(a*0.6)*L*0.55
    tube(c, [(sx, sy, th), (mx, my, th*0.95), (ex, ey, th*0.85)], mat, (sx, sy+L/2, L, L))
    c.ellipse(ex+side*0.3, ey+0.3, hr or th*1.35, (hr or th*1.35)*0.9, hand or mat)
    return ex, ey
def leg(c, x, top, bot, th, mat, foot=None, fw=None):
    bot -= C.LIFT(x)
    tube(c, [(x, top, th), (x, bot-1, th*0.95)], mat, (x, (top+bot)/2, th*2, bot-top))
    c.ellipse(x, bot, fw or th*1.5, th*0.85, foot or mat)

EXTRA = {}
def ex(name):
    def d(fn): EXTRA[name] = fn; return fn
    return d

@ex('BLIPLET')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 41+dy*0.5, 46.2, 1.9, 'yel2')
    else:
        for x in (19, 29): c.ellipse(x, 46.2-C.LIFT(x), 3.0, 1.7, 'yel2')
        for s in (-1, 1): arm(c, 24+s*10.5, 36.5+dy, s, 5.5, 1.5, 'yel2', pose, 0.9)
@ex('SPROUTLE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (20, 28): leg(c, x, 38, 46, 2.3, 'mint2', fw=3.4)
    else:
        for s in (-1, 1): arm(c, 24+s*9.5, 29+dy, s, 9, 1.7, 'mint', pose, 0.55, 'mint2', 2.2)
@ex('FLUFFBUD')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19, 29): leg(c, x, 38, 46, 2.4, 'puff', 'hoof', 3)
    else:
        for x in (18.5, 29.5): c.ellipse(x, 46-C.LIFT(x), 3.2, 1.8, 'hoof')
        for s in (-1, 1): arm(c, 24+s*12, 36+dy, s, 6.5, 1.9, 'puff', pose, 0.7, 'patch', 2.3)
@ex('FRILLO')
def _(c, pose, stage):
    if stage == 'back':
        for x in (19, 29): leg(c, x, 38, 45.5, 2.2, 'body2', fw=3.2)
@ex('STARWING')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (17, 31): leg(c, x, 38+dy, 46, 1.9, 'gold', fw=3.2)
    else:
        for s in (-1, 1): arm(c, 24+s*13, 25+dy, s, 7, 1.6, 'blue', pose, 1.1, 'wing', 2.2)
@ex('MOSSMOO')
def _(c, pose, stage):
    if stage == 'front':
        for x in (9, 18, 30, 39): c.ellipse(x, 45.6-C.QLIFT(x), 3.4, 1.4, 'hoof')
@ex('DRIZZLE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19, 29): leg(c, x, 36+dy, 44.5, 1.8, 'cloud', 'pud', 2.8)
    else:
        for s in (-1, 1): arm(c, 24+s*15, 29+dy, s, 7, 1.6, 'cloud', pose, 0.8, 'cloud', 2.2)
@ex('SUNPUFF')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19, 29): leg(c, x, 41, 46, 2.4, 'fur2', fw=3.4)
    else:
        for s in (-1, 1): arm(c, 24+s*7.5, 35+dy*0.5, s, 7, 1.8, 'fur', pose, 0.6, 'fur2', 2.2)
@ex('CHONKLE')
def _(c, pose, stage):
    if stage == 'back':
        for x in (16, 32): leg(c, x, 40, 46, 2.6, 'ham', 'inner', 3.4)
    else:
        for x in (16.5, 31.5): u = C.LIFT(x); c.ellipse(x, 46-u, 3.8, 2.0, 'ham'); c.ellipse(x, 46.6-u, 2.2, 1.0, 'inner', shade=False)
@ex('ZIPPIT')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (20.5, 27.5): leg(c, x, 38, 46, 2.1, 'fox2', 'chest', 3.0)
    else:
        for s in (-1, 1): arm(c, 24+s*7, 30+dy, s, 8, 1.5, 'fox', pose, 0.45, 'chest', 2.0)
@ex('GLIMMER')
def _(c, pose, stage):
    dy, fl = body_xf(pose)[0], body_xf(pose)[0]-2
    if stage == 'front':
        for s in (-1, 1):
            up = -6 if pose == 'happy' else 3 if pose in ('sad', 'sleep') else 0
            up -= 5*C.ARMX(s)
            pts = [(24+s*13, 28+fl, 1.7), (24+s*18, 31+fl+up*0.4, 1.4), (24+s*20.5, 36+fl+up, 1.2), (24+s*18.5, 40+fl+up, 1.0)]
            tube(c, pts, 'lil', (24+s*18, 34+fl, 6, 8))
            c.ellipse(pts[-1][0], pts[-1][1]+1, 1.8, 1.8, 'crys')
@ex('NESTLE')
def _(c, pose, stage):
    if stage == 'back':
        for x in (20, 28): leg(c, x, 40, 45.2, 2.3, 'chest', 'nest', 2.8)
    else:
        op = -0.7 if pose == 'happy' else 0
        for s in (-1, 1):
            # feather "fingers" at the wing tips
            for k in range(3):
                o2 = op-0.5*C.ARMX(s); bx = 24+s*(12.5+k*1.6 - (o2*-4)); by = 41.5+k*0.3+(o2*8)-2*C.ARMX(s)
                c.ellipse(bx, by, 1.4, 2.6, 'wing', rot=s*(0.3+k*0.25))


@ex('OAKFAWN')
def _(c, pose, stage):
    if stage == 'back':
        for x in (18, 30): leg(c, x, 38, 45.5, 1.5, 'body', 'hoof', 2.0)
    else:
        for x in (21, 27): leg(c, x, 39, 46, 1.6, 'body', 'hoof', 2.1)
@ex('OAKHART')
def _(c, pose, stage):
    if stage == 'back':
        for x in (15, 33): leg(c, x, 40, 45.5, 1.9, 'body', 'hoof', 2.4)
    else:
        for x in (20, 28): leg(c, x, 41, 46, 2.0, 'body', 'hoof', 2.5)
@ex('NOXPUP')
def _(c, pose, stage):
    if stage == 'back':
        for x in (20.5, 27.5): leg(c, x, 40, 45.5, 1.5, 'fur2', 'fur2', 2.2)
@ex('NOXWING')
def _(c, pose, stage):
    if stage == 'back':
        for x in (20, 28): leg(c, x, 40, 46, 1.7, 'fur2', 'fur2', 2.4)
@ex('DUNELET')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'front':
        for s in (-1, 1): arm(c, 24+s*9, 38+dy, s, 4, 1.4, 'fur2', pose, 0.9, 'fur2', 1.7)
@ex('DUNEKIT')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (20, 28): leg(c, x, 40, 46, 1.9, 'fur2', 'fur2', 2.6)
    else:
        for s in (-1, 1): arm(c, 24+s*6.5, 32+dy*0.5, s, 6.5, 1.5, 'fur', pose, 0.45, 'fur2', 1.9)
@ex('DUNEROSE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 41, 46, 2.1, 'fur2', 'fur2', 2.9)
    else:
        for s in (-1, 1): arm(c, 24+s*8.5, 31+dy*0.5, s, 7.5, 1.8, 'fur', pose, 0.45, 'fur2', 2.2)


@ex('BLOOMLE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (20, 28): leg(c, x, 37, 46, 2.0, 'mint2', 'mint2', 2.6)
    else:
        for s in (-1, 1): arm(c, 24+s*9, 27+dy, s, 7, 1.6, 'mint2', pose, 0.6, 'mint2', 2.0)
@ex('THORNLE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 40, 46, 2.1, 'mint2', 'mint2', 2.7)
    else:
        for s in (-1, 1): arm(c, 24+s*10, 31+dy, s, 6.5, 1.7, 'mint2', pose, 0.75, 'mint2', 2.1)
@ex('SNAPWORT')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'front':
        for s in (-1, 1):    # leaf arms
            a = arm_angle(pose, 0.9, s); L = 7
            c.ellipse(24+s*(9.5+math.sin(a)*L*0.5), 37+dy*0.5+math.cos(a)*L*0.5, 4.2, 1.9, 'stem2', rot=s*(1.57-a))


@ex('CANCOON')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 41, 46, 2.0, 'fur2', 'mask', 2.6)
    else:
        for s in (-1, 1): arm(c, 24+s*8, 32+dy*0.5, s, 7, 1.7, 'fur', pose, 0.45, 'mask', 2.0)
@ex('JARDEN')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 41, 46, 2.0, 'fur2', 'mask', 2.6)
    else:
        for s in (-1, 1): arm(c, 24+s*8, 32+dy*0.5, s, 7, 1.7, 'fur', pose, 0.45, 'mask', 2.0)


@ex('SMOGWOOL')
def _(c, pose, stage):
    if stage == 'back':
        for x in (19, 29): leg(c, x, 38, 45, 1.6, 'hoof', 'hoof', 2.2)
@ex('PETALAMB')
def _(c, pose, stage):
    if stage == 'back':
        for x in (19, 29): leg(c, x, 38, 45, 1.6, 'hoof', 'hoof', 2.2)
@ex('GUNKBILL')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'front':
        for s in (-1, 1): arm(c, 24+s*10, 31+dy*0.5, s, 6, 1.6, 'fur', pose, 0.6, 'foot', 2.0)
@ex('LILYBILL')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'front':
        for s in (-1, 1): arm(c, 24+s*10, 31+dy*0.5, s, 6, 1.6, 'fur', pose, 0.6, 'foot', 2.0)


# v8 phase 3: limbs for the redrawn forms (replace the entries above)
@ex('FLUFFBUD')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 40, 46, 2.3, 'puff2', 'hoof', 3.2)
    else:
        for s in (-1, 1): arm(c, 24+s*8.5, 35+dy*0.5, s, 6, 1.8, 'puff2', pose, 0.55, 'puff', 2.2)
@ex('ZIPPIT')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 40, 46, 2.2, 'fur2', 'hoof', 3.4)
    else:
        for s in (-1, 1): arm(c, 24+s*8, 34+dy*0.5, s, 6.5, 1.7, 'fur', pose, 0.6, 'puff', 2.0)
@ex('FRILLO')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19, 29): leg(c, x, 40, 45.8, 2.2, 'body2', fw=3.2)
    else:
        for s in (-1, 1): arm(c, 24+s*8.5, 35+dy*0.5, s, 5.5, 1.7, 'body', pose, 0.6, 'body2', 2.0)
@ex('NESTLE')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19, 29): leg(c, x, 40, 45.8, 2.2, 'body2', fw=3.2)
    else:
        for s in (-1, 1): arm(c, 24+s*8.5, 35+dy*0.5, s, 5.5, 1.7, 'body', pose, 0.6, 'body2', 2.0)
@ex('SUNPUFF')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (18, 30): leg(c, x, 40, 46, 2.1, 'fur2', 'hoof', 2.6)
    else:
        for s in (-1, 1): arm(c, 24+s*9.5, 33+dy*0.5, s, 7, 1.8, 'fur', pose, 0.45, 'hoof', 2.0)
@ex('GLIMMER')
def _(c, pose, stage):
    dy = body_xf(pose)[0]
    if stage == 'back':
        for x in (19.5, 28.5): leg(c, x, 41, 46, 2.1, 'fur2', 'fur2', 2.9)
    else:
        for s in (-1, 1): arm(c, 24+s*8.5, 31+dy*0.5, s, 7.5, 1.8, 'fur', pose, 0.45, 'fur2', 2.2)

def make(name):
    ramps, draw = C.FORMS[name]; ex = EXTRA.get(name) or EXTRA.get(name.split('~')[0])
    if not ex: return ramps, draw
    def d(c, pose):
        ex(c, pose, 'back'); draw(c, pose); ex(c, pose, 'front')
    return ramps, d
def render(name, pose, size=64):
    ramps, draw = make(name); c = Canvas(size/48); draw(c, pose); return c.rgb(ramps)

if __name__ == '__main__':
    from PIL import Image, ImageDraw
    names = [n for n in C.ORDER if n != 'EGG']
    S = 2; cell = 64*S+10
    rows = [('NOW', lambda n: C.render(n, 'idle')), ('WITH LIMBS', lambda n: render(n, 'idle')), ('HAPPY', lambda n: render(n, 'happy')), ('SAD', lambda n: render(n, 'sad'))]
    im = Image.new('RGB', (6*cell+90, 2*(len(rows)*cell+14)+10), (236, 226, 206)); d = ImageDraw.Draw(im)
    for blk in range(2):
        for r, (lbl, fn) in enumerate(rows):
            y = 8+blk*(len(rows)*cell+14)+r*cell
            d.text((6, y+60), lbl, fill=(40, 30, 70))
            for i, n in enumerate(names[blk*6:blk*6+6]):
                g = fn(n); x = 90+i*cell
                t = Image.new('RGBA', (64, 64), (0, 0, 0, 0)); p = t.load()
                for yy in range(64):
                    for xx in range(64):
                        if g[yy][xx] is not None: p[xx, yy] = g[yy][xx]+(255,)
                t = t.resize((64*S, 64*S), Image.NEAREST); im.paste(t, (x, y), t)
                if r == 0: d.text((x+4, y-2), n, fill=(40, 30, 70))
    im.save('/tmp/limbs.png')
