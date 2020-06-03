// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_CONTRACT_BRIEF
#define NEST_CONTRACT_BRIEF
#include<contract/contract.h>
class Brief:public Contract{
public:
    Brief(uint256 h, string data);
    Brief(uint256 h, string data,char c);
    //TODO: create Brief using contract
    // Brief(Contract *c);
    Brief(string path);
    string ContractHash();
    string UnionID();
    string InnerPos();

    bool ContractHash(string param);
    bool UnionID(string param);
    bool InnerPos(string param);


    string data(); 
    string data(char c);
    bool verify();
};

#endif