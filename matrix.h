#ifndef MATRIX_H_
#define MATRIX_H_
#include <stdint.h>
void timer_matrix_init(void);
void output_matrix();
void change_col(uint8_t col, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4);
extern uint8_t matrix_data[4][4];
#endif // MATRIX_H_
