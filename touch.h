#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdbool.h>
#include <stdint.h>

extern uint32_t but_left_analog;
extern uint32_t but_right_analog;
extern bool but_left;
extern bool but_right;

extern void touch_init(void);
extern void touch_loop(void);

#endif
