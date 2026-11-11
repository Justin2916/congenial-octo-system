# A "less flat" renderer for the critter mockups: smooth dithered shading instead of 5 hard bands,
import os as _os; _ROOT = _os.environ.get('SFM_ROOT') or _os.path.dirname(_os.path.dirname(_os.path.abspath(__file__)))
# contact shadows where one part sits in front of another, reflected rim light on the shadow side,
# specular glints, and a soft ground shadow.
import math, sys
sys.path.insert(0, _ROOT + '/gen')
import critters as C
from critters import Canvas, mix, OUTLINE
L = (-0.66, -0.58, 0.48); _n = math.sqrt(sum(v*v for v in L)); L = tuple(v/_n for v in L)

BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]
class Canvas3(Canvas):
    def __init__(s, k=1.0):
        super().__init__(k); N = s.N
        s.d = [[0.0]*N for _ in range(N)]; s.pid = [[0]*N for _ in range(N)]; s.part = 0; s.patches = set()
    def shade_px(s, x, y, col, nx, ny, shade=True, bias=0.0):
        if not (0 <= x < s.N and 0 <= y < s.N): return
        if shade:
            nz = math.sqrt(max(0.0, 1-nx*nx-ny*ny)); d = nx*L[0]+ny*L[1]+nz*L[2]+bias
        else:   # flat patches (bellies, muzzles) follow the lighting of what they sit on, so they stay round
            s.patches.add(s.part)
            d = (s.d[y][x] if s.mat[y][x] not in (None, '__flat__', '__eye__') else 0.3) + 0.12 + bias
        s.mat[y][x] = col; s.flat[y][x] = None; s.d[y][x] = d; s.pid[y][x] = s.part
        s.tone[y][x] = 0 if d > 0.82 else 1 if d > 0.55 else 2 if d > 0.18 else 3 if d > -0.15 else 4
    def shape(s, *a, **k):
        s.part += 1; return super().shape(*a, **k)
    def ellipse(s, *a, **k):
        s.part += 1; return super().ellipse(*a, **k)
    def poly(s, pts, col, tone=None, nc=None):
        s.part += 1
        r = super().poly(pts, col, tone, nc)
        if tone is not None:
            dmap = {0: 0.9, 1: 0.65, 2: 0.3, 3: 0.0, 4: -0.3}
            for y in range(s.N):
                for x in range(s.N):
                    if s.pid[y][x] != s.part and s.mat[y][x] == col and s.tone[y][x] == tone: s.d[y][x] = dmap[tone]; s.pid[y][x] = s.part
        return r
    MODE = 'soft'
    LINES = False
    def rgb(s, ramps, ground=True):
        N = s.N; k = s.k
        solid = lambda x, y: 0 <= x < N and 0 <= y < N and s.mat[y][x] not in (None, '__flat__', '__eye__')
        D = [row[:] for row in s.d]
        rim = [[False]*N for _ in range(N)]; s.rim = rim
        sx, sy = -L[0], -L[1]                    # direction shadows fall (away from the light)
        reach = max(2, int(round(3.0*k)))
        for y in range(N):
            for x in range(N):
                if not solid(x, y): continue
                p = s.pid[y][x]
                # contact shadow: a part drawn later (in front) just toward the light casts onto this one
                for t in range(1, reach+1):
                    qx, qy = int(round(x-sx*t)), int(round(y-sy*t))
                    if 0 <= qx < N and 0 <= qy < N and s.pid[qy][qx] > p and s.mat[qy][qx] not in (None, '__flat__', '__eye__') \
                       and s.mat[qy][qx] != s.mat[y][x]:
                        D[y][x] -= 0.6*(1-(t-1)/reach); break
                # reflected rim light on the shadow side of the silhouette
                edge = any(not (0 <= x+dx < N and 0 <= y+dy < N) or s.mat[y+dy][x+dx] is None for dx, dy in ((1, 0), (0, 1), (1, 1)))
                if edge and D[y][x] < 0.1:
                    if Canvas3.MODE == 'bold': rim[y][x] = True
                    else: D[y][x] += 0.32
        for x in range(N):
            col_b = [y for y in range(N) if solid(x, y)]
            if not col_b: continue
            yb = max(col_b)
            for y in range(max(0, yb-int(2.5*k)), yb+1):
                if solid(x, y): D[y][x] -= 0.18
        out = [[None]*N for _ in range(N)]
        for y in range(N):
            for x in range(N):
                m = s.mat[y][x]
                if m is None: continue
                if m == '__flat__': out[y][x] = s.flat[y][x]; continue
                if m == '__eye__': out[y][x] = (34, 28, 64); continue
                hi, li, ba, sh, de = ramps[m]
                if Canvas3.MODE == 'bold':
                    # pixel-art volume: highlight, light, base, a hue-shifted core shadow and a cool reflected rim
                    d = (D[y][x]-0.32)*1.45+0.32
                    rimc = s.rim[y][x] if hasattr(s, 'rim') else False
                    if d > 1.08: col = mix(hi, (255, 252, 236), 0.6)
                    elif d > 0.8: col = hi
                    elif d > 0.52: col = li
                    elif d > 0.12: col = ba
                    elif d > -0.35: col = mix(sh, (70, 50, 120), 0.18)
                    else: col = mix(de, (50, 34, 100), 0.30)
                    if rimc: col = mix(col, mix(ba, (170, 200, 255), 0.35), 0.55)
                    # dither the boundary rows between bands for a softer turn
                    out[y][x] = col; continue
                stops = [(-0.55, mix(de, (40, 24, 80), 0.25)), (-0.15, de), (0.15, sh), (0.42, ba), (0.68, li), (0.9, hi), (1.05, mix(hi, (255, 255, 250), 0.6))]
                d = max(stops[0][0], min(stops[-1][0], D[y][x]))
                # dither between the two nearest stops
                for i in range(len(stops)-1):
                    if d <= stops[i+1][0]:
                        t = (d-stops[i][0])/(stops[i+1][0]-stops[i][0]); break
                c0, c1 = stops[i][1], stops[i+1][1]
                q = (BAYER[y & 3][x & 3]+0.5)/16
                # snap to the nearest stop, dithering only in a narrow band around the halfway point
                col = c1 if (t > 0.62 or (t > 0.38 and q < (t-0.38)/0.24)) else c0
                out[y][x] = col
        if Canvas3.LINES:            # inner contour lines: where a part sits in front of another, draw a dark line on the part behind
            lines = []
            for y in range(N):
                for x in range(N):
                    if not solid(x, y): continue
                    p = s.pid[y][x]
                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                        qx, qy = x+dx, y+dy
                        if solid(qx, qy) and s.pid[qy][qx] > p and s.pid[qy][qx] not in s.patches:
                            lines.append((x, y)); break
            for (x, y) in lines:
                m = s.mat[y][x]; de = ramps[m][4]
                out[y][x] = mix(de, OUTLINE, 0.55)
        res = [row[:] for row in out]
        for y in range(N):           # coloured outline as before
            for x in range(N):
                if out[y][x] is not None: continue
                ins = [(a, b) for a, b in ((x+1, y), (x-1, y), (x, y+1), (x, y-1)) if 0 <= a < N and 0 <= b < N and out[b][a] is not None]
                if ins:
                    a, b = ins[0]; m = s.mat[b][a]
                    lit = x < N*0.46 and y < N*0.54
                    res[y][x] = mix(ramps[m][4], OUTLINE, 0.45) if (lit and m in ramps) else OUTLINE
        if ground:                   # soft ground shadow under the feet
            ys = [y for y in range(N) for x in range(N) if res[y][x] is not None]; yb = max(ys)
            xs = [x for x in range(N) if any(res[y][x] is not None for y in range(yb-3, yb+1))]
            cx = (min(xs)+max(xs))/2; rx = (max(xs)-min(xs))/2+2*k; ry = 1.6*k
            for y in range(int(yb-ry), min(N, int(yb+ry+2))):
                for x in range(int(cx-rx), int(cx+rx)+1):
                    if 0 <= x < N and res[y][x] is None and ((x-cx)/rx)**2+((y-yb-0.5)/ry)**2 <= 1 and (x+y) & 1 == 0:
                        res[y][x] = (90, 80, 110)
        return res
