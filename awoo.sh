#!/bin/bash
#for testing
#mv src/WOLF3D.EXE ./wolf3d.exe
mv *.wl6 ../wolf3d/wolf3d/
if [ -z "$*" ]; then
		pee="wwww"
	else
		pee="$*"
fi
git add .
. ./boop.sh "$pee"
cp ../wolf3d/wolf3d/*.wl6 ./
