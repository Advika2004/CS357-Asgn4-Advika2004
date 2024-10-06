#include "decode.h"

int readCode(FILE *file) {
    static uint32_t buffer = 0;   // Buffer to hold up to 3 bytes of data
    static int flag = 1;          

    if (flag) {
        buffer = 0;
        size_t bytesRead = fread(&buffer, 1, 3, file); 
        if (bytesRead == 0) {
            return -1;  
        } else if(bytesRead < 3){
            int extraBuffer = 0;
            extraBuffer = (((buffer>>12) & 0xFF)|(buffer<<4)) & 0xFFF;

            return extraBuffer;
        }

        buffer = (buffer & 0xFF) << 16 | ((buffer >> 8) & 0xFF) << 8 | ((buffer >> 16) & 0xFF);

        buffer >>= (3 - bytesRead) * 8;

        flag = 0; 
        return ((buffer >> 12) & 0xFFF);  
    } else {
        flag = 1; 
        return (buffer & 0xFFF); 
    }
}
int nextCode = 0x80;
char *codeToString(int code) {
    static char buffer[10];
    sprintf(buffer, "%d\0", code);
    return buffer;
}

#include <stdio.h>
#include <stdlib.h>

#define MAX_STRINGS 4096  // Adjust size as needed based on expected usage

int decodeIt(Dict *dct, FILE* input, FILE* output) {
    int prevCode = NULL;
    int currCode = NULL;
    char *prevEntry, *currEntry;
    char outputString[4096];
    char *strings[MAX_STRINGS];  // Array to hold pointers to all new strings
    int stringCount = 0;         // Counter to track number of strings

    while ((currCode = readCode(input)) != -1) {
        char *currCodeStr = codeToString(currCode);
        currEntry = dctget(dct, currCodeStr);

        if (currEntry == NULL || currEntry[0] == '\0') {
            if (prevCode != NULL) {
                char *prevCodeStr = codeToString(prevCode);
                prevEntry = dctget(dct, prevCodeStr);
                sprintf(outputString, "%s%c", prevEntry, prevEntry[0]);
                if (stringCount < MAX_STRINGS) {
                    strings[stringCount] = strdup(outputString);
                    dctinsert(dct, codeToString(nextCode++), strings[stringCount]);
                    fputs(outputString, output);
                    stringCount++;
                }
            }
        } else {
            fputs(currEntry, output);
            if (prevCode != NULL) {
                char *prevCodeStr = codeToString(prevCode);
                prevEntry = dctget(dct, prevCodeStr);
                sprintf(outputString, "%s%c", prevEntry, currEntry[0]);
                if (stringCount < MAX_STRINGS) {
                    strings[stringCount] = strdup(outputString);
                    dctinsert(dct, codeToString(nextCode++), strings[stringCount]);
                    stringCount++;
                }
            }
        }
        prevCode = currCode;
    }

    // Free all allocated strings
    for (int i = 0; i < stringCount; i++) {
        free(strings[i]);
    }

    return EXIT_SUCCESS;
}