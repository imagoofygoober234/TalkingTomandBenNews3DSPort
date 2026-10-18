#pragma once
#include <3ds.h>
#include <stdbool.h>
enum { VOICE_BEN=0, VOICE_TOM=1 };
bool voiceInit(void);
void voiceUpdate(u32 down,u32 held);
const char *voiceStatus(void);
void voiceExit(void);
