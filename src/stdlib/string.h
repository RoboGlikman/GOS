#pragma once
#include "../stdint.h"

int memcmp(const void*, const void*, uint32_t size);
void* memcpy(void* __restrict, const void* __restrict, uint32_t size);
void* memmove(void*, const void*, uint32_t size);
uint32_t strlen(const char*);
int32_t strcmp(char s1[], char s2[]);
