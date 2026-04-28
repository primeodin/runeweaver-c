#!/usr/bin/env bash
set -euo pipefail
make clean >/dev/null 2>&1 || true
make >/tmp/runeweaver-c.build.log 2>&1 || { cat /tmp/runeweaver-c.build.log; exit 1; }
./main
./test_core
echo "The rune holds."
