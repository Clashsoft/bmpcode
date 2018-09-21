#ifndef BMPCODE_DECODE_H
#define BMPCODE_DECODE_H

#include <stdio.h>

void bmp_decode(FILE* inputFile, FILE* outputFile);

void bmp_decode_pf(char* input, FILE* outputFile);

void bmp_decode_pp(char* input, char* output);

void bmp_decode_p(char* input);

#endif //BMPCODE_DECODE_H
