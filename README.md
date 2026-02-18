# bfx: an interpreter, compiler, and REPL for brainfuck-like languages

[![CI status](https://github.com/bmoneill/bfx/actions/workflows/cmake-single-platform.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/cmake-single-platform.yml).
[![clang-format status](https://github.com/bmoneill/bfx/actions/workflows/clang-format.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/clang-format.yml)
[![Doxygen status](https://github.com/bmoneill/bfx/actions/workflows/doxygen.yml/badge.svg?branch=main)](https://github.com/bmoneill/bfx/actions/workflows/doxygen.yml)

This is an interpreter, compiler, and REPL for the following Turing-complete esoteric
programming languages:

- [Brainfork](https://esolangs.org/wiki/Brainfork).
- [Brainfuck](https://esolangs.org/wiki/Brainfuck),
- [Grin](https://esolangs.org/wiki/Grin)
- [Pbrain](https://esolangs.org/wiki/Pbrain)
- [Weave](https://esolangs.org/wiki/Weave),

## Building

### Linux

```shell
cmake . -B build
cmake --build build
sudo cmake --install build
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
- `-i`: Separate code from input using `!` (Note: Grin is not supported here).
- `-l language`: Specify the language to use (Default: brainfuck).
- `-r`: Run in interactive REPL mode (Note: Weave is not supported here).
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
  is 4.
- `-v`: Print version information.

### Languages

- `brainfork`
- `brainfuck` (Default)
- `grin`
- `pbrain`
- `weave`

If `file` is not specified, `bfx` will read source code from standard input.

## Extending

It is possible to add support for new languages pretty easily by following these
steps:

1. Create a new source and header file for the language in [libbfx/src/langs](libbfx/src/langs).
2. Implement the language using the conventional init and run functions and
   operations array (see [libbfx/src/langs/brainfuck.c](libbfx/src/langs/brainfuck.c) and
   [libbfx/src/langs/brainfuck.h](libbfx/src/langs/brainfuck.h)).
3. Add the source file to [libbfx/src/CMakeLists.txt](libbfx/src/CMakeLists.txt).
4. Add the language to the BFX_Languages enum in [libbfx/src/bfx.h](libbfx/src/bfx.h).
5. Add a BFX_LANG(NAME) macro to the list in [libbfx/src/interpret.c](libbfx/src/interpret.c).

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
