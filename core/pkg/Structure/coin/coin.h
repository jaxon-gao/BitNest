// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_COIN
#define NEST_COIN
#include<string.h>
#include<FileSystem/NFile.h>
#include<contract/contract.h>
using namespace std;
class coin:public NFile{
public:
    coin(string path);
    Mint(payment c);
    Mint(storage c);
    Mint(backup c);
    Mint(coin c, int num);
    pay(int num);

}
#endif