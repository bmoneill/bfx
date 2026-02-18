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

void setUp(void) {}

void tearDown(void) {}

void test_bfx_op_grin_putchar_ascii(void) {
    INITIALIZE(".");
    GRIN_DATA->tape[0] = 'A';
    REDIRECT_STDOUT;
    bfx_grin_run(&bfx);
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("A", out);
}
