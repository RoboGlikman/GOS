#include "ramfs.h"
#include "../ramdisk/ramdisk.h"
#include "../../stdlib/memutil/memutil.h"
#include "../../stdlib/string/string.h"
#include "../../stdlib/stdio/stdio.h"

static ramfs_file_t files[MAX_FILES];
static uint32_t file_count = 0;

//TODO implement deleteFile,writeFile,readFile

int ramfsCreateFile(const char *name, uint32_t size){

    uint32_t numBlocksNeeded = CEIL_DIV(size, BLOCK_SIZE);

    if (file_count >= MAX_FILES || ramdiskSize+numBlocksNeeded > MAX_NUM_BLOCKS){
        printf("could not create file! too many files/ramdisk size too small \n");
        return -1;
    }

    uint32_t numBlocksNeededCpy = numBlocksNeeded;

    while (totalBlocksAllocated + numBlocksNeededCpy > ramdiskSize){
        increaseRamdiskSize();
        numBlocksNeededCpy--;
    }

    int i = findBestFit_fs(numBlocksNeeded);

    if (i < 0){
        printf("couldnt create file! no fit was found.\n");
        return -1;
    }

    if((catchBestFit_fs(i, numBlocksNeeded)) != 0){
        printf("couldnt occupy ramdisk bitmap\n");
        return -1;
    }

    strncpy(files[file_count].name, name, sizeof(files[file_count].name));
    files[file_count].size = numBlocksNeeded;
    files[file_count].start_block = i;
    file_count++;

    totalBlocksAllocated += numBlocksNeeded;

    return 0;
    
}

int catchBestFit_fs(uint32_t i, uint32_t size){
    if (i >= ramdiskSize || (i + size) > ramdiskSize) {
        return -1; 
    }

    for (uint32_t i = 0; i < size; i++) {
        if (!isBlockFree(i + i)) {
            return -1;
        }
        setBlockUsed(i + i);
    }

    return 0;
}

int findBestFit_fs(uint32_t size){
    uint32_t best_i = -1;
    uint32_t best_fit_size = MAX_NUM_BLOCKS; 

    uint32_t current_start = 0;
    uint32_t current_size = 0;

    for (uint32_t i = 0; i < ramdiskSize; i++) {
        if (isBlockFree(i)) {
            if (current_size == 0) {
                current_start = i;
            }
            current_size++;
        } else {
            if (current_size >= size && current_size < best_fit_size) {
                best_i = current_start;
                best_fit_size = current_size;
            }
            current_size = 0; 
        }
    }

    if (current_size >= size && current_size < best_fit_size) {
        best_i = current_start;
    }

    return best_i; 
}


int ramfsDeleteFile(const char *name);
void ramfsWriteFile(const char *name, uint32_t offset, const void *buffer, uint32_t size);
void ramfsReadFile(const char *name, uint32_t offset, void *buffer, uint32_t size);