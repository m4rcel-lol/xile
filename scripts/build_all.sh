#!/usr/bin/env bash
set -euo pipefail
for preset in windows-x64 windows-arm64 macos-x86_64 macos-arm64; do
  cmake --preset "$preset"
  cmake --build --preset "$preset" --parallel
done
