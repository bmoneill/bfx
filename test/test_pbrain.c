#include "unity.h"

#include "interpret.h"
#include "langs/pbrain.h"
#include "util.c"

BFX_PBrainData data;

void           setUp(void) {
    memset(stdout_buffer, 0, BUFFER_LENGTH);
    memset(&data, 0, sizeof(BFX_PBrainData));
}

void tearDown(void) {}

void test_bfx_op_pbrain_procedures(void) {
    // Make a procedure with identifier 1 that prints a newline and call it,
    // then add 2 and print again.
    initialize("+([-]++++++++++.): ++.");
    bfx.lang_data = &data;
    bfx.lang      = BFX_LANG_PBRAIN;

    REDIRECT_STDOUT;
    bfx_interpret(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(10, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(12, stdout_buffer[1]);
}

void test_bfx_op_pbrain_procedures_nested(void) {
    // Make a procedure with identifier 1 that prints a newline.
    // Make a procedure with identifier 2 that calls procedure 1,
    // adds 1, and prints again. Call procedure 2.
    initialize("+([-]++++++++++.) +([-]+:+.):");
    bfx.lang_data = &data;
    bfx.lang      = BFX_LANG_PBRAIN;

    REDIRECT_STDOUT;
    bfx_interpret(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(10, stdout_buffer[0]);
    TEST_ASSERT_EQUAL_INT(11, stdout_buffer[1]);
}
