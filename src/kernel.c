#include "stdlib/stdio/stdio.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "vga/vga.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "multiboot.h"
#include "memory/memory.h"
#include "MemoryManager/mm.h"
#include "stdlib/string/string.h"
#include "fs/ramfs/ramfs.h"
#include "stdlib/memutil/memutil.h"
#include "stdint.h"
//#include "testing/ramfsTest/ramfsTest.h"
//#include "testing/kmallocTest/kmallocTest.h"

void kmain(uint32_t magic, struct multiboot_info* bootInfo);
uint32_t getPhysicalAllocStart(struct multiboot_info* bootInfo);
extern uint32_t _kernel_end;

void kmain(uint32_t magic, struct multiboot_info* bootInfo) {
    Reset();
    initGdt();
    initIdt();
    initTimer();
    initKeyboard();

    uint32_t phys_alloc_start = getPhysicalAllocStart(bootInfo);
    initMemory(bootInfo->mem_upper*1024, phys_alloc_start);

    kmallocInit(0x1000);
    ramfsInit();

    printf("all completed!\n");
    for(;;);
}

uint32_t getPhysicalAllocStart(struct multiboot_info* bootInfo) {
    struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*) bootInfo->mmap_addr;
    uint32_t end_entry = bootInfo->mmap_addr + bootInfo->mmap_length;

    uint32_t kernel_end = (uint32_t)&_kernel_end;
    uint32_t kernel_size = kernel_end - 0xC0000000;
    uint32_t alloc_start = 0;
    while ((uint32_t)entry < end_entry) {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t region_start = entry->addr_low+ 0xC0000000;
            uint32_t region_end = region_start + entry->len_low + 0xC0000000;
            if (region_start + kernel_size > kernel_end  && (alloc_start == 0 || region_start < alloc_start)) {
                alloc_start = region_start+kernel_size;
            }
        }
       
        entry = (struct multiboot_mmap_entry*) ((uint32_t)entry + entry->size + sizeof(entry->size));
    }
    return alloc_start-0xC0000000;
}

