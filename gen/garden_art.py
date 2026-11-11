# Starfall Meadow v8.1 B2: moonflower and night-visitor sprites (one shared 36-colour palette).
# Each sprite is a list of frames; each frame is rows of characters. '.' is see-through.

PAL = {   # char: (r,g,b) 8-bit, rounded to the GBA's 5 bits on export
 'k': (24, 20, 40),    # outline
 'd': (70, 52, 44),    # dark brown
 'b': (122, 92, 70),   # mid brown
 't': (198, 166, 128), # tan
 'c': (236, 220, 188), # cream
 'w': (246, 246, 252), # white
 'y': (250, 222, 88),  # yellow (eyes, flower centres)
 'o': (236, 140, 60),  # orange
 'p': (232, 148, 160), # pink
 'g': (40, 96, 62),    # dark green
 'G': (84, 164, 92),   # green
 'L': (166, 224, 132), # light green
 'u': (186, 246, 208), # luna light
 'U': (112, 196, 156), # luna mid
 'f': (244, 252, 150), # firefly glow
 'v': (56, 48, 92),    # bat dark
 'V': (98, 88, 144),   # bat
 'r': (208, 118, 86),  # shell
 'R': (142, 72, 56),   # shell dark
 's': (196, 184, 152), # snail body
 'x': (200, 206, 222), # silver
 'X': (140, 146, 172), # silver shade
 'z': (78, 84, 116),   # slate
 'n': (22, 26, 60),    # moon lily: the moon's shadowed side
 '5': (52, 56, 62), '6': (62, 67, 75), '7': (70, 76, 85),   # moon lily: its soft glow, darkest to lightest
 'C': (200, 220, 255), # moon lily: the crescent
 'e': (148, 170, 220), # moon lily: the crescent's inner edge
 'N': (32, 38, 80), 'M': (44, 54, 102),   # moon lily: its shadowed side, lighter toward the rim (earthshine)
 'Y': (255, 240, 160), # moon lily: its heart
 'S': (88, 60, 44),    # soil
 # petals: '1' light, '2' mid, recoloured per flower colour
}
FLOWER_COLS = {   # name: (light, mid)
 'SILVER': ((236, 240, 255), (168, 178, 216)),
 'ROSE':   ((255, 182, 206), (226, 108, 150)),
 'GOLD':   ((255, 232, 124), (236, 168, 52)),
 'SKY':    ((172, 216, 255), (90, 150, 232)),
 'MINT':   ((182, 255, 216), (88, 210, 160)),
}

def F(s):   # a frame from a block of text
    rows = [r for r in s.strip('\n').split('\n')]
    w = max(len(r) for r in rows)
    return [r.ljust(w, '.') for r in rows]

# ---------------------------------------------------------------- flowers (growth: 4 nights)
MOUND = F('''
...SSSSSSS...
.SSSdSSSSdSS.
SSdSSSSSSSdSS
''')
SEED = F('''
......c......
.....kyk.....
...SSkkkSS...
.SSSdSSSSdSS.
SSdSSSSSSSdSS
''')
SPROUT = F('''
.............
..........
....kk.kk....
...kGLkLGk...
....kGgGk....
.....kgk.....
...SSkgkSS...
.SSSdSSSSdSS.
SSdSSSSSSSdSS
''')
BUD = F('''
......k......
.....k2k.....
....k212k....
....k222k....
.....kgk.....
...kk.g......
..kGLkg.kk...
...kkkgkLGk..
......gkkk...
......g......
...SSSgSSS...
.SSSdSSSSdSS.
SSdSSSSSSSdSS
''')
BLOOM_HEAD = [F('''
....k.k....
...k1k1k...
.kk11k11kk.
k111k2k111k
k1112y2111k
.k12yyy21k.
k1112y2111k
k111k2k111k
.kk11k11kk.
...k1k1k...
....k.k....
'''), F('''
....kk.....
...k11kk...
.kk111k1kk.
k111k2k111k
k1112y2111k
.k12yyy21k.
k1112y2111k
k111k2k111k
.kk1k111kk.
...kk11k...
.....kk....
''')]
STEM = F('''
.....g.....
.....g.....
..kk.g.....
.kGLkg.....
..kkkgkk...
.....gLGk..
.....gkk...
.....g.....
''')

