#ifndef MATRIX_LETTERS_H_
#define MATRIX_LETTERS_H_

#include <stdint.h>

#define MAX_COLS 5

static const int8_t ascii_printable[][MAX_COLS] = {
  // SPACE
  {
    0b0000000,
    -1
  },
  // EXCL
  {
    0b1011111,
    -1
  },
  // "
  {
    0b0000011,
    0b0000000,
    0b0000011,
    -1
  },
  // #
  {
    0b0010100,
    0b1111111,
    0b0010100,
    0b1111111,
    0b0010100,
  },
  // $
  {
    0b0100110,
    0b1001001,
    0b1111111,
    0b1001001,
    0b0110010,
  },
  // %
  {
    0b0100010,
    0b0010000,
    0b0001000,
    0b0000100,
    0b0100010,
  },
  // &
  {
    0b0110110,
    0b1001001,
    0b1010001,
    0b0100010,
    0b1010000,
  },
  // '
  {
    0b0000011,
    -1
  },
  // (
  {
    0b0111110,
    0b1000001,
    -1
  },
  // )
  {
    0b1000001,
    0b0111110,
    -1
  },
  // *
  {
    0b0101010,
    0b0011100,
    0b1111111,
    0b0011100,
    0b0101010,
  },
  // +
  {
    0b0001000,
    0b0001000,
    0b0111110,
    0b0001000,
    0b0001000,
  },
  // ,
  {
    0b1100000,
    -1
  },
  // -
  {
    0b0001000,
    0b0001000,
    0b0001000,
    -1
  },
  // .
  {
    0b1000000,
    -1
  },
  // /
  {
    0b1100000,
    0b0011100,
    0b0000011,
    -1
  },
  // 0
  {
    0b0111110,
    0b1010001,
    0b1001001,
    0b1000101,
    0b0111110,
  },
  // 1
  {
    0b0000010,
    0b1111111,
    -1
  },
  // 2
  {
    0b1000010,
    0b1100001,
    0b1010001,
    0b1001001,
    0b1000110,
  },
  // 3
  {
    0b1000001,
    0b1001001,
    0b1001001,
    0b0110110,
    -1
  },
  // 4
  {
    0b0000111,
    0b0001000,
    0b0001000,
    0b1111111,
    -1
  },
  // 5
  {
    0b1001111,
    0b1001001,
    0b1001001,
    0b0110001,
    -1
  },
  // 6
  {
    0b0111110,
    0b1001001,
    0b1001001,
    0b0110010,
    -1
  },
  // 7
  {
    0b0000001,
    0b1100001,
    0b0011001,
    0b0000111,
    -1
  },
  // 8
  {
    0b0110110,
    0b1001001,
    0b1001001,
    0b0110110,
    -1
  },
  // 9
  {
    0b0100110,
    0b1001001,
    0b1001001,
    0b0111110,
    -1
  },
  // :
  {
    0b1000100,
    -1
  },
  // ;
  {
    0b1100100,
    -1
  },
  // <
  {
    0b0001000,
    0b0010100,
    0b0100010,
    -1
  },
  // =
  {
    0b0010100,
    0b0010100,
    0b0010100,
    -1
  },
  // >
  {
    0b0100010,
    0b0010100,
    0b0001000,
    -1
  },
  // ?
  {
    0b0000010,
    0b0000001,
    0b1011001,
    0b0001001,
    0b0000110,
  },
  // @
  {
    0b0111110,
    0b1000001,
    0b0011001,
    0b0100101,
    0b0111110,
  },
  // A
  {
    0b1111110,
    0b0001001,
    0b0001001,
    0b1111110,
    -1
  },
  // B
  {
    0b1111111,
    0b1001001,
    0b1001001,
    0b0110110,
    -1
  },
  // C
  {
    0b0111110,
    0b1000001,
    0b1000001,
    0b0100010,
    -1
  },
  // D
  {
    0b1111111,
    0b1000001,
    0b1000001,
    0b0111110,
    -1
  },
  // E
  {
    0b1111111,
    0b1001001,
    0b1000001,
    -1
  },
  // F
  {
    0b1111111,
    0b0001001,
    0b0000001,
    -1
  },
  // G
  {
    0b0111110,
    0b1000001,
    0b1001001,
    0b0111010,
    -1
  },
  // H
  {
    0b1111111,
    0b0001000,
    0b0001000,
    0b1111111,
    -1
  },
  // I
  {
    0b1000001,
    0b1111111,
    0b1000001,
    -1
  },
  // J
  {
    0b0110000,
    0b1000001,
    0b1000001,
    0b0111111,
    -1
  },
  // K
  {
    0b1111111,
    0b0001000,
    0b0010100,
    0b0100010,
    0b1000001,
  },
  // L
  {
    0b1111111,
    0b1000000,
    0b1000000,
    0b1000000,
    -1
  },
  // M
  {
    0b1111111,
    0b0000010,
    0b0000100,
    0b0000010,
    0b1111111,
  },
  // N
  {
    0b1111111,
    0b0000100,
    0b0001000,
    0b0010000,
    0b1111111,
  },
  // O
  {
    0b0111110,
    0b1000001,
    0b1000001,
    0b0111110,
    -1
  },
  // P
  {
    0b1111111,
    0b0001001,
    0b0001001,
    0b0000110,
    -1
  },
  // Q
  {
    0b0111110,
    0b1000001,
    0b0100001,
    0b1011110,
    -1
  },
  // R
  {
    0b1111111,
    0b0001001,
    0b0001001,
    0b1110110,
    -1
  },
  // S
  {
    0b1000110,
    0b1001001,
    0b1001001,
    0b0110001,
    -1
  },
  // T
  {
    0b0000001,
    0b0000001,
    0b1111111,
    0b0000001,
    0b0000001,
  },
  // U
  {
    0b0111111,
    0b1000000,
    0b1000000,
    0b0111111,
    -1
  },
  // V
  {
    0b0000111,
    0b0111000,
    0b1000000,
    0b0111000,
    0b0000111,
  },
  // W
  {
    0b1111111,
    0b0100000,
    0b0011100,
    0b0100000,
    0b1111111,
  },
  // X
  {
    0b1100011,
    0b0010100,
    0b0001000,
    0b0010100,
    0b1100011,
  },
  // Y
  {
    0b0000011,
    0b0000100,
    0b1111000,
    0b0000100,
    0b0000011,
  },
  // Z
  {
    0b1100001,
    0b1010001,
    0b1001001,
    0b1000101,
    0b1000011,
  },
  // [
  {
    0b1111111,
    0b1000001,
    -1
  },
  // '\'
  {
    0b0000011,
    0b0011100,
    0b1100000,
    -1
  },
  // ]
  {
    0b1000001,
    0b1111111,
    -1
  },
  // ^
  {
    0b0000010,
    0b0000001,
    0b0000010,
    -1
  },
  // _
  {
    0b1000000,
    0b1000000,
    0b1000000,
    0b1000000,
    -1
  },
  // `
  {
    0b0000001,
    0b0000010,
    -1
  },
  // a
  {
    0b0100000,
    0b1010100,
    0b1010100,
    0b1111000,
    -1
  },
  // b
  {
    0b1111111,
    0b1000100,
    0b1000100,
    0b0111000,
    -1
  },
  // c
  {
    0b0111000,
    0b1000100,
    0b1000100,
    -1
  },
  // d
  {
    0b0111000,
    0b1000100,
    0b1000100,
    0b1111111,
    -1
  },
  // e
  {
    0b0111000,
    0b1010100,
    0b1010100,
    0b0011000,
    -1
  },
  // f
  {
    0b1111110,
    0b0000101,
    0b0000001,
    -1
  },
  // g
  {
    0b0101100,
    0b1010010,
    0b1010010,
    0b0111100,
    -1
  },
  // h
  {
    0b1111111,
    0b0000100,
    0b0000100,
    0b1111000,
    -1
  },
  // i
  {
    0b1111101,
    -1
  },
  // j
  {
    0b0100000,
    0b1000000,
    0b0111101,
    -1
  },
  // k
  {
    0b1111111,
    0b0010000,
    0b0101000,
    0b1000100,
    -1
  },
  // l
  {
    0b0111111,
    0b1000000,
    -1
  },
  // m
  {
    0b1111000,
    0b0000100,
    0b1111000,
    0b0000100,
    0b1111000,
  },
  // n
  {
    0b1111000,
    0b0000100,
    0b0000100,
    0b1111000,
    -1
  },
  // o
  {
    0b0111000,
    0b1000100,
    0b1000100,
    0b0111000,
    -1
  },
  // p
  {
    0b1111110,
    0b0010010,
    0b0010010,
    0b0001100,
    -1
  },
  // q
  {
    0b0001110,
    0b0010010,
    0b0010010,
    0b1111100,
    -1
  },
  // r
  {
    0b1111000,
    0b0000100,
    0b0000100,
    -1
  },
  // s
  {
    0b1001000,
    0b1010100,
    0b1010100,
    0b0100100,
    -1
  },
  // t
  {
    0b0000100,
    0b0111111,
    0b1000100,
    0b0100000,
    -1
  },
  // u
  {
    0b0111100,
    0b1000000,
    0b1000000,
    0b1111100,
    -1
  },
  // v
  {
    0b0001100,
    0b0110000,
    0b1000000,
    0b0110000,
    0b0001100,
  },
  // w
  {
    0b0111100,
    0b1000000,
    0b0111100,
    0b1000000,
    0b0111100,
  },
  // x
  {
    0b1000100,
    0b0101000,
    0b0010000,
    0b0101000,
    0b1000100,
  },
  // y
  {
    0b0001110,
    0b1010000,
    0b1010000,
    0b0111110,
    -1
  },
  // z
  {
    0b1000100,
    0b1100100,
    0b1010100,
    0b1001100,
    0b1000100,
  },
  // {
  {
    0b0001000,
    0b0110110,
    0b1000001,
    -1
  },
  // |
  {
    0b1110111,
    -1
  },
  // }
  {
    0b1000001,
    0b0110110,
    0b0001000,
    -1
  },
  // ~
  {
    0b0001000,
    0b0000100,
    0b0001000,
    0b0010000,
    0b0001000,
  },
};

static const int8_t letter_unknown[] = {
  0b1111111,
  0b1000001,
  0b1000001,
  0b1000001,
  0b1111111,
};

static const int8_t letter_ue[] = {
  0b0111101,
  0b1000000,
  0b1000000,
  0b0111101,
  -1
};

static const int8_t *get_letter(char c) {
  if (' ' <= c && c <= '~') {
    return ascii_printable[c - ' '];
  }

  switch (c) {
    case 'ü':
      return letter_ue;
    default:
      return letter_unknown;
  }
}

static int8_t get_col(char c, int col) {
  if (col >= MAX_COLS) {
    return -1;
  }

  return get_letter(c)[col];
}


#endif
