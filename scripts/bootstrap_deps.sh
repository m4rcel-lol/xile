#!/usr/bin/env bash
set -euo pipefail
cat <<'MSG'
Xile third-party dependencies are pinned in docs/BUILD.md.
Populate third_party/ with the exact source releases before enabling ExternalProject builds.
MSG
