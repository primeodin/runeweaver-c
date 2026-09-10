# Runeweaver C

> Retrospective chapter for 2003. This repository is part of the honest `primeodin-legacy` archive: a reconstruction, study, and mythic technical autobiography forged later, not a falsified historical record.

I call this one `runeweaver-c` because a C89 rune-script interpreter reconstructing the first winter when constants, chalk dust, and machine words began to speak. The old lesson still bites: if a thing is worth understanding, it is worth reducing until you can hold it in your hand, turn it over, and see where the light leaks through. Feynman had that cheerful cruelty toward confusion. Tryndamere had the less cheerful habit of refusing to fall over. Between those two poles I learned to work: chalk in one hand, blade in the other, patience somewhere under the frost.

The code is intentionally written in a 2003-shaped accent. That does not mean every compiler warning has been preserved like a holy relic, but the dependencies, file shapes, and engineering mood are chosen to fit the period. The goal is not cosplay. The goal is disciplined archaeology: to ask what I would have built if this chapter had been sitting on the bench in 2003, with the tools of that season and the questions that were burning then.

There is a pleasure in finding things out that no trophy can replace. You run the little program, it gives the wrong number, you curse, you sharpen the knife, and then one line moves from darkness into sense. That is the private thunder of computing. Most of the world only sees the finished artifact; the builder remembers the winter: the stale coffee, the bad stack trace, the tiny victory when the rune finally holds.

This chapter favors smallness. The main program is deliberately readable, with comments that explain the pressure behind the choices. If the project is in C, memory is treated like weather: always present, sometimes murderous. If it is Python, the script tries to be plain enough that a student can pull it apart. If it is Java or shell, it carries the blunt habits of its era. I have resisted modern polish where polish would make the fossil lie.

The mythic language is not there to hide weak engineering. It is there because humans remember stories better than configuration flags. A rage meter is just state under stress. A rune is just an invariant you promised not to break. A kingdom is just a system with users depending on it after you have gone to sleep. The words are theatrical; the obligations are plain.

How to read this repository: start with the source. Then read the notes. Then change one parameter and watch what breaks. I have always trusted examples more than sermons. A good example is a small animal: alive, limited, and surprisingly hard to fake. If you can make it fail and then explain the failure, it has taught you something better than success.

This is therefore an artifact of memory rather than evidence of 2003 activity. The commits created by the setup script are current commits with retrospective messages. The tag marks the chapter, not a claim that the public record existed in that year. Legends are strongest when they can stand in daylight. No false snow is needed.

If you fork this, keep the honesty file. If you improve the code, say what you changed and when. If you use the voice, use it to make hard ideas clearer, not to make the past blurrier. The frozen path is long enough without inventing footprints behind us.

## Try it

See the source file in this repository and run the small command shown below. The output is intentionally modest: a number, a trace, a decision, a diagram, or a line of text. Small output is good. Small output lets the mind inspect the machinery.

```sh
make
./main examples/orbit.rune
./scripts/smoke.sh
```

The rune-script interpreter is deliberately small but real. It supports:

- Constants: `PI`, `HBAR`, `ALPHA`, `C`
- Variables: single lowercase names `a` through `z`
- Statements: `let x = expression` and `print expression`
- Arithmetic: `+`, `-`, `*`, `/`, parentheses, unary signs, and decimal/scientific numbers
- Comments: `#` to the end of a line
- Errors with line/column context for unknown names, undefined variables, bad syntax, and division by zero

This does not change the retrospective framing: the implementation is current archive work in a 2003-shaped style, not a claim of newly discovered historical commits.


## Built-out archive contents

This public-ready build-out adds `docs/CHAPTER.md`, `examples/transcript.txt`, and `scripts/smoke.sh` so the chapter is not only literary but inspectable. The smoke script is intentionally plain: it compiles or runs the small artifact, prints a short trace, and refuses to hide failure. That is the old bargain. If the rune breaks, the console should say so without ceremony.

## GitHub publication note

If this repository appears under `primeodin/runeweaver-c`, read it as a chapter of the retrospective archive. The history is honest current work, not an invented twenty-year activity record. The myth is in the voice and the learning arc; the truth is in the archive note.

## What this repository is

This is a runnable retrospective chapter for **runeweaver-c**: C89 rune-script interpreter and physics constants sketch.
It is not padded to impress a counter. The implementation is deliberately compact, tested by `./scripts/smoke.sh`, and written so a reader can follow the idea without spelunking through generated fog.

## Public-readiness notes

- The year marker is narrative context, not a forged GitHub timestamp.
- The `.retrospective` tag marks this as part of the honest archive reconstruction.
- Contributions should improve behavior, tests, explanation, or safety — not bulk.

## Help / good first issues

Archive chapters still take first patches. Scoped tickets:

- **#1** — [`docs/start-here.md`](https://github.com/primeodin/runeweaver-c/issues/1) walkthrough (`make` → `orbit.rune` → smoke)
- **#2** — [`examples/constants.rune`](https://github.com/primeodin/runeweaver-c/issues/2) + expected transcript line

See [CONTRIBUTING.md](CONTRIBUTING.md). Keep the retrospective framing honest — improve the tool, do not invent 2003 footprints.

