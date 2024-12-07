#include "vga.h"
#include "../stdint.h"

uint16_t column = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const) 0xC00B8000;
const uint16_t defaultColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
uint16_t currentColor = defaultColor;
uint16_t lastNonSpaceColumn[height] = {0};

void Reset(){
    line = 0;
    column = 0;
    currentColor = defaultColor;

    for (uint16_t y = 0; y < height; y++){
        for (uint16_t x = 0; x < width; x++){
            vga[y * width + x] = ' ' | defaultColor;
        }
        lastNonSpaceColumn[y] = 0;
    }
}

void newLine(){
    if (line < height - 1){
        line++;
        column = 0;
    }else{
        scrollUp();
        column = 0;
    }
}

void scrollUp(){
    for (uint16_t y = 1; y < height; y++){
        for (uint16_t x = 0; x < width; x++){
            vga[(y - 1) * width + x] = vga[y * width + x];
        }
        lastNonSpaceColumn[y - 1] = lastNonSpaceColumn[y];
    }

    for (uint16_t x = 0; x < width; x++){
        vga[(height - 1) * width + x] = ' ' | currentColor;
    }
    lastNonSpaceColumn[height - 1] = 0;
}

void print(const char* s){
    while (*s) {
        switch (*s) {
            case '\n':
                newLine();
                break;
            case '\r':
                column = 0;
                break;
            case '\b':
                if (column > 0) {
                    column--;
                } else if (line > 0) {
                    line--;
                    column = lastNonSpaceColumn[line];
                }
                vga[line * width + column] = ' ' | currentColor;
                lastNonSpaceColumn[line] = (column > 0) ? column - 1 : 0;
                break;
            case '\t':
                if (column == width) {
                    newLine();
                }
                uint16_t tabLen = 4 - (column % 4);
                while (tabLen != 0) {
                    vga[line * width + (column++)] = ' ' | currentColor;
                    tabLen--;
                }
                break;
            default:
                if (column == width) {
                    newLine();
                }
                vga[line * width + column] = *s | currentColor;
                lastNonSpaceColumn[line] = column+1; 
                column++;
                break;
        }
        s++;
    }
}
