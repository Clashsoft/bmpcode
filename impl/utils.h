#ifndef BMPCODE_UTILS_H
#define BMPCODE_UTILS_H


#include <zconf.h>
#include <stdint.h>
#include <stdio.h>

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

#define BUFFER_SIZE 4096

bmp_header_t bmp_header(size_t size);

void bmp_write_header(FILE* file, bmp_header_t);

void write4le(uint8_t* a, uint32_t value);

void write8le(uint8_t* a, uint64_t value);

uint32_t read4le(const uint8_t* a);

uint64_t read8le(const uint8_t* a);

void bmp_write_header(FILE* file, bmp_header_t header);

bmp_header_t bmp_read_header(FILE* file);

#endif //BMPCODE_UTILS_H
