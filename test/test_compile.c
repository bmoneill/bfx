#include "unity.h"

#include "compile.h"
#include "util.c"

BFX  bfx;

void setUp(void) { memset(&bfx, 0, sizeof(BFX)); }
void tearDown(void) {}

void test_bfx_compile(void) {
    // Find BFX library and add to LIBRARY_PATH
    const char* possible_paths[] = { "libbfx/", "build/libbfx", "bfx/build/libbfx", "../libbfx" };
    int         success          = 0;
    char        pwdbuf[1024];
    getcwd(pwdbuf, 1024);

    for (int i = 0; i < 4; i++) {
        if (access(possible_paths[i], F_OK) == 0) {
            setenv("LIBRARY_PATH", possible_paths[i], 1);
            success = 1;
            break;
        }
    }

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, success, "Failed to find BFX library");

    REDIRECT_STDOUT;
    bfx_compile(get_path("brainfuck/cat.b"), "a.out", &bfx);
    RESTORE_STDOUT;

#ifndef WIN32
    TEST_ASSERT_EQUAL_INT(0, strlen(stdout_buffer));
#endif
}
