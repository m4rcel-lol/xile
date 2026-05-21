#!/usr/bin/env bash
set -euo pipefail
python3 - <<'PY'
import os
print(os.urandom(16).hex())
PY
