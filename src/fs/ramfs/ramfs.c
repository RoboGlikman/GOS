#include "ramfs.h"
#include "../ramdisk/ramdisk.h"
#include "../../stdlib/memutil/memutil.h"
#include "../../stdlib/string/string.h"
#include "../../stdlib/stdio/stdio.h"

static ramfs_file_t files[MAX_FILES];
static uint32_t file_count = 0;

//TODO check catchBestFit, findBestFit
//TODO check ramdisk size increase and memory mapping. 

//! there's probably a problem with the ramdisk increase and the ramfsReadFile and possibly with the ramfsWriteFile

//* delete file, list file, create file (only one block) work.

int ramfsCreateFile(const char *name, uint32_t size){ //size = size in bytes

    if (ramdiskInitialized == false){
        printf("couldnt create file, ramdisk is not initialized!\n");
        return -1;
    }

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

    int i = findBestFit_fs(numBlocksNeeded, name);

    if (i < 0){
        printf("couldnt create file! no fit was found.\n");
        return -1;
    }

    if((catchBestFit_fs(i, numBlocksNeeded)) != 0){
        printf("couldnt occupy ramdisk bitmap\n");
        return -1;
    }

    strncpy(files[file_count].name, name, sizeof(files[file_count].name));
    files[file_count].size = size;
    files[file_count].start_block = i;
    file_count++;

    totalBlocksAllocated += numBlocksNeeded;

    return 0;
    
}

int catchBestFit_fs(uint32_t i, uint32_t size){ //size = size in blocks
    if (i >= ramdiskSize || (i + size) > ramdiskSize) {
        return -1; 
    }

    for (uint32_t j = 0; j < size; j++) {
        if (!isBlockFree(j + i)) {
            return -1;
        }
        setBlockUsed(j + i);
    }

    return 0;
}

int findBestFit_fs(uint32_t size, const char *name){ // size = size on blocks
    if (locateFileByName(name) >= 0){
        printf("file already exists!\n");
        return -1;
    }

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


int ramfsDeleteFile(const char *name){
    for (uint32_t i=0; i < file_count; i++){
        if (strcmp(files[i].name, name) == 0){
            uint32_t numBlocks = CEIL_DIV(files[i].size, BLOCK_SIZE);

            for (uint32_t j=0; j<numBlocks; j++){
                setBlockFree(files[i].start_block + j);
            }

            files[i] = files[file_count - 1];
            file_count--;

            return 0;
        }
    }
    return -1;
}

int ramfsWriteFile(const char *name, uint32_t offset, const void *buffer, uint32_t size){ //size = size in bytes
    int fileIndex = locateFileByName(name);

    if (fileIndex < 0){
        printf("couldnt write, file not found!\n");
        return -1; 
    }


    ramfs_file_t *file = &files[fileIndex];
    if (offset + size > file->size){
        printf("couldnt write, writing beyond file size is not allowed!\n");
        return -1; 
    }

    // Calculate starting block and initial block offset
    uint32_t currentBlock = file->start_block + (offset / BLOCK_SIZE);
    uint32_t blockOffset = offset % BLOCK_SIZE;

    // Write data block by block
    uint32_t bytesWritten = 0;
    const char *src = (const char *)buffer;
    while (bytesWritten < size) {
 
        uint32_t bytesToWrite = BLOCK_SIZE - blockOffset;
        if (bytesToWrite > size - bytesWritten)
            bytesToWrite = size - bytesWritten;

        void *blockAddr = (void *)(ramdiskBase + currentBlock * BLOCK_SIZE);
        memcpy((char *)blockAddr + blockOffset, src + bytesWritten, bytesToWrite);

        bytesWritten += bytesToWrite;
        currentBlock++;
        blockOffset = 0; // Reset offset for subsequent blocks
    }

    return 0; 
}


int ramfsReadFile(const char *name, uint32_t offset, void *buffer, uint32_t size){ //size = size in bytes
    int fileIndex = locateFileByName(name);

    if (fileIndex < 0){
        printf("couldnt read, file not found!\n");
        return -1;
    }

    ramfs_file_t *file = &files[fileIndex];

    if (offset >= file->size){
        printf("couldnt read, offset beyond file size!\n");
        return -1;
    }

    // Adjust size to prevent reading beyond the file's size
    if (offset + size > file->size)
        size = file->size - offset;
    
    // Calculate starting block and initial block offset
    uint32_t currentBlock = file->start_block + (offset / BLOCK_SIZE);
    uint32_t blockOffset = offset % BLOCK_SIZE;

    // Read data block by block
    uint32_t bytesRead = 0;
    char *dest = (char *)buffer;
    while (bytesRead < size) {

        uint32_t bytesToRead = BLOCK_SIZE - blockOffset;
        if (bytesToRead > size - bytesRead)
            bytesToRead = size - bytesRead;

        void *blockAddr = (void *)(ramdiskBase + currentBlock * BLOCK_SIZE);
        memcpy(dest + bytesRead, (char *)blockAddr + blockOffset, bytesToRead);

        bytesRead += bytesToRead;
        currentBlock++;
        blockOffset = 0; // Reset offset for subsequent blocks
    }

    return 0;
}

void ramfsListFiles(){
    if (file_count == 0)
        printf("no files to list\n");
    for (uint32_t i=0; i<file_count; i++){
        printf("name: %s\tsize: %u bytes\n", files[i].name, files[i].size);
    }
}

int locateFileByName(const char* name){
    for (uint32_t i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}