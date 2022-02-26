#include <stdlib.h>
#include <stdio.h>

#include "../header/des.h"

int main() {
    __uint64_t clair = 0xA1F8ADF7767C5B6D;
    __uint64_t chiffre = 0x35D12C82F252DC5B;

    __uint64_t chiffres_faux[32] = { 0x37D02CC6F253DC5F, 0x35C32CC2F253DC5B, 0x35D12EC6F252DC5B, 0x35812884E252DC5B,  
                                     0x34C128C6E052DC5B, 0x35912882E250DC5B, 0x34D12882F252DE5B, 0x35912C83A256DC59,
                                     0x3C912883E242DC5B, 0x35D92C82B256DC5B, 0x35D12482B242DC5B, 0x35D12C8AB206DC5A,
                                     0x75D13C82BA16DC5B, 0x75D13C82F25ADC5B, 0x35D13C82F212D45B, 0x75D12C82F252CD52,
                                     0x55D13C82F612CC1B, 0x35F12C82F652CC5B, 0x35D10C82F652DD1B, 0x31D16CA2F252DD1B,
                                     0x31D12D82D652DD1B, 0x31D16C82F272DC5B, 0x25D12D82F252FC5B, 0x21D16C82F352987B,
                                     0xA1D12C92F3529C5B, 0x35512C92F2529C5B, 0x35D1AC82F352985B, 0x35D42C02F3529C5F,
                                     0x35D42C927352985F, 0x35D02C82F2D2DC4F, 0x35D42C82F2525C5B, 0x35D42C86F253DCCF };

    __uint64_t res = initial_permutation(chiffre);

    __uint32_t L16 = 0x00;
    __uint32_t R16 = 0x00;

    get_L16_R16(res, &L16, &R16);

    print_binary_32(L16);
    printf("\n");
    print_binary_64(expansion(L16));
    printf("\n");
    
    print_binary_32(R16);
    printf("\n");
    print_binary_64(expansion(R16));
    printf("\n");    

    return 0;
}