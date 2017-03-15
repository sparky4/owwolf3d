#!/bin/bash
if [ -z "$*" ]; then
		pee="src/obj/signon.obj"
	else
		pee="$*"
fi

wdis "$pee"
