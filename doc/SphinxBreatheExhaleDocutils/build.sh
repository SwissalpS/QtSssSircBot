#!/usr/bin/sh
## QtSssSircBot/doc/SphinxBreatheExhaleDocutils/build.sh
## script to build documentation with Sphinx toolchain.

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

make clean && make html;

