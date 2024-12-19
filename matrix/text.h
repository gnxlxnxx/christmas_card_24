#ifndef MATRIX_TEXT_H_
#define MATRIX_TEXT_H_

#include <stdint.h>
#include <stdbool.h>

void text_start(const char *text, uint8_t text_brightness);
bool text_step(void);

void utoa10(unsigned int val, char *str);

#endif
