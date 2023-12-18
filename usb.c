#include <stdint.h>
#include <tinyusb_hid.h>
#include <stddef.h>
#include <ch32v003fun.h>
#include <rv003usb.h>
#include "usb.h"
#include "main.h"

typedef struct {
  uint8_t modifier;
  uint8_t keycode;
} kc_element;

typedef struct {
  enum kbd_op_type {
    KBD_OP_BTN = 0,
    KBD_OP_STR,
    KBD_OP_KEY,
    KBD_OP_SLEEP
  } type;
  union kbd_op_param {
    const kc_element *op_str_kc_str;
    kc_element op_key_kc;
    uint32_t op_sleep_ticks;
  } param;
} kbd_op; // This would be more elegant in rust


static const kc_element url_kcstr[] = {
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

static const kbd_op kbd_op_initial[2] = {KBD_OP_BTN}; //TODO: Investigate section type conflict
static const kbd_op kbd_ops_url[] = {
  {KBD_OP_SLEEP, {.op_sleep_ticks = Ticks_from_Ms(2000)}},
  {KBD_OP_KEY, {.op_key_kc = {0, 0xf0}}},
  {KBD_OP_SLEEP, {.op_sleep_ticks = Ticks_from_Ms(4000)}},
  {KBD_OP_STR, {.op_str_kc_str = url_kcstr}},
  {KBD_OP_BTN}
};
static const kbd_op kbd_ops_url_win[] = {
  {KBD_OP_SLEEP, {.op_sleep_ticks = Ticks_from_Ms(2000)}},
  {KBD_OP_KEY, {.op_key_kc = {KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_R}}},
  {KBD_OP_SLEEP, {.op_sleep_ticks = Ticks_from_Ms(1000)}},
  {KBD_OP_STR, {.op_str_kc_str = url_kcstr + 1}},
  {KBD_OP_BTN}
};

volatile static struct kbd_state {
  const kbd_op *op;
  union kbd_op_state {
    struct kbd_op_str_state {
      const kc_element *pos;
      unsigned int i;
    } op_str;
    unsigned int op_key_i;
    uint32_t op_sleep_begin;
  } state;
} kbd_state = {kbd_op_initial};

static void kbd_goto(const kbd_op *op) {
  switch (op->type) {
    case KBD_OP_SLEEP:
      kbd_state.state.op_sleep_begin = SysTick->CNT;
      break;
    case KBD_OP_KEY:
      kbd_state.state.op_key_i = 0;
      break;
    case KBD_OP_STR:
      kbd_state.state.op_str.pos = op->param.op_str_kc_str;
      kbd_state.state.op_str.i = 0;
      break;
    case KBD_OP_BTN:
  }

  kbd_state.op = op;
}

void open_url(void) {
  kbd_goto(kbd_ops_url);
}

void open_url_windows(void) {
  kbd_goto(kbd_ops_url_win);
}

void usb_handle_user_in_request(struct usb_endpoint *e, uint8_t *scratchpad,
                                int endp, uint32_t sendtok,
                                struct rv003usb_internal *ist) {
  if (endp == 1) {
    // Keyboard (8 bytes)
    int i = 0;
    hid_keyboard_report_t report = {};
    const kbd_op *op = kbd_state.op;
    struct kbd_op_str_state *op_str_state = &kbd_state.state.op_str;

    switch (op->type) {
      case KBD_OP_SLEEP:
        if (SysTick->CNT - kbd_state.state.op_sleep_begin >= op->param.op_sleep_ticks) {
          kbd_goto(op + 1);
        }
        break;
      case KBD_OP_KEY:
        if (kbd_state.state.op_key_i++ <= 1) {
          report.modifier = op->param.op_key_kc.modifier;
          report.keycode[0] = op->param.op_key_kc.keycode;
        } else {
          kbd_goto(op + 1);
        }
        break;
      case KBD_OP_STR:
        if (op_str_state->pos->keycode != (uint8_t)-1) {
          if (op_str_state->i <= 1) {
            report.modifier = op_str_state->pos->modifier;
            report.keycode[0] = op_str_state->pos->keycode;
          }
          if (op_str_state->i++ >= 3) {
            op_str_state->i = 0;
            op_str_state->pos++;
          }
        } else {
          kbd_goto(op + 1);
        }
        break;
      case KBD_OP_BTN:
        if (b2) {
          report.keycode[i++] = HID_KEY_ARROW_LEFT;
        }
        if (b1) {
          report.keycode[i++] = HID_KEY_ARROW_RIGHT;
        }
        break;
    }

    usb_send_data(&report, sizeof(report), 0, sendtok);
  } else if (endp == 2) {
    // Gamepad (2 bytes)
    int8_t report[2] = {0};

    if (b1 || b2) {
      int shift_val = (b1 && b2) ? 4 : 6;
      int x = (b1a >> shift_val) - (b2a >> shift_val);

      if (x > INT8_MAX) {
        report[0] = INT8_MAX;
      } else if (x <= INT8_MIN) {
        report[0] = INT8_MIN + 1;
      } else {
        report[0] = x;
      }
    }

    report[1] = b2 | (b1 << 1);

    usb_send_data(report, sizeof(report), 0, sendtok);
  } else {
    // If it's a control transfer, empty it.
    usb_send_empty(sendtok);
  }
}
