#include "ramdisk.h"
#include "../../memory/memory.h"
#include "../../stdlib/memutil/memutil.h"
#include "../../stdlib/string/string.h"
#include "../../stdlib/stdio/stdio.h"


uint8_t blockBitmap[BITMAP_SIZE];
uint32_t ramdiskSize;
uint32_t ramdiskBase;

void ramdiskInit(){
    ramdiskBase = RAMDISK_START;
    ramdiskSize = 0;
    
    memset(blockBitmap, 0, sizeof(blockBitmap));

    increaseRamdiskSize();
}

bool isBlockFree(uint32_t block_index){
    return !(blockBitmap[block_index / 8] & (1 << (block_index % 8)));
}

void setBlockUsed(uint32_t block_index){
    blockBitmap[block_index / 8] |= (1 << (block_index % 8));
}

void setBlockFree(uint32_t block_index){
    blockBitmap[block_index / 8] &= ~(1 << (block_index % 8));
}

void increaseRamdiskSize() {
    if (ramdiskSize >= MAX_NUM_BLOCKS) { 
        printf("Error: RAM disk has reached maximum size\n"); 
        return; 
    }

    uint32_t pageTop = ramdiskSize;
    
    uint32_t phys = pmmAllocPageFrame();
    if (phys == 0){
        printf("Error: Failed to allocate a page frame!\n"); 
        return;
    }
    memMapPage(RAMDISK_START + pageTop * 0x1000, phys, PAGE_FLAG_WRITE);
    
    ramdiskSize++;
}
