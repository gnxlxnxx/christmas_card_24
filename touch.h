#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdbool.h>
#include <stdint.h>

extern uint16_t but_left_analog;
extern uint16_t but_right_analog;
extern bool but_left;
extern bool but_right;

void measure_touch(void);

#endif
