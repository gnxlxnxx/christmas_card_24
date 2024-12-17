#include <ch32v003fun.h>
#include <stdint.h>
#include "touch.h"
#include "random.h"
#include "touch.h"

#define WS2812DMA_IMPLEMENTATION
#define WSGRB // For SK6805-EC15
#define NR_LEDS 5

static uint16_t phases[NR_LEDS];
static int frameno = 0;
volatile static int tween = -NR_LEDS;

#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"

#define NUM_MODES_B 3
static int mode_b = 0;

static int ws2812_counter = 0;
static uint32_t snow_balls[6] = {0}; // TODO: Hardcoded LED count
static uint32_t desired_output[NR_LEDS] = {0};
static uint32_t output[NR_LEDS] = {0};

static volatile int b1counter = 0;
static volatile int b2counter = 0;

static const uint8_t led_angles[] = {
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

  for (int i = 0; i < 3; i++) {
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

void ws2812_init(void) {
  WS2812BDMAInit();

  // Fix WS2812 levels, needs 10K/1K pullup across Din and Vdd
  GPIOC->CFGLR |= GPIO_CNF_OUT_OD_AF << (4 * 6);

  for (int k = 0; k < NR_LEDS; k++)
    phases[k] = k << 8;
}

void ws2812_update(void) {
  static int tweendir = 0;
  static int ws2812counter = 0;

  if (ws2812counter == 48) {
    if (!WS2812BLEDInUse) {
      if (but_left) {
        b1counter = 128;
      }
      if (but_right) {
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

      for (int k = 0; k < NR_LEDS; k++) {
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
