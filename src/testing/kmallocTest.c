#include "kmallocTest.h"
#include "../MemoryAllocator/ma.h"
#include "../stdlib/stdio/stdio.h"

void test_kmalloc_kfree() {
    void* ptr1 = kmalloc(100);
    if (ptr1 != (void*)0) {
        printf("Test 1: Single allocation successful.\n");
        kfree(ptr1);
        printf("Test 1: Single deallocation successful.\n");
    } else {
        printf("Test 1: Single allocation failed.\n");
    }

    void* ptr2 = kmalloc(50);
    void* ptr3 = kmalloc(128);
    if (ptr2 != (void*)0 && ptr3 != (void*)0) {
        printf("Test 2: Multiple allocations successful.\n");
        kfree(ptr3);
        kfree(ptr2);
        printf("Test 2: Multiple deallocations successful.\n");
    } else {
        printf("Test 2: Multiple allocations failed.\n");
    }


    void* ptr4 = kmalloc(300);
    if (ptr4 != (void*)0){
        printf("test 3: 3 blocks allocation successful\n");
        kfree(ptr4);
        printf("test 3: 3 block deallocation successful\n");
    } else {
        printf("test 3: 3 block allocation failed\n");
    }

    void * ptr5 = kmalloc(5000);
    if (ptr5 != (void*)0){
        printf("test 4: increasing heap size, allocation successful\n");
        kfree(ptr5);
        printf("test 4: increasing heap size, deallocation successful\n");
    } else {
        printf("test 4: increasing heap size, allocation failed\n");
    }

    void *ptr6 = kmalloc(2000000); //!if couldnt allocate is printed, it worked.
}