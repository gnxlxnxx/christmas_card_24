#include <ch32v003fun.h>
#include <rv003usb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include "usb.h"

#define WS2812DMA_IMPLEMENTATION
#define WSGRB // For SK6805-EC15
#define NR_LEDS 6

#define TOUCH_ITERATIONS 3
#define TOUCH_HIST_HIGH 60
#define TOUCH_HIST_LOW 50

volatile bool b1 = 0;
volatile bool b2 = 0;
volatile uint32_t b1a = 0;
volatile uint32_t b2a = 0;
volatile int b1counter = 0;
volatile int b2counter = 0;

uint16_t phases[NR_LEDS];
int frameno;
volatile int tween = -NR_LEDS;

#define WS2812DMA_IMPLEMENTATION
#include "ch32v003_touch_fix.h"
#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"

// TODO implement more "christmassy" effects
uint32_t WS2812BLEDCallback(int ledno) {
  uint8_t index = (phases[ledno]) >> 8;
  uint8_t rsbase = sintable[index];
  uint8_t rs = rsbase >> 3;
  uint32_t fire = ((huetable[(rs + 190) & 0xff] >> 3) << 16) |
                  (huetable[(rs + 30) & 0xff] >> 2) |
                  ((huetable[(rs + 0)] >> 3) << 8);
  if (ledno == 5) {
    if (b1counter) {
      fire |= b1counter << 16;
    }
  }
  if (ledno == 2) {
    if (b2counter) {
      fire |= b2counter << 16;
    }
  }

  return fire;
}

static void read_touches(void) {
  b1a = ReadTouchPin(GPIOC, 4, 2, TOUCH_ITERATIONS);
  b2a = ReadTouchPin(GPIOA, 2, 0, TOUCH_ITERATIONS);
  b1 = b1a > (b1 ? TOUCH_HIST_LOW : TOUCH_HIST_HIGH);
  b2 = b2a > (b2 ? TOUCH_HIST_LOW : TOUCH_HIST_HIGH);
}

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1;

  InitTouchADC();

  // Initialize rows
  GPIOC->CFGLR &= ~(0xffff << (4 * 0));
  GPIOC->CFGLR |= ((GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * 0)) |
                  ((GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * 1)) |
                  ((GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * 2)) |
                  ((GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * 3));

  // Initialize cols
  GPIOC->CFGLR &= ~(0xf << (4 * 5)) & ~(0xf << (4 * 7));
  GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 5) |
                  (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 7);
  GPIOA->CFGLR &= ~(0xf << (4 * 1));
  GPIOA->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 1);
  GPIOD->CFGLR &= ~(0xf << (4 * 2));
  GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 2);

  WS2812BDMAInit();
  GPIOC->CFGLR |= GPIO_CNF_OUT_OD_AF << (4 * 6); // Fix WS2812 levels, needs 10K/1K pullup across Din and Vdd
  usb_setup();
  timer_matrix_init();
  // Each collumn is hooked up to a timer PWM output, use a separate timer to
  // schedule changing the row
  frameno = 0;
  int k;
  for (k = 0; k < NR_LEDS; k++)
    phases[k] = k << 8;
  int tweendir = 0;
  /* matrix_data[1][1] = 128; */
  int ws2812counter = 0;
  read_touches();
  if (b1 || b2) {
    open_website();
  }
  while (1) {
    output_matrix();
    Delay_Us(300);
    read_touches();

    if (ws2812counter == 48) {
      if (!WS2812BLEDInUse) {
        if (b1) {
          b1counter = 256;
        }
        if (b2) {
          b2counter = 256;
        }
        if (b1counter) {
          b1counter -= 8;
        }
        if (b2counter) {
          b2counter -= 8;
        }
        ws2812counter = 0;
        frameno++;

        if (frameno == 1024) {
          tweendir = 1;
        }
        if (frameno == 2048) {
          tweendir = -1;
          frameno = 0;
        }

        if (tweendir) {
          int t = tween + tweendir;
          if (t > 255)
            t = 255;
          if (t < -NR_LEDS)
            t = -NR_LEDS;
          tween = t;
        }

        for (k = 0; k < NR_LEDS; k++) {
          phases[k] += ((((rands[k & 0xff]) + 0xf) << 2) +
                        (((rands[k & 0xff]) + 0xf) << 1)) >>
                       1;
        }

        WS2812BDMAStart(NR_LEDS);
      }
    } else {
      ws2812counter++;
    }
  }
}
