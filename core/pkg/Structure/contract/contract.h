// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_CONTRACT
#define NEST_CONTRACT
#include<string>
#include<FileSystem/NFile.h>
#include<uint256.h>
#include<sstream>
// #include<contract/payment.h>
// #include<contract/storage.h>
// #include<contract/backup.h>

class Contract : public NFile{
    protected:
        string path;
        uint256 blockNum;
        uint256 location;
        string K;
        bool fixed;//已经固定
    public:
        Contract(string path);
        Contract(uint256 hash,string kind);
        uint256 hash();
        bool hash(uint256_t h);
        bool Write(string key, string value);
        string Get(string key);
        bool kind(string param);
        string kind();
        virtual string data(); 
        virtual bool verify();
        virtual string data(char c); 
};
#endif