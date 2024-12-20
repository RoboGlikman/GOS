#pragma once
#include "../../stdint.h"

uint32_t strlen(const char*);
int32_t strcmp(char s1[], char s2[]);
char* strcat(char *dest, char *src);
char * strncpy(char *dest, const char *src, uint32_t n);