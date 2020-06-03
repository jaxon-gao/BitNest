/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "uECC.h"

#include <stdio.h>
#include <string.h>
#include<iostream>
using namespace std;
int main() {
    int i, c;
    uint8_t Private[32] = {0};
    uint8_t Public[64] = {0};
    uint8_t hash[32] = {0};
    uint8_t sig[64] = {0};

    const struct uECC_Curve_t * curves[5];
    int num_curves = 0;
#if uECC_SUPPORTS_secp160r1
    curves[num_curves++] = uECC_secp160r1();
#endif
#if uECC_SUPPORTS_secp192r1
    curves[num_curves++] = uECC_secp192r1();
#endif
#if uECC_SUPPORTS_secp224r1
    curves[num_curves++] = uECC_secp224r1();
#endif
#if uECC_SUPPORTS_secp256r1
    curves[num_curves++] = uECC_secp256r1();
#endif
#if uECC_SUPPORTS_secp256k1
    curves[num_curves++] = uECC_secp256k1();
#endif
    printf("Testing 256 signatures\n");
    for (c = 0; c < num_curves; ++c) {
        for (i = 0; i < 256; ++i) {
            printf(".");
            fflush(stdout);

            if (!uECC_make_key(Public, Private, curves[c])) {
                printf("uECC_make_key() failed\n");
                return 1;
            }
            memcpy(hash, Public, sizeof(hash));
            
            if (!uECC_sign(Private, hash, sizeof(hash), sig, curves[c])) {
                printf("uECC_sign() failed\n");
                return 1;
            }

            if (!uECC_verify(Public, hash, sizeof(hash), sig, curves[c])) {
                printf("uECC_verify() failed\n");
                return 1;
            }
        }
        printf("\n");
    }
    
    return 0;
}
