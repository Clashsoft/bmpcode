#ifndef BMPCODE_ENCODE_H
#define BMPCODE_ENCODE_H

#include <stdio.h>

void bmp_encode(FILE* inputFile, FILE* outputFile);

void bmp_encode_pf(char* input, FILE* outputFile);

void bmp_encode_pp(char* input, char* output);

void bmp_encode_p(char* input);

#endif //BMPCODE_ENCODE_H
