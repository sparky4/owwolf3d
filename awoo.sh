#!/bin/bash
if [ -z "$*" ]; then
		pee="wwww"
	else
		pee="$*"
fi
git add .
. ./boop.sh "$pee"
