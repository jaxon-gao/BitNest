#ifndef NEST_KEYPAIR
#define NEST_KEYPAIR
#include<uint256.h>
struct KeyPair
{
    uint256 sk;
    uint256 pk[2];
};

#endif