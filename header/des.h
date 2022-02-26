#pragma once

#include <stdio.h>

//
void print_binary_64(__uint64_t number);

//
void print_binary_32(__uint32_t number);

//
__uint64_t initial_permutation(__uint64_t value);

//
__uint64_t expansion(__uint32_t R);

//
__uint32_t permutation_inv(__uint32_t R);

//
void get_L16_R16(__uint64_t value, __uint32_t *L16, __uint32_t *R16);