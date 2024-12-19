#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "matrix.h"

#define IMG_HEIGHT(img) (sizeof (img) / sizeof ((img[0])))

static const uint8_t logo_fs_ei[][MATRIX_WIDTH / 2] = {
  { 0x00, 0x0f, 0x00, 0x00 },
  { 0x00, 0xff, 0xf0, 0x00 },
  { 0x0f, 0x00, 0x0f, 0x00 },
  { 0xf0, 0x00, 0x00, 0xf0 },
  { 0xf0, 0x00, 0x00, 0xf0 },
  { 0xf0, 0xff, 0xf0, 0xf0 },
  { 0x0f, 0x00, 0x0f, 0x00 },
  { 0xf0, 0xff, 0xf0, 0xf0 },
  { 0x55, 0x00, 0x05, 0x50 },
  { 0xf0, 0x55, 0x50, 0xf0 },
  { 0x0f, 0x05, 0x0f, 0x00 },
  { 0x00, 0xf5, 0xf0, 0x00 },
  { 0x00, 0x05, 0x00, 0x00 },
};

static int pos = 0;
static int shl_val = 0;

static void scroll_up(void) {
  memmove(matrix_data, matrix_data + 1, sizeof (matrix_data) - sizeof (matrix_data[0]));
  memset(matrix_data[MATRIX_HEIGHT - 1], 0, sizeof (matrix_data[0]));
}

void img_scroll_start(int shift_value) {
  pos = 0;
  shl_val = shift_value;

  scroll_up();
}

bool img_scroll_step(void) {
  scroll_up();

  if (pos < IMG_HEIGHT(logo_fs_ei)) {
    for (int i = 0; i < MATRIX_WIDTH / 2; i++) {
      uint8_t field = logo_fs_ei[pos][i];
      matrix_data[MATRIX_HEIGHT - 1][i * 2] = (field & 0xf0)>>(4 - shl_val);
      matrix_data[MATRIX_HEIGHT - 1][i * 2 + 1] = (field & 0x0f)<<(shl_val);
    }
  } else if (pos >= IMG_HEIGHT(logo_fs_ei) + MATRIX_HEIGHT) {
    return false;
  }

  pos++;

  return true;
}
