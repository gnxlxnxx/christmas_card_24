#include <stdint.h>

/* White Noise Generator State */
#define NOISE_BITS 8
#define NOISE_MASK ((1 << NOISE_BITS) - 1)
#define NOISE_POLY_TAP0 31
#define NOISE_POLY_TAP1 21
#define NOISE_POLY_TAP2 1
#define NOISE_POLY_TAP3 0
uint32_t lfsr = 1;

uint8_t rand8(void) {
  uint8_t bit;
  uint32_t new_data;

  for (bit = 0; bit < NOISE_BITS; bit++) {
    new_data = ((lfsr >> NOISE_POLY_TAP0) ^ (lfsr >> NOISE_POLY_TAP1) ^
                (lfsr >> NOISE_POLY_TAP2) ^ (lfsr >> NOISE_POLY_TAP3));
    lfsr = (lfsr << 1) | (new_data & 1);
  }

  return lfsr & NOISE_MASK;
}
