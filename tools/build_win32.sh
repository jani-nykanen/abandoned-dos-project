#!/bin/sh
cd "$(dirname "$0")"
# Build tools
mkdir bin
gcc src/png2bin.c -o bin/png2bin.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lm
