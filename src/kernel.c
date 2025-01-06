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
#include "fs/ramdisk/ramdisk.h"
#include "fs/ramfs/ramfs.h"
#include "stdlib/memutil/memutil.h"
#include "stdint.h"
//#include "testing/ramfsTest/ramfsTest.h"
//#include "testing/kmallocTest/kmallocTest.h"

extern uint32_t _kernel_end;
void kmain(uint32_t magic, struct multiboot_info* bootInfo);
uint32_t get_physical_alloc_start(struct multiboot_info* bootInfo);
void kmain(uint32_t magic, struct multiboot_info* bootInfo) {
    Reset();
    initGdt();
    initIdt();
    initTimer();
    initKeyboard();

    uint32_t phys_alloc_start = get_physical_alloc_start(bootInfo);
    printf("Physical allocation start: 0x%x, mem_upper*1024: %u\n", phys_alloc_start, bootInfo->mem_upper * 1024);
    initMemory(bootInfo->mem_upper*1024, phys_alloc_start);

    kmallocInit(0x1000);

    ramdiskInit();

    //! check initMemory arguments

    printf("all completed!\n");
    for(;;);
}

uint32_t get_physical_alloc_start(struct multiboot_info* bootInfo){
    struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*) bootInfo->mmap_addr;
    uint32_t end_entry = bootInfo->mmap_addr + bootInfo->mmap_length;

    uint32_t kernel_end = (uint32_t)&_kernel_end;
    uint32_t alloc_start = 0;
    while ((uint32_t)entry < end_entry) {
        printf("Memory region: start=0x%x, length=0x%x, type=%u\n", entry->addr_low, entry->len_low, entry->type);
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t region_start = entry->addr_low;
            uint32_t region_end = region_start + entry->len_low;

            if (region_start >= kernel_end && (alloc_start == 0 || region_start < alloc_start)) {
                alloc_start = region_start;
            }
        }
       
        entry = (struct multiboot_mmap_entry*) ((uint32_t)entry + entry->size + sizeof(entry->size));
    }

    return alloc_start;
}

//memory region: start=0x0, length=0x9fc00, type=1
//memory region: start=0x9fc00, length=0x400, type=2
//memory region: start=0xf0000, length=0x10000, type=2
//memory region: start=0x100000, length=0x7ee0000, type=1
//memory region: start=0x7fe0000, length=0x20000, type=2
//memory region: start=0xfffc0000, length=0x40000, type=2