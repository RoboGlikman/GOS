#include "../stdlib/stdio/stdio.h"
#include "../MemoryManager/mm.h"
#include "../stdlib/string/string.h"
#include "../fs/ramfs/ramfs.h"
#include "../stdlib/memutil/memutil.h"
#include "../stdint.h"
#include "kshell.h"
#include "../vga/vga.h"
#include "../shared/shared.h"

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
    else 
        return ERROR;
}

void ls(){ //will probably be inline
    ramfsListFiles();
}

void cat(){

}

void clear(){ //will probably be inline
    Reset();
}

void wf(){

}

void touch(){
    char fname[32];
    char sizeStr[64];
    uint32_t i;
    for (i=6; i<31 + 6 && sequence[i] != ' '; i++){
        fname[i] = sequence[i];
    }
    fname[31] = '\0';

    for (; i < 64; i++){
        sizeStr[i] = sequence[i];
    }

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
    printf("\twf: write to file. usage: [wf] [content]\n");
    printf("\ttouch: create a file. usage: [touch] [filename] (size)\n");
    printf("\techo: prints whatever. usage: [echo] [contentToPrint]\n");
    printf("\tclear: clears the screen. usage: [clear]\n\n");
    printf("notes:\n");
    printf("\tPlease do not create a command line longer than 64 characters.\n");
    printf("\tDeleting the '$> ' and trying a command won't work,\n\tpress enter then try again.\n");
    printf("\tFilenames are up to 31 characters long!\n");
    printf("\tthis shell can be exploited, try it!\n");
    
}
