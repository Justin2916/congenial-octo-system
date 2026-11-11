# v8 phase 3: the older forms redrawn in the current style (exec'd by critters.py after newforms.py; replaces their
# entries in FORMS, keeping the form numbers). Each secret carries its grown-up's motif. Mockup: roster_v8_mock.py.
def v8_blossom(c, x, y, r, petal, ctr, n=5, rot=-math.pi/2, rr=0.6):
    for i in range(n):
        a = rot + i*2*math.pi/n
        c.ellipse(x+math.cos(a)*r*0.78, y+math.sin(a)*r*0.78, r*rr, r*rr, petal)
    c.ellipse(x, y, r*0.38, r*0.38, ctr, shade=False)
def v8_seed(c, x, y, s=1.0, col=(250, 248, 240), stem=(176, 160, 140)):   # a floating dandelion seed
    c.line(x, y, x, y+2.4*s, stem)
    for a in (-0.9, -0.45, 0, 0.45, 0.9): c.line(x, y, x+math.sin(a)*1.8*s, y-math.cos(a)*1.4*s, col)

# ------------------------------------------------------------------ FLUFFBUD: a dandelion-puff bunny (play)
FLUFF = {'puff': ramp((250, 244, 252)), 'puff2': ramp((232, 222, 246)), 'fur': ramp((252, 214, 226)), 'inner': ramp((255, 170, 196)),
         'face': ramp((255, 238, 226)), 'flower': ramp((255, 214, 72)), 'ctr': ramp((240, 150, 40)), 'hoof': ramp((214, 160, 186)), 'leaf': ramp((120, 190, 96))}
def bunny_head(c, pose, dy, sx, sy, puff, puff2, ear, inner, facem, ear_back=0.0):
    droop = 0.5 if pose in ('sad', 'sleep') else -0.15 if pose == 'happy' else 0
    for s in (-1, 1):   # long ears
        a = s*(0.18+droop+ear_back)
        c.ellipse(24+s*5.5+math.sin(a)*6, 11.5+dy+droop*5, 2.9, 6.8, ear, rot=a)
        c.ellipse(24+s*5.5+math.sin(a)*6, 12+dy+droop*5, 1.4, 5.0, inner, rot=a, shade=False)
    for i in range(16):  # a dandelion clock of fluff around the head
        a = i*2*math.pi/16
        c.ellipse(24+math.cos(a)*10.5*sx, 22+dy+math.sin(a)*9.5*sy, 3.6, 3.6, puff2 if i % 2 else puff)
    c.ellipse(24, 22+dy, 10.5*sx, 9.5*sy, puff, nc=(24, 21+dy, 11, 10))
    c.ellipse(24, 25+dy, 7*sx, 5.6*sy, facem, shade=False)
def F_FLUFFBUD_V8(c, pose):
    dy, sx, sy = body_xf(pose)
    c.ellipse(36.5, 39+dy*0.5, 4.2, 4.0, 'puff')                               # puffball tail
    c.ellipse(24, 37+dy*0.5, 9.5*sx, 8*sy, 'puff', nc=(24, 36, 10, 9))
    c.ellipse(24, 38+dy*0.5, 5, 5, 'face', shade=False)
    bunny_head(c, pose, dy, sx, sy, 'puff', 'puff2', 'fur', 'inner', 'face')
    v8_blossom(c, 31.5, 13.5+dy, 2.6, 'flower', 'ctr', n=8, rr=0.5)                # a dandelion tucked by its ear
    c.ellipse(29.5, 15.5+dy, 1.8, 0.8, 'leaf', rot=0.6)
    if pose in ('idle', 'happy'):
        for (x, y) in ((8, 14), (41, 9), (44, 20)): v8_seed(c, x, y+dy*0.5, 0.9)
    face(c, pose, 24, 4.2, 23.5+dy, 24, 27.6+dy, style='round', mo='smile', blush=True)
