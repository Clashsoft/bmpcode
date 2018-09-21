
#include "main.h"

int main(int argc, char** args)
{
	if (strcmp("-e", args[1]) == 0)
	{
		switch (argc)
		{
			case 2:
				bmp_encode(stdin, stdout);
				return 0;
			case 3:
				bmp_encode_p(args[2]);
				return 0;
			case 4:
				bmp_encode_pp(args[2], args[3]);
				return 0;
			default:;
		}
	}
	else if (strcmp("-d", args[1]) == 0)
	{
		switch (argc)
		{
			case 2:
				bmp_decode(stdin, stdout);
				return 0;
			case 3:
				bmp_decode_p(args[2]);
				return 0;
			case 4:
				bmp_decode_pp(args[2], args[3]);
				return 0;
			default:;
		}
	}

	fprintf(stderr, "usage: bmpdat [-e | -d] [input-file [output-file]]");
}

bmp_header_t bmp_header(size_t size)
{
	uint32_t wh = (uint32_t) ceil(sqrt((size + 8) / 3.0));
	wh += 4 - wh % 4; // ensure wh is a multiple of 4
	return (bmp_header_t) {wh, wh, size};
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
	return read4le(&a[0]) | read4le(&a[4]) << 32u;
}

void bmp_write_header(FILE* file, bmp_header_t header)
{
	uint32_t w = header.width;
	uint32_t h = header.height;
	size_t   d = header.size;
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

	uint32_t width  = read4le(&header[BMP_HEADER_SIZE + 4]);
	uint32_t height = read4le(&header[BMP_HEADER_SIZE + 8]);
	size_t   data   = read8le(&header[BMP_HEADER_SIZE + BMP_INFO_SIZE]);

	return (bmp_header_t) {width, height, data};
}

// --------------------------------------------------- ENCODING ---------------------------------------------------

void bmp_encode_p(char* input)
{
	char* output = malloc(strlen(input) + 4);
	strcpy(output, input);
	strcat(output, ".bmp");
	bmp_encode_pp(input, output);
	free(output);
}

void bmp_encode_pp(char* input, char* output)
{
	if (strcmp(input, output) == 0)
	{
		fprintf(stderr, "error: input and output file must not be the same (%s)\n", input);
		return;
	}

	FILE* outputFile;
	if ((outputFile = fopen(output, "wb")) == 0)
	{
		fprintf(stderr, "error: failed to open output file %s: ", output);
		perror("");
		return;
	}

	bmp_encode_pf(input, outputFile);
	fclose(outputFile);
}

void bmp_encode_pf(char* input, FILE* outputFile)
{
	FILE* inputFile;
	if ((inputFile = fopen(input, "rb")) == 0)
	{
		fprintf(stderr, "error: failed to open input file %s: ", input);
		perror("");
		return;
	}
	bmp_encode(inputFile, outputFile);
	fclose(inputFile);
}

void bmp_encode(FILE* inputFile, FILE* outputFile)
{
	bmp_write_header(outputFile, (bmp_header_t) {});

	uint8_t buffer[BUFFER_SIZE];
	size_t read;
	size_t totalSize = 0;

	while ((read = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
	{
		fwrite(buffer, 1, read, outputFile);
		totalSize += read;
	}
	fclose(inputFile);

	bmp_header_t wh = bmp_header(totalSize);

	size_t pixelSize = 3 * (size_t) wh.width * (size_t) wh.height;
	size_t diff      = pixelSize - totalSize;

	for (size_t i = 0; i < diff; i++)
	{
		fputc(0, outputFile);
	}

	rewind(outputFile);
	bmp_write_header(outputFile, wh);

	fclose(outputFile);
}

// --------------------------------------------------- DECODING ---------------------------------------------------

void bmp_decode_p(char* input)
{

	size_t len = strlen(input);
	if (strncmp(input + len - 4, ".bmp", 4) != 0)
	{
		fprintf(stderr, "error: cannot automatically determine name of output file from %s\n", input);
		return;
	}

	char* output = malloc(len - 4);
	strncpy(output, input, len - 4);
	bmp_decode_pp(input, output);
	free(output);
}

void bmp_decode_pp(char* input, char* output)
{
	if (strcmp(input, output) == 0)
	{
		fprintf(stderr, "error: input and output file must not be the same (%s)\n", input);
		return;
	}

	FILE* outputFile;
	if ((outputFile = fopen(output, "wb")) == 0)
	{
		fprintf(stderr, "error: failed to open output file %s: ", output);
		perror("");
		return;
	}

	bmp_decode_pf(input, outputFile);
	fclose(outputFile);
}

void bmp_decode_pf(char* input, FILE* outputFile)
{
	FILE* inputFile;
	if ((inputFile = fopen(input, "rb")) == 0)
	{
		fprintf(stderr, "error: failed to open input file %s: ", input);
		perror("");
		return;
	}
	bmp_decode(inputFile, outputFile);
	fclose(inputFile);
}

void bmp_decode(FILE* inputFile, FILE* outputFile)
{
	bmp_header_t header = bmp_read_header(inputFile);
	if (IS_ERROR_HEADER(header))
	{
		fprintf(stderr, "error: invalid bmp file: invalid header\n");
		return;
	}

	uint8_t buffer[BUFFER_SIZE];
	size_t  read;
	size_t  bytesLeft   = header.size;

	while (bytesLeft > 0 && (read = fread(buffer, 1, bytesLeft < BUFFER_SIZE ? bytesLeft : BUFFER_SIZE, inputFile)) > 0)
	{
		fwrite(buffer, 1, read, outputFile);
		bytesLeft -= read;
	}

	if (bytesLeft != 0)
	{
		fprintf(stderr, "error: invalid bmp file: invalid length\n");
		return;
	}
}
