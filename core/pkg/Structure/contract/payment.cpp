#include<contract/contract.h>
#include<contract/payment.h>

#include <sstream>
using namespace std;
const string seq_pay[] = {
    "createTime",
    "keySend",
    "payNum",
    "auditResult",
    "sig_send",
    "sig_recv",
    "PreHash",
    "PrePos",
    "CurrPos"
};
payment::payment(string path):Contract(path){}


payment::payment(uint256 h, string data):Contract(h,"payment"){
    int index=0;
    int beg=0;
    int k=0;
    string s;
    while(index!=data.size()){
        if(data[index]=='\n'){
            s=data.substr(beg,index-beg);
            if(k>8){
                cerr<<"[Payment] Data Packet Wrong"<<endl;
                return;
            }
            Write(seq_pay[k],s);
            beg=index+1;
            k++;
        }
        index++;
    }
}
payment::payment(uint256 h, string data,char c):Contract(h,"payment"){
    int index=0;
    int beg=0;
    int k=0;
    string s;
    while(index!=data.size()){
        if(data[index]==c){
            s=data.substr(beg,index-beg);
            if(k>8){
                cerr<<"[Payment] Data Packet Wrong"<<endl;
                return;
            }
            Write(seq_pay[k],s);
            beg=index+1;
            k++;
        }
        index++;
    }
}
string payment::createTime(){
    return Get("createTime");
}
string payment::keySend(){
    return Get("keySend");
}
string payment::payNum(){
    return Get("payNum");
}
string payment::auditResult(){
    return Get("auditResult");
}
string payment::sig_send(){
    return Get("sig_send");
}
string payment::sig_recv(){
    return Get("sig_recv");
}
string payment::PreHash(){
    return Get("PreHash");
}
string payment::PrePos(){
    return Get("PrePos");
}
string payment::CurrPos(){
    return Get("CurrPos");
}


bool payment::createTime(string param){
    return Write("createTime",param);
}
bool payment::keySend(string param){
    return Write("keySend",param);
}
bool payment::payNum(string param){
    return Write("payNum",param);
}
bool payment::auditResult(string param){
    return Write("auditResult",param);
}
bool payment::sig_send(string param){
    return Write("sig_send",param);
}
bool payment::sig_recv(string param){
    return Write("sig_recv",param);
}
bool payment::PreHash(string param){
    return Write("PreHash",param);
}
bool payment::PrePos(string param){
    return Write("PrePos",param);
}
bool payment::CurrPos(string param){
    return Write("CurrPos",param);
}

string payment::data(){
    return data('\n');
}

string payment::data(char c){
    string ans;
    uint256 h = hash();
    stringstream ss;
    ss<<h;
    ss>>ans;
    ans+=c;
    ans+=kind()+c;
    ans+=createTime()+c;
    ans+=keySend()+c;
    ans+=payNum()+c;
    ans+=auditResult()+c;
    ans+=sig_send()+c;
    ans+=sig_recv()+c;
    ans+=PreHash()+c;
    ans+=PrePos()+c;
    ans+=CurrPos()+c;
    return ans;
    
}
bool payment::verify(){

}