#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdint.h>

extern uint8_t matrix_data[7][8];

void matrix_init(void);
void matrix_update(void);

#endif // MATRIX_H_
