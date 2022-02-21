#!/usr/bin/sh
## QtSssSircBot/doc/build.sh
## script to combine Lua documentation with LDoc into
## build with Sphinx-chain.

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

# update templated files
#./updateVersionTags.sh

# remove old LDoc in Sphinx-tree, if exists
sOutPath=SphinxBreatheExhaleDocutils/source/_static/LDoc;
if [ -d "$sOutPath" ]; then
  rm -r $sOutPath;
fi;

# build the LDoc-umentation
LDoc/build.sh;

echo "*************************************";
echo "*************************************";
echo "*************************************";
echo "************ LDoc built *************";
echo "*************************************";
echo "*************************************";
echo "*************************************";

# move result to Sphinx-tree
mv LDoc/html $sOutPath

# start the Sphinx-chain
SphinxBreatheExhaleDocutils/build.sh;

echo "output is in QtSssSircBot/doc/SphinxBreatheExhaleDocutils/build/html";

# move result to staging
#mv SphinxBreatheExhaleDocutils/build/html ../../SwissalpS.github.io/QtSssSircBot/branch${branch}/${version}
