#include <ch32v003fun.h>
#include <stdint.h>
#include "matrix.h"

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 7

#define ROWCOUNT (MATRIX_WIDTH)
#define GROUPCOUNT 3

#define PWM_MAX 1023

#define ROW_PWM_CNF (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF)

/// LED1:   PD2 *
/// LED2:   PC7 *
/// LED3:   PC4 *
/// LED4:   PC3 *
/// LED5:   PC2 *
/// LED6:   PC1 *
/// LED7:   PC0 *
/// LED8:   PA1 *
///
/// TIM2:
/// 00:     PD4  PD3  PC0  PD7
/// 01:     PC5  PC2* PD2* PC1* *
/// 10:     PC1  PD3  PC0  PD7 
/// 11:     PC1  PC7  PD6  PD5 
///
/// TIM1:
/// 00:     PD2 /PD0  PA1*/PA2  PC3*/PD1  PC4* *
/// 01:     PC6 /PC3  PC7*/PC4  PC0*/PD1  PD3  *
/// 10:     PC4 /PC3  PC7 /PD2  PC5 /PC6  PD4

uint8_t matrix_data[7][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

static uint8_t row = 0, group = 0;

struct row {
  GPIO_TypeDef *port;
  uint8_t pin;
};

static const struct row rows[] = {
  { GPIOD, 2 },
  { GPIOC, 7 },
  { GPIOC, 4 },
  { GPIOC, 3 },
  { GPIOC, 2 },
  { GPIOC, 1 },
  { GPIOC, 0 },
  { GPIOA, 1 },
};

static void next_row(void) {
  rows[row].port->CFGLR = (rows[row].port->CFGLR & ~(0xf<<(4*rows[row].pin)))
    | ROW_PWM_CNF<<(4*rows[row].pin);

  row = (row + 1) % ROWCOUNT;

  rows[row].port->CFGLR = (rows[row].port->CFGLR & ~(0xf<<(4*rows[row].pin)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*rows[row].pin);
}

static void next_group(void) {
  group++;
  if (group >= GROUPCOUNT) {
    group = 0;
    next_row();
  }

  switch (group) {
  }
}

void matrix_init(void) {
  RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
  RCC->APB2PRSTR &= ~(RCC_APB2Periph_TIM1);
  RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
  RCC->APB1PRSTR &= ~(RCC_APB1Periph_TIM2);

  TIM1->ATRLR = PWM_MAX;
  TIM2->ATRLR = PWM_MAX;

  TIM1->SWEVGR |= TIM_UG;
  TIM2->SWEVGR |= TIM_UG;

  rows[0].port->CFGLR |= ROW_PWM_CNF<<(4*rows[0].pin);
  GPIOC->CFGLR |= ROW_PWM_CNF<<(4*7) | ROW_PWM_CNF<<(4*4) | ROW_PWM_CNF<<(4*3)
    | ROW_PWM_CNF<<(4*2) | ROW_PWM_CNF<<(4*1) | ROW_PWM_CNF<<(4*0);
  rows[7].port->CFGLR |= ROW_PWM_CNF<<(4*rows[7].pin);

  rows[0].port->OUTDR |= 1<<rows[0].pin;
  GPIOC->OUTDR |= 0b10011111;
  rows[7].port->OUTDR |= 1<<rows[7].pin;
}

void matrix_update(void) {
}
