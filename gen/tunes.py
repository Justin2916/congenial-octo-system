# Critter Keep soundtrack v2 (cozy). Melody on the wave channel, soft percussion on the noise channel.
# Notation: tokens separated by spaces; NOTE[/steps] (default 1 step), r[/steps] = rest, | = bar line (checked).
# Drum pattern: one char per step, repeated: k kick, s brush snare, h hat, z shaker, c cricket, . rest
import sys
NAMES={'C':0,'C#':1,'Db':1,'D':2,'D#':3,'Eb':3,'E':4,'F':5,'F#':6,'Gb':6,'G':7,'G#':8,'Ab':8,'A':9,'A#':10,'Bb':10,'B':11}
def midi(t): n,o=t[:-1],int(t[-1]); return 12*(o+1)+NAMES[n]
def parse(name,s,bar):
    out=[]; cur=0; bars=0
    for tok in s.split():
        if tok=='|':
            if cur!=bar: sys.exit('%s: bar %d has %d steps'%(name,bars+1,cur))
            cur=0; bars+=1; continue
        n,_,l=tok.partition('/'); l=int(l) if l else 1
        out.append((0 if n=='r' else midi(n),l)); cur+=l
    if cur: sys.exit('%s: unfinished last bar (%d steps)'%(name,cur))
    return out,bars

