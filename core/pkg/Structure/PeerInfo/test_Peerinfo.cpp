#include <PeerInfo/PeerInfo.h>
#include<iostream>
using namespace std;
bool test_peerinfo(){
    PeerInfo *my = new PeerInfo(pow_2(3)+1,0);
    PeerInfo *_0100 = new PeerInfo(pow_2(3),0);
    PeerInfo *_1000 = new PeerInfo(pow_2(4),0);
    if(my->layer(_0100)!=1){
        return false;
    }
    if(my->layer(_1000)!=32){
        return false;
    }
    cout<<"test: PeerInfo test finished"<<endl;
    return true;
}