# rare forms
STARBLOOM = [F('''
.....k.....
....kwk....
....k1k....
kkkk111kkkk
kw11yyy11wk
.k1yywyy1k.
..k1yyy1k..
..k11k11k..
.k11k.k11k.
.kwk...kwk.
.kk.....kk.
'''), F('''
.....k.....
....kwk....
....k1k....
kkkk111kkkk
k111yyy111k
.k1yyyyy1k.
..k1yyy1k..
..k11k11k..
.k11k.k11k.
.k1k...k1k.
.kk.....kk.
''')]
import math as _m3
def _moonlily(t):
    """as first drawn: a dark round bloom rimmed with a soft glow, a thin crescent moon on one side (the shadowed
    rest of the moon only just shows), and a little golden sparkle at its heart"""
    out=[]
    for y in range(15):
        r=''
        for x in range(15):
            d=_m3.hypot(x-7,y-7)
            if d>7.3: r+='.'
            elif d>6.4: r+='q'
            elif d>5.6: r+='Q' if (x+y<15 or t==1) else 'q'
            else:
                inA=_m3.hypot(x-7,y-7)<=5.4; inB=_m3.hypot(x-8.7,y-6.6)<=5.0
                r+=('1' if _m3.hypot(x-8.7,y-6.6)>5.8 else '2') if (inA and not inB) else 'n'
        out.append(r)
    out=[list(q) for q in out]
    cx,cy=9,7
    out[cy][cx]='w' if t==1 else 'y'
    for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)): out[cy+dy][cx+dx]='y'
    if t==1:
        for dx,dy in ((2,0),(-2,0),(0,2),(0,-2)): out[cy+dy][cx+dx]='y'
    return [''.join(q) for q in out]
MOONLILY = [F('''
.....555.....
...5777775...
..57CeMMN65..
.57CeMNMNN65.
.7CCeNNnnnN6.
57CeMNNnYnN65
57CeMNNYYYN65
57CCeMNNYNN65
.7CCeeMNNMM6.
.56CCeeMMM65.
..56CCCee65..
...5666665...
.....555.....
'''), F('''
.....555.....
...5777775...
..57CeMMN65..
.57CeMNMNN65.
.7CCeNNnnnN6.
57CeMNNnYnN65
57CeMNNYwYN65
57CCeMNNYNN65
.7CCeeMNNMM6.
.56CCeeMMM65.
..56CCCee65..
...5666665...
.....555.....
''')]   # (v4: smaller, a fuller crescent, depth in its shadowed side)
TWIN = F('''
..kkk...kkk..
.k1A1k.k3B3k.
k1AyA1k3ByB3k
.k1A1k.k3B3k.
..kgk...kgk..
...g.....g...
...kg...gk...
....kg.gk....
.....kgk.....
......g......
''')   # A/1: rose, B/3: gold (light/mid)
import math as _m
def _aurora():
    out=[]
    for y,row in enumerate(BLOOM_HEAD[0]):
        r=''
        for x,ch in enumerate(row):
            if ch in '12':
                a=(_m.atan2(y-5,x-5)/(2*_m.pi)+0.25)%1.0; r+='abcde'[int(a*5)%5]
            else: r+=ch
        out.append(r)
    return out
AURORA = _aurora()   # a-e: five petal sectors in the five colours, turning

# ---------------------------------------------------------------- visitors
MOTH = [F('''
...k.....k...
....k...k....
.kkk.kdk.kkk.
kbtttkdktttbk
kbtcttdttctbk
.kbtttdtttbk.
..kbbkdkbbk..
...kk.d.kk...
......k......
'''), F('''
....k...k....
.....k.k.....
...kkkdkkk...
..kbttdttbk..
..kbtcdctbk..
...kbtdtbk...
...kbkdkbk...
....k.d.k....
......k......
'''), F('''
.....k.k.....
.....k.k.....
.....kdk.....
....kbdbk....
....ktdtk....
....kbdbk....
....kbdbk....
.....kdk.....
......k......
''')]
MOTH_FR = [0, 1, 2, 1]

FIREFLY = [F('''
.k.k.
..z..
.XzX.
..f..
.fyf.
..f..
'''), F('''
.k.k.
..z..
.XzX.
.fyf.
fyyyf
.fyf.
'''), F('''
.k.k.
..z..
XXzXX
..f..
.....
.....
'''), F('''
.k.k.
..z..
.XzX.
..k..
.....
.....
''')]
FIREFLY_FR = [0, 1, 0, 2, 3, 3, 2]

