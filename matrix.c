#include <ch32v003fun.h>
#include <stdint.h>
#include "matrix.h"

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 7

#define ROWCOUNT (MATRIX_WIDTH)
#define GROUPCOUNT 2

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
  {0, 255, 0, 255, 0, 255, 0, 255},
  {255, 0, 255, 0, 255, 0, 255, 0},
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
  uint8_t group;
  uint8_t timer;
  uint8_t channel;
};

static const struct row rows[] = {
  { GPIOD, 2, 0, 1, 2 },
  { GPIOC, 7, 0, 0, 1 },
  { GPIOC, 4, 1, 0, 3 },
  { GPIOC, 3, 1, 0, 2 },
  { GPIOC, 2, 0, 1, 1 },
  { GPIOC, 1, 0, 1, 3 },
  { GPIOC, 0, 0, 0, 2 },
  { GPIOA, 1, 1, 0, 1 },
};

static void float_all(void) {
  for (int i = 0; i < ROWCOUNT; i++) {
    rows[i].port->CFGLR &= ~(0xf<<(4*rows[i].pin));
  }
}

static void attach_group(uint8_t group, uint8_t row) {
  for (int i = 0; i < ROWCOUNT; i++) {
    if (i != row && rows[i].group == group) {
      rows[i].port->CFGLR = (rows[i].port->CFGLR & ~(0xf<<(4*rows[i].pin)))
        | (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF)<<(4*rows[i].pin);
    }
  }
}

static void set_high(uint8_t row) {
  rows[row].port->CFGLR = (rows[row].port->CFGLR & ~(0xf<<(4*rows[row].pin)))
    | (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*rows[row].pin);
}

static void next_group(void) {
  float_all();

  TIM1->BDTR &= ~TIM_MOE;
  TIM2->BDTR &= ~TIM_MOE;
  TIM1->CTLR1 &= ~TIM_CEN;
  TIM2->CTLR1 &= ~TIM_CEN;
  TIM1->CNT = 0;
  TIM2->CNT = 0;

  group++;
  if (group >= GROUPCOUNT) {
    group = 0;
    row = (row + 1) % ROWCOUNT;
  }

  switch (group) {
    case 0:
      AFIO->PCFR1 = (AFIO->PCFR1 & ~(AFIO_PCFR1_TIM1_REMAP | AFIO_PCFR1_TIM2_REMAP))
        | AFIO_PCFR1_TIM1_REMAP_PARTIALREMAP | AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP1;
      TIM1->CCER = TIM_CC2E | TIM_CC2P | TIM_CC3E | TIM_CC3P;
      TIM2->CCER = TIM_CC2E | TIM_CC2P | TIM_CC3E | TIM_CC3P | TIM_CC4E | TIM_CC4P;

      attach_group(group, row);

      // TODO: Load values
      TIM1->CH2CVR = 12;
      TIM1->CH3CVR = 12;
      TIM2->CH2CVR = 12;
      TIM2->CH3CVR = 12;
      TIM2->CH4CVR = 12;

      TIM1->BDTR |= TIM_MOE;
      TIM2->BDTR |= TIM_MOE;
      TIM1->CTLR1 |= TIM_CEN;
      TIM2->CTLR1 |= TIM_CEN;
      break;
    case 1:
      AFIO->PCFR1 = (AFIO->PCFR1 & ~AFIO_PCFR1_TIM1_REMAP) | AFIO_PCFR1_TIM1_REMAP_NOREMAP;
      TIM1->CCER = TIM_CC2E | TIM_CC2P | TIM_CC3E | TIM_CC3P | TIM_CC4E | TIM_CC4P;

      attach_group(group, row);

      // TODO: Load values
      TIM1->CH2CVR = 128;
      TIM1->CH3CVR = 128;
      TIM1->CH4CVR = 128;

      TIM1->BDTR |= TIM_MOE;
      TIM1->CTLR1 |= TIM_CEN;
      break;
  }

  set_high(row);
}

void matrix_init(void) {
  RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
  RCC->APB2PRSTR &= ~(RCC_APB2Periph_TIM1);
  RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
  RCC->APB1PRSTR &= ~(RCC_APB1Periph_TIM2);

  TIM1->PSC = 0;
  TIM2->PSC = 0;

  TIM1->ATRLR = PWM_MAX;
  TIM2->ATRLR = PWM_MAX;

  TIM1->SWEVGR |= TIM_UG;
  TIM2->SWEVGR |= TIM_UG;

  TIM1->CTLR1 |= TIM_ARPE;
  TIM2->CTLR1 |= TIM_ARPE;

  TIM1->CHCTLR1 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM1->CHCTLR2 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM2->CHCTLR1 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM2->CHCTLR2 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;

  rows[0].port->OUTDR |= 1<<rows[0].pin;
  GPIOC->OUTDR |= 0b10011111;
  rows[7].port->OUTDR |= 1<<rows[7].pin;
}

void matrix_update(void) {
  next_group();
}
