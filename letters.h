#ifndef LETTERS_H_
#define LETTERS_H_

#include <ch32v003fun.h>
extern uint8_t matrix_data[7][8];


const uint8_t letter_A[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_B[7][5] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_C[7][5] = {
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1},
};

const uint8_t letter_D[7][5] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_E[7][5] = {
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1},
};

const uint8_t letter_F[7][5] = {
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
};

const uint8_t letter_G[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_H[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_I[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_J[7][5] = {
  {0, 0, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_K[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 1, 0},
  {1, 0, 1, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1},
};

const uint8_t letter_L[7][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1},
};

const uint8_t letter_M[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_N[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 0, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 0, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_O[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_P[7][5] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
};

const uint8_t letter_Q[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 1, 0},
  {0, 1, 1, 0, 1},
};

const uint8_t letter_R[7][5] = {
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_S[7][5] = {
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_T[7][5] = {
  {1, 1, 1, 1, 1},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_U[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_V[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_W[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 0, 1, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_X[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_Y[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_Z[7][5] = {
  {1, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1},
};

const uint8_t letter_a[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
};

const uint8_t letter_b[7][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_c[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1},
};

const uint8_t letter_d[7][5] = {
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
};

const uint8_t letter_e[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_f[7][5] = {
  {0, 0, 1, 1, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0},
};

const uint8_t letter_g[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_h[7][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_i[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_j[7][5] = {
  {0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0},
};

const uint8_t letter_k[7][5] = {
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1},
};

const uint8_t letter_l[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0},
};

const uint8_t letter_m[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
};

const uint8_t letter_n[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_o[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_p[7][5] = {
  {0, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
};

const uint8_t letter_q[7][5] = {
  {0, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
};

const uint8_t letter_r[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
};

const uint8_t letter_s[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_t[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1},
};

const uint8_t letter_u[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_v[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_w[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {0, 1, 0, 1, 0},
};

const uint8_t letter_x[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1},
};

const uint8_t letter_y[7][5] = {
  {0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_z[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {1, 1, 1, 1, 1},
};

const uint8_t letter_0[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 1, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_1[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 1, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_2[7][5] = {
  {1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0},
  {1, 1, 1, 1, 1},
};

const uint8_t letter_3[7][5] = {
  {1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_4[7][5] = {
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
};

const uint8_t letter_5[7][5] = {
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {1, 1, 1, 1, 0},
};

const uint8_t letter_6[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_7[7][5] = {
  {1, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_8[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_9[7][5] = {
  {0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},
};

const uint8_t letter_DASH[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
};

const uint8_t letter_DOT[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_EXCL[7][5] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0},
};

const uint8_t letter_SPACE[7][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
};

uint8_t get_letter(char letter, int row, int col){
  switch(letter){
    case 'A':
      return letter_A[row][col];
    case 'B':
      return letter_B[row][col];
    case 'C':
      return letter_C[row][col];
    case 'D':
      return letter_D[row][col];
    case 'E':
      return letter_E[row][col];
    case 'F':
      return letter_F[row][col];
    case 'G':
      return letter_G[row][col];
    case 'H':
      return letter_H[row][col];
    case 'I':
      return letter_I[row][col];
    case 'J':
      return letter_J[row][col];
    case 'K':
      return letter_K[row][col];
    case 'L':
      return letter_L[row][col];
    case 'M':
      return letter_M[row][col];
    case 'N':
      return letter_N[row][col];
    case 'O':
      return letter_O[row][col];
    case 'P':
      return letter_P[row][col];
    case 'Q':
      return letter_Q[row][col];
    case 'R':
      return letter_R[row][col];
    case 'S':
      return letter_S[row][col];
    case 'T':
      return letter_T[row][col];
    case 'U':
      return letter_U[row][col];
    case 'V':
      return letter_V[row][col];
    case 'W':
      return letter_W[row][col];
    case 'X':
      return letter_X[row][col];
    case 'Y':
      return letter_Y[row][col];
    case 'Z':
      return letter_Z[row][col];
    case 'a':
      return letter_a[row][col];
    case 'b':
      return letter_b[row][col];
    case 'c':
      return letter_c[row][col];
    case 'd':
      return letter_d[row][col];
    case 'e':
      return letter_e[row][col];
    case 'f':
      return letter_f[row][col];
    case 'g':
      return letter_g[row][col];
    case 'h':
      return letter_h[row][col];
    case 'i':
      return letter_i[row][col];
    case 'j':
      return letter_j[row][col];
    case 'k':
      return letter_k[row][col];
    case 'l':
      return letter_l[row][col];
    case 'm':
      return letter_m[row][col];
    case 'n':
      return letter_n[row][col];
    case 'o':
      return letter_o[row][col];
    case 'p':
      return letter_p[row][col];
    case 'q':
      return letter_q[row][col];
    case 'r':
      return letter_r[row][col];
    case 's':
      return letter_s[row][col];
    case 't':
      return letter_t[row][col];
    case 'u':
      return letter_u[row][col];
    case 'v':
      return letter_v[row][col];
    case 'w':
      return letter_w[row][col];
    case 'x':
      return letter_x[row][col];
    case 'y':
      return letter_y[row][col];
    case 'z':
      return letter_z[row][col];
    case '0':
      return letter_0[row][col];
    case '1':
      return letter_1[row][col];
    case '2':
      return letter_2[row][col];
    case '3':
      return letter_3[row][col];
    case '4':
      return letter_4[row][col];
    case '5':
      return letter_5[row][col];
    case '6':
      return letter_6[row][col];
    case '7':
      return letter_7[row][col];
    case '8':
      return letter_8[row][col];
    case '9':
      return letter_9[row][col];
    case '!':
      return letter_EXCL[row][col];
    case '.':
      return letter_DOT[row][col];
    case '-':
      return letter_DASH[row][col];
    case ' ':
    default:
      return letter_SPACE[row][col];
  }
}

int letter_offset_counter = 0;

uint32_t get_scrolled_letter(char* message, int counter, int row, int col){
  int offset = letter_offset_counter;
  uint8_t pixel = 0;
  if(col < 5-offset){
    pixel = get_letter(message[counter], row, col+offset);
  } else if (col == 5-offset) {
  } else if (col == 11-offset || col == 12-offset) {
  } else {
    pixel = get_letter(message[1+(counter)], row, col-(6-offset));
  }
  return pixel;
}

#endif // LETTERS_H_