FROG_V1 = [F('''
...kkk...kkk...
..kwwwk.kwwwk..
..kwkwk.kwkwk..
.kGGGGGGGGGGGk.
kGGLGGGGGGGLGGk
kGGGkkkkkkkGGGk
.kGGLLLLLLLGGk.
.kGkLLLLLLLkGk.
kGGkLLLLLLLkGGk
kLkGkkkkkkkGkLk
.k.kk.....kk.k.
'''), F('''
...kkk...kkk...
..kwwwk.kwwwk..
..kwkwk.kwkwk..
.kGGGGGGGGGGGk.
kGGLGGGGGGGLGGk
kGGGkkkkkkkGGGk
.kGLLLLLLLLLGk.
kGkLLLcccLLLkGk
kGGkLLcccLLkGGk
kLkGkkkkkkkGkLk
.k.kk.....kk.k.
'''), F('''
...kkk...kkk...
..kGGGk.kGGGk..
..kkkkk.kkkkk..
.kGGGGGGGGGGGk.
kGGLGGGGGGGLGGk
kGGGkkkkkkkGGGk
.kGGLLLLLLLGGk.
.kGkLLLLLLLkGk.
kGGkLLLLLLLkGGk
kLkGkkkkkkkGkLk
.k.kk.....kk.k.
'''), F('''
...kkk...kkk...
..kwwwk.kwwwk..
..kwkwk.kwkwk..
.kGGGGGGGGGGGk.
kGGLGGGGGGGLGGk
kGGGkkkkkkkGGGk
.kGGLLLLLLLGGk.
..kkLLLLLLLkk..
.kGkkLLLLLkkGk.
kGk..kkkkk..kGk
kk...........kk
''')]
FROG_FR = [0, 0, 1, 0, 2, 0, 3, 3]   # sit, puff, blink, hop
FROG_HOP = [0, 0, 0, 0, 0, 0, -3, -2]

HEDGEHOG_V1 = [F('''
...k.k.k.k.......
..kdkdkdkdk......
.kdbdbdbdbdkk....
kdbdbdbdbdbkttk..
kbdbdbdbdbktcktk.
kdbdbdbdbdkttttpk
kbdbdbdbdbkttttkk
.kddddddddkkttk..
..kt.kt..kt.kk...
..kk.kk..kk......
'''), F('''
...k.k.k.k.......
..kdkdkdkdk......
.kdbdbdbdbdkk....
kdbdbdbdbdbkttk..
kbdbdbdbdbktcktk.
kdbdbdbdbdkttttpk
kbdbdbdbdbkttttkk
.kddddddddkkttk..
...kt.kt.kt..kk..
...kk.kk.kk......
'''), F('''
...k.k.k.k.......
..kdkdkdkdk......
.kdbdbdbdbdkk....
kdbdbdbdbdbkttk..
kbdbdbdbdbktcktk.
kdbdbdbdbdkttttk.
kbdbdbdbdbkttttkpk
.kddddddddkkttkk.
..kt.kt..kt.kk...
..kk.kk..kk......
''')]
HEDGEHOG_FR = [0, 1, 0, 1, 2, 0, 2, 0]   # walk, sniff
HEDGEHOG_DX = [1, 1, 1, 1, 0, 0, 0, 0]

BAT = [F('''
k.................k
kVk.............kVk
kvVk....k.k....kVvk
.kvVk..kvvvk..kVvk.
.kvvVk.kyvyk.kVvvk.
..kvvVkvvvvvkVvvk..
...kvvvvvvvvvvvk...
....kk..kvk..kk....
'''), F('''
...................
...................
........k.k........
.kkk...kvvvk...kkk.
kVVVkk.kyvyk.kkVVVk
kvvVVVkvvvvvkVVVvvk
.kvvkvvvvvvvvvkvvk.
..kk.kk.kvk.kk.kk..
'''), F('''
...................
...................
........k.k........
.......kvvvk.......
.....kkkyvykkk.....
...kkVVvvvvvVVkk...
.kkVVvvkvvvkvvVVkk.
kVVvvkk.kvk.kkvvVVk
kvvk...........kvvk
kk...............kk
''')]
BAT_FR = [0, 1, 2, 1]

