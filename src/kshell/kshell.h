#pragma once
#include "../stdint.h"
void initKshell();
void ls();
void cat(const char *fname);
void wf(const char *fname, const char *content);
void touch(const char *fname, const char *size);
void echo(const char *content);
void startCalc();
void quitCalc();
void help();
void welcome();
uint32_t getCommand();
int getSecondArgIndex(int firstArgIndex);
enum commands {
    LS,
    CAT,
    TOUCH,
    WF,
    ECHO,
    CALC,
    HELP,
    WELCOME,
    ERROR
};
