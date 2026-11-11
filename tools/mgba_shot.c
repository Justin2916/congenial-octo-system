// Runs the ROM in mGBA (libmgba) from a script, for screenshots on a real emulator core.
// Build: cc -O2 tools/mgba_shot.c -o mgba_shot $(pkg-config --cflags --libs libmgba 2>/dev/null || echo -lmgba) -lpng
// Usage: mgba_shot rom.gba script.txt [save.sav]
// Script: wait N | press KEY [frames] | hold KEY | release KEY | poke8 addr val | poke32 addr val | peek addr n | shot file.ppm
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba/gba/core.h>
#include <mgba-util/vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
static void nolog(struct mLogger *l, int c, enum mLogLevel lv, const char *f, va_list a){ (void)l;(void)c;(void)lv;(void)f;(void)a; }
static struct mLogger quiet = { .log = nolog };
static const char *KN[10]={"A","B","SELECT","START","RIGHT","LEFT","UP","DOWN","R","L"};
static int key(const char *s){ for(int i=0;i<10;i++) if(!strcmp(s,KN[i])) return 1<<i; return 0; }
int main(int argc,char **argv){
  if(argc<3){ fprintf(stderr,"usage: mgba_shot rom script [sav]\n"); return 1; }
  mLogSetDefaultLogger(&quiet);
  struct mCore *core=GBACoreCreate(); core->init(core); mCoreInitConfig(core,NULL);
  unsigned w,h; core->desiredVideoDimensions(core,&w,&h);
  color_t *buf=malloc(w*h*sizeof(color_t)); core->setVideoBuffer(core,buf,w);
  if(!mCoreLoadFile(core,argv[1])){ fprintf(stderr,"cannot load rom\n"); return 1; }
  if(argc>3){ struct VFile *sv=VFileOpen(argv[3],O_RDWR|O_CREAT); if(sv) core->loadSave(core,sv); }
  core->reset(core);
  FILE *sc= strcmp(argv[2],"-")? fopen(argv[2],"r") : stdin; char line[512]; unsigned keys=0;
  #define FRAME() core->runFrame(core)
  while(fgets(line,sizeof line,sc)){
    char cmd[32],a1[256]="",a2[64]=""; int n=sscanf(line,"%31s %255s %63s",cmd,a1,a2); if(n<1||cmd[0]=='#') continue;
    if(!strcmp(cmd,"wait")){ int k=atoi(a1); for(int i=0;i<k;i++){ core->setKeys(core,keys); FRAME(); } }
    else if(!strcmp(cmd,"press")){ int b=key(a1), k=n>2?atoi(a2):4; core->setKeys(core,keys|b); for(int i=0;i<k;i++) FRAME(); core->setKeys(core,keys); for(int i=0;i<4;i++) FRAME(); }
    else if(!strcmp(cmd,"hold")) keys|=key(a1);
    else if(!strcmp(cmd,"release")) keys&=~key(a1);
    else if(!strcmp(cmd,"poke8")) core->rawWrite8(core,strtoul(a1,0,16),-1,strtoul(a2,0,16));
    else if(!strcmp(cmd,"poke32")) core->rawWrite32(core,strtoul(a1,0,16),-1,strtoul(a2,0,16));
    else if(!strcmp(cmd,"peek")){ unsigned a=strtoul(a1,0,16); int k=n>2?atoi(a2):16; printf("%08x:",a); for(int i=0;i<k;i++) printf(" %02x",core->rawRead8(core,a+i,-1)); printf("\n"); }
    else if(!strcmp(cmd,"shot")){ FILE *f=fopen(a1,"wb"); fprintf(f,"P6\n%u %u\n255\n",w,h);
      for(unsigned i=0;i<w*h;i++){ color_t c=buf[i]; unsigned char p[3]={c&0xFF,(c>>8)&0xFF,(c>>16)&0xFF}; fwrite(p,1,3,f); } fclose(f); }
  }
  core->deinit(core); return 0;
}
