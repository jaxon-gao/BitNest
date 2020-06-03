#include<block/publicBlock.h>
#include<FileSystem/NFile.h>
using namespace std;
    map<uint256,UserOvershot*> publicBlock::overshots(){
        vector<string> vv = gtv("overshots");
        UserOvershot*p[vv.size()];
        for(int i=0;i<vv.size();i++){
            p[i]=new UserOvershot(vv[i]);
        }
        map<uint256,UserOvershot*> ret;
        for(int i=0;i<vv.size();i++){
            ret[p[i]->h]=p[i];
        }
        return ret;
    }
    bool publicBlock::overshots(vector<UserOvershot*> data){
        vector<string> vv;
        for(int i=0;i<data.size();i++){
            vv.push_back(data[i]->data());
            delete data[i];
        }
        return write("overshots",vv);
    }
    vector<uint256> publicBlock::contracts(){
        vector<string>vv = gtv("contracts");
        vector<uint256> ret;
        uint256 tmp;
        for(int i =0;i<vv.size();i++){
            tmp=uint256(vv[i]);
            ret.push_back(tmp);
        }
        return ret;
    }
    bool publicBlock::contracts(vector<uint256> cs){
        stringstream ss;
        vector<string> vv;
        string tmp;
        for(int i=0;i<cs.size();i++){
            ss<<hex<<cs[i];
            ss>>tmp;
            vv.push_back(tmp);
        }
        return write("contracts",vv);
    }

    //<UserID,CashNum>
    map<uint256,uint256> publicBlock::frezon(){

    }
    bool publicBlock::frezon(map<uint256,uint256> m){

    }





    publicBlock::publicBlock():block(){
        kind="publicBlock";
    }
        string publicBlock::data(){

    }
        bool publicBlock::packer(uint256 p){
        stringstream ss;
        ss<<hex<<p;
        string pack;
        ss>>pack;
        write("packer",pack);
    }    
    uint256 publicBlock::packer(){
        string s = get("packer");
        return uint256(s);
    }