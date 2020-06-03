#include<contract/storage.h>
#include<contract/contract.h>
#include<sstream>
using namespace std;
const string seq_storage[]={
    "createTime",
    "runTime",
    "storageDuration",
    "storageSize",
    "PoST",
    "FinishProof",
    "Sig_Ser",
    "Sig_cli",
    "createdCash",
    "paidCash"
};

Storage::Storage(string path):Contract(path){}


Storage::Storage(uint256 hash, string data):Contract(hash,"storage"){
    int index=0;
    int beg=0;
    int k=0;
    string s;
    while(index!=data.size()){
        if(data[index]=='\n'){
            s=data.substr(beg,index-beg);
            if(k>9){
                cerr<<"[Storage] Data Packet Wrong"<<endl;
                return;
            }
            Write(seq_storage[k],s);
            beg=index+1;
            k++;
        }
        index++;
    }
}
Storage::Storage(uint256 hash, string data,char c):Contract(hash,"storage"){
    int index=0;
    int beg=0;
    int k=0;
    string s;
    while(index!=data.size()){
        if(data[index]==c){
            s=data.substr(beg,index-beg);
            if(k>9){
                cerr<<"[Storage] Data Packet Wrong"<<endl;
                return;
            }
            Write(seq_storage[k],s);
            beg=index+1;
            k++;
        }
        index++;
    }
}
    string Storage::createTime(){
        return Get("createTime");
    }
    string Storage::runTime(){
        return Get("runTime");
    }
    string Storage::storageDuration(){
        return Get("storageDuration");
    }
    string Storage::storageSize(){
        return Get("storageSize");
    }
    string Storage::PoST(){
        return Get("PoST");
    }
    string Storage::FinishProof(){
        return Get("FinishProof");
    }
    string Storage::Sig_Ser(){
        return Get("Sig_Ser");
    }
    string Storage::Sig_cli(){
        return Get("Sig_cli");
    }
    string Storage::createdCash(){
        return Get("createdCash");
    }
    string Storage::paidCash(){
        return Get("paidCash");
    }


    bool Storage::createTime(string param){
        return Write("createTime",param);
    }
    bool Storage::runTime(string param){
        return Write("runTime",param);
    }
    bool Storage::storageDuration(string param){
        return Write("storageDuration",param);
    }
    bool Storage::storageSize(string param){
        return Write("storageSize",param);
    }
    bool Storage::PoST(string param){
        return Write("PoST",param);
    }
    bool Storage::FinishProof(string param){
        return Write("FinishProof",param);
    }
    bool Storage::Sig_Ser(string param){
        return Write("Sig_Ser",param);
    }
    bool Storage::Sig_cli(string param){
        return Write("Sig_cli",param);
    }
    bool Storage::createdCash(string param){
        return Write("createdCash",param);
    }
    bool Storage::paidCash(string param){
        return Write("paidCash",param);
    }

    string Storage::data(){
        return data('\n');
    }

    string Storage::data(char c){
        string ans;
        uint256 h = hash();
        stringstream ss;
        ss<<h;
        ss>>ans;
        ans+=c;
        ans+=kind()+c;
        ans+=createTime()+c;
        ans+=runTime()+c;
        ans+=storageDuration()+c;
        ans+=storageSize()+c;
        ans+=PoST()+c;
        ans+=FinishProof()+c;
        ans+=Sig_Ser()+c;
        ans+=Sig_cli()+c;
        ans+=createdCash()+c;
        ans+=paidCash()+c;
        return ans;
    }

    bool Storage::verify(){

    }