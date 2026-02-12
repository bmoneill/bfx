#ifndef BFX_TEST_UTIL_H
#define BFX_TEST_UTIL_H

#include "bfx.h"

BFX       bfx;
char      program[1024];
uint8_t   tape[BFX_DEFAULT_TAPE_SIZE];
BFX_Block loops[1024];

void      initialize(const char*);

#endif
