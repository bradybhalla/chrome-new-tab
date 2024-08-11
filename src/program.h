#ifndef PROGRAM_H
#define PROGRAM_H

#include "utils.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct program program_t;

/* Create a new program and return a pointer */
program_t *program_init();

/* Destroy program and window. Free all allocated space. */
void program_destroy(program_t *program);

/* Handles events and updates the program.
 * Return true if the program should keep running. */
bool program_update(program_t *program, uint64_t time);

#endif
