#ifndef MATRIX_MATRIX_H_
#define MATRIX_MATRIX_H_

#include <stdint.h>

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 7

extern uint8_t matrix_data[MATRIX_HEIGHT][MATRIX_WIDTH];

void matrix_init(void);
void matrix_update(void);
void matrix_next_mode(void);

#endif
