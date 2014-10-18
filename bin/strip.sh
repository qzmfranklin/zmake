#!/bin/sh

# Strip trailing white space from source files

strip1()
{
	TMP=$(mktemp /tmp/XXXXXX)
	cp "$1" "$TMP"
	sed -e '/^+/s/[ 	]*$//' <"$TMP" >"$1"
	rm "$TMP"
}

for i in "$@"
do
	strip1 "$i"
done
