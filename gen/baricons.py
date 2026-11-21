# Phase 3: the eight menu icons, redrawn at their full 24x24 size (the game drew 12x12 icons doubled).
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
# 'x' = icon colour, 'h' = its highlight, 's' = its shade, '.' = see-through.
import math
N = 24
def disc(cx, cy, r): return lambda x, y: (x-cx)**2+(y-cy)**2 <= r*r
def ell(cx, cy, rx, ry): return lambda x, y: ((x-cx)/rx)**2+((y-cy)/ry)**2 <= 1
def rell(cx, cy, rx, ry, a):
    ca, sa = math.cos(a), math.sin(a)
    return lambda x, y: (((x-cx)*ca+(y-cy)*sa)/rx)**2+((-(x-cx)*sa+(y-cy)*ca)/ry)**2 <= 1
def box(x0, y0, x1, y1, r=0):
    def f(x, y):
        if not (x0 <= x <= x1 and y0 <= y <= y1): return False
        if r:
            dx = max(x0+r-x, 0, x-(x1-r)); dy = max(y0+r-y, 0, y-(y1-r)); return dx*dx+dy*dy <= r*r
        return True
    return f
def poly(pts):
    def f(x, y):
        ins = False; j = len(pts)-1
        for i in range(len(pts)):
            xi, yi = pts[i]; xj, yj = pts[j]
            if ((yi > y) != (yj > y)) and x < (xj-xi)*(y-yi)/(yj-yi)+xi: ins = not ins
            j = i
        return ins
    return f
def U(*fs): return lambda x, y: any(f(x, y) for f in fs)
def M(a, *holes): return lambda x, y: a(x, y) and not any(h(x, y) for h in holes)
def seg(x0, y0, x1, y1, w):
    def f(x, y):
        dx, dy = x1-x0, y1-y0; L = dx*dx+dy*dy; t = max(0, min(1, ((x-x0)*dx+(y-y0)*dy)/L))
        return (x-x0-dx*t)**2+(y-y0-dy*t)**2 <= w*w
    return f

ICONS = {
  'apple': U(M(ell(12, 14.5, 9.5, 8.8), ell(12, 5.4, 2.6, 1.6)), seg(11.8, 6.5, 12.8, 1.2, 0.9), rell(16.6, 3.6, 4.2, 1.9, -0.45)),
  'bulb': M(U(disc(12, 9.5, 7.6), box(8.5, 14, 15.5, 18.5), box(9.5, 18.5, 14.5, 22.5, 1.5)),
            seg(8.6, 11.5, 10.3, 8.4, 0.5), seg(10.3, 8.4, 12, 11.5, 0.5), seg(12, 11.5, 13.7, 8.4, 0.5), seg(13.7, 8.4, 15.4, 11.5, 0.5), box(8, 17.2, 16, 17.9), box(9, 19.6, 15, 20.3)),
  'ball': M(disc(12, 12, 10.6), poly([(12, 8), (15.8, 10.8), (14.4, 15.2), (9.6, 15.2), (8.2, 10.8)]),
            seg(12, 8, 12, 2, 0.5), seg(15.8, 10.8, 21.5, 9, 0.5), seg(14.4, 15.2, 18, 20.5, 0.5), seg(9.6, 15.2, 6, 20.5, 0.5), seg(8.2, 10.8, 2.5, 9, 0.5)),
  'meds': M(U(box(1.5, 6, 22.5, 22.5, 3), M(box(7.5, 1.5, 16.5, 7, 2), box(9.5, 3.5, 14.5, 7))), box(10, 9, 14, 20), box(6.5, 12.5, 17.5, 16.5)),
  'drop': M(U(lambda x, y: (y >= 11 and disc(10, 14, 8)(x, y)) or poly([(10, 1), (17.6, 12), (2.4, 12)])(x, y), disc(20, 20, 2.6),
              poly([(20, 15.5), (21, 19), (24.5, 20), (21, 21), (20, 24.5), (19, 21), (15.5, 20), (19, 19)])),
            seg(5.5, 14, 6.5, 18, 0.7)),
  'bars': U(box(1, 17, 4.5, 22.5, 0.8), box(7, 12.5, 10.5, 22.5, 0.8), box(13, 8, 16.5, 22.5, 0.8), box(19, 2.5, 22.5, 22.5, 0.8)),
  'heart': U(disc(7, 8, 6), disc(17, 8, 6), poly([(1.4, 10.5), (22.6, 10.5), (12, 22.5)])),
  'speech': M(U(box(1, 2, 23, 17, 4), poly([(5, 15), (11, 15), (4.5, 22.5)])), box(10.5, 4.5, 13.5, 11.5, 1), box(10.5, 13, 13.5, 15.5, 1)),
}
ORDER = ['apple', 'bulb', 'ball', 'meds', 'drop', 'bars', 'heart', 'speech']
def raster(f):
    g = [[0.0]*N for _ in range(N)]
    for y in range(N):
        for x in range(N):
            n = 0
            for sy in range(3):
                for sx in range(3):
                    if f(x+(sx+0.5)/3, y+(sy+0.5)/3): n += 1
            g[y][x] = n/9
    return g
