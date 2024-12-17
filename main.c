#include <ch32v003fun.h>
#include <rv003usb.h>
#include <stdbool.h>
#include "main.h"
#include "matrix.h"
#include "usb.h"
#include "ws2812.h"

volatile bool b1 = 0;
volatile bool b2 = 0;
volatile uint32_t b1a = 0;
volatile uint32_t b2a = 0;

#define NUM_MODES_F 1
int mode_f = 0;
bool lock_animations = false;

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO;

  ws2812_init();

  usb_setup();

  // touch buttons
  AFIO->EXTICR |= AFIO_EXTICR_EXTI2_PA | AFIO_EXTICR_EXTI5_PC;
  EXTI->FTENR |= EXTI_FTENR_TR2 | EXTI_FTENR_TR5;
  EXTI->INTENR |= EXTI_INTENR_MR2 | EXTI_INTENR_MR5;


  if (b1 && b2) {
    lock_animations = true;
  } else if (b1) {
    open_url();
  } else if (b2) {
    open_url_windows();
  }

  while (1) {

    // switch between modes on the front side
    switch (mode_f) {
    case 0:
      // random led pulse inverted
      for(int i=0; i<7; i++){
        matrix_data[i][i] = 1;
      }
      for(int i=1; i<8; i++){
        matrix_data[7-i][i] = 1;
      }
      break;

    }

    output_matrix();
    Delay_Us(300);

    ws2812_loop();
  }
}
