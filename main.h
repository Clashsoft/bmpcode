//
// Created by Adrian Kunz on 2018-05-11.
//

#ifndef FILE2BMP_MAIN_H
#define FILE2BMP_MAIN_H

#include <string.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct width_height_t
{
	int width;
	int height;
} width_height_t;

width_height_t bmp_width_height(size_t size);

void bmp_encode(FILE* inputFile, FILE* outputFile);

void bmp_encode_pf(char* input, FILE* outputFile);

void bmp_encode_pp(char* input, char* output);

void bmp_encode_p(char* input);

void bmp_write_header(FILE* file, unsigned int w, unsigned int h);

#endif //FILE2BMP_MAIN_H
