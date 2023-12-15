#include "matrix.h"
#include "ch32v003fun.h"

uint8_t matrix_data[4][4] = {{128, 10, 255, 128},
                             {128, 255, 64, 128},
                             {128, 10, 255, 128},
                             {128, 255, 64, 128}};
uint8_t col = 0;

void output_matrix() {
  col = (col + 1) % 4;
  change_col(col, matrix_data[col][0], matrix_data[col][1], 128,
             matrix_data[col][3]);
}

void change_col(uint8_t col, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4) {
  switch (col) {
  case 0:
    GPIOD->BSHR = (1 << (16 + 2));
    GPIOC->BSHR = (1 << (5)) | (1 << (7));
    GPIOA->BSHR = (1 << (1));
    break;
  case 1:
    GPIOA->BSHR = (1 << (16 + 1));
    GPIOC->BSHR = (1 << (5)) | (1 << (7));
    GPIOD->BSHR = (1 << (2));
    break;
  case 2:
    GPIOC->BSHR = (1 << (5)) | (1 << (16 + 7));
    GPIOA->BSHR = (1 << (1));
    GPIOD->BSHR = (1 << (2));
    break;
  case 3:
    GPIOC->BSHR = (1 << (16 + 5)) | (1 << (7));
    GPIOA->BSHR = (1 << (1));
    GPIOD->BSHR = (1 << (2));
    break;
  }
  TIM1->CH3CVR = r1;
  TIM2->CH4CVR = r2;
  TIM2->CH2CVR = r3;
  TIM1->CH1CVR = r4;
  // First disable all timer outputs and stop it (i.e. disable reload)
  // Now change row
  // load new values
  // Start again in the same order as before
}

// Timer 2 has TIM2_RM=01, PC2 CH2, PC1 CH4
// Timer 1 has TIM1_RM=01, PC0 CH3, PC3 CH1N

// There is no reason to run at anything but the highest rate....
// LETS GO!!!

void timer_matrix_init(void) {
  // Enable peripherals
  RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD |
                    RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1;
  RCC->APB1PCENR |= RCC_APB1Periph_TIM2;
  AFIO->PCFR1 =
      AFIO_PCFR1_TIM1_REMAP_PARTIALREMAP | AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP1;

  // CTLR1: default is up, events generated, edge align
  // SMCFGR: default clk input is CK_INT

  // Prescaler
  TIM1->PSC = 0x0000;

  // Auto Reload - sets period
  TIM1->ATRLR = 255;

  // CH1 Mode is output, PWM1 (CC1S = 00, OC1M = 110)
  TIM1->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1 | TIM_OC1PE;

  TIM1->CHCTLR2 |= TIM_OC3M_2 | TIM_OC3M_1 | TIM_OC3PE;

  TIM1->CTLR1 |= TIM_ARPE;

  // Enable CH1N output, positive pol
  TIM1->CCER |= TIM_CC1NE | TIM_CC1NP;
  // Enable CH3 output, positive pol
  TIM1->CCER |= TIM_CC3E;

  // Set the Capture Compare Register value to 50% initially
  TIM1->CH1CVR = 128;
  TIM1->CH3CVR = 128;

  // Reload immediately
  TIM1->SWEVGR |= TIM_UG;

  // Enable TIM1 outputs
  TIM1->BDTR |= TIM_MOE;

  // Enable TIM1
  TIM1->CTLR1 |= TIM_CEN;

  // SMCFGR: default clk input is CK_INT
  // set TIM2 clock prescaler divider
  TIM2->PSC = 0x0000;
  // set PWM total cycle width
  TIM2->ATRLR = 255;

  // for channel 1 and 2, let CCxS stay 00 (output), set OCxM to 110 (PWM I)
  // enabling preload causes the new pulse width in compare capture register
  // only to come into effect when UG bit in SWEVGR is set (= initiate update)
  // (auto-clears)
  TIM2->CHCTLR1 |= TIM_OC2M_2 | TIM_OC2M_1 | TIM_OC2PE;
  TIM2->CHCTLR2 |= TIM_OC4M_2 | TIM_OC4M_1 | TIM_OC4PE;
  // CTLR1: default is up, events generated, edge align
  // enable auto-reload of preload
  TIM2->CTLR1 |= TIM_ARPE;

  // Enable Channel outputs, set default state (based on TIM2_DEFAULT)
  TIM2->CCER |= TIM_CC2E;
  TIM2->CCER |= TIM_CC4E;
  // initialize counter
  TIM2->SWEVGR |= TIM_UG;

  // Set the Capture Compare Register value to 50% initially
  TIM2->CH2CVR = 128;
  TIM2->CH4CVR = 128;

  // Enable TIM2
  TIM2->CTLR1 |= TIM_CEN;
}
