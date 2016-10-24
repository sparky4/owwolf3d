#!/bin/bash
rm *.obj
#wmake -f mk.mak -h clean
#wmake -f mk.mak -h
wmake -h clean
wmake -h wolf3d.exe
wmake -h
##rm *.obj
