#!/usr/bin/env bash
set -euo pipefail
make clean >/dev/null 2>&1 || true
make >/tmp/runeweaver-c.build.log 2>&1 || { cat /tmp/runeweaver-c.build.log; exit 1; }
./main
./test_core
./test_script

valid_script=/tmp/runeweaver-c.valid.rune
invalid_script=/tmp/runeweaver-c.invalid.rune
cat > "$valid_script" <<'SCRIPT'
# smoke script: constants, variables, precedence
let a = PI * 2
let b = (a + 4) / 2
print b + ALPHA
SCRIPT
cat > "$invalid_script" <<'SCRIPT'
let a = 10 / (5 - 5)
SCRIPT

valid_output=$(./main "$valid_script")
case "$valid_output" in
  5.14889*) ;;
  *) echo "unexpected valid script output: $valid_output"; exit 1 ;;
esac

if ./main "$invalid_script" >/tmp/runeweaver-c.invalid.out 2>/tmp/runeweaver-c.invalid.err; then
  echo "invalid script unexpectedly succeeded"
  exit 1
fi
grep "division by zero" /tmp/runeweaver-c.invalid.err >/dev/null

echo "The rune holds."
