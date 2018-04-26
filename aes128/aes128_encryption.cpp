#include "globals.h"
#include "aes128_encryption.h"

#define DEBUG 0

uint8_t s_box[256] =
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

uint8_t inv_s_box[256] =
{
     0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
     0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
     0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
     0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
     0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
     0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
     0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
     0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
     0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
     0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
     0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
     0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
     0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
     0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
     0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
     0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

uint8_t rcon[256] =
{
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

static void rotate(uint8_t *);
static void schedule_core(uint8_t *, uint8_t);
static void expand_key(uint8_t *);
static void subbytes_step(uint8_t *);
static void inv_subbytes_step(uint8_t *);
static void shiftrows_step(uint8_t *);
static void inv_shiftrows_step(uint8_t *);
static void mixcolumns_step(uint8_t *);
static void inv_mixcolumns_step(uint8_t *);
static void mixcolumns_step_col(uint8_t *);
static void addroundkey_step(uint8_t *, uint8_t *);
#if DEBUG==1
static void print_state(uint8_t *);
#endif


void rotate(uint8_t *in) {
        uint8_t a;
	uint8_t c;
        a = in[0];
        for(c=0;c<3;c++)
        	in[c] = in[c + 1];
        in[3] = a;
        return;
}

void schedule_core(uint8_t *in, uint8_t i) {
        int8_t a;

        rotate(in);

        for(a = 0; a < 4; a++)
        	in[a] = s_box[in[a]];

        in[0] ^= rcon[i];
}

void expand_key(uint8_t *in)
{
        uint8_t t[4];
        uint8_t c = 16;
	uint8_t i = 1;
        uint8_t a;
#if DEBUG==1
	printf("Expanded Keys:\r\n");
	for(a = 0; a < 16; a++)
	{
		printf("0x%x, ", in[a]);
	}
#endif
        while(c < 176) {
                for(a = 0; a < 4; a++)
                        t[a] = in[a+c-4];
                if((c % 16) == 0) {
			schedule_core(t ,i);
			i++;
#if DEBUG==1
			printf("\r\n");
#endif
		}
                for(a = 0; a < 4; a++) {
			t[a] ^= in[c+a-16];
			in[c+a] = t[a];
#if DEBUG==1
			printf("0x%x, ", in[c+a]);
#endif
		}
		c+=4;
        }
#if DEBUG==1
	printf("\r\n");
#endif
}

void subbytes_step(uint8_t *in)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		in[i] = s_box[in[i]];
	}
}

void inv_subbytes_step(uint8_t *in)
{
        for(uint8_t i = 0; i < 16; i++)
        {
                in[i] = inv_s_box[in[i]];
        }
}

void shiftrows_step(uint8_t *in)
{
	matrix_4x4_transpose(in);
	rotate(&in[4]);
	rotate(&in[8]);
	rotate(&in[8]);
	rotate(&in[12]);
	rotate(&in[12]);
	rotate(&in[12]);
	matrix_4x4_transpose(in);
}

void inv_shiftrows_step(uint8_t *in)
{
	matrix_4x4_transpose(in);
	rotate(&in[4]);
	rotate(&in[4]);
	rotate(&in[4]);
	rotate(&in[8]);
	rotate(&in[8]);
	rotate(&in[12]);
	matrix_4x4_transpose(in);
}

#if DEBUG==1
void print_state(uint8_t *state_ptr)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		if((0 == (i % 4)) && (0 != i))
		{
			printf("\r\n");
		}
		if(i != 15)
			printf("0x%x, ", state_ptr[i]);
		else
			printf("0x%x\r\n", state_ptr[i]);
	}
}
#endif

