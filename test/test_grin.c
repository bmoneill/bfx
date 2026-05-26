#include "unity.h"

#include "langs/grin.h"
#include "langs/util.h"
#include "util.c"

#define GRIN_DATA ((BFX_GrinData*) bfx.lang_data)

BFX_GrinData data;
int          ret;

#define INITIALIZE(s)                                                                              \
    bfx.tape = NULL;                                                                               \
    initialize(s);                                                                                 \
    memset(&data, 0, sizeof(BFX_GrinData));                                                        \
    bfx.lang       = BFX_LANG_grin;                                                                \
    bfx.lang_data  = (BFX_GrinData*) &data;                                                        \
    data.unit      = BFX_GRIN_RAD;                                                                 \
    data.precision = BFX_GRIN_DEFAULT_PRECISION;

#define EXECUTE(s)                                                                                 \
    INITIALIZE(s);                                                                                 \
    bfx_grin_run(&bfx);

void setUp(void) { memset(stdout_buffer, 0, BUFFER_LENGTH); }

void tearDown(void) {}

void test_bfx_op_grin_putchar_ascii(void) {
    INITIALIZE(".");
    GRIN_DATA->tape[0] = 'A';
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("A", stdout_buffer);
#endif
}

void test_bfx_op_grin_putchar_number(void) {
    INITIALIZE(":");
    GRIN_DATA->tape[0] = 42.0;
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("42.0000", stdout_buffer);
#endif
}

void test_bfx_op_grin_getchar_ascii(void) {
#ifdef WIN32
    TEST_IGNORE_MESSAGE(
        "Test not supported on Windows due to lack of support for redirecting stdin");
#else
    INITIALIZE(",");
    WRITE_TO_STDIN("A");
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT('A', GRIN_DATA->tape[0]);
#endif
}

void test_bfx_op_grin_getchar_number(void) {
#ifdef WIN32
    TEST_IGNORE_MESSAGE(
        "Test not supported on Windows due to lack of support for redirecting stdin");
#else
    INITIALIZE(";");
    WRITE_TO_STDIN("42.5");
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL_INT(42.5, GRIN_DATA->tape[0]);
#endif
}

void test_bfx_op_grin_putchar_register_ascii(void) {
    INITIALIZE("'");
    GRIN_DATA->reg = 'A';
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("A", stdout_buffer);
#endif
}

void test_bfx_op_grin_putchar_register_number(void) {
    INITIALIZE("\"");
    GRIN_DATA->reg = 42.0;
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("42.0000", stdout_buffer);
#endif
}

