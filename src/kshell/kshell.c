#include "../stdlib/stdio/stdio.h"
#include "../MemoryManager/mm.h"
#include "../stdlib/string/string.h"
#include "../fs/ramfs/ramfs.h"
#include "../stdlib/memutil/memutil.h"
#include "../stdint.h"
#include "kshell.h"
#include "../vga/vga.h"
#include "../shared/shared.h"
#include "../MemoryManager/mm.h"

const char SPACE = 28;
//! in touch, echo, wf, cat: start of i=indexOfSpaceBeforeSecondArg
uint32_t getCommand(){
    char command[32];

    uint32_t i=0;
    for (; i<32 && sequence[i] != ' '; i++)
        command[i] = sequence[i];
    
    command[i] = '\0';
    if (strcmp(command, "ls") == 0)
        return LS;
    else if (strcmp(command, "cat") == 0)
        return CAT;
    else if (strcmp(command, "touch") == 0)
        return TOUCH;
    else if (strcmp(command, "wf") == 0)
        return WF;
    else if (strcmp(command, "echo") == 0)
        return ECHO;
    else if (strcmp(command, "help") == 0 || strcmp(command, "man") == 0)
        return HELP;
    else if (strcmp(command, "clear") == 0)
        return CLEAR;
    else if (strcmp(command, "rm") == 0)
        return RM;
    else 
        return ERROR;
}

void ls(){ //will probably be inline
    ramfsListFiles();
}

void cat(){
    char fname[32];
    for (uint32_t i=4; i<31 + 4; i++){
        fname[i-4] = sequence[i];
    }

    fname[31] = '\0';
    uint32_t fd = ramfsOpenFile(fname, R);
    if (fd < 0)
        return;
    
    uint32_t size = ramfsGetFileSize(fd);
    
    void *buffer = kmalloc(size);
    if (ramfsReadFile(fd, 0, buffer, size) != 0){ // file contents len <<<< size
        printf("error reading file!\n");
        return;
    }
    
    printf("%s\n", (const char*)buffer);

    ramfsCloseFile(fd);
    kfree(buffer);
}

void clear(){ //will probably be inline
    Reset();
}

void wf(){
    char fname[32];
    char content[64];
    char offsetStr[64];

    uint32_t i;
    for (i=3; i<31 + 3; i++){
        if (sequence[i] == ' '){
            i++;
            break;
        }
        fname[i-3] = sequence[i];
    }
    fname[i-3-1] = '\0';

    uint32_t j=0;
    for (; i < 64; i++){
        if (sequence[i] == '-' || sequence[i] == '\0'){
            i++;
            j++;
            break;
        }
        content[j] = sequence[i];
        j++;
    }
    content[j] = '\0';

    uint32_t m=0;
    for(; i<64 && sequence[i] != '\0'; i++){
        offsetStr[m] = sequence[i];
        m++;
    }
    offsetStr[m] = '\0';

    uint32_t offset = atoi((const char*)offsetStr) == 0 ? 0 : atoi((const char*)offsetStr);

    uint32_t fd = ramfsOpenFile((const char*)fname, W);
    if (fd < 0) return;
    if (ramfsWriteFile(fd, offset, (const void*)content, strlen((const char*)content)+1) != 0){
        printf("error writing to file!\n");
        ramfsCloseFile(fd);
        return;
    }
    ramfsCloseFile(fd);
}

void touch(){
    char fname[32];
    char sizeStr[64];
    uint32_t i;
    for (i=6; i<31 + 6; i++){
        if (sequence[i] == ' '){
            i++;
            break;
        }
        fname[i-6] = sequence[i];
    }
    fname[i-6-1] = '\0';

    uint32_t j=0;
    for (; i < 64 && sequence[i] != '\0'; i++){
        sizeStr[j] = sequence[i];
        j++;
    }
    sizeStr[j] = '\0';
    uint32_t size = atoi((const char*)sizeStr) == 0 ? 4096 : atoi((const char*)sizeStr);
    ramfsCreateFile(fname, size); 
}

void echo(){
    for (uint32_t i=5; i< (64-5); i++){
        printf("%c", sequence[i]);
    }
    printf("\n");
}


void help(){
    printf("command guide:\n");
    printf("\tsquare brackets: necessary, normal brackets: optional\n\n");
    printf("commands:\n");
    printf("\thelp/man: shows this page. usage: [help/man]\n");
    printf("\tls: lists files in entire fs. usage: [ls]\n");
    printf("\tcat: outputs content of a file. usage: [cat] [filename]\n");
    printf("\twf: write to file. usage: [wf] [filename] [content](-offset)\n");
    printf("\ttouch: create a file. usage: [touch] [filename] (size)\n");
    printf("\techo: prints whatever. usage: [echo] [contentToPrint]\n");
    printf("\tclear: clears the screen. usage: [clear]\n");
    printf("\trm: removes specified file. usage: [rm] [filename]\n\n");
    printf("notes:\n");
    printf("\tPlease do not create a command line longer than 64 characters.\n");
    printf("\tDeleting the '$> ' and trying a command won't work,\n\tpress enter then try again.\n");
    printf("\tFilenames are up to 31 characters long!\n");
    printf("\tthis shell can be exploited, try it!\n");
    
}

void rm(){
    char fname[32];
    uint32_t i;
    for (i=3; i<31 + 3; i++){
        if (sequence[i] == ' '){
            i++;
            break;
        }
        fname[i-3] = sequence[i];
    }
    fname[i-3-1] = '\0';

    uint32_t fd = ramfsOpenFile(fname, RWX);
    ramfsDeleteFile(fd);
    ramfsCloseFile(fd);
}
