#!/usr/bin/sh
## QtSssSircBot/doc/updateVersionTags.sh
## script to update configuration files for documentation build with
## current version and branch information.
## Run this as a build step in QtCreator or manually before building documentation
## to be sure that version in documentation is same as checked out state in repo.
###

# change into directory where script resides in
ME=$(readlink -f "$0");
MEPATH=$(dirname "$ME");
cd $MEPATH;

## gather some information
sBranch=`git branch --show-current`;
sVersion=`cat ../QtSssSircBot.pro | grep "VERSION =" | grep -o -E '[0-9]+\.[0-9]+\.[0-9]+'`;
sVersionShort=`echo $sVersion | grep -o -E '[0-9]+\.[0-9]+'`;

echo "branch: $sBranch  version: $sVersion  short: $sVersionShort";

## Doxygen standalone Doxyfile
sDT=Doxygen/Doxyfile.template;
sDW=Doxygen/Doxyfile.wrk;
sDO=Doxygen/Doxyfile;
cp $sDT $sDW;
sed -i "s/{{VERSION}}/${sVersion}/g" $sDW;
sed -i "s/{{BRANCH}}/${sBranch}/g" $sDW;
mv $sDW $sDO;

## Sphinx configuration
sPS=SphinxBreatheExhaleDocutils/source;
sPT=$sPS/conf.py.template;
sPW=$sPS/conf.py.wrk;
sPO=$sPS/conf.py;
cp $sPT $sPW;
sed -i "s/{{VERSIONSHORT}}/${sVersionShort}/g" $sPW;
sed -i "s/{{VERSION}}/${sVersion}/g" $sPW;
sed -i "s/{{BRANCH}}/${sBranch}/g" $sPW;
mv $sPW $sPO;

echo "Done, have a nice day.";

