#!/usr/bin/env bash
set -euo pipefail
cmake --build --preset "${1:-macos-arm64}" --target test
