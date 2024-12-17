#include <ch32v003fun.h>
#include <stdint.h>
#include "touch.h"

#define BUT_LEFT_PORT GPIOC
#define BUT_LEFT_PIN 5
#define BUT_RIGHT_PORT GPIOA
#define BUT_RIGHT_PIN 2

#define HIST_LOW (1<<6)
#define HIST_HIGH (1<<7) 
#define MAX_TIME (Ticks_from_Us(100))

/*#define DEBUG*/
#ifdef DEBUG
#include "matrix.h"
#endif

uint16_t but_left_analog = 0;
uint16_t but_right_analog = 0;
bool but_left = false;
bool but_right = false;

static uint8_t but_left_tcount = 0;
static uint8_t but_right_tcount = 0;

void measure_touch(void) {
  // Set output high
  BUT_LEFT_PORT->OUTDR |= 1<<BUT_LEFT_PIN;
  BUT_LEFT_PORT->CFGLR = (BUT_LEFT_PORT->CFGLR & ~(0xf<<(4*BUT_LEFT_PIN)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*BUT_LEFT_PIN);
  BUT_RIGHT_PORT->OUTDR |= 1<<BUT_RIGHT_PIN;
  BUT_RIGHT_PORT->CFGLR = (BUT_RIGHT_PORT->CFGLR & ~(0xf<<(4*BUT_RIGHT_PIN)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*BUT_RIGHT_PIN);

  Delay_Us(5);

  // Set input with pull-down
  BUT_LEFT_PORT->CFGLR = (BUT_LEFT_PORT->CFGLR & ~(0xf<<(4*BUT_LEFT_PIN)))
    | (GPIO_CNF_IN_PUPD)<<(4*BUT_LEFT_PIN);
  BUT_LEFT_PORT->OUTDR &= ~(1<<BUT_LEFT_PIN);
  BUT_RIGHT_PORT->CFGLR = (BUT_RIGHT_PORT->CFGLR & ~(0xf<<(4*BUT_RIGHT_PIN)))
    | (GPIO_CNF_IN_PUPD)<<(4*BUT_RIGHT_PIN);
  BUT_RIGHT_PORT->OUTDR &= ~(1<<BUT_RIGHT_PIN);

  uint32_t start = SysTick->CNT;

#ifdef DEBUG
  GPIOD->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP)<<(4*0);
  GPIOD->OUTDR |= 1<<0;
#endif

  uint32_t butl_elapsed = -1, butr_elapsed = -1;

  while ((butl_elapsed == -1 || butr_elapsed == -1) && SysTick->CNT - start < MAX_TIME) {
    if (butl_elapsed == -1 && !(BUT_LEFT_PORT->INDR & 1<<BUT_LEFT_PIN)) {
      butl_elapsed = SysTick->CNT - start;
    }
    if (butr_elapsed == -1 && !(BUT_RIGHT_PORT->INDR & 1<<BUT_RIGHT_PIN)) {
      butr_elapsed = SysTick->CNT - start;
    }
  }

  if (but_left ^ (butl_elapsed > ((but_left) ? HIST_LOW : HIST_HIGH))) {
    if (but_left_tcount++ >= 3) {
      but_left ^= 1;
      but_left_tcount = 0;
    }
  } else {
    but_left_tcount = 0;
  }
  but_left_analog = (but_left) ? butl_elapsed : 0;

  if (but_right ^ (butr_elapsed > ((but_right) ? HIST_LOW : HIST_HIGH))) {
    if (but_right_tcount++ >= 3) {
      but_right ^= 1;
      but_right_tcount = 0;
    }
  } else {
    but_right_tcount = 0;
  }
  but_right_analog = (but_right) ? butr_elapsed : 0;

#ifdef DEBUG
  GPIOD->OUTDR &= ~(1<<0);

  uint8_t *pos = &matrix_data[0][0];
  for (int8_t i = 31; i >= 0; i--) {
    *pos++ = (butl_elapsed & 1<<i) ? 255 : 0;
  }

  matrix_data[6][0] = (but_left) ? 255 : 0;
  matrix_data[6][7] = (but_right) ? 255 : 0;
#endif
}
