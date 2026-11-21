# Renders the soundtrack the way the game plays it: wave channel melody (triangle wave, note gaps)
# plus noise-channel percussion with the same envelope/noise settings as drum_hit() in ext.c.
import wave, struct, subprocess, sys
from tunes import build, ORDER, DRUM_CODE, rate
SR=32768; FR=SR*4389/262144/ (280896/262144) # samples per GBA frame (~59.73 fps)
FR=SR/59.7275
TRI=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0]
ENV={1:(6,1),2:(4,2),3:(3,1),4:(2,2),5:(2,1)}
NZ={1:0x56,2:0x21,3:0x01,4:0x11,5:0x08}
NIGHT={'INSIDE_NIGHT','OUTSIDE_NIGHT','LULLABY'}
def render(name,seconds=None):
    step,bar,dr,seq,bars=build()[name]
    vol=3 if name in NIGHT else 2
    loopf=sum(l for _,l in seq); total=int(loopf if seconds is None else min(seconds*60,loopf*3))
    # per-frame melody state
    frames=[]; i=0; t=0; gap=0; cur=0
    for f in range(total):
        if t==0:
            n,l=seq[i]; i=(i+1)%len(seq); cur=n; t=l; gap= 2 if l>4 else 0
        on = cur and not (gap and t<=gap)
        frames.append(cur if on else 0); t-=1
    out=[]; ph=0.0
    # drums
    dpat=[DRUM_CODE[c] for c in dr] if dr else None
    lfsr=0x7FFF; nacc=0.0; nvol=0; nstep=0; ncount=0; nfreq=0; w7=False
    for f in range(total):
        n=frames[f]
        if dpat and f%step==0:
            c=dpat[(f//step)%len(dpat)]
            if c:
                v,stp=ENV[c]
                if vol==3: v=max(1,v>>1)
                z=NZ[c]; r=z&7; s=(z>>4)&15; w7=bool(z&8)
                nfreq=524288/(0.5 if r==0 else r)/(2**(s+1)); nvol=v; nstep=stp; ncount=0; lfsr=0x7F if w7 else 0x7FFF
        fs=int(FR*(f+1))-int(FR*f)
        if n: hz=65536/(2048-rate(n))
        for k in range(fs):
            a=0.0
            if n:
                ph=(ph+hz*32/SR)%32; a=(TRI[int(ph)]-7.5)/7.5*(0.5 if vol==2 else 0.25)
            if nvol>0:
                nacc+=nfreq/SR
                while nacc>=1:
                    nacc-=1; b=(lfsr^(lfsr>>1))&1; lfsr=(lfsr>>1)|(b<<(6 if w7 else 14))
                    if w7: lfsr&=0x7F
                a+= ((1 if lfsr&1 else -1)*nvol/15)*0.5
                ncount+=1
                if ncount>=nstep*SR/64: ncount=0; nvol-=1
            out.append(a)
    return out
def save(samples,fn):
    peak=max(1e-6,max(abs(x) for x in samples)); g=0.8/peak
    with wave.open(fn,'wb') as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
        w.writeframes(b''.join(struct.pack('<h',int(max(-1,min(1,x*g))*32000)) for x in samples))
if __name__=='__main__':
    import os
    os.makedirs('/tmp/prev',exist_ok=True)
    med=[]; marks=[]; t=0.0
    for k in ORDER:
        s=render(k); save(s,'/tmp/prev/%s.wav'%k)
        subprocess.run(['ffmpeg','-y','-loglevel','error','-i','/tmp/prev/%s.wav'%k,'-b:a','96k','/tmp/prev/%s.mp3'%k])
        clip=s[:int(SR*22)]; peak=max(1e-6,max(abs(x) for x in clip)); clip=[x*0.8/peak for x in clip]
        fade=int(SR*1.5)
        for j in range(fade): clip[-1-j]*=j/fade
        marks.append((t,k)); med+=clip+[0.0]*int(SR*1.2); t+=len(clip)/SR+1.2
        print(k,'%.1fs'%(len(s)/SR))
    save(med,'/tmp/prev/medley.wav')
    subprocess.run(['ffmpeg','-y','-loglevel','error','-i','/tmp/prev/medley.wav','-b:a','96k','/tmp/prev/CritterKeep_soundtrack_preview.mp3'])
    for tt,k in marks: print('%d:%02d %s'%(tt//60,tt%60,k))
