#include "unity.h"

#include "bfx.h"
#include "interpret.h"
#include "util.c"

#define INITIALIZE(s)                                                                              \
    initialize(s);                                                                                 \
    bfx.lang = BFX_LANG_GRIN;

#define EXECUTE(s)                                                                                 \
    INITIALIZE(s);                                                                                 \
    bfx_interpret(&bfx);

BFX  bfx;

void setUp(void) {}

void tearDown(void) { free(bfx.lang_data); }

void test_bfx_op_grin_inc_t(void) {
    // TODO implement
    TEST_ASSERT_EQUAL_INT(1, 1);
}
