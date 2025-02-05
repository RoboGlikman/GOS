#pragma once
#include "../stdint.h"
void ls();
void cat();
void wf();
void touch();
void echo();
void help();
void clear();
uint32_t getCommand();
void rm();
void clrf();
enum commands {
    LS,
    CAT,
    TOUCH,
    WF,
    ECHO,
    HELP,
    CLEAR,
    RM,
    ERROR,
    CLRF
};
