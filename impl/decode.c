#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "decode.h"
#include "utils.h"

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
	if (strcmp(output, "-") == 0)
	{
		// output file "-" is interpreted as stdout
		bmp_decode_pf(input, stdout);
		return;
	}

	if (strcmp(input, output) == 0)
	{
		// simple check to avoid corruption
		fprintf(stderr, "error: input and output file must not be the same (%s)\n", input);
		return;
	}

	FILE* outputFile;
	if ((outputFile = fopen(output, "wb")) == 0)
	{
		// failed to open
		fprintf(stderr, "error: failed to open output file %s: ", output);
		perror("");
		return;
	}

	bmp_decode_pf(input, outputFile);
	fclose(outputFile);
}

void bmp_decode_pf(char* input, FILE* outputFile)
{
	if (strcmp(input, "-") == 0)
	{
		// input file "-" is interpreted as stdin
		bmp_decode(stdin, outputFile);
		return;
	}

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
	size_t read;
	size_t bytesLeft = header.size;

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
