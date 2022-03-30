#include <stdio.h>

#include "../header/manipulation_bits.h"
#include "../header/attack.h"
#include "../header/sbox.h"
#include "../header/des.h"

// Fichier qui va contenir les 256 clés K possible
const char* filename = "potential_K.txt";

// Message chiffré correcte
static const __uint64_t chiffre = 0x35D12C82F252DC5B;

// Tous les messages chiffrés fautés
static const __uint64_t chiffres_faux[32] = { 
    0x37D02CC6F253DC5F, 0x35C32CC2F253DC5B, 0x35D12EC6F252DC5B, 0x35812884E252DC5B,  
    0x34C128C6E052DC5B, 0x35912882E250DC5B, 0x34D12882F252DE5B, 0x35912C83A256DC59,
    0x3C912883E242DC5B, 0x35D92C82B256DC5B, 0x35D12482B242DC5B, 0x35D12C8AB206DC5A,
    0x75D13C82BA16DC5B, 0x75D13C82F25ADC5B, 0x35D13C82F212D45B, 0x75D12C82F252CD52,
    0x55D13C82F612CC1B, 0x35F12C82F652CC5B, 0x35D10C82F652DD1B, 0x31D16CA2F252DD1B,
    0x31D12D82D652DD1B, 0x31D16C82F272DC5B, 0x25D12D82F252FC5B, 0x21D16C82F352987B,
    0xA1D12C92F3529C5B, 0x35512C92F2529C5B, 0x35D1AC82F352985B, 0x35D42C02F3529C5F,
    0x35D42C927352985F, 0x35D02C82F2D2DC4F, 0x35D42C82F2525C5B, 0x35D42C86F253DCCF };

// Tableau contenant le numéro des messages fautés
static const int faulted_bits[8][6] = {
    {  0, 31, 30, 29, 28, 27 },
    { 28, 27, 26, 25, 24, 23 },
    { 24, 23, 22, 21, 20, 19 },
    { 20, 19, 18, 17, 16, 15 },
    { 16, 15, 14, 13, 12, 11 },
    { 12, 11, 10,  9,  8,  7 },
    {  8,  7,  6,  5,  4,  3 },
    {  4,  3,  2,  1,  0, 31 }};

// Permet de déterminer une portion de clé correct en fonction des résultats des Sbox
static inline 
__uint64_t find_K16(__int64_t bit_Ki[8][6][64], __int64_t num_solution[8][6], int sbox, __uint64_t K_16) {
    int sol_2, sol_1 = 0;

    for (int fault = 1; fault < 6; fault++) {
        for (sol_2 = 0; sol_2 < num_solution[sbox][fault]; sol_2++) {
            if (bit_Ki[sbox][0][0] == bit_Ki[sbox][fault][sol_2]) {
                break;
            }
        }
        
        if (sol_2 == num_solution[sbox][fault]) {
            if (sol_1 + 1 >= num_solution[sbox][0]) {
                return K_16;
            }
            
            else {    
                fault = 1;
                sol_1++;
                bit_Ki[sbox][0][0] = bit_Ki[sbox][0][sol_1];
            }
        }
    }

    return ((K_16 << 0x06) | bit_Ki[sbox][0][0]);
}

// Permet de retrouver K16
static inline 
__uint64_t get_K16() {    
    __uint64_t K16 = 0x00;
    __uint64_t chiffre_IP = 0x00;
    __uint32_t L16_err, R16_err, res_P_inv = 0x00;
    
    __int64_t num_solution[8][6] = {{0}};
    __int64_t bit_Ki[8][6][64] = {{{0}}};

    __uint64_t res = initial_permutation(chiffre);

    __uint32_t L16 = get_L(res);
    __uint32_t R16 = get_R(res);

    for (int sbox = 0; sbox < 8; sbox++) {
        for(int fault=0; fault < 6; fault++) {
            chiffre_IP = initial_permutation(chiffres_faux[faulted_bits[sbox][fault]]);

            L16_err = get_L(chiffre_IP);
            R16_err = get_R(chiffre_IP);
        
            res_P_inv = permutation_inv(R16 ^ R16_err);

            __uint64_t mask_64 = (0xFC0000000000 >> (sbox * 6));
            __uint32_t mask_32 = (0x0000F0000000 >> (sbox * 4));

           for (__uint8_t Ki = 0; Ki < 64; Ki++) {
                __uint8_t e1 = ((expansion(L16)     & mask_64) >> (42 - (6 * sbox))) ^ Ki;
                __uint8_t e2 = ((expansion(L16_err) & mask_64) >> (42 - (6 * sbox))) ^ Ki;

                __uint8_t row = (2 * ((e1 & 0x20) >> 5) | (e1 & 0x1));
                __uint8_t column = (e1 & 0x1E) >> 1;

                __uint8_t row_err = (2 * ((e2 & 0x20) >> 5)) | (e2 & 0x1);
                __uint8_t column_err = (e2 & 0x1E) >> 1;
                
                __uint8_t res = ((res_P_inv & mask_32) >> ((7 - sbox) * 4));
                __uint8_t result_sbox = Sbox[sbox][row][column] ^ Sbox[sbox][row_err][column_err];

                if (res == result_sbox) {                 
                    bit_Ki[sbox][fault][num_solution[sbox][fault]] = Ki;
                    num_solution[sbox][fault]++;                   
                }
           }
        }

        K16 = find_K16(bit_Ki, num_solution, sbox, K16);
    }
    
    return K16;
}

