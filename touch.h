#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdbool.h>
#include <stdint.h>

extern uint16_t btn_left_analog;
extern uint16_t btn_right_analog;
extern bool btn_left;
extern bool btn_left_toggled;
extern bool btn_right;
extern bool btn_right_toggled;

void touch_init(void);
void touch_update(void);

#endif