OWL_V1 = [F('''
.kk........kk.
.kbk......kbk.
.kbbkkkkkkbbk.
kbcccbbbbcccbk
kbcyyyccyyycbk
kbcykyccykycbk
kbcyyyccyyycbk
kbbccccooccbbk
kbdbbbbbbbbdbk
kbdbtbbbbtbdbk
kbdtbtbbtbtdbk
kbdbtbttbtbdbk
kbbdtbtbtbdbbk
.kbbdbtbtdbbk.
.kkbbbbbbbbkk.
..kook..kook..
'''), F('''
.kk........kk.
.kbk......kbk.
.kbbkkkkkkbbk.
kbcccbbbbcccbk
kbccccccccccbk
kbckkkcckkkcbk
kbccccccccccbk
kbbccccooccbbk
kbdbbbbbbbbdbk
kbdbtbbbbtbdbk
kbdtbtbbtbtdbk
kbdbtbttbtbdbk
kbbdtbtbtbdbbk
.kbbdbtbtdbbk.
.kkbbbbbbbbkk.
..kook..kook..
'''), F('''
.kk........kk.
.kbk......kbk.
.kbbkkkkkkbbk.
kbcccbbbbcccbk
kbcyyyccyyycbk
kbcyykccyykcbk
kbcyyyccyyycbk
kbbccccooccbbk
kbdbbbbbbbbdbk
kbdbtbbbbtbdbk
kbdtbtbbtbtdbk
kbdbtbttbtbdbk
kbbdtbtbtbdbbk
.kbbdbtbtdbbk.
.kkbbbbbbbbkk.
..kook..kook..
'''), F('''
.kk........kk....
.kbk......kbk....
.kbbkkkkkkbbk....
kbcccbbbbcccbk...
kbcyyyccyyycbk...
kbcykyccykycbk...
kbcyyyccyyycbk...
kbbccccooccbbk...
kbdbbbbbbbbdbkk.
kbdbtbbbbtbdbdbk
kbdtbtbbtbtdbtdk
kbdbtbttbtbdkbdk
kbbdtbtbtbdbbkbk
.kbbdbtbtdbbk.kk
.kkbbbbbbbbkk...
..kook..kook....
''')]
OWL_FR = [0, 0, 1, 0, 2, 2, 0, 3, 3, 0]

SNAIL = [F('''
..k.k...........
..kxk...........
...k.k..kkkk....
...kk..kRrrRk...
..ksk.kRrkkrRk..
..kssskrkrRkrRk.
..ksskkRkrrkrRk.
.kssskkRRkkrRRk.
kssssssskkkkkksk
.kkkkkkkkkkkkkk.
'''), F('''
.k..k...........
.kx.kx..........
..k.k...kkkk....
...kk..kRrrRk...
..ksk.kRrkkrRk..
..kssskrkrRkrRk.
..ksskkRkrrkrRk.
.kssskkRRkkrRRk.
kssssssskkkkkksk
.kkkkkkkkkkkkkk.
'''), F('''
.k.k............
.kxkx...........
..kk...kkkk.....
..kk..kRrrRk....
.ksk.kRrkkrRk...
.ksssrkrRkrRk...
.ksskRkrrkrRkk..
kssskRRkkrRRkssk
ssssssskkkkkssssk
.kkkkkkkkkkkkkkk.
''')]
SNAIL_FR = [0, 0, 1, 1, 0, 2, 2, 0]
SNAIL_DX = [0, 0, 0, 0, 0, -1, 0, 0]

CRICKET = [F('''
k.............
.k............
..k..kkkkkk...
..kkkgLLLLLkk.
.kGkgGLLLLLLGk
.kyGgGGGGGGGkk
..kkkkkkkkGk..
..k.k.k.kGGkk.
.k..k..kGk..kk
'''), F('''
.k............
..k...........
..k..kkk......
..kkkgLLkkk...
.kGkgGLLLLLkkk
.kyGgGGGGGGGkk
..kkkkkkkkGk..
..k.k.k.kGGkk.
.k..k..kGk..kk
'''), F('''
k.............
.k............
..k..kkkkkk...
..kkkgLLLLLkk.
.kGkgGLLLLLLGk
.kyGgGGGGGGGkk
..kkkkkkkkkkGk
.k..k..k...kGk
k...k...k...kk
''')]
CRICKET_FR = [0, 1, 0, 1, 0, 0, 2, 0]   # chirp (wing up), hop
CRICKET_HOP = [0, 0, 0, 0, 0, 0, -4, 0]

