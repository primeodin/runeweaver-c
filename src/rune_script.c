#include "rune_script.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNE_PI 3.14159265358979323846
#define RUNE_HBAR 1.054571817e-34
#define RUNE_ALPHA 7.2973525693e-3
#define RUNE_C 299792458.0

typedef struct {
    RuneScript *script;
    const char *src;
    const char *p;
    FILE *out;
} Parser;

static void clear_error(RuneScript *script)
{
    script->error[0] = '\0';
    script->line = 0;
    script->column = 0;
}

static void set_error(Parser *parser, const char *message)
{
    int col;

    if (parser->script->error[0] != '\0') {
        return;
    }
    col = (int)(parser->p - parser->src) + 1;
    if (col < 1) {
        col = 1;
    }
    parser->script->column = col;
    sprintf(parser->script->error, "line %d, column %d: %s",
            parser->script->line, col, message);
}

static void skip_space(Parser *parser)
{
    while (*parser->p != '\0' && isspace((unsigned char)*parser->p)) {
        parser->p++;
    }
}

static int eat(Parser *parser, char ch)
{
    skip_space(parser);
    if (*parser->p == ch) {
        parser->p++;
        return 1;
    }
    return 0;
}

static int word_is(const char *start, int len, const char *word)
{
    return (int)strlen(word) == len && strncmp(start, word, (size_t)len) == 0;
}

static int parse_expression(Parser *parser, double *out);

static int constant_value(const char *start, int len, double *out)
{
    if (word_is(start, len, "PI")) {
        *out = RUNE_PI;
        return 1;
    }
    if (word_is(start, len, "HBAR")) {
        *out = RUNE_HBAR;
        return 1;
    }
    if (word_is(start, len, "ALPHA")) {
        *out = RUNE_ALPHA;
        return 1;
    }
    if (word_is(start, len, "C")) {
        *out = RUNE_C;
        return 1;
    }
    return 0;
}

static int parse_factor(Parser *parser, double *out)
{
    char *end;
    const char *start;
    int len;
    int idx;

    skip_space(parser);

    if (eat(parser, '+')) {
        return parse_factor(parser, out);
    }
    if (eat(parser, '-')) {
        if (!parse_factor(parser, out)) {
            return 0;
        }
        *out = -*out;
        return 1;
    }
    if (eat(parser, '(')) {
        if (!parse_expression(parser, out)) {
            return 0;
        }
        if (!eat(parser, ')')) {
            set_error(parser, "expected ')'");
            return 0;
        }
        return 1;
    }
    if (isdigit((unsigned char)*parser->p) || *parser->p == '.') {
        *out = strtod(parser->p, &end);
        if (end == parser->p) {
            set_error(parser, "expected number");
            return 0;
        }
        parser->p = end;
        return 1;
    }
    if (isalpha((unsigned char)*parser->p)) {
        start = parser->p;
        while (isalpha((unsigned char)*parser->p)) {
            parser->p++;
        }
        len = (int)(parser->p - start);
        if (len == 1 && start[0] >= 'a' && start[0] <= 'z') {
            idx = start[0] - 'a';
            if (!parser->script->defined[idx]) {
                set_error(parser, "variable is undefined");
                return 0;
            }
            *out = parser->script->vars[idx];
            return 1;
        }
        if (constant_value(start, len, out)) {
            return 1;
        }
        set_error(parser, "unknown name");
        return 0;
    }

    set_error(parser, "expected expression");
    return 0;
}

static int parse_term(Parser *parser, double *out)
{
    double right;
    char op;

    if (!parse_factor(parser, out)) {
        return 0;
    }
    for (;;) {
        skip_space(parser);
        op = *parser->p;
        if (op != '*' && op != '/') {
            break;
        }
        parser->p++;
        if (!parse_factor(parser, &right)) {
            return 0;
        }
        if (op == '*') {
            *out *= right;
        } else {
            if (right == 0.0) {
                set_error(parser, "division by zero");
                return 0;
            }
            *out /= right;
        }
    }
    return 1;
}

static int parse_expression(Parser *parser, double *out)
{
    double right;
    char op;

    if (!parse_term(parser, out)) {
        return 0;
    }
    for (;;) {
        skip_space(parser);
        op = *parser->p;
        if (op != '+' && op != '-') {
            break;
        }
        parser->p++;
        if (!parse_term(parser, &right)) {
            return 0;
        }
        if (op == '+') {
            *out += right;
        } else {
            *out -= right;
        }
    }
    return 1;
}

