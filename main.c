#include "main.h"
#include "matrix.h"
#include "usb.h"
#include <ch32v003fun.h>
#include <rv003usb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION
#define WSGRB // For SK6805-EC15
#define NR_LEDS 5

volatile bool b1 = 0;
volatile bool b2 = 0;
volatile uint32_t b1a = 0;
volatile uint32_t b2a = 0;
volatile int b1counter = 0;
volatile int b2counter = 0;

#define NUM_MODES_F 1
#define NUM_MODES_B 3
int mode_f = 0;
int mode_b = 0;
bool lock_animations = false;

uint16_t phases[NR_LEDS];
int frameno;
volatile int tween = -NR_LEDS;

#define WS2812DMA_IMPLEMENTATION
#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"

/* White Noise Generator State */
#define NOISE_BITS 8
#define NOISE_MASK ((1 << NOISE_BITS) - 1)
#define NOISE_POLY_TAP0 31
#define NOISE_POLY_TAP1 21
#define NOISE_POLY_TAP2 1
#define NOISE_POLY_TAP3 0
uint32_t lfsr = 1;

uint8_t rand8(void) {
  uint8_t bit;
  uint32_t new_data;

  for (bit = 0; bit < NOISE_BITS; bit++) {
    new_data = ((lfsr >> NOISE_POLY_TAP0) ^ (lfsr >> NOISE_POLY_TAP1) ^
                (lfsr >> NOISE_POLY_TAP2) ^ (lfsr >> NOISE_POLY_TAP3));
    lfsr = (lfsr << 1) | (new_data & 1);
  }

  return lfsr & NOISE_MASK;
}

int ws2812_counter = 0;
uint32_t snow_balls[6] = {0}; // TODO: Hardcoded LED count
uint32_t desired_output[NR_LEDS] = {0};
uint32_t output[NR_LEDS] = {0};
uint8_t i = 0;

const uint8_t led_angles[] = {
    0, 51, 102, 154, 204};

uint32_t WS2812BLEDCallback(int ledno) {

  // switch between modes on the back side
  switch (mode_b) {
  case 0:
    // Original "Fire" mode
    uint8_t index = (phases[ledno]) >> 8;
    uint8_t rsbase = sintable[index];
    uint8_t rs = rsbase >> 3;
    desired_output[ledno] = ((huetable[(rs + 190) & 0xff] >> 3) << 16) |
                            (huetable[(rs + 30) & 0xff] >> 2) |
                            ((huetable[(rs + 0)] >> 3) << 8);
    break;
  case 1: // TODO: Hardcoded LED count
    // "Snowball" mode
    if (ws2812_counter++ < 300) {
      desired_output[ledno] = snow_balls[ledno];
    } else {
      int num_snowballs_l = 0;
      for (int i = 4; i <= 6; i++)
        num_snowballs_l += !!snow_balls[i % 6];
      int num_snowballs_r = 0;
      for (int i = 1; i <= 3; i++)
        num_snowballs_r += !!snow_balls[i % 6];
      uint8_t hue = rand8();
      snow_balls[3] = snow_balls[2];
      snow_balls[2] = snow_balls[1];
      snow_balls[1] = rand8() % (3 + num_snowballs_r)
                          ? 0
                          : (huetable[(hue + 170) & 0xff] << 16) |
                                huetable[(hue + 85) & 0xff] |
                                (huetable[(hue + 0)] << 8);

      hue = rand8();
      snow_balls[4] = snow_balls[5];
      snow_balls[5] = snow_balls[0];
      snow_balls[0] = rand8() % (3 + num_snowballs_l)
                          ? 0
                          : (huetable[(hue + 170) & 0xff] << 16) |
                                huetable[(hue + 85) & 0xff] |
                                (huetable[(hue + 0)] << 8);

      ws2812_counter = 0;
    }
    break;
  case 2:
    // "Huewheel" mode
    uint8_t ang = (ws2812_counter >> 3) + led_angles[ledno];
    desired_output[ledno] = huetable[(ang + 170) & 0xff] << 16 |
                            huetable[(ang + 85) & 0xff] |
                            huetable[ang & 0xff] << 8;
    if (ledno == NR_LEDS - 1) {
      ws2812_counter = (ws2812_counter + 1) & 0x7ff;
    }
    break;
  }

  for (i = 0; i < 3; i++) {
    if (((output[ledno] >> (i << 3)) & 0xFF) >
        ((desired_output[ledno] >> (i << 3)) & 0xFF)) {
      output[ledno] -= 1 << (i << 3);
    } else if (((output[ledno] >> (i << 3)) & 0xFF) <
               ((desired_output[ledno] >> (i << 3)) & 0xFF)) {
      output[ledno] += 1 << (i << 3);
    }
  }

  uint32_t value = output[ledno];
  if (b1counter) {
    switch (ledno) {
    case 4:
    case 0:
      value |= (b1counter >> 3) << 16;
      break;
    case 5:
      value |= b1counter << 16;
      break;
    }
  }

  if (b2counter) {
    switch (ledno) {
    case 1:
    case 3:
      value |= (b2counter >> 3) << 16;
      break;
    case 2:
      value |= b2counter << 16;
      break;
    }
  }

  return value;
}

static void read_touches(void) {
  b1a = 0;
  b2a = 0;
  b1 = 0;
  b2 = 0;
}

int main(void) {
  SystemInit();
  RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC |
                    RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1;

  WS2812BDMAInit();
  GPIOC->CFGLR |=
      GPIO_CNF_OUT_OD_AF
      << (4 * 6); // Fix WS2812 levels, needs 10K/1K pullup across Din and Vdd
  usb_setup();
  frameno = 0;
  int k;
  for (k = 0; k < NR_LEDS; k++)
    phases[k] = k << 8;
  int tweendir = 0;
  int ws2812counter = 0;

  /* read_touches(); */
  if (b1 && b2) {
    lock_animations = true;
  } else if (b1) {
    open_url();
  } else if (b2) {
    open_url_windows();
  }

  int ledcounter = 0, ledcounter2 = 0;

  int num_x = 0;

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
    read_touches();

    if (ws2812counter == 48+255) {
      if (!WS2812BLEDInUse) {
        if (b1) {
          if (!b1counter && !lock_animations)
            mode_f = (mode_f + 1) % NUM_MODES_F;
          b1counter = 128;
        }
        if (b2) {
          if (!b2counter && !lock_animations)
            mode_b = (mode_b + 1) % NUM_MODES_B;
          b2counter = 128;
        }
        if (b1counter) {
          b1counter -= 4;
        }
        if (b2counter) {
          b2counter -= 4;
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
