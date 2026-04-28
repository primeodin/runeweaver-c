#!/usr/bin/env bash
set -euo pipefail
make >/tmp/runeweaver.build.log
./main examples/orbit.rune
echo "The rune holds."
