#!/usr/bin/sh
## QtSssSircBot/doc/Doxygen/build.sh
## script to build documentation with Doxygen.

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

rm -r html;

doxygen;