form('FLUFFBUD', FLUFF, F_FLUFFBUD_V8)

# ------------------------------------------------------------------ ZIPPIT: FLUFFBUD's secret - a comet hare with a seed-fluff tail (games)
ZIP = {'fur': ramp((96, 200, 196)), 'fur2': ramp((70, 164, 170)), 'puff': ramp((250, 250, 244)), 'puff2': ramp((226, 240, 246)),
       'inner': ramp((255, 196, 140)), 'face': ramp((236, 250, 246)), 'gold': ramp((255, 210, 80)), 'hoof': ramp((60, 120, 130))}
def F_ZIPPIT_V8(c, pose):
    dy, sx, sy = body_xf(pose)
    wag = {'walk1': -1, 'walk2': 1}.get(FULL, 0) + (-1.5 if pose == 'happy' else 0)
    # the comet tail: a stream of dandelion fluff with seeds blowing off it
    for k, (x, y, r) in enumerate(((33, 39, 4.6), (37.5, 35, 5.2), (41, 29+wag*0.5, 4.8), (43, 22.5+wag, 4.0), (43.5, 16.5+wag, 3.0))):
        c.ellipse(x, y+dy*0.3, r, r, 'puff2' if k % 2 else 'puff')
    for (x, y) in ((44, 10), (39, 7), (46, 4)): FULL in ('idle', 'walk1', 'walk2') and v8_seed(c, x, y+wag, 0.9)
    c.poly(star(43, 16.5+wag+dy*0.3, 2.4, 1.0, 4), 'gold')
    c.ellipse(24, 37+dy*0.5, 8.5*sx, 7.5*sy, 'fur', nc=(24, 36, 9, 8))
    c.ellipse(24, 38+dy*0.5, 4.5, 5, 'face', shade=False)
    droop = 0.5 if pose in ('sad', 'sleep') else 0
    for s in (-1, 1):   # long ears swept back like a racing hare
        a = s*(0.45+droop)
        c.ellipse(24+s*4+math.sin(a)*7, 12+dy+droop*4, 2.6, 7, 'fur', rot=a)
        c.ellipse(24+s*4+math.sin(a)*7, 12.5+dy+droop*4, 1.2, 5.2, 'inner', rot=a, shade=False)
    c.ellipse(24, 22+dy, 9.5*sx, 8.5*sy, 'fur', nc=(24, 21+dy, 10, 9))
    for s in (-1, 1): c.ellipse(24+s*8.5, 25+dy, 3.2, 2.6, 'puff')             # fluffy cheeks
    c.ellipse(24, 26+dy, 5.5, 4, 'face', shade=False)
    c.poly(star(24, 15+dy, 2.0, 0.8, 4), 'gold')                                # a little comet mark
    face(c, pose, 24, 4.0, 22+dy, 24, 27.5+dy, style='sharp', mo='grin' if pose == 'idle' else 'smile', blush=True)
form('ZIPPIT', ZIP, F_ZIPPIT_V8)

# ------------------------------------------------------------------ FRILLO: an axolotl with fern-frond gills (the all-rounder)
FRIL = {'body': ramp((252, 186, 196)), 'body2': ramp((236, 150, 170)), 'belly': ramp((255, 228, 222)), 'frond': ramp((132, 196, 112)),
        'frond2': ramp((255, 140, 168)), 'spot': ramp((228, 128, 150)), 'leaf': ramp((120, 190, 96))}
