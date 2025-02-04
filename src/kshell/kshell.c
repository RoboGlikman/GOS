#include "../stdlib/stdio/stdio.h"
#include "../MemoryManager/mm.h"
#include "../stdlib/string/string.h"
#include "../fs/ramfs/ramfs.h"
#include "../stdlib/memutil/memutil.h"
#include "../stdint.h"
#include "kshell.h"
#include "../vga/vga.h"
#include "../shared/shared.h"


int getSecondArgIndex(int firstArgIndex){
    for (int i=firstArgIndex; i < 64; i++){
        if (sequence[i] == ' ')
            return i;
    }
}

uint32_t getCommand(){
    char command[32];
    uint32_t i=0;
    
    memcpy(command, sequence, i);
    command[i] = '\0';
    printf("cm: %s\n", command);
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
    else if (strcmp(command, "calc") == 0)
        return CALC;
    else if (strcmp(command, "help") == 0)
        return HELP;
    else if (strcmp(command, "welcome"))
        return WELCOME; 
    else 
        return ERROR;
}

void ls(){
    
}

void cat(const char *fname){

}

void wf(const char *fname, const char *content){

}

void touch(const char *fname, const char *size){

}

void echo(const char *content){

}

void startCalc(){

}

void quitCalc(){

}

void help(){

}

void welcome(){

}