cmake_minimum_required(VERSION 3.7)
project(bmpcode C)

set(CMAKE_C_STANDARD 11)

add_executable(bmpcode bmpcode.c impl/utils.h impl/utils.c impl/encode.h impl/encode.c impl/decode.h impl/decode.c)
add_executable(bmpencode bmpencode.c impl/utils.h impl/utils.c impl/encode.h impl/encode.c)
add_executable(bmpdecode bmpdecode.c impl/utils.h impl/utils.c impl/decode.h impl/decode.c)