// Permet de faire l'inverse de key schedule
static inline 
__uint64_t key_schedule_inv(__uint64_t K16) {
    return pc1_inv(pc2_inv(K16));
}

// Permet d'ajouter le bit manquant dans la clé
static inline 
__uint64_t missing_bit_14(__uint64_t K) { return K | 0x4000000000000; }

static inline 
__uint64_t missing_bit_15(__uint64_t K) { return K | 0x2000000000000; }

static inline 
__uint64_t missing_bit_19(__uint64_t K) { return K | 0x200000000000; }

static inline 
__uint64_t missing_bit_20(__uint64_t K) { return K | 0x100000000000; }

static inline 
__uint64_t missing_bit_51(__uint64_t K) { return K | 0x2000; }

static inline 
__uint64_t missing_bit_54(__uint64_t K) { return K | 0x400; }

static inline 
__uint64_t missing_bit_58(__uint64_t K) { return K | 0x40; }

static inline 
__uint64_t missing_bit_60(__uint64_t K) { return K | 0x10; }

// Permet d'écrire la clé potentiel dans un fichier
static inline 
void write_result(__uint64_t K) {
    FILE* output_file = fopen(filename, "a");
    if (!output_file) {
        perror("fopen");
    }

    char hex[17];
    sprintf(hex, "%lx", K);
    fputs(hex, output_file);
    fputs("\n", output_file);

    fclose(output_file);
}

// Permet d'avoir les 256 possibilité de clé en fonction des 8 bits manquant
static inline 
void missing_2_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_58(K);
    write_result(tmp_K);

    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_3_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_54(K);
    write_result(tmp_K);

    missing_2_bit(tmp_K);

    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_4_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_51(K);
    write_result(tmp_K);

    missing_3_bit(tmp_K);
    missing_2_bit(tmp_K);
    
    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_5_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_20(K);
    write_result(tmp_K);

    missing_4_bit(tmp_K);
    missing_3_bit(tmp_K);
    missing_2_bit(tmp_K);
    
    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_6_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_19(K);
    write_result(tmp_K);

    missing_5_bit(tmp_K);
    missing_4_bit(tmp_K);
    missing_3_bit(tmp_K);
    missing_2_bit(tmp_K);

    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_7_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_15(K);
    write_result(tmp_K);

    missing_6_bit(tmp_K);
    missing_5_bit(tmp_K);
    missing_4_bit(tmp_K);
    missing_3_bit(tmp_K);
    missing_2_bit(tmp_K);
    
    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

static inline 
void missing_8_bit(__uint64_t K) {
    __uint64_t tmp_K = 0x00;
    
    tmp_K = missing_bit_14(K);
    write_result(tmp_K);
    
    missing_7_bit(tmp_K);
    missing_6_bit(tmp_K);
    missing_5_bit(tmp_K);
    missing_4_bit(tmp_K);
    missing_3_bit(tmp_K);
    missing_2_bit(tmp_K);
    
    tmp_K = missing_bit_60(tmp_K);
    write_result(tmp_K);
}

// Permet d'écrire les 256 clés possible dans un fichier
static inline 
void find_K(__uint64_t K) {
    FILE* output_file = fopen(filename, "w");
    if (!output_file) {
        perror("fopen");
    }

    char hex[17];
    sprintf(hex, "%lx", missing_bit_60(K));
    fputs(hex, output_file);
    fputs("\n", output_file);
 
    fclose(output_file);
    
    missing_2_bit(K);
    missing_3_bit(K);
    missing_4_bit(K);
    missing_5_bit(K);
    missing_6_bit(K);
    missing_7_bit(K);
    missing_8_bit(K);
}

// Permet de lancer les fonctions permettant l'attaque
void attack() {
    printf("Recherche de K16:\n");
    __uint64_t K16 = get_K16();

    printf("Résultat de K16 en binaire:\n");
    print_binary_64(K16);

    printf("\n\nRésultat de K16 en hexadecimal:\n");
    char hex[17];
    sprintf(hex, "%lx", K16);
    puts(hex);
    printf("\n");

    __uint64_t K_48_bit = key_schedule_inv(K16);
    find_K(K_48_bit);
}


