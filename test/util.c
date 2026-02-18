#include "bfx.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define REDIRECT_STDOUT                                                                            \
    freopen("/dev/null", "a", stdout);                                                             \
    setbuf(stdout, stdout_buffer);
#define RESTORE_STDOUT freopen("/dev/tty", "w", stdout);

#define WRITE_TO_STDIN(s)                                                                          \
    int fds[2];                                                                                    \
    pipe(fds);                                                                                     \
    write(fds[1], s, strlen(s));                                                                   \
    close(fds[1]);                                                                                 \
    dup2(fds[0], STDIN_FILENO);                                                                    \
    close(fds[0]);

BFX       bfx;
char      program[1024];
uint8_t   tape[BFX_DEFAULT_TAPE_SIZE];
BFX_Block loops[1024];
char      stdout_buffer[1024];

void      initialize(const char*);

void      initialize(const char* program_str) {
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
    memcpy(program, program_str, strlen(program_str));
}
