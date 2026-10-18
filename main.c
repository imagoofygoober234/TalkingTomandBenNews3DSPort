#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "voice.h"
#define W 400
#define H 240
#define ROOT "sdmc:/3ds/tom_ben_news/frames/"
#define UI "sdmc:/3ds/tom_ben_news/ui/touchscreen.jpg"
typedef struct {const char *folder; int count;} Clip;
static const Clip clips[]={
 {"newsIntro",30}, {"tom_ben_fight",29}, {"technical_difficulties",1},
 {"ben_gun_hit",31},{"ben_gun_miss",22},{"tom_airhorn",15},
 {"tom_squirt",33},{"ben_poke",22},{"tom_poke",21},
 {"tom_rolleyes",24},{"ben_box",21},{"tom_straw",14},
 {"ben_shooes",81},{"tom_impatient",60},
 {"tom_talk",16},{"ben_talk",16}
};
#define CLIP_COUNT (sizeof(clips)/sizeof(clips[0]))
static u8 topFrame[W*H*3],bottomFrame[320*240*3];
static int bottomReady=0;
struct jpg_error {struct jpeg_error_mgr pub; jmp_buf ret;};
static void on_jpg_error(j_common_ptr c){struct jpg_error *e=(struct jpg_error*)c->err;longjmp(e->ret,1);}
static bool load_jpg(const char *file,u8 *buffer,int w,int h){
 FILE *f=fopen(file,"rb");if(!f)return false;
 struct jpeg_decompress_struct c; struct jpg_error e;
 c.err=jpeg_std_error(&e.pub);e.pub.error_exit=on_jpg_error;
 if(setjmp(e.ret)){jpeg_destroy_decompress(&c);fclose(f);return false;}
 jpeg_create_decompress(&c);jpeg_stdio_src(&c,f);jpeg_read_header(&c,TRUE);jpeg_start_decompress(&c);
 if(c.output_width!=(unsigned)w||c.output_height!=(unsigned)h||c.output_components!=3){jpeg_destroy_decompress(&c);fclose(f);return false;}
 while(c.output_scanline<(unsigned)h){JSAMPROW row=buffer+c.output_scanline*w*3;jpeg_read_scanlines(&c,&row,1);}
 jpeg_finish_decompress(&c);jpeg_destroy_decompress(&c);fclose(f);return true;
}
static void blit(GFX_SCREEN screen,u8 *data,int w,int h){
 u16 fw,fh;u8 *dst=gfxGetFramebuffer(screen,GFX_LEFT,&fw,&fh);
 for(int y=0;y<h;y++)for(int x=0;x<w;x++){
  u8 *p=data+(y*w+x)*3,*q=dst+(x*h+(h-1-y))*3;
  q[0]=p[2];q[1]=p[1];q[2]=p[0];
 }
 gfxFlushBuffers();gfxSwapBuffers();
}
static int sequence=0,frameIndex=0,active=1,specialFight=0;
static unsigned int seed=0x93825;
static unsigned int rng(void){seed=seed*1664525u+1013904223u;return seed;}
static void play(int seq){sequence=seq;frameIndex=0;active=1;specialFight=(seq==1);}
// Six button hit regions from Dan's original square touchscreen sketch.
// Asset library does NOT contain separate chair-fall clips yet. Both chair regions
// intentionally play placeholders and must be remapped when proper clips are found.
static void touch_action(int x,int y){
 // sketch occupies x=40..279, y=0..239. Right-edge controls overlap slightly.
 if(x>=246 && y>=133 && y<198){return; /* FACTY: custom sprite still needed */}
 if(x>=246 && y>=69 && y<133){play(3+(rng()%2));return;} // Tom attacks Ben (temporary)
 if(x<89 && y>=107 && y<172){play(7);return;} // Ben attacks Tom (temporary)
 if(x<89 && y>=30 && y<107){play(8);return;} // separate attack region
 if(x<94 && y>=172){play(1);return;} // fight + technical difficulties
 if(x>=93 && x<178 && y>=82 && y<170){play(12);return;} // Ben chair PLACEHOLDER
 if(x>=178 && x<247 && y>=94 && y<169){play(13);return;} // Tom chair PLACEHOLDER
}
int main(void){
 gfxInitDefault();gfxSetScreenFormat(GFX_TOP,GSP_RGB8_OES);gfxSetScreenFormat(GFX_BOTTOM,GSP_RGB8_OES);
 bool micReady=voiceInit(); (void)micReady;
 bottomReady=load_jpg(UI,bottomFrame,320,240);
 if(bottomReady)blit(GFX_BOTTOM,bottomFrame,320,240);
 char path[256];int ticks=0,last=-1;
 while(aptMainLoop()){
  hidScanInput();u32 down=hidKeysDown(),held=hidKeysHeld();voiceUpdate(down,held);
  if(down&KEY_START)break;
  if(down&KEY_TOUCH){touchPosition p;hidTouchRead(&p);touch_action(p.px,p.py);last=-1;}
  if(down&KEY_A){play(0);last=-1;}
  if(down&KEY_B){play(1);last=-1;}
  if(down&KEY_L){play(3+(rng()%2));last=-1;}
  if(down&KEY_R){play(5+(rng()%2));last=-1;}
  if(frameIndex!=last){
   snprintf(path,sizeof(path),ROOT "%s/%04d.jpg",clips[sequence].folder,frameIndex);
   if(load_jpg(path,topFrame,W,H))blit(GFX_TOP,topFrame,W,H);
   last=frameIndex;
  }
  gspWaitForVBlank();
  if(active && ++ticks%5==0){
   frameIndex++;
   if(frameIndex>=clips[sequence].count){
    if(specialFight){specialFight=0;sequence=2;frameIndex=0;}
    else if(sequence==0){sequence=14;frameIndex=0;active=0;}
    else {sequence=14;frameIndex=0;active=0;}
   }
  }
 }
 voiceExit();gfxExit();return 0;
}