def axolotl(c, pose, dy, sx, sy, body, body2, belly, frond, frond2, perk=0.0):
    wag = {'walk1': -0.25, 'walk2': 0.25}.get(FULL, 0) + (-0.3 if pose == 'happy' else 0)
    tube(c, [(30, 41, 3.4), (37, 39, 3.0), (42, 33+wag*5, 2.2), (43, 28+wag*7, 1.2)], body, (36, 37, 8, 7))
    c.ellipse(41.5, 33+wag*5, 1.6, 4.5, body2, rot=0.5)                          # tail fin
    c.ellipse(24, 37+dy*0.5, 9.5*sx, 8*sy, body, nc=(24, 36, 10, 9))
    c.ellipse(24, 38+dy*0.5, 5.5, 5.5, belly, shade=False)
    droop = 0.35 if pose in ('sad', 'sleep') else -0.25 if pose == 'happy' else 0
    for s in (-1, 1):   # three fern fronds a side
        for j, a0 in enumerate((-0.55, 0.0, 0.55)):
            a = (math.pi if s < 0 else 0) + s*(a0+droop+perk)
            x0, y0 = 24+s*10, 21+dy; L = 7 if j == 1 else 6
            pts = [(x0+math.cos(a)*L*t, y0+math.sin(a)*L*t) for t in (0, 0.35, 0.7, 1.0)]
            c.line(pts[0][0], pts[0][1], pts[-1][0], pts[-1][1], (96, 150, 80), 1.0)
            for k, (px, py) in enumerate(pts[1:]):
                for q in (-1, 1): c.ellipse(px+math.cos(a+q*1.2)*1.3, py+math.sin(a+q*1.2)*1.3, 1.3, 0.8, frond if k < 2 else frond2, rot=a+q*1.2)
            c.ellipse(pts[-1][0], pts[-1][1], 1.3, 1.3, frond2)
    c.ellipse(24, 22+dy, 12*sx, 9*sy, body, nc=(24, 21+dy, 12, 9))
def F_FRILLO_V8(c, pose):
    dy, sx, sy = body_xf(pose)
    axolotl(c, pose, dy, sx, sy, 'body', 'body2', 'belly', 'frond', 'frond2')
    for (x, y) in ((17, 18), (31, 17.5), (19.5, 15.5)): c.ellipse(x, y+dy, 1.1, 0.9, 'spot', shade=False)
    c.ellipse(24, 14.4+dy, 2.6, 1.2, 'leaf', rot=-0.3)                           # a little leaf stuck on its head
    face(c, pose, 24, 5.6, 22+dy, 24, 26.5+dy, style='round', mo='smile', blush=True)
form('FRILLO', FRIL, F_FRILLO_V8)

# ------------------------------------------------------------------ NESTLE: FRILLO's secret - its fronds woven into a cozy nest (a full home)
NEST = {'body': ramp((246, 196, 214)), 'body2': ramp((226, 160, 186)), 'belly': ramp((255, 236, 232)), 'frond': ramp((150, 196, 110)),
        'frond2': ramp((176, 128, 92)), 'twig': ramp((160, 118, 80)), 'twig2': ramp((126, 90, 62)), 'egg': ramp((176, 214, 246)), 'spot': ramp((120, 160, 210)),
        'scarf': ramp((236, 120, 104))}
def F_NESTLE_V8(c, pose):
    dy, sx, sy = body_xf(pose)
    axolotl(c, pose, dy, sx, sy, 'body', 'body2', 'belly', 'frond', 'frond2', perk=-0.45)
    # a woven nest on its head, an egg tucked inside
    c.ellipse(24, 7.5+dy, 3.0, 3.6, 'egg'); c.ellipse(23, 6.5+dy, 0.7, 0.6, 'spot', shade=False); c.ellipse(25.2, 8.4+dy, 0.6, 0.5, 'spot', shade=False)
    c.ellipse(24, 11.5+dy, 8.5, 3.4, 'twig', nc=(24, 11+dy, 9, 4))
    for k in range(7): c.line(16.5+k*2.4, 10.2+dy+(k % 2), 18.5+k*2.4, 13+dy-(k % 2), (110, 78, 54), 0.8)
    for s in (-1, 1): c.line(24+s*7, 10+dy, 24+s*10, 7.5+dy, (126, 90, 62), 0.9)
    c.ellipse(24, 29.5+dy*0.5, 9, 2.4, 'scarf')                                 # a knitted scarf: home comforts
    c.ellipse(30.5, 32+dy*0.5, 1.8, 3.4, 'scarf', rot=0.3)
    face(c, pose, 24, 5.4, 22+dy, 24, 26.5+dy, style='lash', mo='smile', blush=True)
