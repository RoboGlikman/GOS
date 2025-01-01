#pragma once
#include "../../stdint.h"

#define RAMDISK_MAX_SIZE (256 * 1024 * 1024)  // 256 MB
#define BLOCK_SIZE 0x1000 // page size
#define MAX_NUM_BLOCKS (RAMDISK_MAX_SIZE / BLOCK_SIZE)
#define BITMAP_SIZE (MAX_NUM_BLOCKS / 8)

void ramdiskInit();
bool isBlockFree(uint32_t block_index);
void setBlockUsed(uint32_t block_index);
void setBlockFree(uint32_t block_index);
void increaseRamdiskSize();