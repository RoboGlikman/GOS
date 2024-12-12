#include "gdt.h"
#include "../stdlib/memutil/memutil.h"

extern void gdt_flush(uint32_t);
extern void tss_flush();

struct gdt_entry_struct gdtEntries[6];
struct gdt_ptr_struct gdtPtr;
struct tss_entry_struct tss_entry;

void writeTSS(uint32_t num, uint16_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    setGdtGate(num, base, limit, 0xE9, 0x00); 
    memset(&tss_entry, 0, sizeof(tss_entry)); 

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;

}

void initGdt(){
    gdtPtr.limit = (sizeof(struct gdt_entry_struct) * 6) - 1;
    gdtPtr.base = (uint32_t)&gdtEntries;

    setGdtGate(0,0,0,0,0); //Null segment
    setGdtGate(1,0,0xFFFFFFFF, 0x9A, 0xCF); //Kernel code segment
    setGdtGate(2,0,0xFFFFFFFF, 0x92, 0xCF); //Kernel data segment
    setGdtGate(3,0,0xFFFFFFFF, 0xFA, 0xCF); //User code segment
    setGdtGate(4,0,0xFFFFFFFF, 0xF2, 0xCF); //User data segment
    writeTSS(5,0x10, 0x0);


    gdt_flush((uint32_t)&gdtPtr);
    tss_flush();
}


void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdtEntries[num].base_low = (base & 0xFFFF);
    gdtEntries[num].base_middle = (base >> 16) & 0xFF;
    gdtEntries[num].base_high = (base >> 24) & 0xFF;

    gdtEntries[num].limit = (limit & 0xFFFF);
    gdtEntries[num].flags = (limit >> 16) & 0x0F;
    gdtEntries[num].flags |= (gran & 0xF0);

    gdtEntries[num].access = access;

}