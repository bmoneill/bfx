#include "unity.h"

#include "bfx.h"
#include "interpret.h"
#include "util.c"

#define INITIALIZE(s)                                                                              \
    initialize(s);                                                                                 \
    bfx.lang = BFX_LANG_brainfuck;

#define EXECUTE(s)                                                                                 \
    INITIALIZE(s);                                                                                 \
    ret = bfx_interpret(&bfx);

BFX  bfx;
int  ret;

void setUp(void) { memset(stdout_buffer, 0, BUFFER_LENGTH); }
void tearDown(void) {}

void test_bfx_op_brainfuck_inc_tp(void) {
    EXECUTE(">");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(1, bfx.tp);
}

void test_bfx_op_brainfuck_inc_tp_WhereTapePointerIsOverflowed(void) {
    INITIALIZE(">");
    bfx.tp = BFX_DEFAULT_TAPE_SIZE - 1;
    ret    = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_dec_tp(void) {
    INITIALIZE("<");
    bfx.tp = 1;
    ret    = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_dec_tp_WhereTapePointerIsUnderflowed(void) {
    EXECUTE("<");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_inc_t(void) {
    EXECUTE("+");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(1, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_inc_t_WhereCellIsOverflowed(void) {
    INITIALIZE("+");
    bfx.tape[bfx.tp] = 255;
    ret              = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_dec_t(void) {
    INITIALIZE("-");
    bfx.tape[bfx.tp] = 1;
    ret              = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_dec_t_WhereCellIsUnderflowed(void) {
    EXECUTE("-");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(255, bfx.tape[bfx.tp]);
}

void test_brainfuck_loops_WhereCellIsZero(void) {
    EXECUTE("[+>]");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[0]);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[1]);
}

void test_brainfuck_loops_WhereCellIsNonZero(void) {
    EXECUTE("+[>+<-]");
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[0]);
    TEST_ASSERT_EQUAL_INT(1, bfx.tape[1]);
}

void test_brainfuck_loops_WithNonMatchingOpenBracket(void) {
    EXECUTE("+[>+<-");
    TEST_ASSERT_EQUAL_INT(BFX_SYNTAX_ERROR, ret);
}

void test_brainfuck_loops_WithNonMatchingCloseBracket(void) {
    EXECUTE("+>+<-]");
    TEST_ASSERT_EQUAL_INT(BFX_SYNTAX_ERROR, ret);
}

void test_bfx_op_getchar_WhereInputIsIntegrated_WhereReceivingIsTrue(void) {
    INITIALIZE(",!A");
    bfx.input_start = 2;
    bfx.input_len   = bfx.input_start + 1;
    bfx.input_ptr   = bfx.input_start;
    bfx.receiving   = true;
    bfx.flags |= BFX_FLAG_SEPARATE_INPUT_AND_SOURCE;
    ret = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT('A', bfx.tape[bfx.tp]);
}

void test_bfx_op_getchar_WhereInputIsIntegrated_WhereReceivingIsFalse(void) {
    INITIALIZE(",!A");
    bfx.input_start = 2;
    bfx.input_len   = bfx.input_start + 1;
    bfx.input_ptr   = bfx.input_start;
    bfx.receiving   = false;
    bfx.flags |= BFX_FLAG_SEPARATE_INPUT_AND_SOURCE;
    ret = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsTrue(void) {
    INITIALIZE(",");
    bfx.receiving = true;

    WRITE_TO_STDIN("A");
    ret = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT('A', bfx.tape[bfx.tp]);
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithZeroEOFBehavior(void) {
    INITIALIZE(",");
    bfx.eof_behavior = BFX_EOF_BEHAVIOR_ZERO;
    bfx.receiving    = false;

    WRITE_TO_STDIN("A");
    bfx.tape[bfx.tp] = 1;
    ret              = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithDecrementEOFBehavior(
    void) {
    INITIALIZE(",");
    bfx.eof_behavior = BFX_EOF_BEHAVIOR_DECREMENT;
    bfx.receiving    = false;

    WRITE_TO_STDIN("A");
    bfx.tape[bfx.tp] = 2;
    ret              = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(1, bfx.tape[bfx.tp]);
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithUnchangedEOFBehavior(
    void) {
    INITIALIZE(",");
    bfx.eof_behavior = BFX_EOF_BEHAVIOR_UNCHANGED;
    bfx.receiving    = false;

    WRITE_TO_STDIN("A");
    bfx.tape[bfx.tp] = 2;
    ret              = bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(2, bfx.tape[bfx.tp]);
}

void test_bfx_op_putchar(void) {
    INITIALIZE(".");
    bfx.tape[bfx.tp] = 'A';
    REDIRECT_STDOUT;
    ret = bfx_interpret(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_STRING("A", stdout_buffer);
}
