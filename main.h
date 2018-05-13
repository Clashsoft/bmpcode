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
	uint32_t width;
	uint32_t height;
	size_t size;
} bmp_header_t;

#define ERROR_HEADER ((bmp_header_t) { UINT32_MAX, UINT32_MAX, SIZE_MAX })

#define IS_ERROR_HEADER(x) ((x).width == UINT32_MAX && (x).height == UINT32_MAX && (x).size == SIZE_MAX)

#define BMP_HEADER_SIZE 14
#define BMP_INFO_SIZE 40

bmp_header_t bmp_header(size_t size);

void bmp_write_header(FILE* file, bmp_header_t);

void bmp_encode(FILE* inputFile, FILE* outputFile);

void bmp_encode_pf(char* input, FILE* outputFile);

void bmp_encode_pp(char* input, char* output);

void bmp_encode_p(char* input);


#endif //FILE2BMP_MAIN_H
