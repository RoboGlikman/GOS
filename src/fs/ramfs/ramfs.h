#pragma once
#include "../../stdint.h"
#include "../ramdisk/ramdisk.h"

#define MAX_FILES 128
#define FDS_RESERVED 3 //0-stdin 1-stdout 2-stderr //!(not implemented yet)
#define R 4
#define W 2
#define X 1
#define RW (R + W)
#define RWX (X + W + X)
#define RX (R + X)
#define WX (W + X)

typedef struct {
    char name[32];
    uint32_t size;
    uint32_t start_block;
} ramfs_file_t;

extern uint8_t blockBitmap[BITMAP_SIZE];
extern uint32_t ramdiskSize;
extern uint32_t ramdiskBase;

int ramfsCreateFile(const char *name, uint32_t size);
int ramfsDeleteFile(uint32_t fd);
int ramfsWriteFile(uint32_t fd, uint32_t offset, const void *buffer, uint32_t size);
int ramfsReadFile(uint32_t fd, uint32_t offset, void *buffer, uint32_t size);
static int catchBestFit_fs(uint32_t i, uint32_t size);
static int findBestFit_fs(uint32_t size, const char *name);
void ramfsListFiles();
static int locateFileByName(const char* name);
static int initFileContents(uint32_t fd, uint32_t size);
void ramfsCloseFile(uint32_t fd);
int ramfsOpenFile(const char *name, uint32_t permissions);
void ramfsInit();



