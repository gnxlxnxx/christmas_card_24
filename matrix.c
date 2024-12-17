#include "matrix.h"
#include "ch32v003fun.h"

/// LED1:   PD2
/// LED2:   PC7
/// LED3:   PC4
/// LED4:   PC3
/// LED5:   PC2
/// LED6:   PC1
/// LED7:   PC0
/// LED8:   PA1

uint8_t matrix_data[7][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};
uint8_t col = 0;
uint8_t brightness_index = 2;

struct Pin{
  GPIO_TypeDef* base_addr;
  uint32_t offset;
};

struct Pin pins[8] = {
  { GPIOD, 2 },
  { GPIOC, 7 },
  { GPIOC, 4 },
  { GPIOC, 3 },
  { GPIOC, 2 },
  { GPIOC, 1 },
  { GPIOC, 0 },
  { GPIOA, 1 },
};

void output_matrix() {
  // set old column pin high and to input
  pins[col].base_addr -> BSHR  |=  (0b1<<(pins[col].offset));
  uint32_t cfg = pins[col].base_addr -> CFGLR;
  cfg &= ~(0b1111<<(4*pins[col].offset));
  cfg |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*pins[col].offset);
  pins[col].base_addr -> CFGLR = cfg;
  /* pins[col].base_addr -> CFGLR &= ~(0b1111<<(4*pins[col].offset)); */
  /* pins[col].base_addr -> CFGLR |=  (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*pins[col].offset); */

  // update column
  col = (col+1)%8;

  // iterate through rows
  for(int row = 0; row < 7; row++){
    int data_row = col + row;
    if(data_row > 6){
        data_row -= 7;
    }
    if(matrix_data[data_row][col] > brightness_index) {
      // set pin high
      pins[(col + 1 + row) % 8].base_addr -> CFGLR &= ~(0b1111<<(4*pins[(col + 1 + row) % 8].offset));
      pins[(col + 1 + row) % 8].base_addr -> CFGLR |=  (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP) << (4*pins[(col + 1 + row) % 8].offset);
      pins[(col + 1 + row) % 8].base_addr -> BSHR  |=  (0b1<<pins[(col + 1 + row) % 8].offset);
    } else {
      // set pin to input
      pins[(col + 1 + row) % 8].base_addr -> CFGLR &= ~(0b1111<<(4*pins[(col + 1 + row) % 8].offset));
      pins[(col + 1 + row) % 8].base_addr -> CFGLR |=  (0b0100) << (4*pins[(col + 1 + row) % 8].offset);
    }
  }

  // set column pin low
  cfg  = pins[col].base_addr -> CFGLR;
  cfg &= ~(0b1111<<(4*pins[col].offset));
  cfg |=  (0b0001<<(4*pins[col].offset));
  pins[col].base_addr -> CFGLR = cfg;

  /* pins[col].base_addr -> CFGLR &= ~(0b1111<<(4*pins[col].offset)); */
  /* pins[col].base_addr -> CFGLR |=  (0b0001<<(4*pins[col].offset)); */
  pins[col].base_addr -> BSHR  |=  (0b1<<(pins[col].offset+16));

  if(col == 0){
    if(brightness_index != 0){
      brightness_index--;
    } else {
      brightness_index = 2;
    }
  }
}
