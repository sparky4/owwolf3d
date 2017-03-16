#!/bin/bash
#for testing
#mv src/WOLF3D.EXE ./wolf3d.exe
rm src/obj/*
. ./movedataout.sh
if [ -z "$*" ]; then
		pee="wwww"
	else
		pee="$*"
fi
git add .
. ./boop.sh "$pee"
. ./movedatain.sh
