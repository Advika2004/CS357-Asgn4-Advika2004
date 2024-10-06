// #include <stdlib.h>
// #include <string.h>
// #include <assert.h>
// #include "dict.h"
#include "encode.h"
// #include <stdio.h>
// #include <stddef.h>
// #include <ctype.h>
// #define MAXCODES 4096 
// #define ARGSCOUNT 2
// #define MAXSIZE 4096

void fillDictionary(Dict *dct){
    char key[2];
    int i = 0;

    while(i != 0x7F){
        key[0] = i;
        key[1] = '\0';
        dctinsert(dct, key, (void*) i);
        i++;
    }
    key[0] = i;
    dctinsert(dct, key, (void*) i);

}

int newCode = 0x80; 

int encodeIt(Dict *dct, FILE *input, FILE *output) {
    char currentString[MAXSIZE] = {0};
    char copiedString[MAXSIZE] = {0};
    int currentLength = 0;
    int arrayOfCodes[MAXCODES] = {0};  
    int codeIndex = 0; 

    int ascii;
    int placeHolder;
    while ((placeHolder = fgetc(input)) != EOF) {
        ascii = placeHolder;
        if (currentLength >= MAXSIZE - 1){
            break; 
        }
        currentString[currentLength] = (char)ascii; 
        currentLength++; 
        currentString[currentLength] = '\0'; 

        if (dctget(dct, currentString) != NULL) {
            continue; 
        }
        else{
            dctinsert(dct, currentString, (void*)(long)newCode);
            newCode++;

            strcpy(copiedString , currentString);
            copiedString[currentLength - 1] = '\0';
            int value = (void*)dctget(dct, copiedString); 
            arrayOfCodes[codeIndex] = (int)value;  
            codeIndex++;
        
            currentString[0] = (char)ascii; 
            currentLength = 1;
        }
    }
    
    if (dctget(dct, currentString) != NULL){
        int value2 = dctget(dct, currentString);
        arrayOfCodes[codeIndex] = value2; 
        codeIndex++;
    }
    else{
        arrayOfCodes[codeIndex] = ascii; 
        codeIndex++;
    }

    int buffer = 0;

    int flag = 0;

    for (int i = 0; i < codeIndex; i++) {
        buffer <<= 12;          
        buffer |= arrayOfCodes[i];


        if (flag != 0) { 
            fputc((buffer >> 16) & 0xFF, output);
            fputc((buffer >> 8) & 0xFF, output);  
            fputc(buffer & 0xFF, output);         
            buffer = 0;           
        }
        flag = !flag;          
    }
    if (buffer!=0){
        fputc((buffer >> 4) & 0xFF, output);
        fputc((buffer<<4) & 0xFF, output);
    }

    return EXIT_SUCCESS;
}