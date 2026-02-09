/**
 * @file langs/pbrain.h
 *
 * This file contains public definitions for the implementation of the P-Brain interpreter.
 */

#ifndef BFX_PBRAIN_H
#define BFX_PBRAIN_H

#include "../bfx.h"

#ifndef BFX_PBRAIN_MAX_PROCEDURES
/**
 * @brief Maximum number of procedures allowed in the P-Brain language.
 *
 * This limit is set to the maximum possible value since procedures are
 * designated by 8-bit identifiers.
 */
#define BFX_PBRAIN_MAX_PROCEDURES 256
#endif

/**
 * @brief Structure to represent a procedure in the pbrain language.
 */
typedef struct {
    char   identifier; //!< Identifier of the procedure
    size_t start_idx; //!< Start index of the procedure
    size_t end_idx; //!< End index of the procedure
} BFX_PBrainProcedure;

/**
 * @brief Structure to represent data for the pbrain language.
 *
 * This populates the lang_data field in a BFX instance.
 */
typedef struct {
    BFX_PBrainProcedure* procedures; //!< Array of procedures
    size_t               procedures_len; //!< Length of the procedures array
    size_t*              stack; //!< Array of stack elements
    size_t               stack_top; //!< Top index of the stack
} BFX_PBrainData;

void bfx_pbrain_init(BFX* bfx);
void bfx_pbrain_run(BFX* bf);

#endif
