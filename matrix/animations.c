#include <stdint.h>
#include <string.h>
#include "matrix.h"
#include "random.h"
#include "text.h"
#include "img_scroll.h"

static const char merry_christmas[] = "Fröhliche Weihnachten wünscht die Fachschaft "
                                     "Elektro- und Informationstechnik der "
                                     "\x80 Universität Stuttgart!";

static enum matrix_modes {
  MODE_MERRY_CHRISTMAS,
  MODE_SNOWFALL,
  MODE_SPARKLE,
  MODE_RANDOM_PULSE,
  MODE_END,
} mode;

static uint32_t matrix_counter = 0;
static uint32_t matrix_counter2 = 0;
static int num_x = 0;

static uint8_t buffer_matrix[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

void matrix_animation_update(void) {
  switch(mode) {
    case MODE_MERRY_CHRISTMAS:
      if (matrix_counter++ >= ((matrix_counter2) ? 600 : 400)) {
        if (matrix_counter2 == 0) {
          if (!text_step()) {
            matrix_counter2 = 1;
            img_scroll_start(3);
          }
        } else {
          if (!img_scroll_step()) {
            matrix_counter2 = 0;
            text_start(merry_christmas, 64);
          }
        }
        matrix_counter = 0;
      }
      break;

    case MODE_SNOWFALL:
      if (matrix_counter++ >= 1000) {
        bool bottom_full = true;
        for (int col = 0; col < MATRIX_WIDTH; col++) {
          if (!matrix_counter2) {
            matrix_data[MATRIX_HEIGHT - 1][col] |= matrix_data[MATRIX_HEIGHT - 2][col];
          }
          if (matrix_data[MATRIX_HEIGHT - 1][col] == 0) {
            bottom_full = false;
          }
        }
        if (bottom_full) {
          matrix_counter2 = 16 << 7;
        }

        memmove(matrix_data + 1, matrix_data, sizeof (matrix_data) - 2 * sizeof (matrix_data[0]));

        for (int col = 0; col < MATRIX_WIDTH; col++) {
          matrix_data[0][col] = (rand8() > 250) ? 127 : 0;
        }
        matrix_counter = 0;
      }
      if (matrix_counter2) {
        if (matrix_counter2-- % 16 == 0) {
          for (int col = 0; col < MATRIX_WIDTH; col++) {
            uint8_t cur = matrix_data[MATRIX_HEIGHT - 1][col];
            matrix_data[MATRIX_HEIGHT - 1][col] = ((cur << 7) - cur) >> 7;
          }
        }
      }
      break;

    case MODE_SPARKLE:
      if (matrix_counter2++ >= 16) {
        for (int row = 0; row < MATRIX_HEIGHT; row++) {
          for (int col = 0; col < MATRIX_WIDTH; col++) {
            uint8_t cur = matrix_data[row][col];
            matrix_data[row][col] = ((cur << 7) - cur) >> 7;
          }
        }
        matrix_counter2 = 0;
      }
      if (matrix_counter++ >= 1024) {
        if (rand8() < 128) {
          uint8_t rand_row = rand8() % MATRIX_HEIGHT;
          uint8_t rand_col = rand8() % MATRIX_WIDTH;
          matrix_data[rand_row][rand_col] = 255;
        }
        matrix_counter = 0;
      }
      break;

    case MODE_RANDOM_PULSE:
      if (matrix_counter++ >= 40) {
        if (num_x++ % 30 == 0){
          uint8_t rand_row = rand8() % MATRIX_HEIGHT;
          uint8_t rand_col = rand8() % MATRIX_WIDTH;
          buffer_matrix[rand_row][rand_col] = 150;
        }
        num_x %= 250;

        for (int row = 0; row < MATRIX_HEIGHT; row++) {
          for (int col = 0; col < MATRIX_WIDTH; col++) {
            if (buffer_matrix[row][col] < matrix_data[row][col]) {
              matrix_data[row][col] -= 1;
            } else if (buffer_matrix[row][col] > matrix_data[row][col]) {
              matrix_data[row][col] += 1;
            }
            if (buffer_matrix[row][col] == matrix_data[row][col]) {
              buffer_matrix[row][col] = 0;
            }
          }
        }
        matrix_counter = 0;
      }
      break;

    default:;
  }
}

void matrix_next_mode(void) {
  mode = (mode + 1) % MODE_END;

  if (mode == MODE_MERRY_CHRISTMAS) {
    text_start(merry_christmas, 64);
  }

  matrix_counter = 0;
  matrix_counter2 = 0;
  num_x = 0;
}
