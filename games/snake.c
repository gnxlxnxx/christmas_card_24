#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix/matrix.h"
#include "matrix/text.h"
#include "touch.h"
#include "random.h"

#define HEAD_BRIGHTNESS 64
#define SNAKE_BRIGHTNESS 32
#define MAULTASCH_BRIGHTNESS 96

#define DIR_MASK 3
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define TYPE_MASK (3<<6)
#define TYPE_NONE 0
#define TYPE_SNAKE (1<<6)
#define TYPE_MAULTASCH (2<<6)

static const char praise_text[] = "UNGLAUBLICH! Ich hätte es nicht für möglich gehalten...";
static const char game_over_text[] = "Game Over";

typedef struct {
  int8_t x;
  int8_t y;
} coord;

static const coord delta_lut[] = {
  { 0, -1 },
  { 1, 0 },
  { 0, 1 },
  { -1, 0 },
};

static const coord start_pos = { 0, MATRIX_HEIGHT / 2 };

static enum state {
  STATE_PLAY,
  STATE_MSG,
  STATE_SCORE,
} state;

static uint8_t snake_field[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

static uint8_t dir = DIR_RIGHT, dir_next = DIR_RIGHT;
static uint8_t length = 1, length_increase = 1;
static uint8_t score = 0;
static coord snake = start_pos, tail = start_pos;

static char score_text[] = "Score: 255";

static int counter = 0;

static int8_t wraparound(int8_t val, int8_t max) {
  if (val >= max) {
    return val - max;
  } else if (val < 0) {
    return val + max;
  } else {
    return val;
  }
}

static coord go_dir(coord pos, uint8_t dir) {
  coord delta = delta_lut[dir];
  pos.x = wraparound(pos.x + delta.x, MATRIX_WIDTH);
  pos.y = wraparound(pos.y + delta.y, MATRIX_HEIGHT);

  return pos;
}

static void gen_new_maultasch(void) {
  coord pos;
  do {
    pos.x = rand8() % MATRIX_WIDTH;
    pos.y = rand8() % MATRIX_HEIGHT;
  } while (snake_field[pos.y][pos.x]);

  snake_field[pos.y][pos.x] = TYPE_MAULTASCH;
  matrix_data[pos.y][pos.x] = MAULTASCH_BRIGHTNESS;
}

static void advance_head(void) {
  dir = dir_next;

  matrix_data[snake.y][snake.x] = SNAKE_BRIGHTNESS;
  snake_field[snake.y][snake.x] = TYPE_SNAKE | dir;

  snake = go_dir(snake, dir);

  switch (snake_field[snake.y][snake.x] & TYPE_MASK) {
    case TYPE_SNAKE:
      text_start(game_over_text, HEAD_BRIGHTNESS);
      state = STATE_MSG;
      break;
    case TYPE_MAULTASCH:
      score++;
      length_increase += 2;
      gen_new_maultasch();
      /* fallthrough */
    case TYPE_NONE:
      snake_field[snake.y][snake.x] = TYPE_SNAKE;
      matrix_data[snake.y][snake.x] = HEAD_BRIGHTNESS;
      break;
  }
}

static void retreat_tail(void) {
  if (length_increase) {
    length_increase--;
    length++;
    return;
  }

  uint8_t dir = snake_field[tail.y][tail.x] & DIR_MASK;

  matrix_data[tail.y][tail.x] = 0;
  snake_field[tail.y][tail.x] = 0;

  tail = go_dir(tail, dir);
}

void snake_start(void) {
  state = STATE_PLAY;
  dir = DIR_RIGHT;
  dir_next = DIR_RIGHT;
  length = 1;
  length_increase = 1;
  score = 0;
  snake = start_pos;
  tail = start_pos;

  counter = 0;

  memset(snake_field, 0, sizeof (snake_field));
  snake_field[snake.y][snake.x] = TYPE_SNAKE;

  memset(matrix_data, 0, sizeof (matrix_data));
  matrix_data[snake.y][snake.x] = HEAD_BRIGHTNESS;

  gen_new_maultasch();
}

bool snake_step(void) {
  switch (state) {
    case STATE_PLAY:
      if (btn_right && btn_right_toggled) {
        dir_next = (dir + 1) & DIR_MASK;
      } else if (btn_left && btn_left_toggled) {
        dir_next = (dir - 1) & DIR_MASK;
      }
      if (counter++ >= 1024) {
        retreat_tail();
        if (length >= MATRIX_WIDTH * MATRIX_HEIGHT) {
          text_start(praise_text, MAULTASCH_BRIGHTNESS);
          state = STATE_MSG;
          return true;
        }

        advance_head();

        counter = 0;
      }
      return true;
    case STATE_MSG:
      if (counter++ >= 400) {
        if (!text_step()) {
          utoa10(score, score_text + sizeof (score_text) - 4);
          text_start(score_text, HEAD_BRIGHTNESS);
          state = STATE_SCORE;
        }
        counter = 0;
      }
      break;
    case STATE_SCORE:
      if (counter++ >= 400) {
        if (!text_step()) {
          text_start(score_text, HEAD_BRIGHTNESS);
        }
        counter = 0;
      }
      break;
  }

  if (btn_left && btn_right && (btn_left_toggled || btn_right_toggled)) {
    return false;
  }

  return true;
}
