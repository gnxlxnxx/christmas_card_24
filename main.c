#include "ch32v003fun.h"
#include "rv003usb.h"
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION
// #define WSRBG //For WS2816C's.
#define WSGRB // For SK6805-EC15
#define NR_LEDS 6

#include "ws2812b_dma_spi_led_driver.h"

#include "color_utilities.h"

// Callbacks that you must implement.
uint32_t WS2812BLEDCallback(int ledno) {
  uint8_t index = (phases[ledno]) >> 8;
  uint8_t rsbase = sintable[index];
  uint8_t rs = rsbase >> 3;
  uint32_t fire = ((huetable[(rs + 190) & 0xff] >> 1) << 16) |
                  (huetable[(rs + 30) & 0xff]) |
                  ((huetable[(rs + 0)] >> 1) << 8);

  return fire
}

int main() {
  SystemInit();
  usb_setup();
  WS2812DMAInit();
  frameno = 0;
  for (k = 0; k < NR_LEDS; k++)
    phases[k] = k << 8;
  int tweendir = 0;
  while (1) {
#if RV003USB_EVENT_DEBUGGING
    uint32_t *ue = GetUEvent();
    if (ue) {
      printf("%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3]);
    }
#endif
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

    // Move the mouse right, down, left and up in a square.
    switch (mode & 3) {
    case 0:
      tsajoystick[1] = 1;
      tsajoystick[2] = 0;
      break;
    case 1:
      tsajoystick[1] = 0;
      tsajoystick[2] = 1;
      break;
    case 2:
      tsajoystick[1] = -1;
      tsajoystick[2] = 0;
      break;
    case 3:
      tsajoystick[1] = 0;
      tsajoystick[2] = -1;
      break;
    }
    usb_send_data(tsajoystick, 4, 0, sendtok);
  } else if (endp == 2) {
    // Keyboard (8 bytes)
    static int i;
    static uint8_t tsajoystick[8] = {0x00};
    usb_send_data(tsajoystick, 8, 0, sendtok);

    i++;

    // Press the 'b' button every second or so.
    if ((i & 0x7f) == 0) {
      tsajoystick[4] = 5;
    } else {
      tsajoystick[4] = 0;
    }
  } else {
    // If it's a control transfer, empty it.
    usb_send_empty(sendtok);
  }
}
