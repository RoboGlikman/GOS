#include "../stdint.h"
#include "../multiboot.h"
#include "memory.h"
#include "../stdlib/stdio/stdio.h"
#include "../stdlib/memutil/memutil.h"

#define NUM_PAGE_DIRS 256
#define NUM_PAGE_FRAMES (0x100000000 / 0x1000 / 8)

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalPagesAllocated;

uint8_t physicalMemoryBitmap[NUM_PAGE_FRAMES / 8]; //todo: dynamic array, bit array instead

static uint32_t pageDirs[NUM_PAGE_DIRS][1024] __attribute__((aligned(4096)));//256 pd's, each has 1024 pt's
static uint8_t usedPageDirs[NUM_PAGE_DIRS];

int mem_number_vpages;

void pmm_init(uint32_t mem_low, uint32_t mem_high){
    pageFrameMin = CEIL_DIV(mem_low, 0x1000); // (low mem / page size) = min physical page
    pageFrameMax = CEIL_DIV(mem_high, 0x1000); // (high mem / page size) = max physical page

    totalPagesAllocated = 0;

    memset(physicalMemoryBitmap, 0 , sizeof(physicalMemoryBitmap));
    
}

void initMemory(uint32_t memHigh, uint32_t physicalAllocStart) {
    mem_number_vpages = 0;
    initial_page_dir[0] = 0;
    invalidatePageDir(0); // invalidate first pd
   
    initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) |  PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidatePageDir(0xFFFFF000); // invalidate another preset pd
        
    pmm_init(physicalAllocStart, memHigh);
    memset(pageDirs, 0, 0x1000 * NUM_PAGE_DIRS);
    memset(usedPageDirs, 0, NUM_PAGE_DIRS);
}

uint32_t pmmAllocPageFrame(){
    uint32_t start = pageFrameMin / 8 + ((pageFrameMin & 7) != 0 ? 1 : 0);
    uint32_t end = pageFrameMax / 8 - ((pageFrameMax & 7) != 0 ? 1 : 0);

    for (uint32_t b = start; b < end; b++){
        uint8_t byte = physicalMemoryBitmap[b];
        if (byte == 0xFF) continue;

        for (uint32_t i = 0; i < 8; i++){
            bool used = byte >> i & 1;

            if (!used){ 
                byte ^= (-1 ^ byte) & (1 << i); // if the byte isnt used, set it to be used
                physicalMemoryBitmap[b] = byte; // set the bitmap[curr location] to the byte (allocate)
                totalPagesAllocated++;

                uint32_t addr = (b*8*i) * 0x1000;
                return addr;
            }
        }
    }

    return 0;
}

uint32_t* memGetCurrentPageDir(){
    uint32_t pd;
    asm volatile("mov %%cr3, %0" : "=r"(pd)); // cr3 contains the addres of the currentPageDir
    pd += KERNEL_START; // because the kernel is in the upper half, we need to explicitly add its base.
    return (uint32_t*)pd;
}

void memChangePageDir(uint32_t *pd){
    pd  = (uint32_t *)(((uint32_t)pd) - KERNEL_START);
    asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(pd));
}

void syncPageDirs(){
    for (int i=0; i<NUM_PAGE_DIRS; i++){
        if (usedPageDirs[i]){
            uint32_t *pageDir = pageDirs[i];

            for (int i=768; i<1023; i++){
                pageDir[i] = initial_page_dir[i] & ~PAGE_FLAG_OWNER;
            }
        }
    }
}

void memMapPage(uint32_t vAddr, uint32_t physAddr, uint32_t flags){
    uint32_t *prevPageDir = 0;

    if (vAddr >= KERNEL_START){
        prevPageDir = memGetCurrentPageDir();
        if (prevPageDir != initial_page_dir){
            memChangePageDir(initial_page_dir); 
        }
    }

    uint32_t pdIndex = vAddr >> 22;
    uint32_t ptIndex = vAddr >> 12 & 0x3FF;

    uint32_t *pageDir = REC_PAGEDIR;
    uint32_t *pageTbl = REC_PAGETBL(pdIndex);

    if (!(pageDir[pdIndex] && PAGE_FLAG_PRESENT)){
        uint32_t ptPAddr = pmmAllocPageFrame();
        pageDir[pdIndex] = ptPAddr | PAGE_FLAG_PRESENT | PAGE_FLAG_PRESENT | PAGE_FLAG_OWNER | flags;
        invalidatePageDir(vAddr);

        for (uint32_t i=0; i<1024; i++){
            pageTbl[i] = 0;
        }
    }

    pageTbl[ptIndex] = physAddr | PAGE_FLAG_PRESENT | flags;
    mem_number_vpages++;
    invalidatePageDir(vAddr);

    if (prevPageDir != 0){
        syncPageDirs();
        if (prevPageDir != initial_page_dir){
            memChangePageDir(initial_page_dir);
        }
    }

}


void invalidatePageDir(uint32_t vaddr){
    asm volatile("invlpg %0" :: "m"(vaddr));
}