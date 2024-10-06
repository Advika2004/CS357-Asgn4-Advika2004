#ifndef DECODE_H
#define DECODE_H

#include "dict.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdint.h>



int readCode(FILE *file);
char *codeToString(int code); 
int decodeIt(Dict *dct, FILE* input, FILE* output);
#endif