#!/usr/bin/env bash
set -euo pipefail
PKG_PATH="${1:?usage: sign_and_notarize.sh package.pkg}"
SIGNED_PKG="${PKG_PATH%.pkg}-signed.pkg"
productsign --sign "${DEVELOPER_ID_INSTALLER:?}" "${PKG_PATH}" "${SIGNED_PKG}"
xcrun notarytool submit "${SIGNED_PKG}" --apple-id "${APPLE_ID:?}" --team-id "${TEAM_ID:?}" --password "${APP_PASSWORD:?}" --wait
xcrun stapler staple "${SIGNED_PKG}"