static void strip_comment(char *line)
{
    char *hash;

    hash = strchr(line, '#');
    if (hash != NULL) {
        *hash = '\0';
    }
}

static int parse_statement(RuneScript *script, char *line, FILE *out)
{
    Parser parser;
    double value;
    int idx;

    parser.script = script;
    parser.src = line;
    parser.p = line;
    parser.out = out;

    strip_comment(line);
    skip_space(&parser);
    if (*parser.p == '\0') {
        return 1;
    }

    if (strncmp(parser.p, "let", 3) == 0 && !isalpha((unsigned char)parser.p[3])) {
        parser.p += 3;
        skip_space(&parser);
        if (*parser.p < 'a' || *parser.p > 'z') {
            set_error(&parser, "expected variable a-z after let");
            return 0;
        }
        idx = *parser.p - 'a';
        parser.p++;
        skip_space(&parser);
        if (!eat(&parser, '=')) {
            set_error(&parser, "expected '='");
            return 0;
        }
        if (!parse_expression(&parser, &value)) {
            return 0;
        }
        skip_space(&parser);
        if (*parser.p != '\0') {
            set_error(&parser, "unexpected text after expression");
            return 0;
        }
        script->vars[idx] = value;
        script->defined[idx] = 1;
        return 1;
    }

    if (strncmp(parser.p, "print", 5) == 0 && !isalpha((unsigned char)parser.p[5])) {
        parser.p += 5;
        if (!parse_expression(&parser, &value)) {
            return 0;
        }
        skip_space(&parser);
        if (*parser.p != '\0') {
            set_error(&parser, "unexpected text after expression");
            return 0;
        }
        fprintf(out, "%.12g\n", value);
        return 1;
    }

    set_error(&parser, "expected 'let' or 'print'");
    return 0;
}

void rune_script_init(RuneScript *script)
{
    int i;

    for (i = 0; i < 26; i++) {
        script->vars[i] = 0.0;
        script->defined[i] = 0;
    }
    clear_error(script);
}

int rune_script_run(RuneScript *script, const char *text, FILE *out)
{
    char line[512];
    int used;
    char ch;

    clear_error(script);
    script->line = 1;
    used = 0;

    while (*text != '\0') {
        ch = *text++;
        if (ch == '\n') {
            line[used] = '\0';
            if (!parse_statement(script, line, out)) {
                return 0;
            }
            script->line++;
            used = 0;
        } else {
            if (used >= (int)sizeof(line) - 1) {
                sprintf(script->error, "line %d, column %d: line is too long",
                        script->line, used + 1);
                script->column = used + 1;
                return 0;
            }
            line[used++] = ch;
        }
    }

    if (used > 0) {
        line[used] = '\0';
        if (!parse_statement(script, line, out)) {
            return 0;
        }
    }
    return 1;
}

int rune_script_run_file(RuneScript *script, const char *path, FILE *out)
{
    FILE *in;
    char *buffer;
    long size;
    size_t got;
    int ok;

    clear_error(script);
    in = fopen(path, "rb");
    if (in == NULL) {
        sprintf(script->error, "could not open script: %s", path);
        return 0;
    }
    if (fseek(in, 0L, SEEK_END) != 0) {
        fclose(in);
        sprintf(script->error, "could not seek script: %s", path);
        return 0;
    }
    size = ftell(in);
    if (size < 0) {
        fclose(in);
        sprintf(script->error, "could not measure script: %s", path);
        return 0;
    }
    rewind(in);
    buffer = (char *)malloc((size_t)size + 1U);
    if (buffer == NULL) {
        fclose(in);
        sprintf(script->error, "out of memory reading script");
        return 0;
    }
    got = fread(buffer, 1U, (size_t)size, in);
    fclose(in);
    buffer[got] = '\0';
    if (got != (size_t)size) {
        free(buffer);
        sprintf(script->error, "could not read complete script: %s", path);
        return 0;
    }
    ok = rune_script_run(script, buffer, out);
    free(buffer);
    return ok;
}

const char *rune_script_error(const RuneScript *script)
{
    if (script->error[0] == '\0') {
        return "no error";
    }
    return script->error;
}
