/**
 * @file bfx.c
 * @brief brainfuck interpreter entry point
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * @copyright Copyright (c) 2022-2026 Ben O'Neill <ben@oneill.sh>.
 * This work is released under the terms of the MIT License. See
 * LICENSE.
 */

#include "bfx.h"
#include "compile.h"
#include "langs/grin.h"

#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRAINFORK_S "brainfork"
#define BRAINFUCK_S "brainfuck"
#define GRIN_S      "grin"
#define PBRAIN_S    "pbrain"
#define WEAVE_S     "weave"

#define EOF_BEHAVIOR_ZERO_S      "zero"
#define EOF_BEHAVIOR_DECREMENT_S "decrement"
#define EOF_BEHAVIOR_UNCHANGED_S "unchanged"

static int          get_eof_behavior(const char*);
static BFX_Language get_language(const char*);
static void         print_usage(const char*);
static void         print_version(const char*);

/**
 * @brief Entry point.
 */
int main(int argc, char* argv[]) {
    int   opt;
    BFX   bfx;
    char* path        = NULL;
    char* output_path = NULL;
    bool  compile     = false;
    int   precision   = BFX_GRIN_DEFAULT_PRECISION;

    memset(&bfx, 0, sizeof(bfx));
    bfx.flags        = 0;
    bfx.input_max    = BFX_DEFAULT_INPUT_MAX;
    bfx.tape_size    = BFX_DEFAULT_TAPE_SIZE;
    bfx.eof_behavior = BFX_DEFAULT_EOF_BEHAVIOR;
    bfx.lang         = BFX_LANG_brainfuck;

    while ((opt = getopt(argc, argv, "cCde:il:o:Prst:v")) != -1) {
        switch (opt) {
        case 'c':
            compile = true;
            break;
        case 'C':
            compile = true;
            bfx.flags |= BFX_FLAG_ONLY_GENERATE_C_SOURCE;
            break;
        case 'd':
            bfx.flags |= BFX_FLAG_DEBUG;
            break;
        case 'D':
            bfx.flags |= BFX_FLAG_DEGREES;
            break;
        case 'e':
            if ((bfx.eof_behavior = get_eof_behavior(optarg)) == -1) {
                print_usage(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case 'G':
            bfx.lang = BFX_LANG_grin;
            break;
        case 'i':
            bfx.flags |= BFX_FLAG_SEPARATE_INPUT_AND_SOURCE;
            break;
        case 'l':
            bfx.lang = get_language(optarg);
            break;
        case 'o':
            output_path = optarg;
            break;
        case 'p':
            bfx.lang = BFX_LANG_pbrain;
            break;
        case 'r':
            bfx.flags |= BFX_FLAG_REPL;
            break;
        case 's':
            bfx.flags |= BFX_FLAG_DISABLE_SPECIAL_INSTRUCTIONS;
            break;
        case 't':
            bfx.tape_size = atoi(optarg);
            break;
        case 'v':
            print_version(argv[0]);
            return EXIT_SUCCESS;
        case 'w':
            bfx.lang = BFX_LANG_weave;
            break;
        case 'Y':
            bfx.lang = BFX_LANG_brainfork;
            break;
        default:
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        path = argv[optind];
    }

    if (bfx.lang == BFX_LANG_UNKNOWN) {
        fprintf(stderr, "Unknown language specified.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (compile) {
        if (bfx.lang != BFX_LANG_brainfuck) {
            fprintf(stderr, "Brainfuck is the only language supported for compilation.\n");
            return EXIT_FAILURE;
        }
        bfx_compile(path, output_path, &bfx);
        return EXIT_SUCCESS;
    }

    if (bfx.lang != BFX_LANG_grin) {
        bfx.tape = calloc(bfx.tape_size, sizeof(uint8_t));
    }
    if (!(bfx.flags & BFX_FLAG_REPL) && path) {
        bfx_run_file(path, &bfx);
    } else if ((bfx.flags & BFX_FLAG_REPL) && !path) {
        bfx_run_repl(&bfx);
    } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    bfx_free(&bfx);

    return EXIT_SUCCESS;
}

static BFX_Language get_language(const char* s) {
    if (!strcmp(s, BRAINFORK_S)) {
        return BFX_LANG_brainfork;
    } else if (!strcmp(s, BRAINFUCK_S)) {
        return BFX_LANG_brainfuck;
    } else if (!strcmp(s, GRIN_S)) {
        return BFX_LANG_grin;
    } else if (!strcmp(s, PBRAIN_S)) {
        return BFX_LANG_pbrain;
    } else if (!strcmp(s, WEAVE_S)) {
        return BFX_LANG_weave;
    }

    return BFX_LANG_UNKNOWN;
}

static int get_eof_behavior(const char* s) {
    int         i;
    const char* eof_behavior[3];
    eof_behavior[BFX_EOF_BEHAVIOR_ZERO]      = EOF_BEHAVIOR_ZERO_S;
    eof_behavior[BFX_EOF_BEHAVIOR_DECREMENT] = EOF_BEHAVIOR_DECREMENT_S;
    eof_behavior[BFX_EOF_BEHAVIOR_UNCHANGED] = EOF_BEHAVIOR_UNCHANGED_S;

    for (i = 0; i < 3; i++) {
        if (!strcmp(s, eof_behavior[i])) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Prints the usage message for the program.
 *
 * @param argv0 The name of the program as it was invoked.
 */
static void print_usage(const char* argv0) {
    fprintf(stderr,
            "Usage: %s [-cCdDirsv] [-e eof_behavior] [-l language] [-o output_file] [-P "
            "precision] [-t tape_size] [file]\n",
            argv0);
}

static void print_version(const char* argv0) { fprintf(stderr, "%s %s\n", argv0, BFX_VERSION); }
