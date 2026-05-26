#include "unity.h"

#include "interpret.h"
#include "langs/weave.c"
#include "util.c"

int  ret;

void setUp(void) {}
void tearDown(void) {}

void test_weave(void) {
    initialize("+.;+.;+.");

    bfx.lang = BFX_LANG_weave;
    REDIRECT_STDOUT;
    ret = bfx_interpret(&bfx);
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
#ifndef WIN32
    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[1]);
    TEST_ASSERT_EQUAL_INT(1, stdout_buffer[2]);
#endif
}

void test_bfx_op_weave_toggle(void) {
    initialize("~+.;~+.;~+.");

    bfx.lang = BFX_LANG_weave;
    REDIRECT_STDOUT;
    ret = bfx_interpret(&bfx);
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(3, bfx.tape[0]);
}

void test_bfx_op_weave_exit(void) {
    initialize("++;");
    ret = bfx_op_weave_exit(&bfx, NULL);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(bfx.program_len, bfx.ip);
}
