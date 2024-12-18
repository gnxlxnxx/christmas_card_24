#include <stdint.h>
#include "matrix.h"
#include "random.h"
#include "text.h"

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

uint8_t buffer_matrix[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

static const char *merry_christmas = "Fröhliche Weihnachten wünscht die Fachschaft "
                                     "Elektro- und Informationstechnik der "
                                     "\x80 Universität Stuttgart!";

void matrix_animation_update(void) {
  switch(mode) {
    case MODE_MERRY_CHRISTMAS:
      if(matrix_counter++ >= 600){
        if (!text_step()) {
          text_start(merry_christmas, 127);
        }
        matrix_counter = 0;
      }
      break;

    case MODE_SNOWFALL:
      if(matrix_counter++ >= 1000){
        int sum = 0;
        for(int col = 0; col < MATRIX_WIDTH; col++){
          matrix_data[6][col] |= matrix_data[5][col];
          sum += (matrix_data[6][col]>0) ?1 :0;
        }
        if(sum == 8){
          for(int col = 0; col < MATRIX_WIDTH; col++){
            matrix_data[6][col] = 0;
          }
        }
        for(int row = MATRIX_HEIGHT-2; row > 0; row--){
          for(int col = 0; col < MATRIX_WIDTH; col++){
            matrix_data[row][col] = matrix_data[row-1][col];
          }
        }
        for(int col = 0; col < MATRIX_WIDTH; col++){
          matrix_data[0][col] = (rand8() > 250)?150:0;
        }
        matrix_counter = 0;
      }
      break;

    case MODE_SPARKLE:
      if (matrix_counter2++ >= 16) {
        for (int row = 0; row < MATRIX_HEIGHT; row++) {
          for (int col = 0; col < MATRIX_WIDTH; col++) {
            uint8_t cur = matrix_data[row][col];
            matrix_data[row][col] = ((cur << 6) - cur) >> 6;
          }
        }
        matrix_counter2 = 0;
      }
      if (matrix_counter++ >= 512) {
        if (rand8() < 128) {
          uint8_t rand_row = rand8()%MATRIX_HEIGHT;
          uint8_t rand_col = rand8()%MATRIX_WIDTH;
          matrix_data[rand_row][rand_col] = 255;
        }
        matrix_counter = 0;
      }
      break;

    case MODE_RANDOM_PULSE:
      if (matrix_counter++ >= 40) {
        if (num_x++ % 30 == 0){
          uint8_t rand_row = rand8()%MATRIX_HEIGHT;
          uint8_t rand_col = rand8()%MATRIX_WIDTH;
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

    default:
      matrix_next_mode();
  }
}

void matrix_next_mode(void) {
  mode = (mode + 1) % MODE_END;

  if (mode == MODE_MERRY_CHRISTMAS) {
    text_start(merry_christmas, 127);
  }

  matrix_counter = 0;
  num_x = 0;
}
