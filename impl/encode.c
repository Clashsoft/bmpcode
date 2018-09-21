#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

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
	if (strcmp(output, "-") == 0)
	{
		// output file "-" is interpreted as stdout
		bmp_encode_pf(input, stdout);
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

	bmp_encode_pf(input, outputFile);
	fclose(outputFile);
}

void bmp_encode_pf(char* input, FILE* outputFile)
{
	if (strcmp(input, "-") == 0)
	{
		// input file "-" is interpreted as stdin
		bmp_encode(stdin, outputFile);
		return;
	}

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

static off_t file_size(FILE* file)
{
	struct stat st;
	fstat(fileno(file), &st);
	return S_ISREG(st.st_mode) ? st.st_size : -1;
}

static size_t copy_data(FILE* source, FILE* dest)
{
	uint8_t buffer[BUFFER_SIZE];
	size_t read;
	size_t total = 0;

	while ((read = fread(buffer, 1, sizeof(buffer), source)) > 0)
	{
		fwrite(buffer, 1, read, dest);
		total += read;
	}

	return total;
}

static void write_padding(FILE* output, bmp_header_t header)
{
	size_t pixel_size = 3 * (size_t) header.width * (size_t) header.height;
	size_t diff = pixel_size - header.size;

	for (size_t i = 0; i < diff; i++)
	{
		fputc(0, output);
	}
}

static void bmp_encode_simple(FILE* input_stream, size_t input_size, FILE* output_stream)
{
	bmp_header_t header = bmp_header(input_size);

	// header
	bmp_write_header(output_stream, header);

	// data
	copy_data(input_stream, output_stream);

	// padding
	write_padding(output_stream, header);
}

static void bmp_encode_rewind(FILE* input_stream, FILE* output_file)
{
	// write an error header at first, because we don't know the real dimensions yet
	bmp_write_header(output_file, ERROR_HEADER);

	// data copy yields length of input
	size_t input_size = copy_data(input_stream, output_file);

	// now we can compute the dimensions
	bmp_header_t header = bmp_header(input_size);

	// padding
	write_padding(output_file, header);

	// rewind before overriding the actual header
	rewind(output_file);

	// override the actual header over the previously written error header
	bmp_write_header(output_file, header);
}

void bmp_encode(FILE* inputFile, FILE* outputFile)
{
	{
		const off_t input_size = file_size(inputFile);

		if (input_size >= 0)
		{
			// input has size, everything is known beforehand so output can also be a stream

			bmp_encode_simple(inputFile, (size_t) input_size, outputFile);
			return;
		}
	}

	// input does not have a size, e.g. stdin

	if (file_size(outputFile) >= 0)
	{
		// output has a size, i.e. is a file

		// strategy: write using rewind

		bmp_encode_rewind(inputFile, outputFile);
	}
	else
	{
		// output does not have a size, e.g. stdout

		// strategy: copy input to tmp file, then encode with size

		FILE* tmp = tmpfile();
		const size_t input_size = copy_data(inputFile, tmp);

		rewind(tmp);

		bmp_encode_simple(tmp, input_size, outputFile);
	}
}
