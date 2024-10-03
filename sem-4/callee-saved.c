#include <inttypes.h>

// inline assembly:
// https://gcc.gnu.org/onlinedocs/gcc/extensions-to-the-c-language-family/how-to-use-inline-assembly-language-in-c-code.html
//
// demonstrates how compiler preverves callee-saved registers
uint64_t inc(uint64_t i) {
    asm(
        ""
        : "+m" (i)
        :
        : "rax",
          "rbx",
          "rcx",
          "rdx",
          "rsi",
          "rdi",
        //   "rbp",
          "rsp",
          "r8",
          "r9",
          "r10",
          "r11",
          "r12",
          "r13",
          "r14",
          "r15",
          "ymm0",
          "ymm1",
          "ymm2",
          "ymm3",
          "ymm4",
          "ymm5",
          "ymm6",
          "ymm7",
          "ymm8",
          "ymm9",
          "ymm10",
          "ymm11",
          "ymm12",
          "ymm13",
          "ymm14",
          "ymm15"
    );
    return i + 1;
}

int main(int argc, char **argv) {
    (void)argv;
    return inc(argc);
}