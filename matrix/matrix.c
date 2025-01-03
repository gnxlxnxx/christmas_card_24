#include <ch32v003fun.h>
#include <stdint.h>
#include "matrix.h"

#include "gamma_lut.h"

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
/// 11:     PC4 /PC3  PC7 /PD2  PC5 /PC6  PD4


#define PWM_MAX 4096

#define ROWCOUNT (MATRIX_WIDTH)
#define GROUPCOUNT 2

#define G0T0C2_COL 1
#define G0T0C3_COL 6
#define G0T1C2_COL 4
#define G0T1C3_COL 0
#define G0T1C4_COL 5
#define G1T0C2_COL 7
#define G1T0C3_COL 3
#define G1T0C4_COL 2

uint8_t col = 0;
uint8_t matrix_data[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

static uint8_t row = 0, group = 0;

static const struct row {
  GPIO_TypeDef *port;
  uint8_t pin;
  uint8_t group;
} rows[ROWCOUNT] = {
  { GPIOD, 2, 0 },
  { GPIOC, 7, 0 },
  { GPIOC, 4, 1 },
  { GPIOC, 3, 1 },
  { GPIOC, 2, 0 },
  { GPIOC, 1, 0 },
  { GPIOC, 0, 0 },
  { GPIOA, 1, 1 },
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

static uint16_t get_val(uint8_t row, uint8_t col) {
  return gamma_lut[matrix_data[(row <= col) ? row : row - 1][col]];
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

  TIM1->CTLR1 |= TIM_ARPE;
  TIM2->CTLR1 |= TIM_ARPE;

  TIM1->CTLR1 |= TIM_CEN;
  TIM2->CTLR1 |= TIM_CEN;

  TIM1->CHCTLR1 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM1->CHCTLR2 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM2->CHCTLR1 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;
  TIM2->CHCTLR2 |= TIM_OCMode_PWM1 | TIM_OCMode_PWM1<<8;

  rows[0].port->OUTDR |= 1<<rows[0].pin;
  GPIOC->OUTDR |= 0b10011111;
  rows[7].port->OUTDR |= 1<<rows[7].pin;
}

void matrix_update(void) {
  float_all();

  TIM1->BDTR &= ~TIM_MOE;
  TIM2->BDTR &= ~TIM_MOE;

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

      TIM1->CH2CVR = get_val(row, G0T0C2_COL);
      TIM1->CH3CVR = get_val(row, G0T0C3_COL);
      TIM2->CH2CVR = get_val(row, G0T1C2_COL);
      TIM2->CH3CVR = get_val(row, G0T1C3_COL);
      TIM2->CH4CVR = get_val(row, G0T1C4_COL);

      TIM1->BDTR |= TIM_MOE;
      TIM2->BDTR |= TIM_MOE;
      break;
    case 1:
      AFIO->PCFR1 = (AFIO->PCFR1 & ~AFIO_PCFR1_TIM1_REMAP) | AFIO_PCFR1_TIM1_REMAP_NOREMAP;
      TIM1->CCER = TIM_CC2E | TIM_CC2P | TIM_CC3E | TIM_CC3P | TIM_CC4E | TIM_CC4P;

      attach_group(group, row);

      TIM1->CH2CVR = get_val(row, G1T0C2_COL);
      TIM1->CH3CVR = get_val(row, G1T0C3_COL);
      TIM1->CH4CVR = get_val(row, G1T0C4_COL);

      TIM1->BDTR |= TIM_MOE;
      break;
  }

  set_high(row);
}
