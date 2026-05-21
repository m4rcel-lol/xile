#!/usr/bin/env bash
set -euo pipefail
plist="$HOME/Library/LaunchAgents/com.xile.server.plist"
mkdir -p "$(dirname "$plist")"
printf '%s\n' '<?xml version="1.0" encoding="UTF-8"?><plist version="1.0"><dict></dict></plist>' > "$plist"
test -f "$plist"
rm -f "$plist"
