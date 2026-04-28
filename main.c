#include "src/rune_core.h"
#include "src/rune_script.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    RuneReport r;
    RuneScript script;
    const char *demo;

    if (argc > 1) {
        rune_script_init(&script);
        if (!rune_script_run_file(&script, argv[1], stdout)) {
            fprintf(stderr, "rune script error: %s\n", rune_script_error(&script));
            return 1;
        }
        return 0;
    }

    r = rune_simulate(180, 0.016);
    printf("%s report steps=%d energy=%.4f drift=%.4f events=%d\n",
           "runeweaver-c", r.steps, r.energy, r.drift, r.events);
    printf("formula %.5f\n", rune_formula(10, 3.0, 4.0));

    demo = "# tiny rune-script demo\n"
           "let x = PI * 2\n"
           "let y = x + ALPHA\n"
           "print y\n";
    rune_script_init(&script);
    printf("script demo:\n");
    if (!rune_script_run(&script, demo, stdout)) {
        fprintf(stderr, "rune script error: %s\n", rune_script_error(&script));
        return 1;
    }
    return 0;
}
