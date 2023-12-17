#include "ch32v003fun.h"
#include "rv003usb.h"
#include <matrix.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION
#define WSGRB // For SK6805-EC15
#define NR_LEDS 6

volatile bool b1 = 0;
volatile bool b2 = 0;
volatile int b1counter = 0;
volatile int b2counter = 0;

#define NUM_MODES_F 2
#define NUM_MODES_B 2
int mode_f = 0;
int mode_b = 0;

uint16_t phases[NR_LEDS];
int frameno;
volatile int tween = -NR_LEDS;

#define WS2812DMA_IMPLEMENTATION
#include "ch32v003_touch_fix.h"
#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"


/* White Noise Generator State */
#define NOISE_BITS 8
#define NOISE_MASK ((1<<NOISE_BITS)-1)
#define NOISE_POLY_TAP0 31
#define NOISE_POLY_TAP1 21
#define NOISE_POLY_TAP2 1
#define NOISE_POLY_TAP3 0
uint32_t lfsr = 1;

uint8_t rand8(void)
{
  uint8_t bit;
  uint32_t new_data;

  for(bit=0;bit<NOISE_BITS;bit++)
  {
    new_data = ((lfsr>>NOISE_POLY_TAP0) ^
          (lfsr>>NOISE_POLY_TAP1) ^
          (lfsr>>NOISE_POLY_TAP2) ^
          (lfsr>>NOISE_POLY_TAP3));
    lfsr = (lfsr<<1) | (new_data&1);
  }

  return lfsr&NOISE_MASK;
}

int ws2812_counter = 0;
uint8_t snow_balls[6] = {0};

// TODO implement more "christmassy" effects
uint32_t WS2812BLEDCallback(int ledno) {
  uint32_t value = 0;

  // switch between modes on the back side
  switch(mode_b){
    case 0:
      // Original "Fire" mode
      uint8_t index = (phases[ledno]) >> 8;
      uint8_t rsbase = sintable[index];
      uint8_t rs = rsbase >> 3;
      value = ((huetable[(rs + 190) & 0xff] >> 3) << 16) |
        (huetable[(rs + 30) & 0xff] >> 2) |
        ((huetable[(rs + 0)] >> 3) << 8);
      break;
    case 1:
      // "Snowball" mode
      if(ws2812_counter++ < 100){
        if(snow_balls[ledno])
          value = 0x7F7F7F;
        else
          value = 0;
      } else {
        int num_snowballs_l = 0;
        for(int i = 4; i<=6; i++)
          num_snowballs_l += snow_balls[i%6];
        int num_snowballs_r = 0;
        for(int i = 1; i<=3; i++)
          num_snowballs_r += snow_balls[i%6];

        snow_balls[3] = snow_balls[2];
        snow_balls[2] = snow_balls[1];
        snow_balls[1] = rand8()%(3+num_snowballs_r)?0:1;

        snow_balls[4] = snow_balls[5];
        snow_balls[5] = snow_balls[0];
        snow_balls[0] = rand8()%(3+num_snowballs_l)?0:1;

        ws2812_counter = 0;
      }
      break;
  }

  if(b1counter){
    switch(ledno){
      case 4:
      case 0:
        value |= (b1counter >> 3) << 16;
        break;
      case 5:
        value |= b1counter << 16;
        break;
    }
  }

  if(b2counter){
    switch(ledno){
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

  int ledcounter = 0;

  int num_x = 0;
  int num_y = 0;
  int num_ang = 0;

  uint8_t future_matrix[4][4] = {};



  while (1) {

    // switch between modes on the front side
    switch(mode_f){
      case 0:
        // random led pulse inverted
        if (ledcounter++ >= 40){
          if(num_x++%30 == 0)
            future_matrix[rand8() & 0b11][(rand8()>>2) & 0b11] = 0;
          num_x %= 250;

          for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
              if(future_matrix[i][j] < matrix_data[i][j]){
                matrix_data[i][j]-=1;
              } else if(future_matrix[i][j] > matrix_data[i][j]){
                matrix_data[i][j]+=1;
              }
              if(future_matrix[i][j] == matrix_data[i][j]){
                future_matrix[i][j] = 255;
              }

            }
          }
          ledcounter = 0;
        }

        break;


      case 1:
        // random led pulse
        if (ledcounter++ >= 40){
          if(num_x++%30 == 0)
            future_matrix[rand8() & 0b11][(rand8()>>2) & 0b11] = 255;
          num_x %= 250;

          for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
              if(future_matrix[i][j] < matrix_data[i][j]){
                matrix_data[i][j]-=1;
              } else if(future_matrix[i][j] > matrix_data[i][j]){
                matrix_data[i][j]+=1;
              }
              if(future_matrix[i][j] == matrix_data[i][j]){
                future_matrix[i][j] = 0;
              }

            }
          }
          ledcounter = 0;
        }

        break;
    }

    output_matrix();
    Delay_Us(300);
    int iterations = 3;
    b1 = ReadTouchPin(GPIOC, 4, 2, iterations) > 50;
    b2 = ReadTouchPin(GPIOA, 2, 0, iterations) > 50;

    if (ws2812counter == 48) {
      if (!WS2812BLEDInUse) {
        if (b1) {
          if(!b1counter)
            mode_f = ++mode_f % NUM_MODES_F;
          b1counter = 128;
        }
        if (b2) {
          if(!b2counter)
            mode_b = ++mode_b % NUM_MODES_B;
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
    tsajoystick[3] = b1 ? 0x4f : 0;
    tsajoystick[4] = b2 ? 0x50 : 0;
    /* i++; */

    /* // Press the 'b' button every second or so. */
    /* if ((i & 0x7f) == 0) { */
    /*   tsajoystick[4] = 0; // was 5 */
    /* } else { */
    /*   tsajoystick[4] = 0; */
    /* } */
  } else {
    // If it's a control transfer, empty it.
    usb_send_empty(sendtok);
  }
}
