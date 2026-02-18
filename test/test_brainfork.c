#include "unity.h"

#include "langs/brainfork.h"
#include "util.c"

void setUp(void) {}
void tearDown(void) {}

void test_bfx_op_brainfork_fork(void) {
    // This program forks (setting child tape[tp] to 1, keeps parent tape[tp] at 0)
    // Output should be: 3,4
    initialize("Y+++.");
    int ret = bfx_brainfork_init(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    REDIRECT_STDOUT;
    ret = bfx_brainfork_run(&bfx);
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    // Assert than output is either [3,4] or [4,3]
    TEST_ASSERT_GREATER_THAN_INT(2, stdout_buffer[0]);
    TEST_ASSERT_GREATER_THAN_INT(2, stdout_buffer[1]);
    TEST_ASSERT_LESS_THAN_INT(5, stdout_buffer[0]);
    TEST_ASSERT_LESS_THAN_INT(5, stdout_buffer[1]);
    TEST_ASSERT_NOT_EQUAL_INT(stdout_buffer[0], stdout_buffer[1]);
}
