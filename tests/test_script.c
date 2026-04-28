#include "../src/rune_script.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    RuneScript script;
    const char *valid;
    const char *bad_name;
    const char *bad_divide;
    const char *bad_var;

    valid = "# constants and precedence\n"
            "let a = PI * 2\n"
            "let b = (a + 4) / 2\n"
            "let c = C / C + HBAR * 0\n"
            "print b + c - ALPHA\n";
    bad_name = "print TAU\n";
    bad_divide = "let z = 1 / (2 - 2)\n";
    bad_var = "print q\n";

    rune_script_init(&script);
    assert(rune_script_run(&script, valid, stdout));
    assert(script.defined[0]);
    assert(script.defined[1]);
    assert(script.vars[1] > 5.0 && script.vars[1] < 6.0);

    rune_script_init(&script);
    assert(!rune_script_run(&script, bad_name, stdout));
    assert(strstr(rune_script_error(&script), "unknown name") != NULL);

    rune_script_init(&script);
    assert(!rune_script_run(&script, bad_divide, stdout));
    assert(strstr(rune_script_error(&script), "division by zero") != NULL);

    rune_script_init(&script);
    assert(!rune_script_run(&script, bad_var, stdout));
    assert(strstr(rune_script_error(&script), "undefined") != NULL);

    puts("script tests pass");
    return 0;
}
