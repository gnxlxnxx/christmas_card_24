#include "ch32v003fun.h"
#include "rv003usb.h"
#include <matrix.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION
// #define WSRBG //For WS2816C's.
#define WSGRB // For SK6805-EC15
#define NR_LEDS 6

volatile bool b1 = 0;
volatile bool b2 = 0;

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
  uint32_t fire = ((huetable[(rs + 190) & 0xff] >> 1) << 16) |
                  (huetable[(rs + 30) & 0xff]) |
                  ((huetable[(rs + 0)] >> 1) << 8);
  if (ledno == 0) {
    if (b1) {
      return 0x0000ff;
    } else {
      return 0x000000;
    }
  } else if (ledno == 1)
    if (b2) {
      return 0x00ff00;
    } else {
      return 0x0000ff;
    }
  else if (ledno == 2)
    return 0xff0000;

  return fire;
}

int main() {
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
  timer_matrix_init();
  GPIOA->CFGLR &= ~(0xf << (4 * 1));
  GPIOA->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 1);
  GPIOD->CFGLR &= ~(0xf << (4 * 2));
  GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4 * 2);
  // Turn off cols
  GPIOC->BSHR = (1 << (5)) | (1 << (7));
  GPIOA->BSHR = (1 << (1));
  GPIOD->BSHR = (1 << (2));

  WS2812BDMAInit();
  usb_setup();
  // TODO implement matrix output
  // Each collumn is hooked up to a timer PWM output, use a separate timer to
  // schedule changing the row
  frameno = 0;
  int k;
  for (k = 0; k < NR_LEDS; k++)
    phases[k] = k << 8;
  int tweendir = 0;
  while (1) {
    change_col(0, 128, 10, 255, 0);
    Delay_Ms(3);
    change_col(1, 128, 255, 64, 0);
    Delay_Ms(3);
    int iterations = 3;
    b1 = ReadTouchPin(GPIOA, 2, 0, iterations);
    b2 = ReadTouchPin(GPIOC, 4, 2, iterations);
    change_col(2, 128, 10, 255, 0);
    Delay_Ms(3);
    change_col(3, 128, 255, 64, 0);
    Delay_Ms(3);

    while (WS2812BLEDInUse)
      ;

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
}

void usb_handle_user_in_request(struct usb_endpoint *e, uint8_t *scratchpad,
                                int endp, uint32_t sendtok,
                                struct rv003usb_internal *ist) {
  if (endp == 1) {
    // Mouse (4 bytes)
    static int i;
    static uint8_t tsajoystick[4] = {0x00, 0x00, 0x00, 0x00};
    i++;
    int mode = i >> 5;

    tsajoystick[1] = 0;
    tsajoystick[2] = 0;
    // Move the mouse right, down, left and up in a square.
    /* switch (mode & 3) { */
    /* case 0: */
    /*   tsajoystick[1] = 1; */
    /*   tsajoystick[2] = 0; */
    /*   break; */
    /* case 1: */
    /*   tsajoystick[1] = 0; */
    /*   tsajoystick[2] = 1; */
    /*   break; */
    /* case 2: */
    /*   tsajoystick[1] = -1; */
    /*   tsajoystick[2] = 0; */
    /*   break; */
    /* case 3: */
    /*   tsajoystick[1] = 0; */
    /*   tsajoystick[2] = -1; */
    /*   break; */
    /* } */
    usb_send_data(tsajoystick, 4, 0, sendtok);
  } else if (endp == 2) {
    // Keyboard (8 bytes)
    static int i;
    static uint8_t tsajoystick[8] = {0x00};
    usb_send_data(tsajoystick, 8, 0, sendtok);

    i++;

    // Press the 'b' button every second or so.
    if ((i & 0x7f) == 0) {
      tsajoystick[4] = 0; // was 5
    } else {
      tsajoystick[4] = 0;
    }
  } else {
    // If it's a control transfer, empty it.
    usb_send_empty(sendtok);
  }
}
