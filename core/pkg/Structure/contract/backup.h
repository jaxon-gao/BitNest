#ifndef NEST_BACKUP
#define NEST_BACKUP
#include<contract/contract.h>
#include<string>
using namespace std;
class BackUp: public Contract{
public:
    BackUp(string path);
    BackUp(uint256 hash, string data);
    BackUp(uint256 hash, string data,char c);

    string StorageHash();
    string sig_1();
    string sig_2();
    string sig_3();
    string preHash();
    string post_1();
    string post_2();
    string post_3();


    bool StorageHash(string param);
    bool sig_1(string param);
    bool sig_2(string param);
    bool sig_3(string param);
    bool preHash(string param);
    bool post_1(string param);
    bool post_2(string param);
    bool post_3(string param);

    string data(); 
    string data(char c);
    bool verify();
};
#endif