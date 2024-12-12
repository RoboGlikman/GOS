#include "mm.h"
#include "../stdint.h"
#include "../memory/memory.h"
#include "../stdlib/memutil/memutil.h"


//TODO create kfree, kmalloc 

static uint32_t heapStart;
static uint32_t heapSize;
static struct heapchunk_t *last;
static uint32_t threshold;
static bool kmallocInitialized = false;
uint32_t allocatedBlocks;

void *kmalloc(uint32_t nBytes){
    if (!kmallocInitialized)
        return (void*)0;
    if (allocatedBlocks == CEIL_DIV(heapSize, 1000)) // if the heap is not big enough, make it larger.
        changeHeapSize(heapSize + CEIL_DIV(nBytes, 0x1000)*0x1000);

    uint32_t pagesNeeded = CEIL_DIV(nBytes, 0x1000);

    struct heapchunk_t *chunk = last+last->pages_offset;
    chunk->inuse = true;
    chunk->size = pagesNeeded;
    chunk->next = last;

    last = chunk;
    return (void*)(KERNEL_MALLOC+chunk->pages_offset);
    //TODO implement logic of searching, retrieving the correct block and heap page offset. 
    //TODO also deal with fragmentation here. 
}

uint32_t kfree(void *ptr){
    if (!ptr)
        return -1;
    ((struct heapchunk_t*)ptr)->inuse = false;
    //TODO 
}

void kmallocInit(uint32_t initialHeapSize){
    heapStart = KERNEL_MALLOC;
    heapSize = 0;
    threshold = 0;
    kmallocInitialized = true;
    allocatedBlocks = 0;
    
    changeHeapSize(initialHeapSize);
    *((uint32_t*)heapStart) = 0; 

    //! start
    struct heapchunk_t *chunk = (struct heapchunk_t*) heapStart;
    chunk->size = 1; ////0x1000 - sizeof(struct heapchunk_t);
    chunk->inuse = false;
    chunk->next = (struct heapchunk_t*)0;
    chunk->pages_offset = ((CEIL_DIV(heapSize, 0x1000)+allocatedBlocks)*sizeof(struct heapchunk_t*));
    last = chunk;
    //! end
}

void changeHeapSize(int newHeapSize){
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