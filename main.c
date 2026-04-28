
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNE_PI 3.14159265358979323846
#define RUNE_ALPHA 0.0072973525693
#define MAX_VARS 32

struct var { char name[32]; double value; } vars[MAX_VARS];
static int nvars = 0;

static void setv(const char *name, double value) { if (nvars < MAX_VARS) { strncpy(vars[nvars].name,name,31); vars[nvars].value=value; nvars++; } }
static double getv(const char *name) { int i; for (i=0;i<nvars;i++) if (strcmp(vars[i].name,name)==0) return vars[i].value; return atof(name); }
static double energy(double m, double v) { return 0.5*m*v*v; }

int main(int argc, char **argv) {
    char op[64], a[64], b[64]; FILE *fp = argc > 1 ? fopen(argv[1], "r") : stdin;
    if (!fp) { perror(argv[1]); return 1; }
    printf("runeweaver-c: C89 rune script\n");
    while (fscanf(fp, "%63s", op) == 1) {
        if (strcmp(op,"let") == 0 && fscanf(fp,"%63s %63s",a,b)==2) { setv(a, atof(b)); printf("%s = %.6f\n", a, getv(a)); }
        else if (strcmp(op,"energy") == 0 && fscanf(fp,"%63s %63s",a,b)==2) printf("E = %.6f\n", energy(getv(a), getv(b)));
        else if (strcmp(op,"constant") == 0 && fscanf(fp,"%63s",a)==1) printf("%s = %.12f\n", a, strcmp(a,"alpha")==0 ? RUNE_ALPHA : RUNE_PI);
        else { printf("unknown rune: %s\n", op); }
    }
    if (fp != stdin) fclose(fp); return 0;
}
