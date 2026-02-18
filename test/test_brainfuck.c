#include "unity.h"

#include "bfx.h"
#include "interpret.h"
#include "util.c"

#define INITIALIZE(s)                                                                              \
    initialize(s);                                                                                 \
    bfx.lang = BFX_LANG_BRAINFUCK;

#define EXECUTE(s)                                                                                 \
    INITIALIZE(s);                                                                                 \
    bfx_interpret(&bfx);

BFX  bfx;

void setUp(void) {}
void tearDown(void) {}

void test_bfx_op_brainfuck_inc_tp(void) {
    EXECUTE(">");
    TEST_ASSERT_EQUAL_INT(1, bfx.tp);
}

void test_bfx_op_brainfuck_inc_tp_WhereTapePointerIsOverflowed(void) {
    INITIALIZE(">");
    bfx.tp = BFX_DEFAULT_TAPE_SIZE - 1;
    bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_dec_tp(void) {
    INITIALIZE("<");
    bfx.tp = 1;
    bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_dec_tp_WhereTapePointerIsUnderflowed(void) {
    EXECUTE("<");
    TEST_ASSERT_EQUAL_INT(0, bfx.tp);
}

void test_bfx_op_brainfuck_inc_t(void) {
    EXECUTE("+");
    TEST_ASSERT_EQUAL_INT(1, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_inc_t_WhereCellIsOverflowed(void) {
    INITIALIZE("+");
    bfx.tape[bfx.tp] = 255;
    bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_dec_t(void) {
    INITIALIZE("-");
    bfx.tape[bfx.tp] = 1;
    bfx_interpret(&bfx);
    TEST_ASSERT_EQUAL_INT(0, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_dec_t_WhereCellIsUnderflowed(void) {
    EXECUTE("-");
    TEST_ASSERT_EQUAL_INT(255, bfx.tape[bfx.tp]);
}

void test_bfx_op_brainfuck_loop_start_WhereCellIsZero(void) {
    // TODO implement
}

void test_bfx_op_brainfuck_loop_start_WhereCellIsNonZero(void) {
    // TODO implement
}

void test_bfx_op_brainfuck_loop_start_WhereMatchingBracketIsMissing(void) {
    // TODO implement
}

void test_bfx_op_brainfuck_loop_end_WhereCellIsZero(void) {
    // TODO implement
}

void test_bfx_op_brainfuck_loop_end_WhereCellIsNonZero(void) {
    // TODO implement
}

void test_bfx_op_brainfuck_loop_end_WhereMatchingBracketIsMissing(void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsIntegrated(void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsIntegrated_WhereReceivingIsTrue(void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated(void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithZeroEOFBehavior(void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithDecrementEOFBehavior(
    void) {
    // TODO implement
}

void test_bfx_op_getchar_WhereInputIsNotIntegrated_WhereReceivingIsFalse_WithUnchangedEOFBehavior(
    void) {
    // TODO implement
}

void test_bfx_op_putchar(void) {
    // TODO implement
}
