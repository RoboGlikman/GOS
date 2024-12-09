#include "string.h"
#include "../stdint.h"

void* memmove(void* dstptr, const void* srcptr, uint32_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (uint32_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (uint32_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

uint32_t strlen(const char* str) {
	uint32_t len = 0;
	while (str[len])
		len++;
	return len;
}

int memcmp(const void* aptr, const void* bptr, uint32_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (uint32_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, uint32_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (uint32_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

int32_t strcmp(char s1[], char s2[]) {
    uint32_t i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

