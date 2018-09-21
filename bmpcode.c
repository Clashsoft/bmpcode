#include <string.h>
#include <stdio.h>

#include "impl/encode.h"
#include "impl/decode.h"

int main(int argc, char** args)
{
	if (argc == 1)
	{
		goto usage;
	}
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
			default:
				goto usage;
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
			default:
				goto usage;
		}
	}

	usage:
	fprintf(stderr, "usage: bmpcode [-e | -d] [input-file [output-file]]\n");
}
