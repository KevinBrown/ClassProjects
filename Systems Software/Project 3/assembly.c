//
// Created by Kevin on 7/14/2015.
//

#include "assembly.h"

#include <uthash/uthash.h>

// Assembler Settings Macros
#define MAX_VAR_SIZE 20

struct token {
    int tokenID;
    char name[MAX_VAR_SIZE];
};

/**
 * Returns 0 if the given series of tokens has no errors or an error code if errors exist
 *
 * @param stuct tokenName the next token in the series
 * @returns int error code
 */

