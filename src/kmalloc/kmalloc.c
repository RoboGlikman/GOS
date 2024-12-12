#include "kmalloc.h"
#include "../stdint.h"
#include "../memory/memory.h"
#include "../stdlib/memutil/memutil.h"

//TODO create kfree, kmalloc

static uint32_t heapStart;
static uint32_t heapSize;
static uint32_t threshold;
static bool kmallocInitialized = false;

void kmallocInit(uint32_t initialHeapSize){
    heapStart = KERNEL_MALLOC;
    heapSize = 0;
    threshold = 0;
    kmallocInitialized = true;
    
    changeHeapSize(initialHeapSize);
    *((uint32_t*)heapStart) = 0; 
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