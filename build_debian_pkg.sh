#!/bin/bash

set -e

# Read the current version from the file VERSION
VERSION=$(cat ../VERSION | tr -d '[[:space:]]')
APP=presi-aoke
[ -z $QMAKE ] && QMAKE=qmake

umask 0022

# Put all of the files in the right places for the Debian package
INSTALL_ROOT=../pkg-debian make -C ../build install

APP_SIZE=$(du -k usr/bin/presi-aoke | cut -f 1)
ARCH=$(uname -p)
mkdir DEBIAN
cat > DEBIAN/control <<EOF
Package: presi-aoke
Version: $VERSION
Section: games
Priority: optional
Architecture: amd64
Depends: libqt5widgets5, libc6
Installed-Size: $APP_SIZE
Maintainer: Frank Hunleth <fhunleth@troodon-software.com>
Description: Presentation Karaoke Player
 Take turns presenting randomized slide decks.
EOF

mkdir -p usr/share/doc/presi-aoke
cat > usr/share/doc/presi-aoke/copyright <<'EOF'
Format: http://dep.debian.net/deps/dep5/
Upstream-Name: Presi-aoke
Upstream-Contact: Frank Hunleth <fhunleth@troodon-software.com>
Source: https://github.com/fhunleth/presi-aoke

Files: *
Copyright: 2015 Frank Hunleth
License: Apache-2.0
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 .
     http://www.apache.org/licenses/LICENSE-2.0
 .
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 .
 On Debian systems, the full text of the Apache License can be found in the file
 `/usr/share/common-licenses/Apache-2.0'.
EOF

# Compute all of the checksums
find usr -type f | xargs md5sum > DEBIAN/md5sums

# Build the package
dpkg -b . ../presi-aoke_${VERSION}_${ARCH}.deb

