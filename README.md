# bfx: an interpreter, compiler, and REPL for brainfuck

[![CI status](https://github.com/bmoneill/bfx/actions/workflows/cmake-single-platform.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/cmake-single-platform.yml).
[![clang-format status](https://github.com/bmoneill/bfx/actions/workflows/clang-format.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/clang-format.yml)
[![Doxygen status](https://github.com/bmoneill/bfx/actions/workflows/doxygen.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/doxygen.yml)

This is an interpreter, compiler, and REPL for the Turing-complete esoteric
programming language brainfuck, as well as an interpreter and REPL for some of
its derivatives ([Pbrain](https://esolangs.org/wiki/Pbrain), [Weave](https://esolangs.org/wiki/Weave),
[Grin](https://esolangs.org/wiki/Grin), and [Brainfork](https://esolangs.org/wiki/Brainfork)).

## Building

### Linux

```shell
cmake . -B build
cmake --build build
```

## Usage

```shell
bfx [options...] [file]
```

### General Interpreter Options

- `-e eof_behavior`: Specify behavior when encountering EOF. Valid values are
  "zero" (the default, sets the current cell to zero),
  "decrement" (subtract one from the current cell), and
  "unchanged" (do not change the current cell).
- `-i`: Separate code from input using `!`.
- `-l language`: Specify the language to use (Default: brainfuck).
- `-r`: Run in interactive REPL mode.
- `-t tape_size`: Specify the size of the tape (default: 30000)

### Compiler Options

Note that the `bfx` library must be installed to your `PATH` in order to use
these options. Compiled code may be run without the `bfx` library.

- `-c`: Compile to native binary.
- `-o output_file`: Specify the output file (default: './a.out' for binaries,
  './a.out.c' for C source)

### Other Options

- `-D`: Default to degrees instead of radians (Grin).
- `-P precision`: Specify the number of decimal places to print (Grin). Default
  is 6.
- `-v`: Print version information.

### Languages

- `brainfork`
- `brainfuck` (Default)
- `grin`
- `pbrain`
- `weave`

If `file` is not specified, `bfx` will read source code from standard input.

## Screenshots

`bfx` running [sierpinski.b](https://brainfuck.org/sierpinski.b)

![bfx sierpinski](https://oneill.sh/img/bfx-sierpinski.png)

## Further Reading

- [Esolang Wiki](https://esolangs.org/wiki/Brainfuck)
- [brainfuck.org](https://brainfuck.org/)

## Bugs

If you find a bug, submit an issue, PR, or email me with a description and/or patch.

## License

Copyright (c) 2022-2026 Ben O'Neill <ben@oneill.sh>. This work is released under
the terms of the MIT License. See [LICENSE](LICENSE) for the license terms.
