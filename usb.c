#include <stdint.h>
#include <tinyusb_hid.h>
#include <stddef.h>
#include <ch32v003fun.h>
#include <rv003usb.h>
#include "usb.h"
#include "main.h"

struct kc_str_element {
  uint8_t modifier;
  uint8_t keycode;
};

volatile enum usb_kbd_state {
  KBD_STATE_BTN = 0,
  KBD_STATE_TYPE,
  KBD_STATE_WAIT,
  KBD_STATE_BROWSER,
  KBD_STATE_START
} usb_kbd_state = KBD_STATE_BTN;

volatile uint32_t kbd_start_time;

static const struct kc_str_element url_kc_str[] = {
  {KEYBOARD_MODIFIER_LEFTCTRL, HID_KEY_L}, {0, HID_KEY_H}, {0, HID_KEY_T}, {0, HID_KEY_T},
  {0, HID_KEY_P}, {0, HID_KEY_S}, {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_PERIOD},
  {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_7}, {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_7},
  {0, HID_KEY_F}, {0, HID_KEY_S}, {0, HID_KEY_SLASH}, {0, HID_KEY_E}, {0, HID_KEY_I},
  {0, HID_KEY_PERIOD}, {0, HID_KEY_D}, {0, HID_KEY_E},
  {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_7}, {0, HID_KEY_D}, {0, HID_KEY_E},
  {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_7}, {0, HID_KEY_X}, {0, HID_KEY_M}, {0, HID_KEY_A},
  {0, HID_KEY_S}, {KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_SLASH}, {0, HID_KEY_C},
  {0, HID_KEY_A}, {0, HID_KEY_R}, {0, HID_KEY_D}, {0, HID_KEY_ENTER}, {-1, -1}
}; // DE layout

void open_website(void) {
  kbd_start_time = SysTick->CNT;
  usb_kbd_state = KBD_STATE_START;
}

void usb_handle_user_in_request(struct usb_endpoint *e, uint8_t *scratchpad,
                                int endp, uint32_t sendtok,
                                struct rv003usb_internal *ist) {
  if (endp == 1) {
    // Keyboard (8 bytes)
    static int cnt = 0;
    static const struct kc_str_element *cur_kc_str = NULL;
    int i = 0;
    hid_keyboard_report_t report = {};

    switch (usb_kbd_state) {
      case KBD_STATE_START:
        if (SysTick->CNT - kbd_start_time >= Ticks_from_Ms(2000)) {
          cnt = 0;
          usb_kbd_state = KBD_STATE_BROWSER;
        }
        break;
      case KBD_STATE_BROWSER:
        if (cnt++ <= 1) {
          report.keycode[0] = 0xf0;
        } else {
          kbd_start_time = SysTick->CNT;
          usb_kbd_state = KBD_STATE_WAIT;
        }
        break;
      case KBD_STATE_WAIT:
        if (SysTick->CNT - kbd_start_time >= Ticks_from_Ms(3000)) {
          cnt = 0;
          cur_kc_str = url_kc_str;
          usb_kbd_state = KBD_STATE_TYPE;
        }
        break;
      case KBD_STATE_TYPE:
        if (cur_kc_str->keycode != (uint8_t)-1) {
          if (cnt <= 1) {
            report.modifier = cur_kc_str->modifier;
            report.keycode[0] = cur_kc_str->keycode;
          }
          if (cnt++ >= 3) {
            cnt = 0;
            cur_kc_str++;
          }
        } else {
          usb_kbd_state = KBD_STATE_BTN;
        }
        break;
      case KBD_STATE_BTN:
        if (b1) {
          report.keycode[i++] = HID_KEY_ARROW_RIGHT;
        }
        if (b2) {
          report.keycode[i++] = HID_KEY_ARROW_LEFT;
        }
        break;
    }
    usb_send_data(&report, sizeof(hid_keyboard_report_t), 0, sendtok);
  } else if (endp == 2) {
    // Mouse (4 bytes)
    static uint8_t tsajoystick[4] = {0x00, 0x00, 0x00, 0x00};

    tsajoystick[1] = 0;
    tsajoystick[2] = 0;
    usb_send_data(tsajoystick, 4, 0, sendtok);
  } else {
    // If it's a control transfer, empty it.
    usb_send_empty(sendtok);
  }
}
