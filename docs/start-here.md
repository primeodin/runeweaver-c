# Start here

A cold-start walkthrough for **runeweaver-c**: the small rune-script interpreter in
this repository. It names the exact commands and what "good" looks like on the
current tree.

## Prerequisites

- A C89-capable `cc` and `make` on `PATH` (a normal C toolchain, no extra
  packages). The `Makefile` defaults to `cc`; if your compiler is `gcc`,
  run `make CC=gcc`.

## 1. Build

```bash
make
```

This builds three programs: `main`, `test_core` and `test_script`.

## 2. Run the example script

```bash
./main examples/orbit.rune
```

Run with no argument, `./main` prints a single number (the base demo). With the
example script it prints two numbers, e.g.:

```text
0.0072973525693
4.14159265359
```

## 3. Smoke test

```bash
./scripts/smoke.sh
```

The script builds, runs `main`, `test_core` and `test_script`, then feeds a valid
rune script (which must print a value starting `5.14889`) and an invalid one
(`let a = 10 / (5 - 5)`, which must fail with a `division by zero` error). On
success it ends with:

```text
The rune holds.
```

## What success looks like

- `make` exits 0 and creates `main`, `test_core`, `test_script`.
- `./scripts/smoke.sh` ends with `The rune holds.`, with no `unexpected valid
  script output` and no missing `division by zero` message.
- The invalid-script path is the expected failure: the interpreter reports the
  division by zero with line/column context instead of crashing.

## Where to go next

- `ARCHIVE_NOTE.md` — the honesty note about what this repository is.
- `CONTRIBUTING.md` — how to propose a change.
- `examples/orbit.rune` — the sample script used above.
- `src/rune_script.c` — the interpreter itself.
