#pragma once
#include "../../stdint.h"
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))
int memcmp(const void*, const void*, uint32_t size);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, uint32_t size);
void* memmove(void*, const void*, uint32_t size);
void *memset(void *dest, int val, uint32_t count);
void outPortB(uint16_t port, uint8_t value);
char inPortB(uint16_t port);

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};