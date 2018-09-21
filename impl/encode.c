#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "encode.h"
#include "utils.h"

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
	size_t diff = pixelSize - totalSize;

	for (size_t i = 0; i < diff; i++)
	{
		fputc(0, outputFile);
	}

	rewind(outputFile);
	bmp_write_header(outputFile, wh);

	fclose(outputFile);
}
