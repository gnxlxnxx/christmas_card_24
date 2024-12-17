#include <ch32v003fun.h>
#include <stdbool.h>
#include "main.h"
#include "matrix.h"
#include "usb.h"
#include "ws2812.h"
#include "touch.h"

#define NUM_MODES_F 1
int mode_f = 0;
bool lock_animations = false;

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO;

  ws2812_init();
  usb_init();

  if (but_left && but_right) {
    lock_animations = true;
  } else if (but_left) {
    open_url();
  } else if (but_right) {
    open_url_windows();
  }

  while (1) {
    touch_update();

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

    output_matrix();
    Delay_Us(300);

    ws2812_update();
  }
}
