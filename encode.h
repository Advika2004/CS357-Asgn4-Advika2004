#ifndef ENCODE_H
#define ENCODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include "dict.h"
#define MAXCODES 4096 
#define ARGSCOUNT 2
#define MAXSIZE 4096

void fillDictionary(Dict *dct);
int encodeIt(Dict *dct, FILE *input, FILE *output);

#endif