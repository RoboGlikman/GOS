#include "mm.h"
#include "../stdint.h"
#include "../memory/memory.h"
#include "../stdlib/memutil/memutil.h"
#include "../stdlib/stdio/stdio.h"

static uint32_t heapStart;
static uint32_t heapSize;
struct heapchunk_t chunks[MAX_NUM_CHUNKS];
static bool kmallocInitialized = false;
uint32_t totalChunksAllocated;

void *kmalloc(uint32_t nBytes){
    if (!kmallocInitialized)
        return (void*)0;

    int numChunksNeeded = CEIL_DIV(nBytes, CHUNK_SIZE_BYTES);
    int numChunksNeededCpy = numChunksNeeded;

    if (numChunksNeeded+totalChunksAllocated > MAX_NUM_CHUNKS){ 
        printf("not enough potential space in heap.\n");
        return (void*)0;
    }

    while ((totalChunksAllocated + numChunksNeededCpy)*CHUNK_SIZE_BYTES > heapSize){ // increase heap size as needed
        increaseHeapSize(heapSize + 0x1000);
        numChunksNeededCpy -= NUM_CHUNKS_IN_PAGE;
    }

    int32_t i = findBestFit_mm(numChunksNeeded);

    if (i < 0){
        printf("Couldnt allocate! no space was found\n");
        return (void*)0;
    }

    catchBestFit_mm(i, numChunksNeeded);
    totalChunksAllocated += numChunksNeeded;
    return (void*)(heapStart+i*CHUNK_SIZE_BYTES);
}

uint32_t kfree(void *ptr){
    if (ptr == NULL) {
        return -1;
    }

    uint32_t index = (uint32_t)ptr - heapStart;
    index /= NUM_CHUNKS_IN_PAGE;

    uint32_t num_chunks = chunks[index].size;

    for (uint32_t i = index; i < index + num_chunks; i++) {
        chunks[i].inuse = false;
        chunks[i].size = 0;
    }
    totalChunksAllocated -= num_chunks;
    return 0;
}

void kmallocInit(uint32_t initialHeapSize){
    heapStart = KERNEL_MALLOC;
    heapSize = 0;
    kmallocInitialized = true;
    totalChunksAllocated = 0;
    memset(chunks, 0, sizeof(struct heapchunk_t)*MAX_NUM_CHUNKS);

    increaseHeapSize(initialHeapSize);
    *((uint32_t*)heapStart) = 0; 

}

void increaseHeapSize(int newHeapSize){
    int oldPageTop = CEIL_DIV(heapSize, 0x1000);
    int newPageTop = CEIL_DIV(newHeapSize, 0x1000);

    if (newPageTop > oldPageTop){
        int diff = newPageTop - oldPageTop;

        for (int i = 0; i < diff; i++){
            uint32_t phys = pmmAllocPageFrame();
            memMapPage(KERNEL_MALLOC + oldPageTop * 0x1000 + i * 0x1000, phys, PAGE_FLAG_WRITE);
            // Calculate the virtual address of the new page:
            // - Start from the base virtual address of the kernel memory region (KERNEL_MALLOC)
            // - Add the offset to the end of the old heap (oldPageTop * 0x1000)
            // - Add the offset within the new pages (i * 0x1000)
        }
    }
    heapSize = newHeapSize;
}


int findBestFit_mm(uint32_t n) {
    int best_fit_index = -1;
    uint32_t best_fit_size = MAX_NUM_CHUNKS;

    for (uint32_t i = 0; i < heapSize / 1000; i++) {
        if (!chunks[i].inuse && i + n <= heapSize) {
            if (i + n < best_fit_size) {
                best_fit_index = i;
                best_fit_size = i + n;
            }
        }
    }

    return best_fit_index;
}

void catchBestFit_mm(uint32_t i, uint32_t size){
    chunks[i].size = size;
    chunks[i].inuse = true;
    for(int j=i+1; j<i+size; j++){
        chunks[j].size = -1;
        chunks[j].inuse = true;
    }
}