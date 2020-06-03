
#include<proof/test_signature.h>
#include <iostream>
#include <sstream>
#include <uint256.h>
using namespace std;
bool test_signature(){
    uint256 t1[2]= {1024,2048};
    uint256 t2[2]= {4069,6553};
    signature test(t1,t2),test2;

    stringstream ss;
    ss<<test<<endl;
    ss>>test2;
    if(!(test==test2)){
        return false;
    }
    
    signature test3(test.data());
    if(!(test==test3)){
        return false;
    }

    cout<<"test: signature test finished"<<endl;
    return true;
}