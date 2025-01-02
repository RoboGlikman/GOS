#include "ramdisk.h"
#include "../../memory/memory.h"
#include "../../stdlib/memutil/memutil.h"
#include "../../stdlib/string/string.h"
#include "../../stdlib/stdio/stdio.h"


uint8_t blockBitmap[BITMAP_SIZE];
bool ramdiskInitialized = false;
uint32_t ramdiskSize;
uint32_t ramdiskBase;
uint32_t totalBlocksAllocated;

void ramdiskInit(){
    ramdiskBase = RAMDISK_START;
    totalBlocksAllocated = 0;
    ramdiskSize = 0;
    ramdiskInitialized = true;
    
    memset(blockBitmap, 0, sizeof(uint8_t)*BITMAP_SIZE);

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

void increaseRamdiskSize(){
    int pageTop = CEIL_DIV(ramdiskSize, 0x1000);
    
    uint32_t phys = pmmAllocPageFrame();
    memMapPage(RAMDISK_START + pageTop * 0x1000 + 0x1000, phys, PAGE_FLAG_WRITE);
    
    ramdiskSize++;
}