# name: (frames per step, steps per bar, drum pattern or None, melody)
T={}
T['TITLE']=(22,8,"z...z...","""
E5/2 G5/2 E5/2 C5/2 | A4/2 C5/2 E5/3 r | F5/2 E5/2 D5/2 C5/2 | D5/6 r/2 |
E5/2 G5/2 C6/2 B5/2 | A5/3 G5 E5/4 | F5/2 E5/2 D5/2 B4/2 | C5/6 r/2 |
A5/2 G5/2 F5/2 A5/2 | G5/3 E5 C5/4 | F5/2 E5/2 D5/2 F5/2 | E5/3 D5 B4/4 |
C5/2 E5/2 A5/2 G5/2 | F5/2 A5/2 C6/4 | B5/2 A5/2 G5/2 D5/2 | C5/6 r/2 |""")
T['INSIDE_DAY']=(20,8,"k.h.s.h.","""
C5/2 F5/2 A5/2 G5 F5 | D5/2 F5/2 A5/4 | Bb5/2 A5/2 G5/2 F5/2 | G5/2 E5/2 C5/4 |
C5/2 F5/2 A5/2 C6/2 | D6/3 C6 A5/4 | Bb5/2 A5/2 G5/2 E5/2 | F5/6 r/2 |
A5/2 G5 F5 C5/2 F5/2 | E5/2 A5/2 C6/4 | D6/2 C6/2 Bb5/2 A5/2 | G5/6 r/2 |
F5/2 A5/2 D6/2 C6/2 | Bb5/3 A5 G5/4 | E5/2 G5/2 Bb5/2 G5/2 | F5/6 r/2 |
D5/3 F5 Bb5/4 | C6/3 Bb5 G5/4 | A5/2 C6/2 E5/2 A5/2 | F5/6 r/2 |
G5/2 Bb5/2 D6/2 Bb5/2 | A5/2 G5/2 E5/2 C5/2 | F5/2 A5/2 C6/2 A5/2 | G5/2 E5/2 C5/2 r/2 |""")
T['OUTSIDE_DAY']=(18,8,"k.hks.h.","""
D5 G5 B5/2 A5 G5 D5/2 | E5/2 G5/2 B5/4 | C6/2 B5/2 A5/2 G5/2 | A5/6 r/2 |
B5/2 D6/2 B5 A5 G5/2 | G5/2 E5/2 B4/4 | C5/2 E5/2 D5/2 F#5/2 | G5/6 r/2 |
G5 A5 B5 D6 B5/2 G5/2 | E5/2 G5/2 C6/4 | A5/2 C6/2 B5/2 A5/2 | F#5/6 r/2 |
E5/2 G5/2 B5/2 E6/2 | D6/3 C6 A5/4 | A5/2 F#5/2 D5/2 F#5/2 | G5/6 r/2 |
E5/2 G5/2 C6/2 B5/2 | B5/2 G5/2 D5/4 | C5/2 E5/2 A5/2 G5/2 | F#5/6 r/2 |
B5/2 G5/2 E5/2 G5/2 | A5/3 G5 E5/4 | F#5/2 A5/2 D6/2 C6/2 | B5/2 A5/2 G5/2 r/2 |""")
T['INSIDE_NIGHT']=(26,8,None,"""
E5/4 C5/4 | A4/4 C5/4 | G4/2 C5/2 E5/4 | D5/6 r/2 |
C5/2 E5/2 A5/4 | G5/3 F5 E5/4 | D5/3 E5 F5/4 | E5/6 r/2 |
F5/4 A5/4 | G5/4 E5/4 | F5/3 E5 D5/4 | B4/6 r/2 |
C5/2 E5/2 A5/2 G5/2 | F5/4 E5/2 D5/2 | D5/4 B4/4 | C5/6 r/2 |""")
T['OUTSIDE_NIGHT']=(24,8,"c.c.............","""
F#5/4 A5/4 | D6/4 B5/4 | G5/2 B5/2 D6/4 | C#6/6 r/2 |
A5/3 F#5 D5/4 | F#5/3 D5 B4/4 | G5/2 F#5/2 E5/2 B4/2 | A4/6 r/2 |
B4/2 D5/2 G5/4 | F#5/4 A5/4 | G5/3 F#5 E5/4 | C#5/6 r/2 |
D5/2 F#5/2 B5/4 | A5/3 G5 D5/4 | E5/2 F#5/2 G5/2 C#5/2 | D5/6 r/2 |""")
T['LULLABY']=(30,6,None,"""
A4/4 F4/2 | G4/4 E4/2 | F4/2 A4/2 D5/2 | C5/6 |
D5/4 C5/2 | A4/4 F4/2 | Bb4/2 A4/2 G4/2 | G4/6 |
A4/4 C5/2 | C5/2 E5/2 C5/2 | D5/4 Bb4/2 | A4/6 |
F4/2 A4/2 D5/2 | Bb4/4 A4/2 | G4/2 E4/2 G4/2 | F4/6 |""")
T['SHOP']=(18,8,"k.h.s.h.","""
C5 E5 G5 E5 A5/2 G5/2 | F5 A5 C6 A5 G5/2 F5/2 | E5/2 G5/2 C6/2 G5/2 | D5/6 r/2 |
A4 C5 E5 C5 G5/2 E5/2 | F5 D5 A4 D5 F5/2 A5/2 | G5/2 F5/2 E5/2 D5/2 | C5/6 r/2 |
A5/2 C6/2 A5 G5 F5/2 | G5/2 E5/2 C5/4 | F#5/2 A5/2 C6/2 A5/2 | G5/6 r/2 |
E5 G5 C6 E6 D6/2 C6/2 | A5/2 E5/2 C5/4 | D5/2 F5/2 B4/2 D5/2 | C5/4 G4/2 C5/2 |""")
T['ALBUM']=(26,8,None,"""
B4/2 D5/2 G5/4 | G5/3 F#5 E5/4 | E5/2 G5/2 C6/4 | B5/3 A5 F#5/4 |
E5/2 G5/2 B5/4 | A5/3 G5 F#5/4 | E5/2 G5/2 A5/2 B5/2 | A5/6 r/2 |
B5/4 D6/4 | A5/4 F#5/4 | G5/3 F#5 E5/4 | D5/6 r/2 |
E5/2 G5/2 C6/4 | B5/3 A5 G5/4 | A5/2 C6/2 B5/2 A5/2 | G5/6 r/2 |""")
T['MINI_CATCH']=(12,8,"k.hks.h.","""
F5 A5 C6 A5 F5 A5 C6 F6 | D6/2 C6/2 Bb5/2 A5/2 | G5 C6 E6 C6 G5 C6 E6 G6 | F6/4 C6/2 A5/2 |
D5 F5 A5 F5 D6/2 A5/2 | Bb5 A5 G5 F5 G5/2 Bb5/2 | C6/2 Bb5/2 A5/2 G5/2 | F5/2 C5/2 F5/4 |""")
T['MINI_HOP']=(11,8,"k..hk.s.","""
G5 r B5 r D6 r B5 r | C6 r E6 r G5/2 r/2 | A5 r F#5 r D5 F#5 A5 r | G5/2 B5/2 G5/2 r/2 |
E5 r G5 r B5 r E6 r | E6 D6 C6 B5 A5/2 G5/2 | F#5 G5 A5 B5 C6 B5 A5 F#5 | G5/4 D5/2 r/2 |""")
T['MINI_GUESS']=(14,8,"h.h.h.s.","""
A4 C5 E5 C5 A4/2 r/2 | D5 F5 A5 F5 D5/2 r/2 | E5 G#5 B5 G#5 E5/2 D5/2 | C5/2 B4/2 A4/4 |
F5/2 E5/2 D5/2 C5/2 | E5/2 G5/2 C6/4 | D5/2 F5/2 E5/2 G#5/2 | A5/4 r/4 |""")
T['MINI_STARS']=(24,8,None,"""
G5/4 E5/4 | C6/4 A5/4 | G5/2 E5/2 D5/4 | C5/6 r/2 |
E5/4 G5/4 | A5/4 C6/4 | D6/2 C6/2 A5/4 | G5/6 r/2 |""")
T['FAREWELL_WONDER']=(24,8,"z.......","""
C5/2 E5/2 G5/4 | A5/3 G5 F5/4 | E5/2 G5/2 C6/4 | D6/6 r/2 |
C6/2 B5/2 A5/2 E5/2 | F5/3 G5 A5/4 | D6/3 C6 A5/4 | B5/6 r/2 |
A5/2 C6/2 F6/4 | E6/3 D6 C6/4 | B5/2 D6/2 G5/2 B5/2 | C6/8 |""")
T['FAREWELL_GOOD']=(20,8,"k.h.s.h.","""
G5/2 B5/2 D6/2 B5/2 | C6/2 B5/2 A5/4 | F#5/2 A5/2 D6/2 C6/2 | B5/6 r/2 |
E5/2 G5/2 B5/2 G5/2 | A5/3 G5 E5/4 | C5/2 E5/2 D5/2 F#5/2 | G5/6 r/2 |
E5/2 G5/2 C6/2 E6/2 | D6/3 B5 G5/4 | A5/2 B5/2 C6/2 A5/2 | G5/6 r/2 |""")
T['FAREWELL_SAD']=(28,8,None,"""
E5/4 C5/4 | D5/3 F5 A4/4 | G#4/2 B4/2 E5/4 | A4/6 r/2 |
A4/2 C5/2 F5/4 | E5/3 D5 C5/4 | D5/2 F5/2 E5/2 D5/2 | B4/6 r/2 |
C5/4 E5/4 | A5/3 G5 F5/4 | E5/2 D5/2 B4/2 G#4/2 | A4/8 |""")

