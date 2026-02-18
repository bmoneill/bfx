#include "unity.h"

#include "interpret.h"
#include "langs/grin.h"
#include "util.c"

#define GRIN_DATA ((BFX_GrinData*) bfx.lang_data)

#define INITIALIZE(s)                                                                              \
    bfx.tape = NULL;                                                                               \
    initialize(s);                                                                                 \
    bfx.lang = BFX_LANG_GRIN;                                                                      \
    bfx_grin_init(&bfx);

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
