// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_SIGNATURE
#define NEST_SIGNATURE
#include <iostream>
#include <uint256.h>
#include <string>
#include <sstream>
using namespace std;
class signature
{
public:
    signature(uint256 PK[2], uint256 SIG[2]);
    signature();
    signature(string data);
    uint256 pk[2];
    uint256 sig[2];
    friend ostream &operator<<(ostream &os, const signature &sig);
    friend bool operator<(const signature &sig1, const signature &sig2);
    friend istream &operator>>(istream &is, signature &sig);
    friend bool operator==(const signature &sig1, const signature &sig2);
    string data();
};
#endif