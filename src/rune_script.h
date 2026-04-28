#ifndef RUNE_SCRIPT_H
#define RUNE_SCRIPT_H

#include <stdio.h>

#define RUNE_SCRIPT_ERROR_SIZE 160

typedef struct {
    double vars[26];
    int defined[26];
    char error[RUNE_SCRIPT_ERROR_SIZE];
    int line;
    int column;
} RuneScript;

void rune_script_init(RuneScript *script);
int rune_script_run(RuneScript *script, const char *text, FILE *out);
int rune_script_run_file(RuneScript *script, const char *path, FILE *out);
const char *rune_script_error(const RuneScript *script);

#endif
