#include "unity.h"

#include "interpret.h"
#include "langs/grin.h"
#include "util.c"

#define GRIN_DATA ((BFX_GrinData*) bfx.lang_data)

BFX_GrinData data;

#define INITIALIZE(s)                                                                              \
    bfx.tape = NULL;                                                                               \
    initialize(s);                                                                                 \
    memset(&data, 0, sizeof(BFX_GrinData));                                                        \
    bfx.lang       = BFX_LANG_GRIN;                                                                \
    bfx.lang_data  = (BFX_GrinData*) &data;                                                        \
    data.unit      = BFX_GRIN_RAD;                                                                 \
    data.precision = BFX_GRIN_DEFAULT_PRECISION;

#define EXECUTE(s)                                                                                 \
    INITIALIZE(s);                                                                                 \
    bfx_grin_run(&bfx);

void setUp(void) { memset(out, 0, 1024); }

void tearDown(void) {}

void test_bfx_op_grin_putchar_ascii(void) {
    INITIALIZE(".");
    GRIN_DATA->tape[0] = 'A';
    REDIRECT_STDOUT;
    bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("A", out);
}

void test_bfx_op_grin_putchar_number(void) {
    INITIALIZE(":");
    GRIN_DATA->tape[0] = 42.0;
    REDIRECT_STDOUT;
    bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("42.0000", out);
}

void test_bfx_op_grin_getchar_ascii(void) {
    // TODO
    TEST_ASSERT_EQUAL(1, 1);
}

void test_bfx_op_grin_getchar_number(void) {
    // TODO
    TEST_ASSERT_EQUAL(1, 1);
}

void test_bfx_op_grin_putchar_register_ascii(void) {
    INITIALIZE("'");
    GRIN_DATA->reg = 'A';
    REDIRECT_STDOUT;
    bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("A", out);
}

void test_bfx_op_grin_putchar_register_number(void) {
    INITIALIZE("\"");
    GRIN_DATA->reg = 42.0;
    REDIRECT_STDOUT;
    bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("42.0000", out);
}

void test_bfx_op_grin_add(void) {
    INITIALIZE("+");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(3.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_sub(void) {
    INITIALIZE("-");
    GRIN_DATA->tape[0] = 2.0;
    GRIN_DATA->reg     = 1.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_mul(void) {
    INITIALIZE("*");
    GRIN_DATA->tape[0] = 2.0;
    GRIN_DATA->reg     = 3.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(6.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_div(void) {
    INITIALIZE("/");
    GRIN_DATA->tape[0] = 6.0;
    GRIN_DATA->reg     = 3.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_exp(void) {
    INITIALIZE("^");
    GRIN_DATA->tape[0] = 5.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(25.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_store(void) {
    INITIALIZE("$");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->reg);
}

void test_bfx_op_grin_load(void) {
    INITIALIZE("\\");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_swap(void) {
    INITIALIZE("~");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->reg);
}

void test_bfx_op_grin_zero(void) {
    INITIALIZE("=");
    GRIN_DATA->tape[0] = 1.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_zero_ptr(void) {
    INITIALIZE("_");
    GRIN_DATA->tape[1] = 5.0;
    GRIN_DATA->reg     = 1.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[1]);
}

void test_bfx_op_grin_mod(void) {
    INITIALIZE("%");
    GRIN_DATA->tape[0] = 5.0;
    GRIN_DATA->reg     = 2.0;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_round(void) {
    INITIALIZE("r");
    GRIN_DATA->tape[0] = 6.5;

    bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL(7.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_nand(void) {
    // TODO
}
