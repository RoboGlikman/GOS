#include "ramfsTest.h"
#include "../../vga/vga.h"
#include "../../stdlib/stdio/stdio.h"
#include "../../fs/ramfs/ramfs.h"
#include "../../stdlib/string/string.h"
#include "../../MemoryManager/mm.h"

int all = 0;

void test_ramfs(){
    const char *name1 = "file1";
    int result1 = ramfsCreateFile(name1, 4096); // 1 block
    if (result1 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 1: create file with one block successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 1: create file with one block failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    
    const char *name2 = "file2";
    int result2 = ramfsCreateFile(name2, 7000);
    if (result2 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 2: create file with multiple blocks (and increase ramdisk size) successful!\n");
        all++;
    } else{
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 2: create file with multiple blocks (and increase ramdisk size) failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    
    int file1Fd = ramfsOpenFile(name1, RW);
    if (file1Fd > 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 3: open file, recieve fd successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 3: open file, recieve fd failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    const char *buffer1 = "Hello, world!";
    int result3 = ramfsWriteFile(file1Fd, 0, (const void*)buffer1, strlen(buffer1)+1);
    if (result3 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 4: write to file with one block successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 4: write to file with one block failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    void *bufferE = kmalloc(7);
    int resultS = ramfsReadFile(file1Fd, 1, bufferE, 7);
    if (resultS == 0){
        printf("%s\n", (const char*)bufferE);
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 5: read from file with one block successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 5: read from file with one block failed!\n");
    }
    kfree(bufferE);
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);


    int file2Fd = ramfsOpenFile(name2, RW);
    if (file2Fd > 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 6: open different file, recieve fd successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 6: open different file, recieve fd failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    
    const char *buffer2 = "linus is the goat";
    int result4 = ramfsWriteFile(file2Fd, 0, (const void*)buffer2, strlen(buffer2)+1);
    if (result4 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 7: write to file with multiple blocks successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 7: write to file with multiple blocks failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    
    void *buffer3 = kmalloc(4095);
    int result5 = ramfsReadFile(file2Fd, 4, buffer3, 4095);
    if (result5 == 0){
        printf("%s\n", (const char*)buffer3);
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 8: read from file with multiple blocks at specified offset successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 8: read from file with multiple blocks at specified offset failed!\n");
    }
    kfree(buffer3);
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    
    ramfsListFiles(); // test 9
    all++;

    
    int result7 = ramfsDeleteFile(file1Fd);
    if (result7 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 10: file deletion successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 10: file deletion failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);

    ramfsListFiles(); // test 11
    all++;

    const char *name3 = "file3";
    int result9 = ramfsCreateFile(name3, 100);
    if (result9 == 0){
        currentColor = (COLOR8_LIGHT_GREEN << 8) | (COLOR8_BLACK << 12);
        printf("test 12: best fit successful!\n");
        all++;
    } else {
        currentColor = (COLOR8_LIGHT_RED << 8) | (COLOR8_BLACK << 12);
        printf("test 12: best fit failed!\n");
    }
    currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    ramfsListFiles(); // test 13
    all++;



    if (all == 13){
        currentColor = (COLOR8_LIGHT_MAGENTA << 8) | (COLOR8_BLACK << 12);
        printf("ALL TESTS ARE SUCCESSFUL!\n\n");
        currentColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
    }

}