void test_bfx_op_grin_add(void) {
    INITIALIZE("+");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(3.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_sub(void) {
    INITIALIZE("-");
    GRIN_DATA->tape[0] = 2.0;
    GRIN_DATA->reg     = 1.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_mul(void) {
    INITIALIZE("*");
    GRIN_DATA->tape[0] = 2.0;
    GRIN_DATA->reg     = 3.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(6.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_div(void) {
    INITIALIZE("/");
    GRIN_DATA->tape[0] = 6.0;
    GRIN_DATA->reg     = 3.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_exp(void) {
    INITIALIZE("^");
    GRIN_DATA->tape[0] = 5.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(25.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_store(void) {
    INITIALIZE("$");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->reg);
}

void test_bfx_op_grin_load(void) {
    INITIALIZE("\\");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_swap(void) {
    INITIALIZE("~");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->reg);
}

void test_bfx_op_grin_zero(void) {
    INITIALIZE("=");
    GRIN_DATA->tape[0] = 1.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_zero_ptr(void) {
    INITIALIZE("_");
    GRIN_DATA->tape[1] = 5.0;
    GRIN_DATA->reg     = 1.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[1]);
}

void test_bfx_op_grin_mod(void) {
    INITIALIZE("%");
    GRIN_DATA->tape[0] = 5.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_round(void) {
    INITIALIZE("r");
    GRIN_DATA->tape[0] = 6.5;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(7.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_nand(void) {
    INITIALIZE("&");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(~(1 & 2), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_or(void) {
    INITIALIZE("|");
    GRIN_DATA->tape[0] = 1.0;
    GRIN_DATA->reg     = 2.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1 | 2, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_not(void) {
    INITIALIZE("!");
    GRIN_DATA->tape[0] = 1.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(~1, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_negate(void) {
    INITIALIZE("@");
    GRIN_DATA->tape[0] = 1.0;

    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(-1.0, GRIN_DATA->tape[0]);

    bfx.ip = 0;
    ret    = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_simplify(void) {
    INITIALIZE("?");

    GRIN_DATA->tape[0] = 10.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1.0, GRIN_DATA->tape[0]);

    bfx.ip             = 0;
    GRIN_DATA->tape[0] = -10.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(-1.0, GRIN_DATA->tape[0]);

    bfx.ip             = 0;
    GRIN_DATA->tape[0] = -0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);

    bfx.ip             = 0;
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_inc_t(void) {
    INITIALIZE("}");
    GRIN_DATA->tape[0] = 1.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(2.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_dec_t(void) {
    INITIALIZE("{");
    GRIN_DATA->tape[0] = 1.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_loops(void) {
    INITIALIZE("}}}[>}<{]")
    bfx_build_loops(&bfx);
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);
    TEST_ASSERT_EQUAL(3.0, GRIN_DATA->tape[1]);
}

void test_bfx_op_grin_print(void) {
    INITIALIZE("(HELLO WORLD)");
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    RESTORE_STDOUT;
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD\n", stdout_buffer);
#endif
}

void test_bfx_op_grin_newline(void) {
    INITIALIZE(")");
    REDIRECT_STDOUT;
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    RESTORE_STDOUT;
#ifndef WIN32
    TEST_ASSERT_EQUAL_STRING("\n", stdout_buffer);
#endif
}

void test_bfx_op_grin_sin(void) {
    INITIALIZE("s");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(sin(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_cos(void) {
    INITIALIZE("c");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(cos(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_tan(void) {
    INITIALIZE("t");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(tan(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_arcsin(void) {
    INITIALIZE("S");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(asin(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_arccos(void) {
    INITIALIZE("C");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(acos(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_arctan(void) {
    INITIALIZE("T");
    GRIN_DATA->tape[0] = 0.5;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(atan(0.5), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_inv(void) {
    INITIALIZE("1");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(1 / 20.0, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_mod2(void) {
    INITIALIZE("2");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(fmod(20.0, 2), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_e(void) {
    INITIALIZE("e");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(M_E, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_pi(void) {
    INITIALIZE("p");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(M_PI, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_ln(void) {
    INITIALIZE("l");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(log(20.0), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_mean(void) {
    INITIALIZE("m");
    GRIN_DATA->tape[0] = 20.0;
    GRIN_DATA->reg     = 10.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL((20.0 + 10.0) / 2, GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_sqrt(void) {
    INITIALIZE("q");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(sqrt(20.0), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_log(void) {
    INITIALIZE("L");
    GRIN_DATA->tape[0] = 20.0;
    ret                = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(log10(20.0), GRIN_DATA->tape[0]);
}

void test_bfx_op_grin_jump(void) {
    INITIALIZE("j");
    GRIN_DATA->reg = 20.0;
    ret            = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(20 + 1, bfx.ip);
}

void test_bfx_op_grin_toggle_deg_rad(void) {
    INITIALIZE("D");
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(BFX_GRIN_DEG, GRIN_DATA->unit);

    bfx.ip = 0;
    ret    = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(BFX_GRIN_RAD, GRIN_DATA->unit);
}

void test_bfx_op_grin_exit(void) {
    INITIALIZE("`{");
    ret = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(0.0, GRIN_DATA->tape[0]);

    bfx.ip = 0;
    ret    = bfx_grin_run(&bfx);
    TEST_ASSERT_EQUAL_INT(BFX_SUCCESS, ret);
    TEST_ASSERT_EQUAL(BFX_GRIN_RAD, GRIN_DATA->unit);
}
