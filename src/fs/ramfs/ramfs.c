#include "ramfs.h"
#include "../ramdisk/ramdisk.h"
#include "../../stdlib/memutil/memutil.h"
#include "../../stdlib/string/string.h"
#include "../../stdlib/stdio/stdio.h"

static ramfs_file_t files[MAX_FILES];
static uint32_t fds[MAX_FILES];
static uint32_t file_count;
static bool ramfsInitialized = false;

void ramfsInit(){
    ramdiskInit();
    memset(fds, 0, sizeof(fds));
    memset(files, 0, sizeof(files));
    file_count = 0;
    ramfsInitialized = true;
}

int ramfsCreateFile(const char *name, uint32_t size){ //size = size in bytes
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return -1;
    }

    uint32_t numBlocksNeeded = CEIL_DIV(size, BLOCK_SIZE);

    if (file_count+FDS_RESERVED >= MAX_FILES || ramdiskSize+numBlocksNeeded > MAX_NUM_BLOCKS){
        printf("could not create file! too many files/ramdisk potential size too small \n");
        return -1;
    }


    uint32_t numBlocksNeededCpy = numBlocksNeeded;
    int j = findBestFit_fs(numBlocksNeeded, name);
    if (j==-2) return -1;
    while (numBlocksNeededCpy != 0 && j < 0){
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
    initFileContents(file_count+2, size);
    return 0;
    
}

int ramfsOpenFile(const char *name, uint32_t permissions){
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return -1;
    }
    int fileIndex = locateFileByName(name);

    if (fileIndex < 0){
        printf("Couldnt open file!\n");
        return -1;
    }

    fds[fileIndex+FDS_RESERVED] = permissions;
    return fileIndex+FDS_RESERVED;
}

void ramfsCloseFile(uint32_t fd){
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return;
    }
    if (fd < 3){
        printf("invalid file descriptor!\n");
        return;
    }
    fds[fd] = 0;
}

static int catchBestFit_fs(uint32_t i, uint32_t size){ //size = size in blocks
    for (uint32_t j = 0; j < size; j++) {
        if (!isBlockFree(j + i)) {
            return -1;
        }
        setBlockUsed(j + i);
    }

    return 0;
}

static int findBestFit_fs(uint32_t size, const char *name){ // size = size in blocks
    if (locateFileByName(name) >= 0){
        printf("file already exists!\n");
        return -2;
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


int ramfsDeleteFile(uint32_t fd){
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return -1;
    }
    uint32_t i = fd-3;

    if (i < 0){
        printf("invalid file descriptor, couldnt create file!\n");
        return -1;
    }
    initFileContents(fd, (files[i]).size);

    uint32_t numBlocks = CEIL_DIV(files[i].size, BLOCK_SIZE);

    for (uint32_t j=0; j<numBlocks; j++)
        setBlockFree(files[i].start_block + j);
    
    files[i] = files[file_count - 1];
    file_count--;
    ramfsCloseFile(fd);
    return 0;

}

int ramfsWriteFile(uint32_t fd, uint32_t offset, const void *buffer, uint32_t size) {
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return -1;
    }
    if (fd < 3){
        printf("invalid file descriptor!\n");
        return -1;
    }

    if (fds[fd] == W || fds[fd] == WX || fds[fd] == RWX || fds[fd] == RW){
        int fileIndex = fd-FDS_RESERVED;
        ramfs_file_t *file = &files[fileIndex];

        if (offset + size > file->size) {
            printf("Error: Writing beyond file size is not allowed!\n");
            return -1;
        }

        uint32_t currentBlock = file->start_block + (offset / BLOCK_SIZE);
        uint32_t blockOffset = offset % BLOCK_SIZE;

        uint32_t bytesWritten = 0;
        const char *src = (const char *)buffer;

        while (bytesWritten < size) {
            uint32_t bytesToWrite = BLOCK_SIZE - blockOffset; 
            if (bytesToWrite > size - bytesWritten) 
                bytesToWrite = size - bytesWritten;
            
            void *blockAddr = (void *)(ramdiskBase + currentBlock * BLOCK_SIZE); 

            memcpy((char *)blockAddr + blockOffset, src + bytesWritten, bytesToWrite); 
            
            bytesWritten += bytesToWrite;
            blockOffset = 0; 
            currentBlock++; 
        }

        return 0;
    }
    printf("You don`t have permission to write to this file!\n");
    return -1;
}

int ramfsReadFile(uint32_t fd, uint32_t offset, void *buffer, uint32_t size) {
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return -1;
    }

    if (fd < 3){
        printf("invalid file descriptor!\n");
        return -1;
    }
    if (fds[fd] >= R){
        int fileIndex = fd-FDS_RESERVED;
        ramfs_file_t *file = &files[fileIndex];

        if (offset >= file->size) {
            printf("Error: Offset %u is beyond the file size %u!\n", offset, file->size);
            return -1;
        }

        if (offset + size > file->size) 
            size = file->size - offset;

        uint32_t currentBlock = file->start_block + (offset / BLOCK_SIZE);
        uint32_t blockOffset = offset % BLOCK_SIZE;

        uint32_t bytesRead = 0;
        char *dest = (char *)buffer;

        while (bytesRead < size) {
            uint32_t bytesToRead = BLOCK_SIZE - blockOffset; 
            if (bytesToRead > size - bytesRead) {
                bytesToRead = size - bytesRead;
                
            }
            void *blockAddr = (void *)(ramdiskBase + currentBlock * BLOCK_SIZE);
            memcpy((void*)(dest + bytesRead), (const void*)(blockAddr + blockOffset), bytesToRead); 
            bytesRead += bytesToRead;
            blockOffset = 0; 
            currentBlock++; 
        }
        const char *tmp = "\0";
        if (file->size == size && size % BLOCK_SIZE == 0){
            memcpy((void*)(dest+bytesRead-1), (const void*)tmp, 1);
        } else { 
            memcpy((void*)(dest+bytesRead), (const void*)tmp, 1);
        }
        return 0;
    }
    printf("You don`t have permission to read from this file!\n");
    return -1;
}

void ramfsListFiles(){
    if (ramfsInitialized == false){
        printf("couldnt perform operation, ramfs not initialized yet!\n");
        return;
    }
    if (file_count == 0)
        printf("no files to list...\n");
    for (uint32_t i=0; i<file_count; i++){
        printf("name: %s\tsize: %u bytes\n", files[i].name, files[i].size);
    }
}

static int locateFileByName(const char* name){
    for (uint32_t i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static int initFileContents(uint32_t fd, uint32_t size){
    const char *buffer = "";
    fds[fd] = W;
    for (int i=0; i<size; i++){
        ramfsWriteFile(fd, i, (const void*)buffer, 1);
    }
    fds[fd] = 0;
    return 0;
}

uint32_t ramfsGetFileSize(uint32_t fd){
    int fileIndex = fd-FDS_RESERVED;
    ramfs_file_t *file = &files[fileIndex];
    return file->size;
}
