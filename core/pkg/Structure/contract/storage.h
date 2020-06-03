#ifndef NEST_STORAGE
#define NEST_STORAGE
#include<contract/contract.h>
#include<string>
using namespace std;
class Storage: public Contract{
public:
    Storage(string path);
    Storage(uint256 hash, string kind);
    Storage(uint256 hash, string kind,char c);
    string createTime();
    string runTime();
    string storageDuration();
    string storageSize();
    string PoST();
    string FinishProof();
    string Sig_Ser();
    string Sig_cli();
    string createdCash();
    string paidCash();


    bool createTime(string param);
    bool runTime(string param);
    bool storageDuration(string param);
    bool storageSize(string param);
    bool PoST(string param);
    bool FinishProof(string param);
    bool Sig_Ser(string param);
    bool Sig_cli(string param);
    bool createdCash(string param);
    bool paidCash(string param);

    string data(); 
    bool verify();
    string data(char c);
};
#endif