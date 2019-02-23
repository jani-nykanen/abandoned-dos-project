#!/bin/sh
cd "$(dirname "$0")"

# Bitmaps
../tools/png2bin bitmaps/font.png ../ASSETS/BITMAPS/FONT.BIN
# ../tools/png2bin bitmaps/player.png ../ASSETS/BITMAPS/PLAYER.BIN
../tools/png2bin bitmaps/tileset.png ../ASSETS/BITMAPS/TILESET.BIN
../tools/png2bin bitmaps/rat.png ../ASSETS/BITMAPS/RAT.BIN

# Maps
../tools/tmx2bin ./maps/1.tmx ../ASSETS/MAPS/1.BIN