form('NESTLE', NEST, F_NESTLE_V8)

# ------------------------------------------------------------------ SUNPUFF: OAKHART's secret - a sunny fawn with a sunflower crown (time outside)
SUN = {'fur': ramp((234, 170, 92)), 'fur2': ramp((204, 136, 70)), 'cream': ramp((255, 238, 204)), 'petal': ramp((255, 206, 60)), 'petal2': ramp((255, 168, 50)),
       'disc': ramp((150, 96, 52)), 'leaf': ramp((112, 186, 84)), 'twig': ramp((140, 100, 66)), 'hoof': ramp((110, 74, 52)), 'inner': ramp((255, 196, 170))}
def F_SUNPUFF_V8(c, pose):
    dy, sx, sy = body_xf(pose); lift = -1 if pose == 'happy' else 1 if pose in ('sad', 'sleep') else 0
    for i in range(16):   # a sunflower ring framing its head
        a = i*2*math.pi/16
        c.ellipse(24+math.cos(a)*11, 18.5+dy+math.sin(a)*10, 2.4, 4.2, 'petal2' if i % 2 else 'petal', rot=a+math.pi/2)
    for s in (-1, 1):     # little oak-twig antlers through the petals
        tube(c, [(24+s*4, 12+dy, 1.1), (24+s*7, 5.5+dy+lift, 0.9)], 'twig', (24+s*6, 8, 3, 4))
        c.ellipse(24+s*8.5, 4.5+dy+lift, 2.4, 1.3, 'leaf', rot=s*0.5)
    for s in (-1, 1):
        c.ellipse(24+s*9.5, 17+dy, 4.4, 2.0, 'fur', rot=s*0.3); c.ellipse(24+s*9.7, 17.2+dy, 2.6, 0.8, 'inner', rot=s*0.3, shade=False)
    c.ellipse(24, 37+dy*0.5, 11*sx, 8*sy, 'fur', nc=(24, 35, 12, 9))
    for (x, y) in ((15, 35), (17, 39), (33, 35), (31, 39), (14, 38.5), (34, 38.5)): c.ellipse(x, y+dy*0.5, 0.9, 0.9, 'cream', shade=False)   # sun dapples
    c.ellipse(24, 36+dy*0.5, 5, 5.5, 'cream', shade=False)
    for s in (-1, 1): c.ellipse(24+s*7, 28.5+dy, 4.2, 2.0, 'leaf', rot=s*0.4)   # a collar of sunflower leaves
    c.ellipse(24, 19.5+dy, 8*sx, 7*sy, 'fur', nc=(24, 19+dy, 9, 8))
    c.ellipse(24, 24+dy, 4.4, 3.0, 'cream', shade=False)
    c.ellipse(24, 22.8+dy, 1.4, 0.9, 'hoof', shade=False)
    face(c, pose, 24, 4, 19+dy, 24, 26+dy, style='round', mo='smile', blush=True)
form('SUNPUFF', SUN, F_SUNPUFF_V8)

# ------------------------------------------------------------------ GLIMMER: DUNEROSE's secret - its cactus blooms glow at night (a garden + love)
GLIM = {'fur': ramp((150, 156, 226)), 'fur2': ramp((116, 120, 196)), 'cream': ramp((236, 236, 255)), 'cac': ramp((96, 196, 176)),
        'flower': ramp((255, 252, 220)), 'ctr': ramp((255, 226, 110)), 'glow': ramp((255, 246, 150)), 'nose': ramp((70, 60, 110)), 'moon': ramp((255, 236, 160))}
def firefly(c, x, y):
    c.ellipse(x, y, 1.6, 1.6, 'glow', shade=False); c.dot(x, y, (255, 255, 240))
