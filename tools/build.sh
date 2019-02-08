#!/bin/sh
cd "$(dirname "$0")"
# Build tools
gcc src/png2bin.c -o png2bin -lSDL2 -lSDL2_image -lm
