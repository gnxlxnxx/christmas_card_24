#include <ch32v003fun.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "matrix.h"

#include "letters.h"

static const char *pos = "";
static int col_pos = MATRIX_WIDTH;
static uint8_t brightness = -1;

static void scroll_left(void) {
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    memmove(matrix_data[y], matrix_data[y] + 1, sizeof (matrix_data[0]) - 1);
    matrix_data[y][MATRIX_WIDTH - 1] = 0;
  }
}

static int8_t get_next_col(void) {
  int8_t col = get_col(*pos, col_pos++);

  if (col < 0) {
    pos++;
    col_pos = 0;

    return 0;
  }

  return col;
}

void text_start(const char *text, uint8_t text_brightness) {
  pos = text;
  brightness = text_brightness;
  col_pos = 0;

  scroll_left();
}

bool text_step(void) {
  scroll_left();

  if (*pos == '\0') {
    return col_pos++ < MATRIX_WIDTH;
  }

  int8_t col = get_next_col();

  for (int y = 0; y < MATRIX_HEIGHT; y++, col >>= 1) {
    if (col & 1) {
      matrix_data[y][MATRIX_WIDTH - 1] = brightness;
    }
  }

  return true;
}