void mixcolumns_step_col(unsigned char *r) {
    unsigned char a[4];
    unsigned char b[4];
    unsigned char c;
    unsigned char h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
    for (c = 0; c < 4; c++) {
        a[c] = r[c];
        /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (unsigned char)((signed char)r[c] >> 7); /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = r[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= 0x1B & h; /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

void mixcolumns_step(uint8_t *inp)
{
	uint8_t l[4];
	for(uint8_t k = 0; k < 4; k++)
	{
		for(uint8_t i = 0; i < 4; i++)
		{
			l[i] = inp[k*4+i];
		}
		mixcolumns_step_col(l);
		for(uint8_t i = 0; i < 4; i++)
		{
			inp[k*4+i] = l[i];
		}
	}
}

uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t hi_bit_set;

    for (uint8_t counter = 0; counter < 8; counter++) {
        if ((b & 1) == 1) {
            p ^= a;
        }

        hi_bit_set = a & 0x80;
        a <<= 1;

        if (hi_bit_set == 0x80) {
            a ^= 0x1b;
        }

        b >>= 1;
    }

    return p;
}

static void inv_mixcolumns_step(uint8_t * state) {
    uint8_t a[4];

    for (uint8_t k = 0; k < 4; k++) {
        for (uint8_t i = 0; i < 4; i++) {
            a[i] = state[i + 4 * k];
        }

        state[4 * k] =\
            gmul(a[0], 14) ^ gmul(a[3], 9) ^ gmul(a[2], 13) ^ gmul(a[1], 11);

        state[1 + 4 * k] =\
            gmul(a[1], 14) ^ gmul(a[0], 9) ^ gmul(a[3], 13) ^ gmul(a[2], 11);

        state[2 + 4 * k] =\
            gmul(a[2], 14) ^ gmul(a[1], 9) ^ gmul(a[0], 13) ^ gmul(a[3], 11);

        state[3 + 4 * k] =\
             gmul(a[3], 14) ^ gmul(a[2], 9) ^ gmul(a[1], 13) ^ gmul(a[0], 11);
    }
}

void addroundkey_step(uint8_t *state, uint8_t *roundKey)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		state[i] ^= roundKey[i];
	}
}

void aes_encrypt(uint8_t *input, uint8_t *key, uint8_t *output)
{
	uint8_t roundKeys[176];
	uint8_t state[16];
	for(uint8_t i = 0; i < 16; i++)
	{
		state[i] = input[i];
		roundKeys[i] = key[i];
	}
	expand_key(roundKeys);
	addroundkey_step(state, &roundKeys[0]);
	for(uint8_t i = 1; i < 10; i++)
	{
		subbytes_step(state);
		shiftrows_step(state);
		mixcolumns_step(state);
		addroundkey_step(state, &roundKeys[(i)*16]);
	}
	subbytes_step(state);
	shiftrows_step(state);
	addroundkey_step(state, &roundKeys[10*16]);
	for(uint8_t i = 0; i < 16; i++)
	{
		output[i] = state[i];
	}
}

void aes_decrypt(uint8_t *input, uint8_t *key, uint8_t *output)
{
        uint8_t roundKeys[176];
        uint8_t state[16];
        for(uint8_t i = 0; i < 16; i++)
        {
                state[i] = input[i];
                roundKeys[i] = key[i];
        }
        expand_key(roundKeys);
        addroundkey_step(state, &roundKeys[160]);
        for(uint8_t i = 1; i < 10; i++)
        {
                inv_shiftrows_step(state);
                inv_subbytes_step(state);
                addroundkey_step(state, &roundKeys[160 - i*16]);
                inv_mixcolumns_step(state);
        }
        inv_shiftrows_step(state);
        inv_subbytes_step(state);
        addroundkey_step(state, &roundKeys[0]);
        for(uint8_t i = 0; i < 16; i++)
        {
                output[i] = state[i];
        }
}

void print_cipher(uint8_t *input)
{
//	printf("Cipher Text: ");
	for(uint8_t i = 0; i < 16; i++)
	{
		printf("%02x", input[i]);
	}
	printf("\r\n");
}
