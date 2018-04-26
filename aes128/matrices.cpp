#include "globals.h"

void matrix_4x4_transpose(uint8_t *mat)
{
        uint8_t temp[16];
        for(uint8_t i = 0; i < 4; i++)
        {
                for(uint8_t j = 0; j < 4; j++)
                {
                        temp[i*4 + j] = mat[j*4 + i];
                }
        }
        for(uint8_t i = 0; i < 16; i++)
        {
                mat[i] = temp[i];
        }
}

void matrix_multiply(uint8_t *mat1, uint8_t rows_1, uint8_t cols_1, uint8_t *mat2, uint8_t rows_2, uint8_t cols_2, uint8_t *res_ptr)
{
        int32_t temp1 = 0;
        if(cols_1  != rows_2)
        {
                perror("Invalid Matrix Multiplication\r\n");
                return;
        }
        for(uint8_t i = 0; i < rows_1; i++)
        {
                for(uint8_t j = 0; j < cols_2; j++)
                {
                        for(uint8_t k = 0; k < cols_1; k++)
                        {
                                temp1 += (mat1[i * cols_1 + k] * mat2[k * cols_2 + j]);
                        }
                        res_ptr[i * cols_2 + j] = temp1;
                        temp1 = 0;
                }
        }
}

