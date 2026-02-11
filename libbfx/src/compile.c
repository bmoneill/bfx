/**
 * @file compile.c
 *
 * This file contains the implementation of the Brainfuck-like language compiler.
 */

#include "compile.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BFX_SET_PROPERTY_BOOL(property, value)                                                     \
    if (value) {                                                                                   \
        fprintf(output, "bfx.%s = true;", property);                                               \
    } else {                                                                                       \
        fprintf(output, "bfx.%s = false;", property);                                              \
    }
#define BFX_SET_PROPERTY_INT(property, value)      fprintf(output, "bfx.%s = %d;", property, value)
#define BFX_SET_PROPERTY_SIZE_T(property, value)   fprintf(output, "bfx.%s = %ld;", property, value)
#define BFX_SET_PROPERTY_UINT16_T(property, value) fprintf(output, "bfx.%s = %d;", property, value)

/**
 * @brief Compile Brainfuck code from input_path to output_path.
 *
 * This function takes a Brainfuck source code file specified by input_path,
 * compiles it into a C program, compiles the C program, and writes the resulting code to the file
 * specified by output_path.
 * If input_path is NULL, the function will read from stdin. If output_path is NULL, the function
 * will write to ./a.out(.c).
 *
 * @param input_path Path to the input Brainfuck source code file.
 * @param output_path Path to the output binary or C file.
 * @param params Compilation parameters
 */
void bfx_compile(const char* input_path, const char* output_path, BFX* bfx) {
    FILE* input;
    FILE* output;
    bool  binary_output = !(bfx->flags & BFX_FLAG_ONLY_GENERATE_C_SOURCE);

    /**** Set up files ****/
    if (!input_path) {
        input = stdin;
    } else if (!(input = fopen(input_path, "r"))) {
        BFX_ERROR("Failed to open input file");
    }

    if (!output_path) {
        output_path = binary_output ? "./a.out" : "./a.out.c";
    }

    if (binary_output && !(output = fopen(BFX_TMP_FILE_PATH, "w"))) {
        BFX_ERROR("Failed to create temporary file");
    } else if (!(output = fopen(output_path, "w"))) {
        BFX_ERROR("Failed to open output file");
    }

    /*** Actual compilation ***/
    fprintf(output, BFX_COMPILE_HEAD);
    BFX_SET_PROPERTY_UINT16_T("flags", bfx->flags);
    BFX_SET_PROPERTY_BOOL("receiving", bfx->receiving);
    BFX_SET_PROPERTY_SIZE_T("program_len", bfx->program_len);
    BFX_SET_PROPERTY_SIZE_T("program_size", bfx->program_size);
    BFX_SET_PROPERTY_SIZE_T("input_start", bfx->input_start);
    BFX_SET_PROPERTY_SIZE_T("input_ptr", bfx->input_ptr);
    BFX_SET_PROPERTY_SIZE_T("input_len", bfx->input_len);
    BFX_SET_PROPERTY_SIZE_T("tape_size", bfx->tape_size);
    BFX_SET_PROPERTY_INT("ip", bfx->ip);
    BFX_SET_PROPERTY_INT("tp", bfx->tp);
    BFX_SET_PROPERTY_INT("tp_max", bfx->tp_max);
    BFX_SET_PROPERTY_SIZE_T("loops_len", bfx->loops_len);
    BFX_SET_PROPERTY_SIZE_T("loops_size", bfx->loops_size);
    BFX_SET_PROPERTY_SIZE_T("input_max", bfx->input_max);
    BFX_SET_PROPERTY_INT("eof_behavior", bfx->eof_behavior);
    BFX_SET_PROPERTY_INT("lang", bfx->lang);

    int c;
    while ((c = fgetc(input)) != EOF) {
    }

    if (depth != 0) {
        fclose(output);
        remove(output_path);
        BFX_ERROR("Unbalanced brackets");
    }

    fprintf(output, "return 0;}");
    fclose(output);

    if (binary_output) {
        char* cmd = malloc(128);
        sprintf(cmd,
                "%s %s -o %s %s",
                BFX_DEFAULT_COMPILER,
                BFX_DEFAULT_COMPILE_FLAGS,
                output_path,
                BFX_TMP_FILE_PATH);
        int sys_ret = system(cmd);
        remove(BFX_TMP_FILE_PATH);
        if (sys_ret != 0) {
            BFX_ERROR("Failed to compile program");
        }
    }
}
