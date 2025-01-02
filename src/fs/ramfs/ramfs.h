#pragma once
#include "../../stdint.h"
#include "../ramdisk/ramdisk.h"

#define MAX_FILES 128

typedef struct {
    char name[32];
    uint32_t size;
    uint32_t start_block;
} ramfs_file_t;

extern uint8_t blockBitmap[BITMAP_SIZE];
extern bool ramdiskInitialized;
extern uint32_t ramdiskSize;
extern uint32_t ramdiskBase;

int ramfsCreateFile(const char *name, uint32_t size);
int ramfsDeleteFile(const char *name);
int ramfsWriteFile(const char *name, uint32_t offset, const void *buffer, uint32_t size);
int ramfsReadFile(const char *name, uint32_t offset, void *buffer, uint32_t size);
int catchBestFit_fs(uint32_t i, uint32_t size);
int findBestFit_fs(uint32_t size, const char *name);
void ramfsListFiles();
int locateFileByName(const char* name);



