#include "bfx.h"
#include "langs/util.h"
#include "unity.h"

#include "util.c"

void setUp(void) {}
void tearDown(void) {}

void test_bfx_build_loops(void) {
    BFX         bfx;
    const char* program = "[-]";
    bfx.program         = (char*) program;
    bfx.program_len     = strlen(program);
    bfx.program_size    = strlen(program);

    BFX_Error ret       = bfx_build_loops(&bfx);

    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_bfx_build_loops_WithUnmatchedBrackets(void) {
    BFX         bfx;
    const char* program = "[-";
    bfx.program         = (char*) program;
    bfx.program_len     = strlen(program);
    bfx.program_size    = strlen(program);

    BFX_Error ret       = bfx_build_loops(&bfx);

    TEST_ASSERT_EQUAL_INT(BFX_SYNTAX_ERROR, ret);
}
