#!/usr/bin/sh
## QtSssSircBot/doc/LDoc/build.sh
## script to build Lua documentation with LDoc.

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

rm -r html;

ldoc . -d html;

