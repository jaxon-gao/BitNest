#include<post/test_post.h>
#include<iostream>
using namespace std;
bool test_post(){
    PoST *proof=new PoST();
    uint256 k[25];
    vector<uint256> p1;
    vector<uint256> pp;
    pp.push_back(1);
    pp.push_back(2);
    pp.push_back(3);

    p1.push_back(5);
    p1.push_back(6);
    p1.push_back(7);
    
    proof->AddProof(pp);
    proof->AddProof(p1);

    PoST *N1 = new PoST(proof->data());
    if(proof->data()!=N1->data()){
        return false;
    }
    cout<<"test: PoST test finished"<<endl;

    return true;
}