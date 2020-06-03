#include <uint256.h>
#include <uint256/test_uint256.h>
#include <iostream>
using namespace std;
bool test_uint256(){
    bool ans=true;
    uint256 add1=2;
    uint256 add2=1;
    ans = ans&&(add1+add2==3);

    uint256 mul1=2;
    uint256 mul2=2;
    ans = ans&&(mul1*mul2==4);

    uint256 xor1=0x07;
    uint256 xor2=0x08;
    ans = ans&&(xor1^xor2==xor2);
    
    uint256 x=2;
    uint256 p=10;
    uint256 x_p=pow(x,p);
    uint256 p_2=pow_2(p);
    ans=ans&&(x_p==p_2);

    cout<<"test: uint256 random test finished"<<endl;
    return ans;
}