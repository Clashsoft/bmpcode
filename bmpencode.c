#include <string.h>
#include <stdio.h>

#include "impl/encode.h"

int main(int argc, char** args)
{
	switch (argc)
	{
		case 1:
			bmp_encode(stdin, stdout);
			return 0;
		case 2:
			bmp_encode_p(args[1]);
			return 0;
		case 3:
			bmp_encode_pp(args[1], args[2]);
			return 0;
		default:
			goto usage;
	}

	usage:
	fprintf(stderr, "usage: bmpencode [input-file [output-file]]\n");
}
