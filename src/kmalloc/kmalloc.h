#pragma once
#include "../stdint.h"
void kmallocInit(uint32_t initialHeapSize);
void changeHeapSize(int newHeapSize);