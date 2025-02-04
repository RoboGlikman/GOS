#include "../stdint.h"
#include "../stdlib/memutil/memutil.h"
#include "../idt/idt.h"
#include "../stdlib/stdio/stdio.h"
#include "keyboard.h"
#include "../kshell/kshell.h"
#include "../shared/shared.h"

bool capsOn;
bool capsLock;
const char BACKSPACE = 14;
const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC = 0xFFFFFFFF - 1;
const uint32_t CTRL = 0xFFFFFFFF - 2;
const uint32_t LSHFT = 0xFFFFFFFF - 3;
const uint32_t RSHFT = 0xFFFFFFFF - 4;
const uint32_t ALT = 0xFFFFFFFF - 5;
const uint32_t F1 = 0xFFFFFFFF - 6;
const uint32_t F2 = 0xFFFFFFFF - 7;
const uint32_t F3 = 0xFFFFFFFF - 8;
const uint32_t F4 = 0xFFFFFFFF - 9;
const uint32_t F5 = 0xFFFFFFFF - 10;
const uint32_t F6 = 0xFFFFFFFF - 11;
const uint32_t F7 = 0xFFFFFFFF - 12;
const uint32_t F8 = 0xFFFFFFFF - 13;
const uint32_t F9 = 0xFFFFFFFF - 14;
const uint32_t F10 = 0xFFFFFFFF - 15;
const uint32_t F11 = 0xFFFFFFFF - 16;
const uint32_t F12 = 0xFFFFFFFF - 17;
const uint32_t SCRLCK = 0xFFFFFFFF - 18;
const uint32_t HOME = 0xFFFFFFFF - 19;
const uint32_t UP = 0xFFFFFFFF - 20;
const uint32_t LEFT = 0xFFFFFFFF - 21;
const uint32_t RIGHT = 0xFFFFFFFF - 22;
const uint32_t DOWN = 0xFFFFFFFF - 23;
const uint32_t PGUP = 0xFFFFFFFF - 24;
const uint32_t PGDOWN = 0xFFFFFFFF - 25;
const uint32_t END = 0xFFFFFFFF - 26;
const uint32_t INS = 0xFFFFFFFF - 27;
const uint32_t DEL = 0xFFFFFFFF - 28;
const uint32_t CAPS = 0xFFFFFFFF - 29;
const uint32_t NONE = 0xFFFFFFFF - 30;
const uint32_t ALTGR = 0xFFFFFFFF - 31;
const uint32_t NUMLCK = 0xFFFFFFFF - 32;


const uint32_t lowercase[128] = {
UNKNOWN,ESC,'1','2','3','4','5','6','7','8',
'9','0','-','=','\b','\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n',CTRL,
'a','s','d','f','g','h','j','k','l',';',
'\'','`',LSHFT,'\\','z','x','c','v','b','n','m',',',
'.','/',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',LEFT,UNKNOWN,RIGHT,
'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};

const uint32_t uppercase[128] = {
    UNKNOWN,ESC,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
'T','Y','U','I','O','P','{','}','\n',CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',LSHFT,'|','Z','X','C',
'V','B','N','M','<','>','?',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',
LEFT,UNKNOWN,RIGHT,'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};

void keyboardHandler(struct InterruptRegisters *regs){
    char scanCode = inPortB(0x60) & 0x7F; //What key is pressed
    char press = inPortB(0x60) & 0x80; //Press down, or released
    switch(scanCode){
        case 1:
        case 29:
        case 56:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 87:
        case 88:
            break;
        case 42:
            //shift key
            if (press == 0){
                capsOn = true;
            }else{
                capsOn = false;
            }
            break;
        case 58:
            if (!capsLock && press == 0){
                capsLock = true;
            }else if (capsLock && press == 0){
                capsLock = false;
            }
            break;
        default:
            if (press == 0){
                if (capsOn || capsLock){
                    //printf("%c", uppercase[scanCode]); //! uppercase letters are not allowed in the current shell, but have a skeleton to later support.
                }else{ //backspace == 14
                    printf("%c", lowercase[scanCode]);
                    if (scanCode == BACKSPACE && sequenceIndex > 0){
                        sequence[sequenceIndex - 1] = 0;
                        sequenceIndex--;
                    }
                    else {
                        sequence[sequenceIndex] = (char)lowercase[scanCode];
                        if (sequence[sequenceIndex] == '\n' && sequenceIndex > 0){
                            sequence[sequenceIndex] = '\0';
                            uint32_t command = getCommand();   
                            switch (command){
                                case LS:
                                    ls();
                                    break;
                                case CAT:
                                    cat();
                                    break;
                                case TOUCH:
                                    touch();
                                    break;
                                case WF:
                                    wf();
                                    break;
                                case ECHO:
                                    echo();
                                    break;
                                case HELP:
                                    help();
                                    break;
                                case CLEAR:
                                    clear();
                                    break;
                                case RM:
                                    rm();
                                    break;
                                case 69:
                                    break;
                                default:
                                    printf("Unknown command! Try 'help'\n");
                                    break;
                            }
                            memset(sequence, 0, sizeof(sequence));
                            sequenceIndex = 0;
                            printf("$> ");
                        } else {
                            if (sequence[sequenceIndex] == '\n'){
                                memset(sequence, 0, sizeof(sequence));
                                sequenceIndex = 0;
                                printf("$> ");
                            } else 
                                sequenceIndex++;}
                    }
                }
            }  
    } 
}

void initKeyboard(){
    printf("$> ");
    sequenceIndex = 0;
    memset(sequence, 0, sizeof(sequence));
    capsOn = false;
    capsLock = false;
    irq_install_handler(1,&keyboardHandler);
}

void killKeyboard(){
    irq_uninstall_handler(1);
}
