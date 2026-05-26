/**
 * @file compile.c
 *
 * This file contains the implementation of the Brainfuck-like language compiler.
 */

#include "compile.h"
#include "bfx.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BFX_SET_PROPERTY_BOOL(property, value)                                                     \
    if (value) {                                                                                   \
        fprintf(output, "bfx.%s = true;", property);                                               \
    } else {                                                                                       \
        fprintf(output, "bfx.%s = false;", property);                                              \
    }
#define BFX_SET_PROPERTY_INT(property, value) fprintf(output, "bfx.%s = %d;", property, value)

#ifdef WIN32
#define BFX_SET_PROPERTY_SIZE_T(property, value) fprintf(output, "bfx.%s = %lld;", property, value)
#else
#define BFX_SET_PROPERTY_SIZE_T(property, value) fprintf(output, "bfx.%s = %ld;", property, value)
#endif

#define BFX_SET_PROPERTY_UINT16_T(property, value) fprintf(output, "bfx.%s = %d;", property, value)
#define BFX_SET_PROPERTY_CONST_CHAR(property, value)                                               \
    fprintf(output, "bfx.%s = \"%s\";", property, value)

/**
 * @brief Compile Brainfuck code from input_path to output_path.
 *
 * This function takes a Brainfuck source code file specified by input_path,
 * compiles it into a C program, compiles the C program, and writes the resulting code to the file
 * specified by output_path.
 * If input_path is NULL, the function will read from stdin. If output_path is NULL, the function
 * will write to ./a.out.
 *
 * @param input_path Path to the input Brainfuck source code file.
 * @param output_path Path to the output binary or C file.
 * @param bfx BFX configuration to use for compilation.
 */
void bfx_compile(const char* input_path, const char* output_path, BFX* bfx) {
    FILE* input;
    FILE* output;

    /**** Set up files ****/
    if (!input_path) {
        input = stdin;
    } else if (!(input = fopen(input_path, "r"))) {
        BFX_ERROR("Failed to open input file");
    }
    if (!output_path) {
        output_path = "./a.out";
    }
    if (!(output = fopen(BFX_TMP_FILE_PATH, "w"))) {
        BFX_ERROR("Failed to create temporary file");
        return;
    }

    /**** Load program ****/
    char   c;
    size_t i       = 0;
    size_t ceiling = 2048;
    char*  program = malloc(2048);
    while ((c = fgetc(input)) != EOF) {
        switch (c) {
        case '\"':
            program[i]     = '\\';
            program[i + 1] = '"';
            i += 2;
            break;
        case '\\':
            program[i]     = '\\';
            program[i + 1] = '\\';
            i += 2;
            break;
        case '\n':
            break;
        default:
            program[i] = c;
            i++;
        }

        if (i >= ceiling - 2) {
            ceiling += 2048;
            program = realloc(program, ceiling);
        }
    }

    /*** Actual compilation ***/
    fprintf(output, BFX_COMPILE_HEAD);
    fprintf(output, "bfx.tape = malloc(%ld * sizeof(char));\n", bfx->tape_size);

    if (bfx->lang == BFX_LANG_grin && bfx->flags & BFX_FLAG_LANG_DATA_FLAGS) {
        fprintf(output,
                "bfx.lang_data = malloc(sizeof(int)); ((int*) bfx.lang_data)[0] = %d;\n",
                ((int*) bfx->lang_data)[0]);
    }

    BFX_SET_PROPERTY_UINT16_T("flags", bfx->flags);
    BFX_SET_PROPERTY_BOOL("receiving", bfx->receiving);
    BFX_SET_PROPERTY_SIZE_T("program_len", i);
    BFX_SET_PROPERTY_SIZE_T("program_size", i);
    BFX_SET_PROPERTY_CONST_CHAR("program", program);
    BFX_SET_PROPERTY_SIZE_T("input_start", bfx->input_start);
    BFX_SET_PROPERTY_SIZE_T("input_ptr", bfx->input_ptr);
    BFX_SET_PROPERTY_SIZE_T("input_len", bfx->input_len);
    BFX_SET_PROPERTY_SIZE_T("tape_size", bfx->tape_size);
    BFX_SET_PROPERTY_INT("ip", bfx->ip);
    BFX_SET_PROPERTY_INT("tp", bfx->tp);
    BFX_SET_PROPERTY_SIZE_T("loops_len", bfx->loops_len);
    BFX_SET_PROPERTY_SIZE_T("loops_size", bfx->loops_size);
    BFX_SET_PROPERTY_SIZE_T("input_max", bfx->input_max);
    BFX_SET_PROPERTY_INT("eof_behavior", bfx->eof_behavior);
    BFX_SET_PROPERTY_INT("lang", bfx->lang);
    fprintf(output, "return bfx_interpret(&bfx);}");
    fclose(output);

    char* cmd = malloc(128);
    sprintf(cmd,
            "%s %s -o %s %s %s",
            BFX_DEFAULT_COMPILER,
            BFX_DEFAULT_COMPILE_FLAGS,
            output_path,
            BFX_TMP_FILE_PATH,
            BFX_COMPILER_LDFLAGS);
    int sys_ret = system(cmd);
    remove(BFX_TMP_FILE_PATH);
    if (sys_ret != 0) {
        BFX_ERROR("Failed to compile program");
    }

    free(cmd);
}
