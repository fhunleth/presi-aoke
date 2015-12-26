#!/bin/sh

set -e

VERSION=$(git describe --always)
APP=presi-aoke
[ -z $QMAKE ] && QMAKE=qmake
[ -z $MACDEPLOYQT ] && MACDEPLOYQT=$(dirname $(which $QMAKE))/macdeployqt

rm -fr build
mkdir build
cd build
$QMAKE ../$APP.pro
make

$MACDEPLOYQT $APP.app -dmg
mv -f $APP.dmg $APP-$VERSION.dmg
