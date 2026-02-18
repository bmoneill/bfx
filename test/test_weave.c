#include "unity.h"

#include "interpret.h"
#include "langs/weave.h"
#include "util.c"

void setUp(void) {}
void tearDown(void) {}

void test_weave(void) {
    initialize("+.;+.;+.");

    bfx.lang = BFX_LANG_WEAVE;
    REDIRECT_STDOUT;
    bfx_interpret(&bfx);
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[1]);
    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[2]);
}

void test_bfx_op_weave_toggle(void) {
    initialize("~+.;~+.;~+.");

    bfx.lang = BFX_LANG_WEAVE;
    REDIRECT_STDOUT;
    bfx_interpret(&bfx);
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_INT(3, bfx.tape[0]);
}