def F_GLIMMER_V8(c, pose):
    dy, sx, sy = body_xf(pose); droop = 0.35 if pose in ('sad', 'sleep') else -0.2 if pose == 'happy' else 0
    wag = (-0.4 if pose == 'happy' else 0.3 if pose in ('sad', 'sleep') else 0) + {'walk1': -0.2, 'walk2': 0.2}.get(FULL, 0)
    tube(c, [(30, 41, 3.0), (37, 40, 4.4), (41, 33+wag*6, 4.0), (40, 26+wag*8, 2.6)], 'fur', (37, 35, 7, 8))
    v8_blossom(c, 40, 25+wag*8, 3.2, 'flower', 'ctr', n=6)                        # a lantern bloom on its tail
    c.ellipse(24, 37+dy*0.5, 10.5*sx, 8.5*sy, 'fur', nc=(24, 36, 11, 10))
    c.ellipse(24, 37+dy*0.5, 5, 5.5, 'cream', shade=False)
    for s in (-1, 1): cactus_ear(c, 24+s*10.5, 10+dy, 5, 10, s*(0.45+droop))
    for s in (-1, 1): v8_blossom(c, 24+s*10.5, 3.5+dy+droop*8, 3.4, 'flower', 'ctr', n=6)   # night-blooming flowers, open wide
    for i in range(5):   # a collar of soft glowing buds
        a = math.pi*(0.15+0.7*i/4); c.ellipse(24-math.cos(a)*10.5, 28.5+dy+math.sin(a)*2.5, 1.6, 1.6, 'glow', shade=False)
    c.ellipse(24, 20+dy, 9*sx, 7.5*sy, 'fur', nc=(24, 19+dy, 10, 8))
    c.ellipse(24, 24+dy, 4.6, 3.2, 'cream', shade=False)
    c.ellipse(24, 22.6+dy, 1.4, 0.9, 'nose', shade=False)
    c.ellipse(24, 14.8+dy, 1.8, 1.8, 'moon', shade=False); c.ellipse(24.9, 14.2+dy, 1.3, 1.3, 'fur', shade=False)   # a crescent moon mark
    if pose != 'sleep':
        for (x, y) in ((7, 22), (41, 13), (9, 33), (44, 40)): firefly(c, x, y+dy*0.3)
    face(c, pose, 24, 4.3, 19+dy, 24, 25.5+dy, style='lash', mo='smile', blush=True, light=(255, 246, 170))
form('GLIMMER', GLIM, F_GLIMMER_V8)

# ------------------------------------------------------------------ STARWING: the capstone - the original soft blue star, brought up to date
# (same five-pointed body, gold tips and big eyes; now chubbier with soft depth, feather wings, arms and legs, a chest star)
STARB = {'blue': ramp((110, 170, 250)), 'blue2': ramp((88, 140, 226)), 'wing': ramp((186, 220, 255)), 'gold': ramp((255, 210, 70)), 'bel': ramp((214, 234, 255))}
def F_STARWING_V8(c, pose):
    dy, sx, sy = body_xf(pose); rot = -math.pi/2+(0.14 if pose == 'happy' else -0.08 if pose == 'sad' else 0)
    fl = (-0.6 if pose == 'happy' else 0.3 if pose in ('sad', 'sleep') else 0) + {'walk1': -0.2, 'walk2': 0.2}.get(FULL, 0)
    for s in (-1, 1):   # little feather wings behind it
        for j, (ox, oy, a, L) in enumerate(((11.5, 32, 0.25, 5.6), (13, 36, 0.7, 4.8), (11, 39.5, 1.15, 3.6))):
            c.ellipse(24+s*ox, oy+dy, L, 2.2, 'wing' if j != 1 else 'blue2', rot=-s*(a+fl))
    pts = star(24, 28+dy, 19, 12, rot=rot)                                      # a slightly chubbier star
    c.poly(pts, 'blue', nc=(24, 28+dy, 17, 17))
    for i in range(0, 10, 2):                                                     # gold star tips
        x, y = pts[i]; x2, y2 = 24+(x-24)*0.85, 28+dy+(y-28-dy)*0.85
        c.ellipse(x2, y2, 2.5, 2.5, 'gold'); c.dot(x2-0.8, y2-0.8, (255, 250, 220))        # round gold tips, as before
    c.ellipse(24, 32.5+dy, 6.5, 4.6, 'bel', shade=False)
    c.poly(star(24, 34.2+dy, 2.2, 0.9, 5), 'gold')                                # a tiny star on its chest
    for (x, y) in ((17, 22), (31, 24), (19, 37), (29.5, 37.5)): c.dot(x, y+dy, (255, 236, 150))
    face(c, pose, 24, 5, 27+dy, 24, 31.4+dy, style='big', mo='smile', blush=True)
