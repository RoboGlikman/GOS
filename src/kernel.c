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
//#include "testing/ramfsTest/ramfsTest.h"

//#include "testing/kmallocTest/kmallocTest.h"
void kmain(uint32_t magic, struct multiboot_info* bootInfo);


void kmain(uint32_t magic, struct multiboot_info* bootInfo) {
    Reset();
    initGdt();
    initIdt();
    initTimer();
    initKeyboard();
    
    initMemory(bootInfo->mem_upper * 1024, bootInfo->mem_lower);
    kmallocInit(0x1000);
    //test_kmalloc_kfree();
    ramdiskInit();
    //test_ramfs();

    //! check reading before EOF
    //! check initMemory arguments

    printf("all completed!\n");
    for(;;);
}