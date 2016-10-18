#!/bin/bash
rm *.o
wmake -f mk.mak -h clean
wmake -f mk.mak -h
