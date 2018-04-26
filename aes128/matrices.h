#ifndef MATRICES_H
#define MATRICES_H
#include <stdint.h>

void matrix_multiply(uint8_t *mat1, uint8_t rows_1, uint8_t cols_1, uint8_t *mat2, uint8_t rows_2, uint8_t cols_2, uint8_t *res_ptr);

void matrix_4x4_transpose(uint8_t *);

#endif

