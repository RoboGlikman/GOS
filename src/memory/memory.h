#pragma ocne
void initMemory(uint32_t memHigh, uint32_t physicalAllocStart);
void invalidate(uint32_t vaddr);
uint32_t pmmAllocPageFrame();
void memMapPage(uint32_t vAddr, uint32_t physAddr, uint32_t flags);
void syncPageDirs();
void pmm_init(uint32_t mem_low, uint32_t mem_high);
uint32_t* memGetCurrentPageDir();
void memChangePageDir(uint32_t *pd);

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define KERNEL_START 0xC0000000
#define KERNEL_MALLOC 0xD0000000
#define RAMDISK_START 0xE0000000
#define REC_PAGEDIR ((uint32_t*)0xFFFFF000)
#define REC_PAGETBL(i) ((uint32_t*)(0xFFC00000 + ((i) << 12)))
#define PAGE_FLAG_OWNER (1 << 9)

extern uint32_t initial_page_dir[1024];