form('STARWING', STARB, F_STARWING_V8)

# ------------------------------------------------------------------ EGG: a star-seed (fallen with a shooting star)
SEEDEGG = {'egg': ramp((250, 242, 222)), 'glow': ramp((255, 248, 200)), 'gold': ramp((255, 206, 90)), 'blue': ramp((150, 176, 246)), 'leaf': ramp((120, 196, 104))}
def F_EGG_V8(c, pose):
    tilt = {'idle': 0, 'blink': -0.10, 'happy': 0.12, 'sleep': 0, 'eat': 0, 'sad': -0.05}[pose]+{'walk1': -0.1, 'walk2': 0.1}.get(FULL, 0)
    hop = -2 if pose == 'happy' else 0
    c.ellipse(24, 27+hop, 14, 18, 'egg', rot=tilt)
    recolor(c, lambda u, v, m: m == 'egg' and (u-24)**2/120+(v-14-hop)**2/40 < 1, 'glow')      # a soft glow at the top
    for (x, y, r, m) in ((18, 22, 2.0, 'gold'), (30, 18, 1.6, 'blue'), (31, 31, 2.2, 'gold'), (17, 34, 1.5, 'blue'), (25, 39, 1.8, 'gold'), (24, 26, 1.3, 'blue')):
        c.poly(star(x+(y-27)*-tilt, y+hop, r*1.5, r*0.55, 4), m)               # star speckles
    c.line(24, 9.5+hop, 25, 6.5+hop, (90, 150, 80), 1.0); c.ellipse(26.6, 6.6+hop, 2.2, 1.1, 'leaf', rot=-0.4)   # a sprout tip peeking out
    if pose == 'sad':
        for (dx, dy) in ((0, 0), (1, 1), (0, 2), (1, 3), (2, 4), (1, 5)): c.dot(20+dx, 16+dy, OUTLINE)
    for (x, y) in ((7, 14), (41, 18), (9, 38), (40, 40)): c.dot(x, y+hop, (255, 240, 170))
form('EGG', SEEDEGG, F_EGG_V8)


# ------------------------------------------------------------------ v8: boy / girl touches (mockup: gender_mock.py)
# Each form gets one small difference. The boy touch is baked into the form itself (FORMS[name]); the girl version is
# an extra form NAME~G that critgen.py renders after the 23 base forms (CR_GIRL maps a form to its girl version).
def bow(c, x, y, col='bow', s=1.0):
    c.ellipse(x-1.7*s, y, 1.6*s, 1.2*s, col, rot=0.3); c.ellipse(x+1.7*s, y, 1.6*s, 1.2*s, col, rot=-0.3); c.ellipse(x, y, 0.8*s, 0.8*s, col)
def dots(c, pts, col, r=1.2): [c.ellipse(x, y, r, r, col) for (x, y) in pts]

