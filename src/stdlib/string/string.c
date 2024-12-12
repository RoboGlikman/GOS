#include "string.h"
#include "../../stdint.h"

char* strcat(char *dest, char *src) {	
    char *rdest = dest;
    while (*dest)
      dest++;
    while (*dest++ = *src++);
    return rdest;
}

uint32_t strlen(const char* str) {
	uint32_t len = 0;
	while (str[len])
		len++;
	return len;
}

int32_t strcmp(char s1[], char s2[]) {
    uint32_t i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

char * strncpy(char *dest, const char *src, uint32_t n){
    uint32_t i;
   for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

   return dest;
}
