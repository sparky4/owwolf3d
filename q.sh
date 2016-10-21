#grep "$1" src/*
#grep '\\nlong' src/*
pcregrep -M "\n$1" src/*
