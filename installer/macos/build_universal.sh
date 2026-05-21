#!/usr/bin/env bash
set -euo pipefail
VERSION="1.0.0"
DIST_XML="payload/macos-universal/Distribution.xml"
./installer/macos/build_pkg_x86_64.sh
./installer/macos/build_pkg_arm64.sh
APP_X64="payload/macos-x86_64/Applications/Xile.app/Contents/MacOS"
APP_ARM="payload/macos-arm64/Applications/Xile.app/Contents/MacOS"
UNIV="payload/macos-universal/Applications/Xile.app/Contents/MacOS"
rm -rf payload/macos-universal
mkdir -p "${UNIV}" dist/macos
for binary in xile-server xile-ctl; do
    lipo -create "${APP_X64}/${binary}" "${APP_ARM}/${binary}" -output "${UNIV}/${binary}"
    lipo -info "${UNIV}/${binary}"
done
cp -R payload/macos-arm64/Applications/Xile.app/Contents/Resources payload/macos-universal/Applications/Xile.app/Contents/
cp payload/macos-arm64/Applications/Xile.app/Contents/Info.plist payload/macos-universal/Applications/Xile.app/Contents/
pkgbuild --root payload/macos-universal --identifier com.xile.pkg --version "${VERSION}" --scripts installer/macos/scripts --install-location / dist/macos/Xile-component-universal.pkg
sed "s/@COMPONENT_PKG@/Xile-component-universal.pkg/g" installer/macos/Distribution.xml > "${DIST_XML}"
productbuild --distribution "${DIST_XML}" --resources installer/macos/resources --package-path dist/macos "dist/macos/Xile-${VERSION}-universal.pkg"
