#include <stdio.h>
#include <stdint.h>
#include <endian.h>

void dump_binary(char* mem, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%08hhb ", mem[i]);
    }
    printf("\n");
}

// bit-fields
typedef struct {
    uint32_t mant: 23;
    uint32_t exp: 8;
    uint32_t sign: 1;
} /*__attribute__((packed))*/ float_bits;

typedef union {
    uint32_t u;
    float f;
    float_bits b;
} float_ext;

float_ext from_ieee754(uint32_t sign, int32_t exponent, uint32_t mantiss);
void dump_float(float_ext x);

int main() {
    printf("float_ext: alignment: %zd, size: %zd\n\n", _Alignof(float_ext), sizeof(float_ext));
    
    float_ext x;
    x = from_ieee754(0, -2, 0b10000000000000000000000);   
    scanf("%f", &x.f);
    dump_float(x);
}

float_ext from_ieee754(uint32_t sign, int32_t exponent, uint32_t mantiss) {
    float_ext res = {
        .b = {
            .mant = mantiss,
            .exp = (uint32_t)(exponent + 127),
            .sign = sign,
        },
    };

    return res;
}

void dump_float(float_ext x) {
    printf("%g\n", x.f);
    
    // x.u = htobe32(x.u);
    dump_binary((char*)(&x), sizeof(float_ext));
    printf("%032b\n", x.u);

    printf("s: %01b, e: %08b (%d), m: %023b\n", x.b.sign, x.b.exp, x.b.exp - 127, x.b.mant);
}

