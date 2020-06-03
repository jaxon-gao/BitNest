#include<BlockManager/BlockManager.h>
    publicBlock* BlockManager::pb_curr(){
        return p_curr[0];
    }
    publicBlock* BlockManager::pb_last(){
        return p_curr[1];
    }

    innerBlock* BlockManager::ib_curr(){
        return curr[0];

    }
    innerBlock* BlockManager::ib_last(){
        return curr[1];

    }

    void BlockManager::Add_InnerBlock(innerBlock*b){
        innerBlock *i;
        i=curr[1];
        curr[1]=curr[0];
        curr[0]=b;
        delete i;

    }
    void BlockManager::Add_PublicBlock(publicBlock*b){
        publicBlock *i;
        i=p_curr[1];
        p_curr[1]=p_curr[0];
        p_curr[0]=b;
        delete i;
    }

    void BlockManager::AddPay(payment* p){
        pays.push_back(p);

    }
    void BlockManager::AddSto(Storage* p){
        stos.push_back(p);
    }
    void BlockManager::AddBak(BackUp* p){
        baks.push_back(p);
    }
    void BlockManager::AddRec(signature* p){
        ElectionRecord.push_back(p);
    }
    void BlockManager::AddSigs(signature* p){
        sigs.push_back(p);
    }

    innerBlock* BlockManager::PackUp(){
        innerBlock *b= new innerBlock();
        return b;
    }
    //联盟打包
    publicBlock* BlockManager::packUp_u(){
        publicBlock *b= new publicBlock();
        return b;
    }