DRUM_CODE={'.':0,'k':1,'s':2,'h':3,'z':4,'c':5}
def rate(n): f=440*2**((n-69)/12); return int(round(2048-65536/f))
ORDER=['TITLE','INSIDE_DAY','OUTSIDE_DAY','INSIDE_NIGHT','OUTSIDE_NIGHT','LULLABY','SHOP','ALBUM',
       'MINI_CATCH','MINI_HOP','MINI_GUESS','FAREWELL_WONDER','FAREWELL_GOOD','FAREWELL_SAD','MINI_STARS']
def build():
    data={}
    for k in ORDER:
        step,bar,dr,s=T[k]; seq,bars=parse(k,s,bar)
        data[k]=(step,bar,dr,[(n,l*step) for n,l in seq],bars)
    return data
if __name__=='__main__':
    data=build()
    L=['// generated by gen/tunes.py','static const u16 NOTE_RATE[128]={'+','.join(str(rate(n) if 36<=n<=96 else 0) for n in range(128))+'};']
    for k in ORDER:
        step,bar,dr,seq,bars=data[k]; flat=[]
        for n,l in seq:
            while l>255: flat+=[n,255]; l-=255
            flat+=[n,l]
        flat+=[255,0]
        L.append('static const u8 TUNE_%s[]={%s};'%(k,','.join(map(str,flat))))
        if dr: L.append('static const u8 DRUM_%s[]={%d,%d,%s};'%(k,step,len(dr),','.join(str(DRUM_CODE[c]) for c in dr)))
        print('%-16s %3d notes  %2d bars  loop %5.1fs  drums %s'%(k,len(seq),bars,sum(l for _,l in seq)/60, dr or '-'))
    L.append('enum { '+', '.join('T_'+k for k in ORDER)+', T_COUNT };')
    L.append('static const u8 *const TUNES[T_COUNT]={'+','.join('TUNE_'+k for k in ORDER)+'};')
    L.append('static const u8 *const DRUMS[T_COUNT]={'+','.join(('DRUM_'+k) if T[k][2] else '0' for k in ORDER)+'};   // {frames per step, length, pattern...}')
    open('../tunes.h','w').write('\n'.join(L)+'\n')
