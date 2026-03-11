#include "unity.h"

#include "bfx.h"
#include "util.c"

BFX  bfx;

void setUp(void) {
    memset(&bfx, 0, sizeof(BFX));
    memset(stdout_buffer, 0, sizeof(stdout_buffer));
}
void tearDown(void) {}

void test_bfx_free_WhereBFXIsNULL(void) {
    bfx_free(NULL); // No crash
}

void test_bfx_free_WhereBFXIsNotNULL(void) {
    bfx.program = malloc(1);
    bfx.tape    = malloc(1);
    bfx.loops   = malloc(1);
    bfx_free(&bfx);

    TEST_ASSERT_NULL(bfx.program);
    TEST_ASSERT_NULL(bfx.tape);
    TEST_ASSERT_NULL(bfx.loops);
}

void test_bfx_run_file_WherePathIsNULL_WhereBFXIsValid(void) {
    TEST_ASSERT_EQUAL_INT(BFX_RUNTIME_ERROR, bfx_run_file(NULL, &bfx));
}

void test_bfx_run_file_WherePathIsNULL_WhereBFXIsNULL(void) {
    TEST_ASSERT_EQUAL_INT(BFX_RUNTIME_ERROR, bfx_run_file(NULL, NULL));
}

void test_bfx_run_file_WherePathIsValid_WhereBFXIsNULL(void) {
    TEST_ASSERT_EQUAL_INT(BFX_RUNTIME_ERROR, bfx_run_file(get_path("brainfork/hello.f"), NULL));
}

void test_bfx_run_file_WherePathIsValid_WhereBFXIsValid(void) {
    TEST_ASSERT_EQUAL_INT(BFX_RUNTIME_ERROR, bfx_run_file(get_path("brainfork/hello.f"), &bfx));
}

void test_bfx_run_file_WherePathIsValid_WhereBFXIsValid_WithSeparatedInputAndSource(void) {
    bfx.lang      = BFX_LANG_brainfuck;
    bfx.receiving = 1;
    bfx.tape      = malloc(1);
    bfx.flags |= BFX_FLAG_SEPARATE_INPUT_AND_SOURCE;
    REDIRECT_STDOUT;
    TEST_ASSERT_EQUAL_INT(0, bfx_run_file(get_path("brainfuck/cat-separated.b"), &bfx));
    RESTORE_STDOUT;

    TEST_ASSERT_EQUAL_STRING("Hello world\n", stdout_buffer);
    free(bfx.tape);
}

void test_bfx_run_repl(void) {
    bfx.lang      = BFX_LANG_grin;
    bfx.input_max = 50;
    bfx.receiving = 1;
    WRITE_TO_STDIN("(HELLO)`(WORLD)\n");
    REDIRECT_STDOUT;
    TEST_ASSERT_EQUAL_INT(0, bfx_run_repl(&bfx));
    RESTORE_STDOUT;
    TEST_ASSERT_EQUAL_STRING("> HELLO\n> ", stdout_buffer);
}
