#include <ch32v003fun.h>
#include <stdbool.h>
#include "main.h"
#include "matrix/matrix.h"
#include "matrix/animations.h"
#include "usb.h"
#include "ws2812.h"
#include "touch.h"

static bool lock_animations = false;

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO |
                    RCC_APB2Periph_TIM1;
  RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

  ws2812_init();
  usb_init();
  matrix_init();

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

    matrix_update();
    matrix_animation_update();
    ws2812_update();

    touch_update();
    if (!lock_animations) {
      if (btn_left && btn_left_toggled) {
        ws2812_next_mode();
      }
      if (btn_right && btn_right_toggled) {
        matrix_next_mode();
      }
    }

    while (SysTick->CNT - start < Ticks_from_Us(256)) {}
  }
}
