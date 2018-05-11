
#include <assert.h>
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
		// decode(args[2], args[3])
		return 0;
	}

	fprintf(stderr, "usage: bmpdat [-e | -d] [input-file [output-file]]");
}

width_height_t bmp_width_height(size_t size)
{
	size_t wh = (size_t) ceil(sqrt(size / 3.0));
	wh += 4 - wh % 4; // ensure wh is a multiple of 4

	width_height_t result = {(int) wh, (int) wh};
	return result;
}

void bmp_write_header(FILE* file, unsigned int w, unsigned int h)
{
	int size = 54 + 3 * w * h;

	unsigned char fileHeader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
	unsigned char infoHeader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

	fileHeader[2] = (unsigned char) (size);
	fileHeader[3] = (unsigned char) (size >> 8);
	fileHeader[4] = (unsigned char) (size >> 16);
	fileHeader[5] = (unsigned char) (size >> 24);

	infoHeader[4]  = (unsigned char) (w);
	infoHeader[5]  = (unsigned char) (w >> 8);
	infoHeader[6]  = (unsigned char) (w >> 16);
	infoHeader[7]  = (unsigned char) (w >> 24);
	infoHeader[8]  = (unsigned char) (h);
	infoHeader[9]  = (unsigned char) (h >> 8);
	infoHeader[10] = (unsigned char) (h >> 16);
	infoHeader[11] = (unsigned char) (h >> 24);

	fwrite(fileHeader, 1, 14, file);
	fwrite(infoHeader, 1, 40, file);
}

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
	bmp_write_header(outputFile, 0, 0);

	char   buffer[4096];
	size_t read;
	size_t totalSize = 0;

	while ((read = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
	{
		fwrite(buffer, 1, read, outputFile);
		totalSize += read;
	}
	fclose(inputFile);

	width_height_t wh = bmp_width_height(totalSize);

	size_t pixelSize = 3 * (size_t) wh.width * (size_t) wh.height;
	size_t diff      = pixelSize - totalSize;

	for (size_t i = 0; i < diff; i++)
	{
		fputc(0, outputFile);
	}

	rewind(outputFile);
	bmp_write_header(outputFile, wh.width, wh.height);

	fclose(outputFile);
}
