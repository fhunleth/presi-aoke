#!/bin/bash

set -e

APP=presi-aoke
[ -z $QMAKE ] && QMAKE=qmake

rm -fr build
mkdir build
cd build
$QMAKE ../$APP.pro
make -j4
cd ..

# Build the Debian package
rm -fr pkg-debian
mkdir pkg-debian
(cd pkg-debian; fakeroot ../build_debian_pkg.sh)
