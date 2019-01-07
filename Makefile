default: all

all: bmpcode bmpencode bmpdecode

bmpcode: bmpcode.c impl/utils.c impl/encode.c impl/decode.c
	gcc -lm bmpcode.c impl/utils.c impl/encode.c impl/decode.c -o bmpcode

bmpencode: bmpencode.c impl/utils.c impl/encode.c
	gcc -lm bmpencode.c impl/utils.c impl/encode.c -o bmpencode

bmpdecode: bmpdecode.c impl/utils.c impl/decode.c
	gcc -lm bmpdecode.c impl/utils.c impl/decode.c -o bmpdecode

test: test_txt test_bin test_stdin test_stdout test_stdinout

test_txt: bmpencode bmpdecode
	# Test 1: Encode and then decode temp/test.txt
	./bmpencode temp/text.txt temp/text.txt.bmp
	./bmpdecode temp/text.txt.bmp temp/text.decoded.txt

	diff temp/text.txt temp/text.decoded.txt
	-rm temp/text.txt.bmp temp/text.decoded.txt

test_bin: bmpencode bmpdecode
	# Test 2: Encode and then decode temp/random.bin
	./bmpencode temp/random.bin temp/random.bin.bmp
	./bmpdecode temp/random.bin.bmp temp/random.decoded.bin

	diff temp/random.bin temp/random.decoded.bin
	-rm temp/random.bin.bmp temp/random.decoded.bin

test_stdin: bmpencode bmpdecode
	# Test 3: Use stdin for input of both programs
	cat temp/text.txt     | ./bmpencode - temp/text.txt.bmp
	cat temp/text.txt.bmp | ./bmpdecode - temp/text.decoded.txt

	diff temp/text.txt temp/text.decoded.txt
	-rm temp/text.txt.bmp temp/text.decoded.txt

test_stdout: bmpencode bmpdecode
	# Test 4: Use stdout for output of both programs
	./bmpencode temp/text.txt     - | cat > temp/text.txt.bmp
	./bmpdecode temp/text.txt.bmp - | cat > temp/text.decoded.txt

	diff temp/text.txt temp/text.decoded.txt
	-rm temp/text.txt.bmp temp/text.decoded.txt

test_stdinout: bmpencode bmpdecode
	# Test 5: Use stdin for input and stdout for output of both programs
	cat temp/text.txt     | ./bmpencode - - | cat > temp/text.txt.bmp
	cat temp/text.txt.bmp | ./bmpdecode - - | cat > temp/text.decoded.txt

	diff temp/text.txt temp/text.decoded.txt
	-rm temp/text.txt.bmp temp/text.decoded.txt

clean:
	-rm bmpcode bmpencode bmpdecode *.o impl/*.o

install: bmpcode bmpencode bmpdecode
	cp bmpcode bmpencode bmpdecode /usr/local/bin/

remove:
	rm /usr/local/bin/bmpcode /usr/local/bin/bmpencode /usr/local/bin/bmpdecode
