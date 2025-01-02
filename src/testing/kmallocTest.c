#include "kmallocTest.h"
#include "../MemoryManager/mm.h"
#include "../stdlib/stdio/stdio.h"
#include "../vga/vga.h"

void test_kmalloc_kfree() {
    void* ptr1 = kmalloc(100);
    if (ptr1 != (void*)0) {
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("Test 1: Single allocation successful.\n");
        char *ptr11 = (char*)ptr1;
        ptr11 = "aaaa";
        currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
        printf("%s\n", ptr11);
        kfree(ptr1);
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("Test 1: Single deallocation successful.\n");
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("Test 1: Single allocation failed.\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    void* ptr2 = kmalloc(50);
    void* ptr3 = kmalloc(128);
    if (ptr2 != (void*)0 && ptr3 != (void*)0) {
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("Test 2: Multiple allocations successful.\n");
        kfree(ptr3);
        kfree(ptr2);
        printf("Test 2: Multiple deallocations successful.\n");
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("Test 2: Multiple allocations failed.\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    void* ptr4 = kmalloc(300);
    if (ptr4 != (void*)0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 3: 3 blocks allocation successful\n");
        kfree(ptr4);
        printf("test 3: 3 block deallocation successful\n");
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 3: 3 block allocation failed\n");
    }

    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    void * ptr5 = kmalloc(5000);
    if (ptr5 != (void*)0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 4: increasing heap size, allocation successful\n");
        kfree(ptr5);
        printf("test 4: increasing heap size, deallocation successful\n");
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 4: increasing heap size, allocation failed\n");
    }

    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    void *ptr6 = kmalloc(160000000);
    if (ptr6 == (void*)0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 6: success\n");
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 6: failed\n");
    }
    kfree(ptr6);
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
}