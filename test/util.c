#include <string.h>

#include "bfx.h"

#define REDIRECT_STDOUT                                                                            \
    freopen("/dev/null", "a", stdout);                                                             \
    setbuf(stdout, out);

BFX       bfx;
char      program[1024];
uint8_t   tape[BFX_DEFAULT_TAPE_SIZE];
BFX_Block loops[1024];
char      out[1024];

void      initialize(const char*);

void initialize(const char* program_str) {
    size_t program_len = strlen(program_str);
    memset(&bfx, 0, sizeof(bfx));
    memset(tape, 0, sizeof(uint8_t) * BFX_DEFAULT_TAPE_SIZE);
    memset(program, 0, sizeof(char) * 1024);
    memset(loops, 0, sizeof(BFX_Block) * 1024);

    bfx.tape         = tape;
    bfx.program      = program;
    bfx.loops        = loops;
    bfx.tape_size    = BFX_DEFAULT_TAPE_SIZE;
    bfx.program_size = 1024;
    bfx.program_len  = program_len;
    bfx.loops_size   = 1024;
    memcpy(program, program_str, program_len);
}
