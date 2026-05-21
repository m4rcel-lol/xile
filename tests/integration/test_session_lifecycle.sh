#!/usr/bin/env bash
set -euo pipefail
"${XILE_SERVER:-./xile-server}" &
server_pid=$!
trap 'kill "$server_pid" 2>/dev/null || true' EXIT
sleep 2
"${XILE_CTL:-./xile-ctl}" status
"${XILE_CTL:-./xile-ctl}" stop || true