LUNA = [F('''
..k.........k..
...k.......k...
.kk.k.....k.kk.
kuuukk.k.kkuuuk
kuouuUkdkUuuouk
kuuuuUkdkUuuuuk
.kuuuUkdkUuuuk.
..kkuUkdkUukk..
..kuUUkdkUUuk..
..kuoUkdkUouk..
...kuUkdkUuk...
...kUk.k.kUk...
...kUk...kUk...
..kUk.....kUk..
..kk.......kk..
'''), F('''
...k.......k...
....k.....k....
...k.k...k.k...
..kuukk.kkuuk..
..kuouUkUuouk..
..kuuuUdUuuuk..
...kuuUdUuuk...
...kkuUdUukk...
...kuUUdUUuk...
...kuoUdUouk...
....kuUdUuk....
....kUkdkUk....
....kUk.kUk....
...kUk...kUk...
...kk.....kk...
'''), F('''
.......k.......
......k.k......
......kdk......
.....kuduk.....
.....kodok.....
.....kudUk.....
.....kUdUk.....
.....kudUk.....
.....kUdUk.....
.....kodok.....
.....kUdUk.....
.....kUkUk.....
.....kUkUk.....
....kUk.kUk....
....kk...kk....
''')]
LUNA_FR = [0, 1, 2, 1]

FOX = [F('''
...k..k...............
..kxkkxk..............
..kXxkXxk.............
..kxxxxxxk............
.kxkxxxxxxk...........
kxxxxxxxxxk...........
kkwwwxxxxxk...........
.kkwwwxxxk............
...kwwxxxxk...........
...kwwxxxxXk..........
...kwwxxxxxXk.........
...kwxxxxxxXXk........
...kwxxxxxxxXXk..kkk..
...kwxxkxxxxXXk.kXxxk.
...kxxkxxxxxXXkkXxxxwk
...kxkkxxxxxXkkXxxxwwk
...kzk.kzkxxkkXxxxwwwk
..kkzk.kzkkkXXxxxwwwk.
..kkkk.kkkkkkkkkkkkk..
'''), F('''
...k..k...............
..kxkkxk..............
..kXxkXxk.............
..kxxxxxxk............
.kxkxxxxxxk...........
kxxxxxxxxxk...........
kkwwwxxxxxk...........
.kkwwwxxxk............
...kwwxxxxk.......kk..
...kwwxxxxXk.....kwwk.
...kwwxxxxxXk...kxwwk.
...kwxxxxxxXXk..kxxwk.
...kwxxxxxxxXXk.kXxxk.
...kwxxkxxxxXXk.kXxk..
...kxxkxxxxxXXkkXxxk..
...kxkkxxxxxXkkXxxk...
...kzk.kzkxxkkXxxk....
..kkzk.kzkkkXXxk......
..kkkk.kkkkkkkk.......
'''), F('''
..k...k...............
.kxk.kxk..............
.kXk.kXxk.............
..kxxxxxxk............
.kxkxxxxxxk...........
kxxxxxxxxxk...........
kkwwwxxxxxk...........
.kkwwwxxxk............
...kwwxxxxk...........
...kwwxxxxXk..........
...kwwxxxxxXk.........
...kwxxxxxxXXk........
...kwxxxxxxxXXk..kkk..
...kwxxkxxxxXXk.kXxxk.
...kxxkxxxxxXXkkXxxxwk
...kxkkxxxxxXkkXxxxwwk
...kzk.kzkxxkkXxxxwwwk
..kkzk.kzkkkXXxxxwwwk.
..kkkk.kkkkkkkkkkkkk..
'''), F('''
...k..k...................
..kxkkxk..................
..kXxkXxk.................
..kxxxxxxk................
.kxkkxxxxxk...............
kxxxxxxxxxxkkkkkkk........
kkwwwxxxxxxxxxxxxxkk......
.kkwwwxxxxxxxxxxxxXXk.....
..kwwxxxxxxxxxxxxXXXXk.kk.
..kwwxxxxxxxxxxxXXXXXkkXxk
.kwwkxxxxxxxxxxXXXXXkXxxwk
kwwkkkkwkxxkkkkkkkkkXxxwwk
kkkk..kkkkkk.....kkkkkkkk.
''')]
FOX_FR = [0, 0, 1, 0, 2, 0, 0, 3, 3, 3]   # sit, tail swish, ear twitch, lie down for a doze


