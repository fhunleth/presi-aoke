#!/bin/sh

set -e

VERSION=$(git describe --always)
APP=presi-aoke
[ -z $QMAKE ] && QMAKE=qmake

rm -fr build
mkdir build
cd build
$QMAKE ../$APP.pro
make

