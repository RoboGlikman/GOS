#include "stdlib/stdio/stdio.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "vga/vga.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "multiboot.h"
#include "memory/memory.h"
#include "kmalloc/kmalloc.h"
#include "stdlib/string/string.h"
void kmain(uint32_t magic, struct multiboot_info* bootInfo);

void kmain(uint32_t magic, struct multiboot_info* bootInfo) {
    Reset();
    initGdt();
    initIdt();
    initTimer();
    initKeyboard();
    
    uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
    uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
    initMemory(bootInfo->mem_upper * 1024, physicalAllocStart);
    kmallocInit(0x1000); 

    printf("all completed!\n");
    for(;;);
}