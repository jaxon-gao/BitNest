#include<post/post.h>
#include <iostream>
using namespace std;

PoST::PoST(){}

PoST::PoST(string data){
    vector<uint256> pro;
    int beg=0;
    string num;
    uint256 *tmp;
    for(int i=0;i<data.size();i++){
        if(data[i]==':'||data[i]=='!'){
            num=data.substr(beg,i-beg);
            tmp=new uint256(num);
            pro.push_back(*tmp);
            beg=i+1;
        }
        if(data[i]=='!'){
            proofs.push_back(pro);
            pro.clear();
        }
    }
}

void PoST::data(string d){
    vector<uint256> pro;
    int beg=0;
    string num;
    uint256 *tmp;
    for(int i=0;i<d.size();i++){
        if(d[i]==':'||d[i]=='!'){
            num=d.substr(beg,i-beg);
            tmp=new uint256(num);
            pro.push_back(*tmp);
            beg=i;
        }
        if(d[i]=='!'){
            proofs.push_back(pro);
            pro.clear();
        }
    }

}
string PoST::data(){
    string ans="";
    stringstream ss;
    string num;
    for(int i=0;i<proofs.size();i++){
        for(int j=0;j<proofs[i].size();j++){
            ss.clear();
            ss<<hex<<proofs[i][j];
            ss>>num;
            ans+=num;
            if(j!=proofs[i].size()-1){
                ans+=":";
            }
        }
        ans+="!";
    }
    return ans;
}
void PoST::AddProof(vector<uint256> p){
    proofs.push_back(p);
}

bool PoST::verify(){

}