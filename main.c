#include <ch32v003fun.h>
#include <stdbool.h>
#include "main.h"
#include "matrix.h"
#include "usb.h"
#include "ws2812.h"
#include "touch.h"

#define NUM_MODES_F 1
int mode_f = 0;
static bool lock_animations = false;

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO;

  ws2812_init();
  usb_init();

  touch_init();
  if (btn_left && btn_right) {
    lock_animations = true;
  } else if (btn_left) {
    open_url_windows();
  } else if (btn_right) {
    open_url();
  }

  while (true) {
    uint32_t start = SysTick->CNT;

    output_matrix();
    ws2812_update();

    // switch between modes on the front side
    /*switch (mode_f) {*/
    /*case 0:*/
    /*  // random led pulse inverted*/
    /*  for(int i=0; i<7; i++){*/
    /*    matrix_data[i][i] = 1;*/
    /*  }*/
    /*  for(int i=1; i<8; i++){*/
    /*    matrix_data[7-i][i] = 1;*/
    /*  }*/
    /*  break;*/
    /**/
    /*}*/

    touch_update();

    if (!lock_animations) {
      if (btn_left && btn_left_toggled) {
        ws2812_next_mode();
      }
    }

    while (SysTick->CNT - start < Ticks_from_Us(300)) {}
  }
}
