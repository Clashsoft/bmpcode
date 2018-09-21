
#include <math.h>
#include "utils.h"

bmp_header_t bmp_header(size_t size)
{
	uint32_t wh = (uint32_t) ceil(sqrt((size + 8) / 3.0));
	wh += 4 - wh % 4; // ensure wh is a multiple of 4
	return (bmp_header_t) { wh, wh, size };
}

void write4le(uint8_t* a, uint32_t value)
{
	a[0] = (uint8_t) (value >> 0u);
	a[1] = (uint8_t) (value >> 8u);
	a[2] = (uint8_t) (value >> 16u);
	a[3] = (uint8_t) (value >> 24u);
}

void write8le(uint8_t* a, uint64_t value)
{
	write4le(&a[0], (uint32_t) value);
	write4le(&a[4], (uint32_t) (value >> 32u));
}

uint32_t read4le(const uint8_t* a)
{
	return ((uint32_t) a[0]) | ((uint32_t) a[1] << 8u) | ((uint32_t) a[2] << 16u) | ((uint32_t) a[3] << 24u);
}

uint64_t read8le(const uint8_t* a)
{
	return read4le(&a[0]) | (uint64_t) read4le(&a[4]) << 32u;
}

void bmp_write_header(FILE* file, bmp_header_t header)
{
	uint32_t w = header.width;
	uint32_t h = header.height;
	size_t d = header.size;
	uint32_t s = 54 + 3 * w * h;

	// file header

	uint8_t fileHeader[BMP_HEADER_SIZE] = {
			'B', 'M', // type
			0, 0, 0, 0, // size
			0, 0, 0, 0, // reserved
			54, 0, 0, 0 // offset
	};

	write4le(&fileHeader[2], s);

	fwrite(fileHeader, 1, BMP_HEADER_SIZE, file);

	// info header

	uint8_t infoHeader[BMP_INFO_SIZE] = {
			40, 0, 0, 0, // info header size
			0, 0, 0, 0, // width
			0, 0, 0, 0, // height
			1, 0, // planes
			24, 0 // bits per pixel
	};

	write4le(&infoHeader[4], w);
	write4le(&infoHeader[8], h);

	fwrite(infoHeader, 1, BMP_INFO_SIZE, file);

	// size

	uint8_t dataHeader[8];

	write8le(dataHeader, d);

	fwrite(dataHeader, 1, 8, file);
}

bmp_header_t bmp_read_header(FILE* file)
{
	uint8_t header[BMP_HEADER_SIZE + BMP_INFO_SIZE + 8];
	if (fread(header, 1, sizeof(header), file) != sizeof(header) || header[0] != 'B' || header[1] != 'M')
	{
		return ERROR_HEADER;
	}

	uint32_t width = read4le(&header[BMP_HEADER_SIZE + 4]);
	uint32_t height = read4le(&header[BMP_HEADER_SIZE + 8]);
	size_t data = read8le(&header[BMP_HEADER_SIZE + BMP_INFO_SIZE]);

	return (bmp_header_t) { width, height, data };
}

