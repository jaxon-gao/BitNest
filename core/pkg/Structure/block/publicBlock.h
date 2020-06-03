#ifndef NEST_INTERUNION_BLOCK
#define NEST_INTERUNION_BLOCK
#include<map>
#include<block/block.h>
#include<UserOvershot/UserOvershot.h>
using namespace std;
class publicBlock:public block{
public:
    publicBlock();
    ~publicBlock(){};
    string kind;
    map<uint256,UserOvershot*> overshots();
    bool overshots(vector<UserOvershot*> data);
    vector<uint256> contracts();
    bool contracts(vector<uint256>  cs);

    //<UserID,CashNum>
    map<uint256,uint256> frezon();
    bool frezon(map<uint256,uint256> m);

    bool packer(uint256 p);
    uint256 packer();
    string data();
};
#endif