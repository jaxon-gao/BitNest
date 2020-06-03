// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include<contract/brief.h>
using namespace std;
string seq_brief[]={
    "ContractHash",
    "UnionID",
    "InnerPos"
};
Brief::Brief(uint256 h, string data):Contract(h,"brief"){
        int index=0;
        int beg=0;
        int k=0;
        string s;
        while(index!=data.size()){
            if(data[index]=='\n'){
                s=data.substr(beg,index-beg);
                if(k>7){
                    cerr<<"[BackUp] Data Packet Wrong"<<endl;
                    return;
                }
                Write(seq_brief[k],s);
                beg=index+1;
                k++;
            }
            index++;
        }
}

    Brief::Brief(uint256 h, string data,char c):Contract(h,"brief"){
            int index=0;
            int beg=0;
            int k=0;
            string s;
            while(index!=data.size()){
                if(data[index]==c){
                    s=data.substr(beg,index-beg);
                    if(k>7){
                        cerr<<"[BackUp] Data Packet Wrong"<<endl;
                        return;
                    }
                    Write(seq_brief[k],s);
                    beg=index+1;
                    k++;
                }
                index++;
            }
    }
    // Brief::Brief(Contract *c){
    //     string s = c->data();
    // }
    Brief::Brief(string path):Contract(path){}
    string Brief::ContractHash(){
        return Get("ContractHash");
    }
    string Brief::UnionID(){
        return Get("UnionID");
    }
    string Brief::InnerPos(){
        return Get("InnerPos");
    }

    bool Brief::ContractHash(string param){
        return Write("ContractHash",param);
    }
    bool Brief::UnionID(string param){
        return Write("UnionID",param);
    }
    bool Brief::InnerPos(string param){
        return Write("InnerPos",param);
    }
    string Brief::data(){
        return data('\n');
    }
    string Brief::data(char c){
        string ans="";
        stringstream ss;
        ss<<hash();
        ss>>ans;
        ans+=c;
        ans+=kind()+c;
        ans+=ContractHash()+c;
        ans+=UnionID()+c;
        ans+=InnerPos()+c;
        return ans;
    }
    bool Brief::verify(){

    }
