// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_COIN
#define NEST_COIN
#include <string.h>
#include <FileSystem/NFile.h>
#include <contract/contract.h>
#include <contract/payment.h>
#include <contract/backup.h>
#include <contract/storage.h>
using namespace std;
class coin : public NFile
{

public:
    int num;
    coin(string path);
    coin Mint(payment c);
    coin Mint(Storage c);
    coin Mint(BackUp c);
    coin Mint(coin c, int num);
    void pay(int num);
};
#endif