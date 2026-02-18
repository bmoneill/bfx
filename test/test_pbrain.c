#include "bfx.h"
#include "unity.h"

#include "interpret.h"
#include "langs/pbrain.h"
#include "util.c"

int  ret;

void setUp(void) { memset(stdout_buffer, 0, BUFFER_LENGTH); }

void tearDown(void) {}

void test_bfx_op_pbrain_procedures(void) {
    // Make a procedure with identifier 1 that prints a newline and call it,
    // then add 2 and print again.
    initialize("+([-]++++++++++.): ++.");
    bfx.lang = BFX_LANG_pbrain;

    REDIRECT_STDOUT;
    ret = bfx_interpret(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(10, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(12, stdout_buffer[1]);
}

void test_bfx_op_pbrain_procedures_nested(void) {
    // Make a procedure with identifier 1 that prints a newline.
    // Make a procedure with identifier 2 that calls procedure 1,
    // adds 1, and prints again. Call procedure 2.
    initialize("+([-]++++++++++.) +([-]+:+.):");
    bfx.lang = BFX_LANG_pbrain;

    REDIRECT_STDOUT;
    ret = bfx_interpret(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(10, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(11, stdout_buffer[1]);
}

void test_bfx_op_pbrain_procedures_stack_overflow(void) {
    // Two procedures that call each other recursively.
    initialize("+([-]++:)   +([-]+:)   :");
    bfx.lang = BFX_LANG_pbrain;

    int ret  = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_STACK_OVERFLOW_ERROR, ret);
}

void test_bfx_op_pbrain_procedures_stack_underflow(void) {
    initialize(")");
    bfx.lang = BFX_LANG_pbrain;

    int ret  = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_STACK_UNDERFLOW_ERROR, ret);
}
