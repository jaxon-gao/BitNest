#ifndef NEST_BMANAGER
#define NEST_BMANAGER
#include<string>
#include<block/block.h>
#include<block/innerBlock.h>
#include<block/publicBlock.h>
#include<contract/contract.h>
#include<contract/payment.h>
#include<contract/storage.h>
#include<vector>
using namespace std;
//临时记录：选举、合同
class BlockManager{
private:
//按照哈希排序
    vector<payment*> pays;
    vector<Storage*> stos;
    vector<BackUp*>  baks;
    vector<signature*> ElectionRecord;
    vector<signature*> sigs;
    innerBlock *curr[2];
    publicBlock *p_curr[2];
public:
    publicBlock* pb_curr();
    publicBlock* pb_last();

    innerBlock* ib_curr();
    innerBlock* ib_last();

    void Add_InnerBlock(innerBlock*b);
    void Add_PublicBlock(publicBlock*b);

    void AddPay(payment* p);
    void AddSto(Storage* p);
    void AddBak(BackUp* p);
    void AddRec(signature* p);
    void AddSigs(signature* p);

    innerBlock* PackUp();
    //联盟打包
    publicBlock* packUp_u();
};
#endif