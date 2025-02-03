#pragma once
#include "../stdint.h"
#ifndef SHAREFILE_INCLUDED
#define SHAREFILE_INCLUDED

#ifdef  MAIN_SHARED_FILE
char sequence[64];
uint32_t sequenceIndex;
#else
extern char sequence[64];
extern uint32_t sequenceIndex;
#endif

#endif

