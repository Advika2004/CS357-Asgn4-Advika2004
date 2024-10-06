#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"
#include "encode.h"
#include "decode.h"
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>

#define ARGSCOUNT 2
#define MAXSIZE 4096


/* test01: Tests creating dictionaries. and empty dictionary */
void test01() {
    Dict *dct = dctcreate();

    assert(dct != NULL);
    assert(dct->cap >= 1);
    assert(dct->arr != NULL);
    assert(dct->size == 0);

    assert(dctkeys(dct) == NULL);
    assert(dctget(dct, "a") == NULL);
    assert(dctremove(dct, "a") == NULL);

    dctdestroy(dct);
}

// test02: Tests creating dictionary and filling it with the values I need
void test02() {
    Dict *dct = dctcreate();  // Creating dictionary
    fillDictionary(dct);      // Filling my dictionary with the codes i know

    // making sure i put all the characters in there

    assert(dct->size == 128); 

    assert(dctget(dct, "+") == (void *)43);
    assert(dctget(dct, "9") == (void *)57);
    assert(dctget(dct, "Q") == (void *)81);
    assert(dctget(dct, "a") == (void *)97);
    assert(dctget(dct, "NUL") == (void *)0);
    assert(dctget(dct, "~") == (void *)126);
    dctdestroy(dct);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
        return EXIT_FAILURE;
    }


    const char *mode = argv[1];
    const char *inputPath = argv[2];
    const char *outputPath = argv[3];

    FILE *input, *output;
    Dict *dict = dctcreate();

    // Open the input file
    input = fopen(inputPath, "rb");
    if (!input) {
        perror("./lzw");
        return EXIT_FAILURE;
    }

    // Open the output file
    output = fopen(outputPath, "wb");
    if (!output) {
        perror("Failed to open output file");
        fclose(input);  // Make sure to close the already opened input file
        return EXIT_FAILURE;
    }

    // Determine operation based on the command line argument
    if (strcmp(mode, "-c") == 0) {
        for (int i = 0; i < 128; i++) {
        char str[2] = {i, 0};
        char* newstring = strdup(str);
        dctinsert(dict, newstring, (void *)(intptr_t)i);
        free(newstring);
        }
        encodeIt(dict, input, output);
    } else if (strcmp(mode, "-x") == 0) {
            char *strings[128];  // Array to hold pointers to all new strings
            int entryCount = 0;  // Counter for dictionary entries
        // for (int i = 0; i <= 0x7f; i++) {
        //     char str[2] = {(char)i, '\0'};
        //     char *newString = strdup(str);
        //     dctinsert(dict, codeToString(i), newString);
        // }
        // decodeIt(dict, input, output);
        for (int i = 0; i <= 0x7f; i++) {
            char str[2] = {(char)i, '\0'};
            char *newString = strdup(str);
            char *codeStr = codeToString(i);
            dctinsert(dict, codeStr, newString);
            strings[entryCount++] = newString;  // Store the pointer for later cleanup
        }

        decodeIt(dict, input, output);  // Perform decoding

        // Free all allocated strings
        for (int i = 0; i < entryCount; i++) {
            free(strings[i]);
        }
    } else {
        fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }

    dctdestroy(dict);

    // Clean up: close opened files
    fclose(input);
    fclose(output);

    return EXIT_SUCCESS;
}
