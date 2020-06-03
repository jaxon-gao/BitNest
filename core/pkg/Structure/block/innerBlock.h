#ifndef NEST_INUNION_BLOCK
#define NEST_INUNION_BLOCK
#include <block/block.h>
#include <FileSystem/NFile.h>
#include<vector>
#include<map>
#include<proof/signature.h>
#include<contract/payment.h>
#include<contract/storage.h>
#include<contract/backup.h>
using namespace std;
class innerBlock:public block{
public:
    innerBlock(uint256 h, string data);
    innerBlock();
    innerBlock(innerBlock &b);
    ~innerBlock(){};
    map<uint256,payment*> Payments();
    map<uint256,Storage*> Stores();
    map<uint256,BackUp*>  Backups();
    map<uint256,PoST *>   PoSTs();

    bool Payments(vector<payment*> payment);
    bool Stores(vector<Storage*> store);
    bool Backups(vector<BackUp*> backup);
    bool PoSTs(vector<PoST *> post);

    void packup();
    string kind;

    string data();
    bool packer(uint256 p);
    uint256 packer();
};
#endif