def grid(name):
    g = raster(ICONS[name]); out = []
    on = lambda x, y: 0 <= x < N and 0 <= y < N and g[y][x] >= 0.5
    for y in range(N):
        row = ''
        for x in range(N):
            if not on(x, y): row += '.'; continue
            if not on(x-1, y) or not on(x, y-1) or not on(x-1, y-1): row += 'h'
            elif not on(x+1, y) or not on(x, y+1) or not on(x+1, y+1): row += 's'
            else: row += 'x'
        out.append(row)
    return out
GRIDS = {n: grid(n) for n in ORDER}

# small status sprites (also drawn doubled by the game): filled heart, empty heart, sick skull
def grid_n(f, w, h, outline_only=False):
    g = [[0.0]*w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            n = 0
            for sy in range(3):
                for sx in range(3):
                    if f(x+(sx+0.5)/3, y+(sy+0.5)/3): n += 1
            g[y][x] = n/9
    on = lambda x, y: 0 <= x < w and 0 <= y < h and g[y][x] >= 0.5
    out = []
    for y in range(h):
        row = ''
        for x in range(w):
            if not on(x, y): row += '.'
            elif outline_only: row += 'x' if (not on(x-1, y) or not on(x+1, y) or not on(x, y-1) or not on(x, y+1)) else '.'
            elif not on(x-1, y) or not on(x, y-1): row += 'h' if y < h*0.55 else 'x'
            elif not on(x+1, y) or not on(x, y+1): row += 's'
            else: row += 'x'
        out.append(row)
    return out
HEART14 = U(disc(3.9, 4.4, 3.5), disc(10.1, 4.4, 3.5), poly([(0.5, 5.6), (13.5, 5.6), (7, 12)]))
def _shine(g):
    g = [list(r) for r in g]
    for (x, y) in ((3, 2), (2, 3), (3, 3)):
        if g[y][x] != '.': g[y][x] = 'h'
    return [''.join(r) for r in g]
SMALL = {
  'heart': _shine(grid_n(HEART14, 14, 12)),
  'heart_empty': [r.replace('x', 'x') for r in grid_n(U(HEART14), 14, 12, outline_only=True)],
  'skull': grid_n(M(U(disc(7, 6.2, 5.6), box(4, 9, 10, 13, 1.2)), disc(4.9, 6.6, 1.35), disc(9.1, 6.6, 1.35), poly([(7, 8.6), (7.9, 10.2), (6.1, 10.2)]),
                    box(6.4, 11.6, 6.7, 13.2), box(7.3, 11.6, 7.6, 13.2)), 14, 14),
}

def write_header(path=_ROOT + '/baricons.h'):
    code = {'.': 0, 'x': 1, 'h': 2, 's': 3}
    def enc(rows):
        data = []; ext = []
        for r in rows:
            xs = [i for i, ch in enumerate(r) if ch != '.']; ext += [xs[0], xs[-1]] if xs else [255, 0]; data += [code[ch] for ch in r]
        return data, ext
    o = ['// generated by gen/baricons.py: the 8 menu icons at 24x24 and small status sprites (0 clear, 1 colour, 2 highlight, 3 shade)\n']
    D = []; E = []
    for n in ORDER: d, e = enc(GRIDS[n]); D.append(d); E.append(e)
    o.append('static const u8 BAR_ICON[8][576]={%s};\n' % ','.join('{%s}' % ','.join(map(str, d)) for d in D))
    o.append('static const u8 BAR_ICON_ROWS[8][48]={%s};\n' % ','.join('{%s}' % ','.join(map(str, e)) for e in E))
    for k, v in SMALL.items():
        d, e = enc(v); o.append('static const u8 SM_%s[%d]={%s};\nstatic const u8 SM_%s_ROWS[%d]={%s};\n' % (k.upper(), len(d), ','.join(map(str, d)), k.upper(), len(e), ','.join(map(str, e))))
    open(path, 'w').write(''.join(o))
if __name__ == '__main__': write_header()
