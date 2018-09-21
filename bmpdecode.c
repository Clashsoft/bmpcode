#include <string.h>
#include <stdio.h>

#include "impl/decode.h"

int main(int argc, char** args)
{
	switch (argc)
	{
		case 1:
			bmp_decode(stdin, stdout);
			return 0;
		case 2:
			bmp_decode_p(args[1]);
			return 0;
		case 3:
			bmp_decode_pp(args[1], args[2]);
			return 0;
		default:
			goto usage;
	}

	usage:
	fprintf(stderr, "usage: bmpdecode [input-file [output-file]]\n");
}
