#ifndef AES128_ENCRYPTION_H
#define AES128_ENCRYPTION_H

#include <stdint.h>

void aes_encrypt(uint8_t *, uint8_t *, uint8_t *);

void aes_decrypt(uint8_t *, uint8_t *, uint8_t *);

void print_cipher(uint8_t *);

#endif
