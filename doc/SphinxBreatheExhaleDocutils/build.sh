#!/usr/bin/sh

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

make clean && make html;