# v2 (Nov 9): rounder, more shading, bigger eyes
OWL = [F('''
..kk........kk..
.kdbk......kbdk.
.kdbbkkkkkkbbdk.
kdbbtbbbbbbtbbdk
kbtccccbbcccctbk
kbccyyyccyyyccbk
ktcywkyccywkyctk
kbcykkyccykkycbk
kbtcyyyooyyyctbk
kdbttttkkttttbdk
kdbtbbtbbtbbtbdk
kdbbtbbttbbtbbdk
kdbtbbtbbtbbtbdk
kddbtbbttbbtbddk
.kdbbtbbbbtbbdk.
.kddbbbbbbbbddk.
..kkddddddddkk..
..kook....kook..
'''), F('''
..kk........kk..
.kdbk......kbdk.
.kdbbkkkkkkbbdk.
kdbbtbbbbbbtbbdk
kbtccccbbcccctbk
kbccccccccccccbk
ktcccccccccccctk
kbckkkkcckkkkcbk
kbtccccoocccctbk
kdbttttkkttttbdk
kdbtbbtbbtbbtbdk
kdbbtbbttbbtbbdk
kdbtbbtbbtbbtbdk
kddbtbbttbbtbddk
.kdbbtbbbbtbbdk.
.kddbbbbbbbbddk.
..kkddddddddkk..
..kook....kook..
'''), F('''
..kk........kk..
.kdbk......kbdk.
.kdbbkkkkkkbbdk.
kdbbtbbbbbbtbbdk
kbtccccbbcccctbk
kbccyyyccyyyccbk
ktcwkyyccwkyyctk
kbckkyycckkyycbk
kbtcyyyooyyyctbk
kdbttttkkttttbdk
kdbtbbtbbtbbtbdk
kdbbtbbttbbtbbdk
kdbtbbtbbtbbtbdk
kddbtbbttbbtbddk
.kdbbtbbbbtbbdk.
.kddbbbbbbbbddk.
..kkddddddddkk..
..kook....kook..
'''), F('''
...kk........kk...
..kdbk......kbdk..
..kdbbkkkkkkbbdk..
.kdbbtbbbbbbtbbdk.
.kbtccccbbcccctbk.
.kbccyyyccyyyccbk.
.ktcywkyccywkyctk.
.kbcykkyccykkycbk.
.kbtcyyyooyyyctbk.
kddbttttkkttttbddk
kddbtbbtbbtbbtbddk
kddbbtbbttbbtbbddk
kddbtbbtbbtbbtbddk
.kddbtbbttbbtbddk.
..kdbbtbbbbtbbdk..
..kddbbbbbbbbddk..
...kkddddddddkk...
...kook....kook...
''')]
OWL_FR = [0, 0, 1, 0, 2, 2, 0, 3, 3, 0]   # sit, blink, look aside, fluff up
FROG = [F('''
..kkkk.....kkkk..
.kwwwwk...kwwwwk.
.kwwkwkkkkkwwkwk.
.kwkkwkGGGkwkkwk.
kGkwwkGLLLGkwwkGk
kGGkkGLLLLLGkkGGk
kGLGGGGGGGGGGGLGk
kgGGGGGGGGGGGGGgk
kgGkkkGGGGGkkkGgk
kgGGGkkkkkkkGGGgk
.kgGcccccccccGgk.
kGkgcccLcLcccgkGk
kLGkgcLLcLLcgkGLk
kkkkkkkkkkkkkkkkk
'''), F('''
..kkkk.....kkkk..
.kwwwwk...kwwwwk.
.kwwkwkkkkkwwkwk.
.kwkkwkGGGkwkkwk.
kGkwwkGLLLGkwwkGk
kGGkkGLLLLLGkkGGk
kGLGGGGGGGGGGGLGk
kgGGGGGGGGGGGGGgk
kgGkkkGGGGGkkkGgk
kgGGGkkkkkkkGGGgk
.kgGcccccccccGgk.
kGkgcccccccccgkGk
kLGkgcLpppLcgkGLk
kkkkkkkkkkkkkkkkk
'''), F('''
..kkkk.....kkkk..
.kGGGGk...kGGGGk.
.kGGGGkkkkkGGGGk.
.kkkkkkGGGkkkkkk.
kGGGGGGLLLGGGGGGk
kGGkkGLLLLLGkkGGk
kGLGGGGGGGGGGGLGk
kgGGGGGGGGGGGGGgk
kgGkkkGGGGGkkkGgk
kgGGGkkkkkkkGGGgk
.kgGcccccccccGgk.
kGkgcccLcLcccgkGk
kLGkgcLLcLLcgkGLk
kkkkkkkkkkkkkkkkk
'''), F('''
..kkkk.....kkkk..
.kwwwwk...kwwwwk.
.kwwkwkkkkkwwkwk.
.kwkkwkGGGkwkkwk.
kGkwwkGLLLGkwwkGk
kGGkkGLLLLLGkkGGk
kGLGGGGGGGGGGGLGk
kgGGGGGGGGGGGGGgk
kgGkkkGGGGGkkkGgk
kgGGGkkkkkkkGGGgk
..kgcccLcLcccgk..
.kGkgcLLcLLcgkGk.
kGk.kkkkkkkkk.kGk
kk.............kk
''')]   # (v3: smaller eyes, cheerful pupils)
FROG_FR = [0, 0, 1, 0, 2, 0, 3, 3]
FROG_HOP = [0, 0, 0, 0, 0, 0, -3, -2]
HEDGEHOG = [F('''
......k..k..k..k........
....kkdkkdkkdkkdkk......
...kddbbdbbdbbdbbtk.....
..kdbbdbbdbbdbbdbbk.....
.kdbdbbdbbdbbdbbdbkk....
.kddbbdbbdbbdbbdkttck...
kdbbdbbdbbdbbdbbkttcck..
kdbdbbdbbdbbdbbkttkcck..
kddbbdbbdbbdbbdkttccccck
.kdbbdbbdbbdbbdktpcccckk
..kdddddddddddkkttcck...
...kt.kt..kt..kt.kkk....
...kk.kk..kk..kk........
'''), F('''
......k..k..k..k........
....kkdkkdkkdkkdkk......
...kddbbdbbdbbdbbtk.....
..kdbbdbbdbbdbbdbbk.....
.kdbdbbdbbdbbdbbdbkk....
.kddbbdbbdbbdbbdkttck...
kdbbdbbdbbdbbdbbkttcck..
kdbdbbdbbdbbdbbkttkcck..
kddbbdbbdbbdbbdkttccccck
.kdbbdbbdbbdbbdktpcccckk
..kdddddddddddkkttcck...
....kt.kt.kt..kt........
....kk.kk.kk..kk........
'''), F('''
......k..k..k..k........
....kkdkkdkkdkkdkk......
...kddbbdbbdbbdbbtk.....
..kdbbdbbdbbdbbdbbk.....
.kdbdbbdbbdbbdbbdbkk....
.kddbbdbbdbbdbbdkttck...
kdbbdbbdbbdbbdbbkttcck..
kdbdbbdbbdbbdbbkttkccck.
kddbbdbbdbbdbbdkttcccccc
.kdbbdbbdbbdbbdktpcccckk
..kdddddddddddkkttcck...
...kt.kt..kt..kt.kkk....
...kk.kk..kk..kk........
''')]
HEDGEHOG_FR = [0, 1, 0, 1, 2, 0, 2, 0]

# moon lily, as first drawn: a dark night-blue bloom holding a crescent moon and a little star


VISITORS = [   # name, frames, sequence, rare
 ('MOTH', MOTH, MOTH_FR, 0), ('FIREFLY', FIREFLY, FIREFLY_FR, 0), ('FROG', FROG, FROG_FR, 0),
 ('HEDGEHOG', HEDGEHOG, HEDGEHOG_FR, 0), ('BAT', BAT, BAT_FR, 0), ('OWL', OWL, OWL_FR, 0),
 ('SNAIL', SNAIL, SNAIL_FR, 0), ('CRICKET', CRICKET, CRICKET_FR, 0),
 ('LUNA MOTH', LUNA, LUNA_FR, 1), ('SILVER FOX', FOX, FOX_FR, 1),
]
