#include "unity.h"

#include "interpret.h"
#include "util.c"

BFX  bfx;

void setUp(void) { memset(&bfx, 0, sizeof(BFX)); }
void tearDown(void) {}
