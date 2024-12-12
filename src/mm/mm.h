#pragma once
#include "../stdint.h"

void kmallocInit(uint32_t initialHeapSize);
void changeHeapSize(int newHeapSize);
void *kmalloc(uint32_t nBytes);
uint32_t kfree(void *ptr);

struct heapchunk_t{
    uint32_t size;
    bool inuse;
    uint32_t pages_offset;
    struct heapchunk_t *next;
};
