bmpcode
=======

**bmpcode** is a small C program that allows storing and retrieving file contents to and from bmp files.

Usage:

Encoding:

    bmpcode -e input.txt output.bmp
    bmpcode -e input.txt # output to input.txt.bmp
    bmpcode -e # encode from stdin to stdout

Decoding:

    bmpcode -d input.bmp output.txt
    bmpcode -d input.txt.bmp # output to input.txt
    bmpcode -d # decode from stdin to stdout
