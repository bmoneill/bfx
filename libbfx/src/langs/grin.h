/**
 * @file langs/grin.h
 *
 * This file contains function definitions related to interpreting the Grin language.
 */
#ifndef BFX_LANGS_GRIN_H
#define BFX_LANGS_GRIN_H

#include "../bfx.h"

/**
 * @brief Unit of measurement for Grin language.
 */
typedef enum {
    BFX_GRIN_DEG = 0, //!< Degrees
    BFX_GRIN_RAD = 1 //!< Radians
} BFX_GrinUnit;

/**
 * @brief Data structure for Grin language.
 */
typedef struct {
    BFX_GrinUnit unit; //!< Unit of measurement.
    double       reg; //!< Register value.
    int          precision; //!< Number of decimal places to print.
    double       tape[BFX_DEFAULT_TAPE_SIZE]; //!< Tape memory.
} BFX_GrinData;

#ifndef BFX_GRIN_DEFAULT_PRECISION
/**
 * @brief Default number of decimal places to print.
 */
#define BFX_GRIN_DEFAULT_PRECISION 4
#endif

void bfx_grin_init(BFX*);
void bfx_grin_run(BFX*);

#endif
