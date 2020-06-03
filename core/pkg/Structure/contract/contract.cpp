#include<contract/contract.h>
Contract::Contract(string path):NFile(path){
}

Contract::Contract(uint256 h,string k):NFile(h,k){
    kind(k);
}

bool Contract::Write(string key, string value){
    return NFile::write(key,value);
}
string Contract::Get(string key){
    return get(key);
}

uint256 Contract::hash(){
    string ha = Get("hash");
    string ssss = "123";
    uint256 h(ha);
    return h;
}

bool Contract::hash(uint256_t h){
    string str;
    stringstream ss(str);
    ss<<h;
    return Write("hash",str);
}

string Contract::data(){

}
bool Contract::verify(){

}
string Contract::data(char c){
}


bool Contract::kind(string param){
    return Write("kind",param);
}
string Contract::kind(){
    return Get("kind");
}