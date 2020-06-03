#include<contract/backup.h>
#include<contract/contract.h>
string seq_backup[]={
    "StorageHash",
    "sig_1",
    "sig_2",
    "sig_3",
    "preHash",
    "post_1",
    "post_2",
    "post_3"
};
    BackUp::BackUp(string path):Contract(path){}
    BackUp::BackUp(uint256 hash, string data):Contract(hash,"backup"){
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
                    Write(seq_backup[k],s);
                    beg=index+1;
                    k++;
                }
                index++;
            }
    }
        BackUp::BackUp(uint256 hash, string data,char c):Contract(hash,"backup"){
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
                    Write(seq_backup[k],s);
                    beg=index+1;
                    k++;
                }
                index++;
            }
    }

    string BackUp::StorageHash(){
        return Get("StorageHash");
    }
    string BackUp::sig_1(){
        return Get("sig_1");
    }
    string BackUp::sig_2(){
        return Get("sig_2");
    }
    string BackUp::sig_3(){
        return Get("sig_3");
    }
    string BackUp::preHash(){
        return Get("preHash");
    }
    string BackUp::post_1(){
        return Get("post_1");
    }
    string BackUp::post_2(){
        return Get("post_2");
    }
    string BackUp::post_3(){
        return Get("post_3");
    }


    bool BackUp::StorageHash(string param){
        return Write("StorageHash",param);
    }
    bool BackUp::sig_1(string param){
        return Write("sig_1",param);
    }
    bool BackUp::sig_2(string param){
        return Write("sig_2",param);
    }
    bool BackUp::sig_3(string param){
        return Write("sig_3",param);
    }
    bool BackUp::preHash(string param){
        return Write("preHash",param);
    }
    bool BackUp::post_1(string param){
        return Write("post_1",param);
    }
    bool BackUp::post_2(string param){
        return Write("post_2",param);
    }
    bool BackUp::post_3(string param){
        return Write("post_3",param);
    }

    string BackUp::data(){
        return data('\n');
    }
        string BackUp::data(char c){
        string ans="";
        stringstream ss;
        uint256 h = hash();
        ss<<h;
        ss>>ans;
        ans+=c;
        ans+=kind()+c;
        ans+=StorageHash()+c;
        ans+=sig_1()+c;
        ans+=sig_2()+c;
        ans+=sig_3()+c;
        ans+=preHash()+c;
        ans+=post_1()+c;
        ans+=post_2()+c;
        ans+=post_3()+c;
        return ans;
    }
    bool BackUp::verify(){

    }