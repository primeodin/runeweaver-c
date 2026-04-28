CC?=cc
CFLAGS?=-std=c89 -Wall -O2 -Isrc
LDFLAGS?=-lm
SCRIPT_OBJS=src/rune_script.c src/rune_script.h
all: main test_core test_script
main: main.c src/rune_core.c src/rune_core.h $(SCRIPT_OBJS)
	$(CC) $(CFLAGS) -o main main.c src/rune_core.c src/rune_script.c $(LDFLAGS)
test_core: tests/test_core.c src/rune_core.c src/rune_core.h
	$(CC) $(CFLAGS) -o test_core tests/test_core.c src/rune_core.c $(LDFLAGS)
test_script: tests/test_script.c $(SCRIPT_OBJS)
	$(CC) $(CFLAGS) -o test_script tests/test_script.c src/rune_script.c $(LDFLAGS)
check: all
	./test_core
	./test_script
clean:
	rm -f main test_core test_script