PINK = ramp((255, 150, 190)); BLUE = ramp((120, 170, 240))
# per form: (boy, girl), each (recolours {material: ramp}, accessory draw or None, a few words)
T = {
 'BLIPLET':    (({}, None, ''), ({'bow': PINK}, lambda c, p, dy: bow(c, 24, 23.5+dy), 'a bow on its sprout')),
 'SPROUTLE':   (({'bud': ramp((246, 214, 100))}, None, 'a yellow bud'), ({}, None, 'a pink bud')),
 'BLOOMLE':    (({'petal': ramp((140, 196, 255))}, None, 'blue petals'), ({}, None, 'pink petals')),
 'THORNLE':    (({}, None, ''), ({'clip': ramp((230, 70, 96))}, lambda c, p, dy: dots(c, ((31, 19+dy), (32.4, 20.2+dy), (30.6, 20.6+dy)), 'clip'), 'a berry clip')),
 'OAKHART':    (({}, None, ''), ({'bl': PINK, 'ctr2': ramp((255, 220, 110))}, lambda c, p, dy: [v8_blossom(c, x, y+dy, 1.8, 'bl', 'ctr2') for (x, y) in ((15.5, 8), (32.5, 8))], 'blossoms on its antlers')),
 'DUNEROSE':   (({'flower': ramp((255, 196, 70))}, None, 'golden blooms'), ({}, None, 'pink blooms')),
 'FLUFFBUD': (({}, None, ''), ({'bow': PINK}, lambda c, p, dy: bow(c, 17.5, 15+dy), 'a ribbon on its ear')),
 'FRILLO':   (({'frond2': ramp((96, 200, 210))}, None, 'teal frond tips'), ({}, None, 'pink frond tips')),
 'THORNHOG':   (({}, None, 'blackberries'), ({'berry': ramp((226, 60, 90))}, None, 'raspberries')),
 'SMOGWOOL':   (({}, None, ''), ({'bow': ramp((214, 150, 190))}, lambda c, p, dy: bow(c, 30, 20+dy, s=0.9), 'a (grubby) bow')),
 'GUNKBILL':   (({}, None, ''), ({'sp': ramp((246, 222, 190))}, lambda c, p, dy: dots(c, ((20, 19+dy), (24, 17.6+dy), (28, 19+dy)), 'sp'), 'spots on its head')),
 'SUNPUFF':  (({}, None, 'gold petals'), ({'petal': ramp((255, 150, 80)), 'petal2': ramp((250, 110, 70))}, None, 'sunset petals')),
 'GLIMMER':  (({}, None, 'white blooms'), ({'flower': ramp((255, 210, 236))}, None, 'pink blooms')),
 'ZIPPIT':   (({}, None, 'a gold star'), ({'gold': ramp((255, 150, 200))}, None, 'a pink star')),
 'NESTLE':   (({'scarf': ramp((110, 150, 230))}, None, 'a blue scarf'), ({}, None, 'a red scarf')),
 'STARWING': (({}, None, 'gold tips'), ({'gold': ramp((255, 176, 196))}, None, 'rose-gold tips')),
 'PETALAMB':   (({'petal': ramp((255, 226, 110))}, None, 'buttercups'), ({}, None, 'cherry blossom')),
 'LILYBILL':   (({'petal': ramp((255, 250, 232))}, None, 'a white lily'), ({}, None, 'a pink lily')),
 'JAMBLE':     (({'berry': ramp((150, 70, 170))}, None, 'blackberry crown'), ({}, None, 'raspberry crown')),
}
GIRL_FORMS = []
def _gender_touches():
    for name, ((brec, bacc, _), (grec, gacc, _2)) in T.items():
        ramps, draw = FORMS[name]
        def wrap(draw, accf):
            if not accf: return draw
            def d(c, pose):
                draw(c, pose); accf(c, pose, body_xf(pose)[0])
            return d
        base = dict(ramps); girl = dict(ramps)
        girl.update(grec); base.update(brec)
        form(name+'~G', girl, wrap(draw, gacc)); GIRL_FORMS.append(name)
        form(name, base, wrap(draw, bacc))
