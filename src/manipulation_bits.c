#include <stdio.h>

#include "../header/manipulation_bits.h"

// Permet d'afficher un binaire de 64 bits
inline void print_binary_64(__uint64_t number) {
    if (number >> 1) {
        print_binary_64(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

// Permet d'afficher un binaire de 32 bits
inline void print_binary_32(__uint32_t number) {
    if (number >> 1) {
        print_binary_32(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}