import sys,subprocess,glob,os
from PIL import Image
# usage: shots.py rom script.txt sram outgrid.png
rom,script,sram,out=sys.argv[1:5]
for f in glob.glob('/tmp/sh_*.ppm'): os.remove(f)
txt=open(script).read()
r=subprocess.run(['./emu',rom,'-',sram],input=txt,text=True,capture_output=True)
print(r.stdout[-3000:],r.stderr[-2000:])
fs=sorted(glob.glob('/tmp/sh_*.ppm'))
n=len(fs); cols=3 if n>4 else 2; rows=(n+cols-1)//cols
W=Image.new('RGB',(cols*480+(cols-1)*6,rows*320+(rows-1)*6),(255,0,255))
for i,f in enumerate(fs):
    W.paste(Image.open(f).resize((480,320),Image.NEAREST),((i%cols)*486,(i//cols)*326))
W.save(out); print(n,'shots')
