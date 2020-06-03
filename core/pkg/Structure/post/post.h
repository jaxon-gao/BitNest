#ifndef NEST_POST
#define NEST_POST
#include<vector>
#include<string>
#include<uint256.h>
#include<sstream>
using namespace std;
class PoST{
public:
    vector<vector<uint256>> proofs;
    void AddProof(vector<uint256> p);
    string data();
    void data(string d);
    PoST(string data);
    PoST();
    bool verify();
};
#endif