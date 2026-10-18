// Uses libctru's built-in MIC service and NDSP playback, following devkitPro APIs.
// X held = Ben (deeper), Y held = Tom (higher). Release to hear playback.
#include <3ds.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "voice.h"
#define CAPACITY 0x20000u
#define SAMPLE_RATE 16360
static u8 *micMemory;
static s16 *playMemory;
static bool recording, playing, micReady, dspReady;
static int who;
static ndspWaveBuf wave;
static u32 recordedBytes;
static bool start_capture(int character) {
 if (!micReady || !dspReady || playing || recording) return false;
 memset(micMemory, 0, CAPACITY);
 recordedBytes=0;
 // One-shot capture; avoid overwriting while user holds the button.
 if (R_FAILED(micStartSampling(MICU_ENCODING_PCM16, MICU_SAMPLE_RATE_16360, 0, CAPACITY, false))) return false;
 recording=true; who=character; return true;
}
static void stop_capture(void) {
 if (!recording) return;
 u32 end=micGetLastSampleOffset();
 micStopSampling(); recording=false;
 // Buffer offset returned in bytes. 16-bit audio needs an even size.
 if(end>CAPACITY)end=CAPACITY;
 recordedBytes=end&~1u;
 if(recordedBytes<2048) return;
 memcpy(playMemory,micMemory,recordedBytes);
 DSP_FlushDataCache(playMemory,recordedBytes);
 memset(&wave,0,sizeof(wave));
 wave.data_pcm16=playMemory;
 wave.nsamples=recordedBytes/2;
 ndspChnWaveBufClear(0);
 ndspChnSetFormat(0,NDSP_FORMAT_MONO_PCM16);
 ndspChnSetInterp(0,NDSP_INTERP_LINEAR);
 // Playing samples slower sounds deeper; faster sounds higher.
 ndspChnSetRate(0, (float)SAMPLE_RATE*(who==VOICE_BEN?0.78f:1.30f));
 ndspChnWaveBufAdd(0,&wave);
 playing=true;
}
bool voiceInit(void) {
 micMemory=(u8*)memalign(0x1000,CAPACITY);
 playMemory=(s16*)linearAlloc(CAPACITY);
 if(!micMemory||!playMemory)return false;
 micReady=R_SUCCEEDED(micInit(micMemory,CAPACITY));
 dspReady=R_SUCCEEDED(ndspInit());
 if(dspReady){ndspSetOutputMode(NDSP_OUTPUT_STEREO);float mix[12]={1.f,1.f}; ndspChnSetMix(0,mix);}
 return micReady && dspReady;
}
void voiceUpdate(u32 down,u32 held){
 if(playing && wave.status==NDSP_WBUF_DONE)playing=false;
 if(recording){
  // Stop when button released or sample area is almost full.
  if(!(held&(who==VOICE_BEN?KEY_X:KEY_Y))||micGetLastSampleOffset()>=CAPACITY-2048)stop_capture();
 } else if (!playing) {
  if(down&KEY_X)start_capture(VOICE_BEN);
  else if(down&KEY_Y)start_capture(VOICE_TOM);
 }
}
const char *voiceStatus(void){return recording?(who==VOICE_BEN?"Recording Ben":"Recording Tom"):(playing?"Repeating voice":"Ready");}
void voiceExit(void){if(recording)micStopSampling();if(micReady)micExit();if(dspReady){ndspChnWaveBufClear(0);ndspExit();}if(micMemory)free(micMemory);if(playMemory)linearFree(playMemory);}
