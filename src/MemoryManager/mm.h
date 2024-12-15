#pragma once
#include "../stdint.h"

void kmallocInit(uint32_t initialHeapSize);
void changeHeapSize(int newHeapSize);
void *kmalloc(uint32_t nBytes);
uint32_t kfree(void *ptr);
void catchBestFit(uint32_t i, uint32_t size);
int findBestFit(uint32_t n);

#define MAX_NUM_CHUNKS 8192
#define NUM_CHUNKS_IN_PAGE 32
#define CHUNK_SIZE_BYTES 128
#define MAX_NUMBER_HEAP_PAGES 256

struct heapchunk_t{
    int32_t size;
    bool inuse;
};
