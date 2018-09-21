default: all

all: bmpcode bmpencode bmpdecode

bmpcode:
	gcc bmpcode.c impl/utils.c impl/encode.c impl/decode.c -o bmpcode

bmpencode:
	gcc bmpencode.c impl/utils.c impl/encode.c -o bmpencode

bmpdecode:
	gcc bmpdecode.c impl/utils.c impl/decode.c -o bmpdecode

test: test_txt test_bin

test_txt: bmpencode bmpdecode
	-rm temp/text.txt.bmp temp/text.decoded.txt

	# Test 1: Encode and then decode temp/test.txt
	./bmpencode temp/text.txt temp/text.txt.bmp
	./bmpdecode temp/text.txt.bmp temp/text.decoded.txt

	diff temp/text.txt temp/text.decoded.txt

test_bin: bmpencode bmpdecode
	-rm temp/random.bin.bmp temp/random.decoded.bin

	# Test 2: Encode and then decode temp/random.bin
	./bmpencode temp/random.bin temp/random.bin.bmp
	./bmpdecode temp/random.bin.bmp temp/random.decoded.bin

	diff temp/random.bin temp/random.decoded.bin

clean:
	-rm bmpcode bmpencode bmpdecode
