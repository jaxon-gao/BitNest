#include<block/block.h>
#include<FileSystem/NFile.h>
using namespace std;
    block::block(uint256 h,string kind):NFile(h,kind){}
    block::block():NFile(0,"block_tmp",1){}
    block::block(block & b):NFile(b.hash(),b.kind){
        //复制构造
    }

    bool block::ElectionRecord(vector<signature*> sigs){
        vector<string> wit;
        for(int i=0;i<sigs.size();i++){
            wit.push_back(sigs[i]->data());
            delete sigs[i];
        }
        return write("ElectionRecord",wit);
    }
    vector<signature*> block::ElectionRecord(){
        vector<string> v=gtv("ElectionRecord");
        vector<signature*> ret;
        signature *p;
        for(int i=0;i<v.size();i++){
            p=new signature(v[i]);
            ret.push_back(p);
        }
        return ret;
    }
    bool block::header(vector<string> data){
        write("header",data);
    }
    bool block::header(blockHeader* data){
        return header(data->data());
    }
    blockHeader* block::header(){
        return new blockHeader(gtv("header"));
    }



    bool block::Signature(vector<signature*> sigs){
        vector<string> v;
        for(int i=0;i<sigs.size();i++){
            v.push_back(sigs[i]->data());
            delete sigs[i];
        }
        return write("Signature",v);
    }
    vector<signature*> block::Signature(){
        vector<string> v=gtv("Signature");
        vector<signature*> ret;
        signature *p;
        for(int i=0;i<v.size();i++){
            p=new signature(v[i]);
            ret.push_back(p);
        }
        return ret;

    }



    uint256 block::hash(){
        return header()->curHash;
    }