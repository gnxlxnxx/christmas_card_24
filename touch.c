#include <ch32v003fun.h>
#include <stdint.h>
#include "touch.h"

#define BTN_LEFT_PORT GPIOC
#define BTN_LEFT_PIN 5
#define BTN_RIGHT_PORT GPIOA
#define BTN_RIGHT_PIN 2

#define HIST_LOW (1<<6)
#define HIST_HIGH (1<<7) 
#define MAX_TIME (Ticks_from_Us(96))

/*#define DEBUG*/
#ifdef DEBUG
#include "matrix/matrix.h"
#endif

uint16_t btn_left_analog = 0;
uint16_t btn_right_analog = 0;
bool btn_left = false;
bool btn_left_toggled = false;
bool btn_right = false;
bool btn_right_toggled = false;

static uint8_t btn_left_tcount = 0;
static uint8_t btn_right_tcount = 0;

void touch_init(void) {
  for (int i = 0; i < 5; i++) {
    touch_update();
  }

  btn_left_toggled = false;
  btn_right_toggled = false;
}

void touch_update(void) {
  // Set output high
  BTN_LEFT_PORT->OUTDR |= 1<<BTN_LEFT_PIN;
  BTN_LEFT_PORT->CFGLR = (BTN_LEFT_PORT->CFGLR & ~(0xf<<(4*BTN_LEFT_PIN)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*BTN_LEFT_PIN);
  BTN_RIGHT_PORT->OUTDR |= 1<<BTN_RIGHT_PIN;
  BTN_RIGHT_PORT->CFGLR = (BTN_RIGHT_PORT->CFGLR & ~(0xf<<(4*BTN_RIGHT_PIN)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*BTN_RIGHT_PIN);

  Delay_Us(5);

  // Set input with pull-down
  BTN_LEFT_PORT->CFGLR = (BTN_LEFT_PORT->CFGLR & ~(0xf<<(4*BTN_LEFT_PIN)))
    | (GPIO_CNF_IN_PUPD)<<(4*BTN_LEFT_PIN);
  BTN_LEFT_PORT->OUTDR &= ~(1<<BTN_LEFT_PIN);
  BTN_RIGHT_PORT->CFGLR = (BTN_RIGHT_PORT->CFGLR & ~(0xf<<(4*BTN_RIGHT_PIN)))
    | (GPIO_CNF_IN_PUPD)<<(4*BTN_RIGHT_PIN);
  BTN_RIGHT_PORT->OUTDR &= ~(1<<BTN_RIGHT_PIN);

  uint32_t start = SysTick->CNT;

#ifdef DEBUG
  GPIOD->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP)<<(4*0);
  GPIOD->OUTDR |= 1<<0;
#endif

  uint32_t btnl_elapsed = MAX_TIME, btnr_elapsed = MAX_TIME;

  while ((btnl_elapsed == MAX_TIME || btnr_elapsed == MAX_TIME) && SysTick->CNT - start < MAX_TIME) {
    if (btnl_elapsed == MAX_TIME && !(BTN_LEFT_PORT->INDR & 1<<BTN_LEFT_PIN)) {
      btnl_elapsed = SysTick->CNT - start;
    }
    if (btnr_elapsed == MAX_TIME && !(BTN_RIGHT_PORT->INDR & 1<<BTN_RIGHT_PIN)) {
      btnr_elapsed = SysTick->CNT - start;
    }
  }

  btn_left_toggled = false;
  if (btn_left ^ (btnl_elapsed > ((btn_left) ? HIST_LOW : HIST_HIGH))) {
    if (btn_left_tcount++ >= 3) {
      btn_left ^= 1;
      btn_left_toggled = true;
      btn_left_tcount = 0;
    }
  } else {
    btn_left_tcount = 0;
  }
  btn_left_analog = (btn_left) ? btnl_elapsed : 0;

  btn_right_toggled = false;
  if (btn_right ^ (btnr_elapsed > ((btn_right) ? HIST_LOW : HIST_HIGH))) {
    if (btn_right_tcount++ >= 3) {
      btn_right ^= 1;
      btn_right_toggled = true;
      btn_right_tcount = 0;
    }
  } else {
    btn_right_tcount = 0;
  }
  btn_right_analog = (btn_right) ? btnr_elapsed : 0;

#ifdef DEBUG
  GPIOD->OUTDR &= ~(1<<0);

  uint8_t *pos = &matrix_data[0][0];
  for (int8_t i = 31; i >= 0; i--) {
    *pos++ = (btnl_elapsed & 1<<i) ? 255 : 0;
  }

  matrix_data[6][0] = (btn_left) ? 255 : 0;
  matrix_data[6][7] = (btn_right) ? 255 : 0;
#endif
}
