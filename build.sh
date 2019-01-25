#!/bin/sh
wine cmd /c build.bat
if [ "$#" -eq 1 ]; then
    if [ "$1" = "run" ] ; then
        dosbox game.exe
    fi
fi