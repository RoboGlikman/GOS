#include "../stdlib/stdio/stdio.h"
#include "../MemoryManager/mm.h"
#include "../stdlib/string/string.h"
#include "../fs/ramfs/ramfs.h"
#include "../stdlib/memutil/memutil.h"
#include "../stdint.h"
#include "kshell.h"
#include "../vga/vga.h"
#include "../shared/shared.h"

void initKshell(){ //TODO please examine todo.txt

    printf("$> ");
    while (1){
        printf("%d\n", sequenceIndex);
        if (sequenceIndex > 0){
            uint32_t command = getCommand();       
            if (command == 69); //magic
            else {
                switch (command) {
                    case LS:
                        ls();
                        break;
                    case CAT:
                        cat((const char *)&sequence[sequenceIndex + 4]);
                        break;
                    case TOUCH:
                        touch((const char *)&sequence[sequenceIndex + 6], (const char *)&sequence[getSecondArgIndex(sequenceIndex + 6)]);
                        break;
                    case WF:
                        wf((const char *)&sequence[sequenceIndex + 3], (const char *)&sequence[getSecondArgIndex(sequenceIndex + 3)]);
                        break;
                    case ECHO:
                        echo((const char *)&sequence[sequenceIndex + 5]);
                        break;
                    case CALC:
                        printf("Option not supported yet!\n");
                        //calc((const char *)&sequence[sequenceIndex + 5]);
                        break;
                    case HELP:
                        help();
                        break;
                    case WELCOME:
                        welcome();
                        break;
                    default:
                        printf("Error! Unknown command!\n");
                        break;
                }
            }
            memset(sequence, 0, sizeof(sequence));
            sequenceIndex = 0;
            
        }
    }
}

static int getSecondArgIndex(int firstArgIndex){
    for (int i=firstArgIndex; i < 64; i++){
        if (sequence[i] == ' ')
            return i;
    }
}

static uint32_t getCommand(){
    char command[32];
    if (sequence[0] == '\n')
        return 69; //magic

    memcpy(command, sequence, sequenceIndex);
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
    else if (strcmp(command, "welcome")){
        
        return WELCOME; }
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