#include<block/innerBlock.h>
#include<string>
#include<vector>
#include<map>
using namespace std;
    innerBlock::innerBlock():block(){
        kind="innerBlock";
    }
    innerBlock::innerBlock(uint256 h, string data):block(h,"innerBlock"){
        kind="innerBlock";
    }
    innerBlock::innerBlock(innerBlock &b):block(b){
        kind="innerBlock";
    }


    bool innerBlock::Payments(vector<payment*> payment){
        vector<string> in;
        for(int i=0;i<payment.size();i++){
            in.push_back(payment[i]->data(' '));
        }
        return add("payment",in);
    }
    bool innerBlock::Stores(vector<Storage*> store){
        vector<string> in;
        for(int i=0;i<store.size();i++){
            in.push_back(store[i]->data(' '));
        }
        return add("stores",in);
    }
    bool innerBlock::Backups(vector<BackUp*> backup){
        vector<string> in;
        for(int i=0;i<backup.size();i++){
            in.push_back(backup[i]->data(' '));
        }
        return add("backup",in);
    }
    bool innerBlock::PoSTs(vector<PoST *> post){
        vector<string> in;
        for(int i=0;i<post.size();i++){
            in.push_back(post[i]->data());
        }
        return add("PoST",in);
    }
    uint256 getHash(string &data){
        for(int i=0;i<data.size();i++){
            if(data[i]==' '){
                uint256 ret(data.substr(0,i));
                data=data.substr(i+1,data.size()-i-1);
                return ret;
            }
        }
    }
    map<uint256,payment*> innerBlock::Payments(){
        vector<string> ret = gtv("payment");
        map<uint256,payment*> ans;
        payment* p ;
        uint256 h;
        for(int i=0;i<ret.size();i++){
            h=getHash(ret[i]);
            p=new payment(h,ret[i]);
            ans[h]=p;
        }
        return ans;
    }
    map<uint256,Storage*> innerBlock::Stores(){
        vector<string> ret = gtv("stores");
        map<uint256,Storage*> ans;
        Storage* p ;
        uint256 h;
        for(int i=0;i<ret.size();i++){
            h=getHash(ret[i]);
            p=new Storage(h,ret[i]);
            ans[h]=p;
        }
        return ans;
    }
    map<uint256,BackUp*>  innerBlock::Backups(){
        vector<string> ret = gtv("backup");
        map<uint256,BackUp*> ans;
        BackUp* p ;
        uint256 h;
        for(int i=0;i<ret.size();i++){
            h=getHash(ret[i]);
            p=new BackUp(h,ret[i]);
            ans[h]=p;
        }
        return ans;
    }
    map<uint256,PoST *>   innerBlock::PoSTs(){
        vector<string> ret = gtv("PoST");
        map<uint256,PoST*> ans;
        PoST* p ;
        uint256 h;
        for(int i=0;i<ret.size();i++){
            h=getHash(ret[i]);
            p=new PoST(ret[i]);
            ans[h]=p;
        }
        return ans;
    }
    void innerBlock::packup(){

    }
    bool innerBlock::packer(uint256 p){
        stringstream ss;
        ss<<hex<<p;
        string pack;
        ss>>pack;
        write("packer",pack);
    }    
    uint256 innerBlock::packer(){
        string s = get("packer");
        return uint256(s);
    }
    


