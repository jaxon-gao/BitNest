// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_PAYMENT
#define NEST_PAYMENT
#include<contract/contract.h>
#include<string>
using namespace std;

class payment: public Contract{
public:
    payment(uint256 hash, string data);
    payment(uint256 hash, string data,char c);
    payment(string path);
    string createTime();
    string keySend();
    string payNum();
    string auditResult();
    string sig_send();
    string sig_recv();
    string PreHash();
    string PrePos();
    string CurrPos();
    bool createTime(string param);
    bool keySend(string param);
    bool payNum(string param);
    bool auditResult(string param);
    bool sig_send(string param);
    bool sig_recv(string param);
    bool PreHash(string param);
    bool PrePos(string param);
    bool CurrPos(string param);
    string data();
    string data(char c);
    bool verify();
};
#endif