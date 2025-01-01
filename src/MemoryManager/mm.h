#pragma once
#include "../stdint.h"

void kmallocInit(uint32_t initialHeapSize);
void increaseHeapSize(int newHeapSize);
void *kmalloc(uint32_t nBytes);
uint32_t kfree(void *ptr);
void catchBestFit(uint32_t i, uint32_t size);
int findBestFit(uint32_t n);

#define MAX_NUM_CHUNKS (NUM_CHUNKS_IN_PAGE*MAX_NUMBER_HEAP_PAGES)
#define NUM_CHUNKS_IN_PAGE 32
#define CHUNK_SIZE_BYTES 128
#define MAX_NUMBER_HEAP_PAGES 0x1000

struct heapchunk_t{
    int32_t size;
    bool inuse;
};
